#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <h5geo/h5seiscontainer.h>
#include <h5geo/h5seis.h>
#include <h5geo/h5core.h>

#include <h5gt/H5File.hpp>
#include <h5gt/H5Group.hpp>
#include <h5gt/H5DataSet.hpp>

#include <filesystem>
namespace fs = std::filesystem;

class H5SeisFixture: public ::testing::Test {
public:

  virtual void SetUp() override{
    // code here will execute just before the test ensues
    static bool trig = false;

    FILE_NAME = "seis.h5";
    SEIS_NAME1 = "path1/to/seis";
    SEIS_NAME2 = "path2/to/seis";

    if (trig){
      h5gt::File file(FILE_NAME, h5gt::File::OpenOrCreate);
      seisContainer = H5SeisCnt_ptr(
            h5geo::createSeisContainer(
              file, h5geo::CreationType::OPEN_OR_CREATE));
    } else {
      h5gt::File file(FILE_NAME, h5gt::File::OpenOrCreate |
                      h5gt::File::Overwrite);
      seisContainer = H5SeisCnt_ptr(
            h5geo::createSeisContainer(
              file, h5geo::CreationType::CREATE_OR_OVERWRITE));
    }

    p.domain = h5geo::Domain::OWT;
    p.lengthUnits = "millimeter";
    p.temporalUnits = "millisecond";
    p.angularUnits = "degree";
    p.dataUnits = "My_data_units";
    p.dataType = h5geo::SeisDataType::PRESTACK;
    p.surveyType = h5geo::SurveyType::TWO_D;
    p.nTrc = 30;
    p.nSamp = 10;
    p.srd = 20;

    trig = true;
  }

  virtual void TearDown() override{
    // code here will be called just after the test completes
    // ok to through exceptions from here if need be
    auto h5File = seisContainer->getH5File();
    h5geo::unlinkContent(h5File);
  }

public:
  H5SeisCnt_ptr seisContainer;
  SeisParam p;
  std::string FILE_NAME, SEIS_NAME1, SEIS_NAME2;
};

TEST_F(H5SeisFixture, createContainer){
  ASSERT_TRUE(fs::exists(FILE_NAME));
}

TEST_F(H5SeisFixture, createContainerWithDifferentCreateFlags){
  std::string fileName = "seis_dif_flag.h5";
  std::string fileName_tmp = fileName;

  std::remove(fileName.c_str());

  H5SeisCnt_ptr seisCnt = H5SeisCnt_ptr(
        h5geo::createSeisContainerByName(
          fileName, h5geo::CreationType::OPEN));
  ASSERT_TRUE(seisCnt == nullptr) << "OPEN";

  seisCnt = H5SeisCnt_ptr(
        h5geo::createSeisContainerByName(
          fileName, h5geo::CreationType::CREATE));
  ASSERT_TRUE(seisCnt != nullptr) << "CREATE";

  seisCnt = H5SeisCnt_ptr(
        h5geo::createSeisContainerByName(
          fileName, h5geo::CreationType::OPEN));
  ASSERT_TRUE(seisCnt != nullptr) << "OPEN";

  seisCnt = H5SeisCnt_ptr(
        h5geo::createSeisContainerByName(
          fileName, h5geo::CreationType::CREATE));
  ASSERT_TRUE(seisCnt == nullptr) << "CREATE";

  seisCnt = H5SeisCnt_ptr(
        h5geo::createSeisContainerByName(
          fileName, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_TRUE(seisCnt != nullptr) << "OPEN_OR_CREATE";

  /* h5gt cannot Truncate/Overwrite h5 file if it is already open */
//  seisCnt = H5SeisCnt_ptr(
//        h5geo::createSeisContainerByName(
//          fileName, h5geo::CreationType::CREATE_OR_OVERWRITE));
//  ASSERT_TRUE(seisCnt != nullptr) << "CREATE_OR_OVERWRITE";

  seisCnt = H5SeisCnt_ptr(
        h5geo::createSeisContainerByName(
          fileName_tmp, h5geo::CreationType::CREATE_UNDER_NEW_NAME));
  ASSERT_TRUE(seisCnt != nullptr && fileName_tmp.compare(fileName)) << "CREATE_UNDER_NEW_NAME";

  std::remove(fileName.c_str());
  std::remove(fileName_tmp.c_str());
}

TEST_F(H5SeisFixture, createSeisWithDifferentCreateFlags){
  H5Seis_ptr seis(
        seisContainer->createSeis(
          SEIS_NAME1, p, h5geo::CreationType::OPEN));
  ASSERT_TRUE(seis == nullptr) << "OPEN";

  seis = H5Seis_ptr(seisContainer->createSeis(
                      SEIS_NAME1, p, h5geo::CreationType::CREATE));
  ASSERT_TRUE(seis != nullptr) << "CREATE";

  seis = H5Seis_ptr(seisContainer->createSeis(
      seis->getObjG(), p, h5geo::CreationType::OPEN));
  ASSERT_TRUE(seis != nullptr) << "OPEN";

  seis = H5Seis_ptr(seisContainer->createSeis(
                      SEIS_NAME1, p, h5geo::CreationType::OPEN));
  ASSERT_TRUE(seis != nullptr) << "OPEN";

  seis = H5Seis_ptr(seisContainer->createSeis(
                      SEIS_NAME1, p, h5geo::CreationType::CREATE));
  ASSERT_TRUE(seis == nullptr) << "CREATE";

  seis = H5Seis_ptr(seisContainer->createSeis(
                      SEIS_NAME1, p, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_TRUE(seis != nullptr) << "OPEN_OR_CREATE";

  seis = H5Seis_ptr(seisContainer->createSeis(
                      SEIS_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(seis != nullptr) << "CREATE_OR_OVERWRITE";

  std::string seisName_tmp = SEIS_NAME1;
  seis = H5Seis_ptr(seisContainer->createSeis(
                      seisName_tmp, p, h5geo::CreationType::CREATE_UNDER_NEW_NAME));
  ASSERT_TRUE(seis != nullptr && seisName_tmp.compare(SEIS_NAME1)) << "CREATE_UNDER_NEW_NAME";
}

TEST_F(H5SeisFixture, createAndGetSeis){
  seisContainer->createSeis(SEIS_NAME1, p, h5geo::CreationType::OPEN_OR_CREATE);
  H5Seis_ptr seis(seisContainer->getSeis(SEIS_NAME1));
  ASSERT_TRUE(seis != nullptr);
}

TEST_F(H5SeisFixture, createAndGetSeisFromGroup){
  h5gt::Group group =
      seisContainer->getH5File().createGroup(SEIS_NAME2);
  seisContainer->createSeis(group, p, h5geo::CreationType::OPEN_OR_CREATE);
  H5Seis_ptr seis(seisContainer->getSeis(SEIS_NAME2));
  ASSERT_TRUE(seis != nullptr);
}

TEST_F(H5SeisFixture, writeAndGetTextHeader){
  H5Seis_ptr seis(seisContainer->createSeis(
                    SEIS_NAME1, p, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_TRUE(seis != nullptr) << "OPEN_OR_CREATE";

  std::vector<std::string> txtHdr;
  for (size_t i = 0; i < 40; i++)
    txtHdr.push_back("Bart Simpson");

  ASSERT_TRUE(seis->writeTextHeader(txtHdr))
      << "write text header from std::vector<std::string>";

  std::vector<std::string> txtHdr_out =
      seis->getTextHeader();

  ASSERT_THAT(txtHdr, ::testing::ElementsAreArray(txtHdr_out))
      << "read text header and compare with original";

  std::string str = "Lisa Simpson";
  char txtHdr_c [40][80];
  for (size_t col = 0; col < str.size(); col++)
    for (size_t row = 0; row < 40; row++)
      txtHdr_c[row][col] = str[col];

  ASSERT_TRUE(seis->writeTextHeader(txtHdr_c))
      << "write text header from char[40][80]";
}

TEST_F(H5SeisFixture, writeAndGetBinHeader){
  H5Seis_ptr seis(seisContainer->createSeis(
                    SEIS_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(seis != nullptr) << "OPEN_OR_CREATE";

  std::vector<double> binHdrStd;
  for (size_t i = 0; i < seis->getNBinHdr(); i++)
    binHdrStd.push_back(i);

  ASSERT_TRUE(seis->writeBinHeader(binHdrStd))
      << "write/read bin header from std::vector";

  Eigen::VectorXd binHdr = Eigen::VectorXd::Random(
        seis->getNBinHdr());

  ASSERT_TRUE(seis->writeBinHeader(binHdr));

  Eigen::VectorXd binHdr_out = seis->getBinHeader();

  ASSERT_TRUE(binHdr_out.isApprox(binHdr))
      << "write/read bin header from Eigen vector";

  ASSERT_TRUE(seis->writeBinHeader("SAMP_RATE", 2000));

  ASSERT_TRUE(seis->getBinHeader("SAMP_RATE") == 2000)
      << "write/read bin header from single value";
}

TEST_F(H5SeisFixture, writeAndGetTrace){
  H5Seis_ptr seis(seisContainer->createSeis(
                    SEIS_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(seis != nullptr) << "OPEN_OR_CREATE";

  Eigen::MatrixXf traces = Eigen::MatrixXf::Random(
        seis->getNSamp(), seis->getNTrc());

  ASSERT_TRUE(seis->writeTrace(traces, 0))
      << "Write all traces at once";

  Eigen::MatrixXf traces_out = seis->getTrace(
        0, seis->getNTrc(), 0, seis->getNSamp());

  ASSERT_TRUE(traces_out.isApprox(traces))
      << "Read all traces and compare them with original";

  traces_out = seis->getTrace(
        3, 10, 2, 5);

  ASSERT_TRUE(traces_out.isApprox(traces.block(2, 3, 5, 10)))
      << "Read all block of traces and compare them with original";

  ASSERT_TRUE(seis->writeTrace(traces.col(10), 20))
      << "Write single traces";

  traces_out = seis->getTrace(
        20, 1, 0, seis->getNSamp());

  ASSERT_TRUE(traces_out.isApprox(traces.col(10)))
      << "Read single trace and compare it with original";
}

TEST_F(H5SeisFixture, writeAndGetTraceHeaderFinalize){
  H5Seis_ptr seis(seisContainer->createSeis(
                    SEIS_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(seis != nullptr) << "OPEN_OR_CREATE";

  Eigen::MatrixXd trcHdr = Eigen::MatrixXi::Random(
        seis->getNTrc(), seis->getNTrcHdr()).cast<double>();

  ASSERT_TRUE(seis->writeTraceHeader(trcHdr, 0))
      << "Write all trace headers at once";

  Eigen::MatrixXd trcHdr_out = seis->getTraceHeader(
        0, seis->getNTrc());

  ASSERT_TRUE(trcHdr_out.isApprox(trcHdr))
      << "Read all trace headers and compare them with original";

  ASSERT_TRUE(seis->writeTraceHeader("CDP", trcHdr.col(40)))
      << "Write single header (CDP for example)";

  trcHdr_out = seis->getTraceHeader("CDP");

  ASSERT_TRUE(trcHdr_out.isApprox(trcHdr.col(40)))
      << "Read and compare single header (CDP for example)";
}

TEST_F(H5SeisFixture, writeAndGetSortedData){
  H5Seis_ptr seis(seisContainer->createSeis(
                    SEIS_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(seis != nullptr) << "OPEN_OR_CREATE";

  Eigen::MatrixXf traces = Eigen::MatrixXf::Random(
        seis->getNSamp(), seis->getNTrc());

  ASSERT_TRUE(seis->writeTrace(traces, 0))
      << "Write all traces at once";

  double HI = 5; // set HI and LO according to your problem.
  double LO = 0;
  double range= HI-LO;
  Eigen::MatrixXd m = Eigen::MatrixXd::Random(seis->getNTrc(), seis->getNTrcHdr()); // 3x3 Matrix filled with random numbers between (-1,1)
  m = (m + Eigen::MatrixXd::Constant(seis->getNTrc(), seis->getNTrcHdr(),1.))*range/2.; // add 1 to the matrix to have values between 0 and 2; multiply with range/2
  m = (m + Eigen::MatrixXd::Constant(seis->getNTrc(), seis->getNTrcHdr(),LO)); //set LO as the lower bound (offset)

  Eigen::MatrixXd trcHdr = m.array().round();

  ASSERT_TRUE(seis->writeTraceHeader(trcHdr, 0))
      << "Write all trace headers at once";

  ASSERT_TRUE(seis->addPKeySort("FFID"))
      << "Add sort";
  ASSERT_TRUE(seis->addPKeySort("CDP"))
      << "Add sort";
  ASSERT_TRUE(seis->addPKeySort("DSREG"))
      << "Add sort";

  Eigen::MatrixXf trc_sorted;
  Eigen::MatrixXd hdr_sorted;
  Eigen::VectorX<size_t> trc_ind = seis->getSortedData(
        trc_sorted, hdr_sorted,
        {"FFID", "CDP", "DSREG"},
        {2, 1, 4},
        {4, 5, 5});

  Eigen::VectorX<size_t> trcHdrInd(3);
  trcHdrInd << 0,1,2;
  seis->getTraceHeader(trc_ind, trcHdrInd);

  std::cout << "hdr_sorted:\n" << hdr_sorted << std::endl;

  Eigen::MatrixXd trcHdr_out = seis->getTraceHeader(
        0, seis->getNTrc());

  ASSERT_TRUE(trcHdr_out.isApprox(trcHdr))
      << "Read all trace headers and compare them with original";

  ASSERT_TRUE(seis->writeTraceHeader("CDP", trcHdr.col(40)))
      << "Write single header (CDP for example)";

  trcHdr_out = seis->getTraceHeader("CDP");

  ASSERT_TRUE(trcHdr_out.isApprox(trcHdr.col(40)))
      << "Read and compare single header (CDP for example)";
}

// ORIGIN POINT1 AND POINT2 ARE DEFINED ONLY FOR 3D STACK DATA AND AFTER 'Finalize()` is called
//TEST_F(H5SeisFixture, writeAndGetOriginPoint1Point2){
//  H5Seis_ptr seis(seisContainer->createSeis(
//                    SEIS_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));

//  double orientation = 1;
//  Eigen::VectorXd origin(2), point1(2), point2(2);
//  origin << 1, 1;
//  point1 << 10, 1;
//  point1 << 1, 10;

//  seis->setOrigin(origin);
//  seis->setPoint1(point1);
//  seis->setPoint2(point2);

//  Eigen::VectorXd originOut = seis->getOrigin();
//  Eigen::VectorXd point1Out = seis->getPoint1();
//  Eigen::VectorXd point2Out = seis->getPoint2();

//  ASSERT_TRUE(origin.isApprox(originOut));
//  ASSERT_TRUE(point1.isApprox(originOut));
//  ASSERT_TRUE(point2.isApprox(point2Out));

//}
