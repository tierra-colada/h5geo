#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <h5geo/h5seiscontainer.h>
#include <h5geo/h5seis.h>
#include <h5geo/private/h5segy.h>

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
    SEIS_NAME3 = "path3/to/seis";

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

//  virtual void TearDown() override{
//    // code here will be called just after the test completes
//    // ok to through exceptions from here if need be
//    auto h5File = seisContainer->getH5File();
//    h5geo::unlinkContent(h5File);
//  }

public:
  H5SeisCnt_ptr seisContainer;
  SeisParam p;
  std::string FILE_NAME, SEIS_NAME1, SEIS_NAME2, SEIS_NAME3;
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
  H5Seis_ptr seis(seisContainer->openSeis(SEIS_NAME1));
  ASSERT_TRUE(seis != nullptr);
}

TEST_F(H5SeisFixture, createAndGetSeisFromGroup){
  h5gt::Group group =
      seisContainer->getH5File().createGroup(SEIS_NAME2);
  seisContainer->createSeis(group, p, h5geo::CreationType::OPEN_OR_CREATE);
  H5Seis_ptr seis(seisContainer->openSeis(SEIS_NAME2));
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
  ASSERT_TRUE(seis != nullptr) << "CREATE_OR_OVERWRITE";

  std::vector<double> binHdrStd;
  for (size_t i = 0; i < seis->getNBinHdr(); i++)
    binHdrStd.push_back(i);

  ASSERT_TRUE(seis->writeBinHeader(binHdrStd))
      << "write/read bin header from std::vector";

  Eigen::VectorXd binHdr = Eigen::VectorXd::Random(
        seis->getNBinHdr());

  ASSERT_TRUE(seis->writeBinHeader(binHdr));

  std::map<std::string, double> binHdr_out = seis->getBinHeader();
  ASSERT_TRUE(binHdr_out.count("SAMP_RATE") > 0)
      << "write/read bin header from Eigen vector";

  ASSERT_TRUE(seis->writeBinHeader("SAMP_RATE", 2000));

  ASSERT_TRUE(seis->getBinHeader("SAMP_RATE") == 2000)
      << "write/read bin header from single value";
}

TEST_F(H5SeisFixture, writeAndGetTrace){
  {
    H5Seis_ptr seis(seisContainer->createSeis(
                      SEIS_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
    ASSERT_TRUE(seis != nullptr) << "CREATE_OR_OVERWRITE";

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

  {
    H5Seis_ptr seis(seisContainer->createSeis(
                      SEIS_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));

    // Write traces by indexes
    size_t nSamp = seis->getNSamp();
    size_t nTrc = seis->getNTrc();
    size_t fromSampInd = 3;
    size_t nSampIO = nSamp - fromSampInd;
    size_t nTrcIO = 3;
    Eigen::VectorX<size_t> trcInd(nTrcIO);
    trcInd << 3, 7, 4;
    Eigen::MatrixXf traces = Eigen::MatrixXf::Random(nSampIO, nTrcIO);

    ASSERT_TRUE(seis->writeTrace(traces, trcInd, fromSampInd))
        << "Write traces by indexes";

    Eigen::MatrixXf traces_out = seis->getTrace(trcInd, fromSampInd, nSampIO);

    ASSERT_TRUE(traces_out.isApprox(traces))
        << "Read traces by indexes and compare it with original";

    traces_out = seis->getTrace(
          trcInd(0), 1, fromSampInd, nSampIO);
    ASSERT_TRUE(traces_out.isApprox(traces.col(0)))
        << "Read traces by indexes and compare it with original (0)";

    traces_out = seis->getTrace(
          trcInd(1), 1, fromSampInd, nSampIO);
    ASSERT_TRUE(traces_out.isApprox(traces.col(1)))
        << "Read traces by indexes and compare it with original (1)";

    traces_out = seis->getTrace(
          trcInd(2), 1, fromSampInd, nSampIO);
    ASSERT_TRUE(traces_out.isApprox(traces.col(2)))
        << "Read traces by indexes and compare it with original (2)";
  }
}

TEST_F(H5SeisFixture, writeAndGetTraceHeader){
  H5Seis_ptr seis(seisContainer->createSeis(
                    SEIS_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(seis != nullptr) << "CREATE_OR_OVERWRITE";

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

  trcHdr_out = seis->getTraceHeader("CDP", 0, seis->getNTrc());

  ASSERT_TRUE(trcHdr_out.isApprox(trcHdr.col(40)))
      << "Read and compare single header (CDP for example)";
}

TEST_F(H5SeisFixture, writeAndGetSortedData){
  H5Seis_ptr seis(seisContainer->createSeis(
                    SEIS_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(seis != nullptr) << "CREATE_OR_OVERWRITE";

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

  trcHdr_out = seis->getTraceHeader("CDP", 0, seis->getNTrc());

  ASSERT_TRUE(trcHdr_out.isApprox(trcHdr.col(40)))
      << "Read and compare single header (CDP for example)";
}

TEST_F(H5SeisFixture, updateBoundary){
  H5Seis_ptr seis(seisContainer->createSeis(
                    SEIS_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(seis != nullptr) << "CREATE_OR_OVERWRITE";

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

  seis->updateBoundary();
  auto boundary = seis->getBoundary();
  ASSERT_TRUE(!boundary.isZero(0));
}

TEST_F(H5SeisFixture, generateGeometry){
  H5Seis_ptr seis(seisContainer->createSeis(
                    SEIS_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(seis != nullptr) << "CREATE_OR_OVERWRITE";

  double src_x0 = 0;
  double src_dx = 1;
  size_t src_nx = 3;
  double src_y0 = 0;
  double src_dy = 2;
  size_t src_ny = 2;
  double src_z = 0;

  ASSERT_TRUE(seis->generateSTKGeometry(
                src_x0, src_dx, src_nx,
                src_y0, src_dy, src_ny,
                src_z));

  Eigen::VectorXd cdp(6), cdpx(6), cdpy(6);
  cdp << 1, 2, 3, 4, 5, 6;
  cdpx << 0, 1, 2, 0, 1, 2;
  cdpy << 0, 0, 0, 2, 2, 2;

  Eigen::VectorXd cdp_out = seis->getTraceHeader("CDP", 0, seis->getNTrc());
  Eigen::VectorXd cdpx_out = seis->getTraceHeader("CDP_X", 0, seis->getNTrc());
  Eigen::VectorXd cdpy_out = seis->getTraceHeader("CDP_Y", 0, seis->getNTrc());

  ASSERT_TRUE(cdp_out.isApprox(cdp));
  ASSERT_TRUE(cdpx_out.isApprox(cdpx));
  ASSERT_TRUE(cdpy_out.isApprox(cdpy));

  ASSERT_TRUE(seis->generatePRESTKGeometry(
                src_x0, src_dx, src_nx,
                src_y0, src_dy, src_ny,
                src_z,
                src_x0, src_dx, src_nx,
                src_y0, src_dy, src_ny,
                src_z,
                true));
}

TEST_F(H5SeisFixture, SEGY){
  // MAPPED
  p.mapSEGY = true;
  p.segyFiles = {
    TEST_DATA_DIR"/1.segy",
    TEST_DATA_DIR"/2.segy",
    TEST_DATA_DIR"/3.segy",
    TEST_DATA_DIR"/4.segy"};

  H5Seis_ptr seis(seisContainer->createSeis(
                    SEIS_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(seis != nullptr) << "CREATE_OR_OVERWRITE";

  std::vector<std::string> txtHdr = seis->getTextHeader();

  ASSERT_TRUE(txtHdr[0].find("Don't worry about") != std::string::npos);
  ASSERT_TRUE(txtHdr[1].find("spelling") != std::string::npos);
  ASSERT_TRUE(txtHdr[5].find("dont touch this data") != std::string::npos);
  ASSERT_EQ(seis->getNSamp(), 500);
  ASSERT_EQ(seis->getNTrc(), 96);
  ASSERT_EQ(seis->getBinHeader("JOB"), 1);
  ASSERT_EQ(seis->getBinHeader("SAMP_RATE"), 2000);
  Eigen::VectorXd grpx = seis->getTraceHeader("GRPX", 0, seis->getNTrc());
  ASSERT_TRUE(grpx(1) == 50000);
  Eigen::VectorXf trace = seis->getTrace(0);


  // NOT MAPPED (read with h5geo::functions)
  p.mapSEGY = false;
  H5Seis_ptr seis2(seisContainer->createSeis(
                     SEIS_NAME2, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(seis2 != nullptr) << "CREATE_OR_OVERWRITE";

  auto endian = h5geo::getSEGYEndian(TEST_DATA_DIR"/1.segy");
  auto nSamp = h5geo::getSEGYNSamp(TEST_DATA_DIR"/1.segy", endian);
  auto nTrc = h5geo::getSEGYNTrc(TEST_DATA_DIR"/1.segy", 0, endian);
  auto format = h5geo::getSEGYFormat(TEST_DATA_DIR"/1.segy", endian);
  ASSERT_TRUE(h5geo::readSEGYTraces(
                seis2.get(),
                TEST_DATA_DIR"/1.segy",
                nSamp, nTrc, 0, 10000, format, endian)); // for testing purpose I need to set value not less than 24 trc (10000) as OMP may save traces in different order
  Eigen::VectorXf trace2 = seis2->getTrace(0);
  ASSERT_TRUE(trace.isApprox(trace2));


  // NOT MAPPED (read with H5Seis::methods)
  H5Seis_ptr seis3(seisContainer->createSeis(
                     SEIS_NAME3, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(seis3 != nullptr) << "CREATE_OR_OVERWRITE";
  ASSERT_TRUE(seis3->readSEGYTextHeader(p.segyFiles[0]));
  ASSERT_TRUE(seis3->readSEGYBinHeader(p.segyFiles[0]));
  ASSERT_TRUE(seis3->readSEGYTraces(p.segyFiles));
  Eigen::VectorXf trace3 = seis3->getTrace(0);
  ASSERT_TRUE(trace.isApprox(trace3));
}
