#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <h5geo/h5seiscontainer.h>
#include <h5geo/h5seis.h>
#include <h5geo/h5horizon.h>
#include <h5geo/h5core.h>

#include <h5gt/H5File.hpp>
#include <h5gt/H5Group.hpp>
#include <h5gt/H5DataSet.hpp>

#include <cstring>
#include <filesystem>
namespace fs = std::filesystem;

class H5SeisFixture: public ::testing::Test {
public:

  virtual void SetUp() override{
    // code here will execute just before the test ensues
    static bool trig = false;

    FILE_NAME = "seis.h5";
    FILE_NAME_BIG = "seis_big.h5";
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

    // always open open or create big file (it allows to avoid rereading)
    h5gt::File fileBig(FILE_NAME_BIG, h5gt::File::OpenOrCreate);
    seisContainerBig = H5SeisCnt_ptr(
          h5geo::createSeisContainer(
            fileBig, h5geo::CreationType::OPEN_OR_CREATE));

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
//    auto h5FileBig = seisContainerBig->getH5File();
//    h5geo::unlinkContent(h5FileBig);
//  }

public:
  H5SeisCnt_ptr seisContainer, seisContainerBig;
  H5SeisParam p;
  std::string FILE_NAME, FILE_NAME_BIG, SEIS_NAME1, SEIS_NAME2, SEIS_NAME3;
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
    binHdrStd.push_back((double)i);

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
  std::cout << "seis->getPKeyValues(\"FFID\"):\n" << seis->getPKeyValues("FFID") << std::endl;
  std::cout << "seis->getPKeySize(\"FFID\", 2,4,1):\t" << seis->getPKeySize("FFID", 2,4,1) << std::endl;
  std::cout << "seis->getPKeySize(\"FFID\", 2,4,2):\t" << seis->getPKeySize("FFID", 2,4,2) << std::endl;
  std::cout << "seis->getPKeySize(\"FFID\", 2,2,2):\t" << seis->getPKeySize("FFID", 2,2,2) << std::endl;

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

TEST_F(H5SeisFixture, boundary){
  H5Seis_ptr seis(seisContainer->createSeis(
                    SEIS_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(seis != nullptr);

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

  Eigen::MatrixXd boundary = seis->calcBoundary();
  ASSERT_TRUE(boundary.size() > 0);

  std::string horizonName = "boundary";
  H5HorizonParam p_hrz;
  p_hrz.components["X"] = 0;
  p_hrz.components["Y"] = 1;
  p_hrz.nPoints = 10;
  p_hrz.pointsChunkSize = 10;
  p_hrz.domain = h5geo::Domain::TWT;
  p_hrz.lengthUnits = p.lengthUnits;
  p_hrz.spatialReference = p.spatialReference;

  H5Horizon_ptr hrz(
        seis->createHorizon(
          horizonName, p_hrz, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(hrz != nullptr);

  ASSERT_TRUE(hrz->writeComponent("X", boundary.col(0)));
  ASSERT_TRUE(hrz->writeComponent("Y", boundary.col(1)));

  Eigen::VectorXd X = hrz->getComponent("X");
  Eigen::VectorXd Y = hrz->getComponent("Y");

  ASSERT_TRUE(boundary.col(0).isApprox(X));
  ASSERT_TRUE(boundary.col(1).isApprox(Y));
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
                src_z, 0));

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
                src_z, 0,
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

  // is necessary to correctly set SAMP_RATE to segy file when writing it
  seis->setTemporalUnits("microsecond");
  std::vector<std::string> txtHdr = seis->getTextHeader();

  ASSERT_TRUE(txtHdr[0].find("Don't worry about") != std::string::npos);
  ASSERT_TRUE(txtHdr[1].find("spelling") != std::string::npos);
  ASSERT_TRUE(txtHdr[5].find("dont touch this data") != std::string::npos);
  ASSERT_EQ(seis->getNSamp(), 500);
  ASSERT_EQ(seis->getNTrc(), 96);
  ASSERT_EQ(seis->getBinHeader("JOB"), 1);
  ASSERT_EQ(seis->getBinHeader("SAMP_RATE"), 2000);
  Eigen::VectorXd grpx = seis->getTraceHeader("GRPX", 0, seis->getNTrc());
  Eigen::VectorXd saed = seis->getTraceHeader("SAED", 0, seis->getNTrc());
  size_t trcInd = 10;
  Eigen::VectorXf trace = seis->getTrace(trcInd);

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
                false, nSamp, nTrc, format, endian)); // for testing purpose I need to set value not less than 24 trc (10000) as OMP may save traces in different order
  Eigen::VectorXf trace2 = seis2->getTrace(trcInd);
  ASSERT_TRUE(trace.isApprox(trace2));
  ASSERT_TRUE(h5geo::readSEGYTracesMMap(
                seis2.get(),
                TEST_DATA_DIR"/1.segy",
                false, nSamp, nTrc, format, endian)); // for testing purpose I need to set value not less than 24 trc (10000) as OMP may save traces in different order
  Eigen::VectorXf trace22 = seis2->getTrace(trcInd);
  ASSERT_TRUE(trace.isApprox(trace22));

  // NOT MAPPED (read with H5Seis::methods)
  H5Seis_ptr seis3(seisContainer->createSeis(
                     SEIS_NAME3, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(seis3 != nullptr) << "CREATE_OR_OVERWRITE";
  ASSERT_TRUE(seis3->readSEGYTextHeader(p.segyFiles[0]));
  ASSERT_TRUE(seis3->readSEGYBinHeader(p.segyFiles[0]));
  ASSERT_TRUE(seis3->readSEGYTraces(p.segyFiles));
  Eigen::VectorXf trace3 = seis3->getTrace(trcInd);
  ASSERT_TRUE(trace.isApprox(trace3));

  // READ USING SEGY FUNCTIONS
  auto grpx4 = h5geo::readSEGYTraceHeader(TEST_DATA_DIR"/1.segy", 80, 4);
  auto saed4 = h5geo::readSEGYTraceHeader(TEST_DATA_DIR"/1.segy", 68, 2);

  ASSERT_TRUE(grpx(Eigen::seq(0,grpx4.size()-1)).isApprox(grpx4.cast<double>()));
  ASSERT_TRUE(saed(Eigen::seq(0,saed4.size()-1)).isApprox(saed4.cast<double>()));

  auto trace4 = h5geo::readSEGYTraces(TEST_DATA_DIR"/1.segy",0,nSamp,trcInd,trcInd);

  ASSERT_TRUE(trace.isApprox(trace4));

  // CHECK TRACE READ THROUGH LOW LEVEL API
  std::ifstream file(TEST_DATA_DIR"/1.segy", std::ifstream::binary | std::ifstream::in);
  ASSERT_TRUE(file.is_open());

  Eigen::VectorXf trace5(nSamp);
  h5geo::readSEGYTrace(file, trcInd, format, endian, trace5);
  ASSERT_TRUE(trace.isApprox(trace5));

  // TEST WRITE
  std::string segy_out = "out.sgy";
  char txtHdr_out[40][80] = { " " };
  for (size_t i = 0; i < txtHdr.size(); i++)
    std::strncpy(std::begin(txtHdr_out[i]), txtHdr[i].c_str(), std::min<size_t>(80,txtHdr[i].size()));

  double binHdr[30] = { 0 };
  ASSERT_TRUE(h5geo::readSEGYBinHeader(p.segyFiles[0], binHdr));

  double binHdr_out[30] = { 0 };
  std::copy(std::begin(binHdr), std::end(binHdr), binHdr_out);
  
  ASSERT_TRUE(h5geo::writeSEGYTextHeader(segy_out, txtHdr_out, true));
  ASSERT_TRUE(h5geo::writeSEGYBinHeader(segy_out, binHdr_out, false));

  Eigen::MatrixXd HDR = seis->getTraceHeader(0,nTrc).transpose();
  Eigen::MatrixXf TRACE = seis->getTrace(0,nTrc);
  ASSERT_TRUE(h5geo::writeSEGYTraces(segy_out, HDR, TRACE));

  std::string segy_out1 = "out1.sgy";
  seis->exportToSEGY(segy_out1, 5, [](double progress) { std::cout << "Progress:\t" << progress << std::endl; });
}

#include <chrono>

// prefix `DISABLED_` is to skip test
TEST_F(H5SeisFixture, DISABLED_SEGY_BIG){
  std::string segy_big = "C:/Users/kerim/dev/DATA/CDP_FOR_REGLO_IL6-250.sgy";

  // MAPPED
  p.mapSEGY = true;
  p.segyFiles = {segy_big};
  // important to create seis with NSamp match exactly to SEGY NSamp (affect of chunking)
  p.nSamp = h5geo::getSEGYNSamp(segy_big);

  H5Seis_ptr seis(seisContainerBig->createSeis(
                    SEIS_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(seis != nullptr) << "CREATE_OR_OVERWRITE";

  std::vector<std::string> txtHdr = seis->getTextHeader();


  // NOT MAPPED (read with h5geo::functions)
  p.mapSEGY = false;
  p.trcChunk = 10000;
  H5Seis_ptr seis2(seisContainerBig->createSeis(
                     SEIS_NAME2, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(seis2 != nullptr) << "CREATE_OR_OVERWRITE";

  seis2->readSEGYTextHeader(segy_big);
  seis2->readSEGYBinHeader(segy_big);

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  seis2->readSEGYTracesMMap(
        {segy_big},
        std::vector<h5geo::SegyFormat>(),
        std::vector<h5geo::Endian>(),
        std::vector<std::vector<std::string>>(),
        10000, -1, [](double progress) { std::cout << "Progress:\t" << progress << std::endl; });
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << "BIG SEGY read time = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[seconds]" << std::endl;

  begin = std::chrono::steady_clock::now();
  seis2->addPKeySort("INLINE");
  end = std::chrono::steady_clock::now();
  std::cout << "BIG SEGY sort INLINE = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[milliseconds]" << std::endl;

  begin = std::chrono::steady_clock::now();
  seis2->addPKeySort("XLINE");
  end = std::chrono::steady_clock::now();
  std::cout << "BIG SEGY sort XLINE = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[milliseconds]" << std::endl;

  begin = std::chrono::steady_clock::now();
  seis2->addPKeySort("CDP");
  end = std::chrono::steady_clock::now();
  std::cout << "BIG SEGY sort CDP = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[milliseconds]" << std::endl;

  Eigen::MatrixXf trc_sorted;
  Eigen::MatrixXd hdr_sorted;
  begin = std::chrono::steady_clock::now();
  seis2->getSortedData(trc_sorted, hdr_sorted,
                       {"XLINE", "INLINE"},
                       {125, -std::numeric_limits<double>::infinity()},
                       {125, std::numeric_limits<double>::infinity()});
  end = std::chrono::steady_clock::now();
  std::cout << "BIG SEGY getSortedData {XLINE, INLINE} = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[microseconds]" << std::endl;
  std::cout << "BIG SEGY getSortedData trc_sorted.rows() = " << trc_sorted.rows() << std::endl;
  std::cout << "BIG SEGY getSortedData trc_sorted.cols() = " << trc_sorted.cols() << std::endl;
  std::cout << "BIG SEGY getSortedData hdr_sorted.rows() = " << hdr_sorted.rows() << std::endl;
  std::cout << "BIG SEGY getSortedData hdr_sorted.cols() = " << hdr_sorted.cols() << std::endl;
}

TEST_F(H5SeisFixture, DISABLED_READ_ARBITRARY_TRACES){
  std::string fileName = FILE_NAME_BIG; // path to h5 container
  std::string seisName = SEIS_NAME2;    // path to seis within h5 container
  H5Seis_ptr seis(h5geo::openSeisByName(fileName, seisName));
  ASSERT_TRUE(seis != nullptr) << "Unable to open seis";

  std::cout << "number of traces in seis: " << seis->getNTrc() << std::endl;
  std::cout << "number of samples in seis: " << seis->getNSamp() << std::endl;

  //==============================
  // PART 1: READ ARBITRARY TRACES
  //==============================
  // N - is the number of traces to be read
  auto get_traces_and_show_elapsed_time = [](H5Seis* seis, size_t nTrc){
    ASSERT_TRUE(seis != nullptr) << "seis object is NULL";

    // set limits for random numbers generation
    size_t LO = 0;
    size_t HI = seis->getNTrc()-1;
    size_t range = HI-LO;

    // generate random numbers in range [0, numbert of traces in file - 1]
    Eigen::VectorX<float> trcIndF = Eigen::VectorX<float>::Random(nTrc); // Vector filled with random numbers between (-1,1)
    trcIndF = (trcIndF + Eigen::VectorX<float>::Constant(nTrc, 1.0))*range/2.0; // add 1 to the matrix to have values between 0 and 2; multiply with range/2
    trcIndF = (trcIndF + Eigen::VectorX<float>::Constant(nTrc, float(LO))); //set LO as the lower bound (offset)
    Eigen::VectorX<size_t> trcInd = trcIndF.cast<size_t>();

    // measure time to read N traces
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    Eigen::MatrixX<float> TRACE = seis->getTrace(trcInd, 1000, 1);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "get " + std::to_string(nTrc) + " arbitrary traces, elapsed time: " <<
                 std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[milliseconds]" << std::endl;

    ASSERT_TRUE(TRACE.size() > 0) << "unable to read traces";
  };

  // call function where second arg is number of traces to be read
  std::cout << "PART 1: reading arbitrary traces" << std::endl;
  get_traces_and_show_elapsed_time(seis.get(), 100);
  get_traces_and_show_elapsed_time(seis.get(), 1000);
  get_traces_and_show_elapsed_time(seis.get(), 10000);
  get_traces_and_show_elapsed_time(seis.get(), 100000);
  get_traces_and_show_elapsed_time(seis.get(), 1000000);


  //=======================
  // PART 2: READ TIMESLICE
  //=======================

  // read timeslice
  auto get_time_slice_and_show_elapsed_time = [](
      H5Seis* seis, size_t fromTrc, size_t nTrc, size_t fromSamp, size_t nSamp)
  {
    ASSERT_TRUE(seis != nullptr) << "seis object is NULL";

    // measure time to read timeslice
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    Eigen::MatrixX<float> TRACE = seis->getTrace(fromTrc, nTrc, fromSamp, nSamp);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "get " + std::to_string(nTrc) + " traces and " +
                 std::to_string(nSamp) + " samples as timeslice, elapsed time: " <<
                 std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[milliseconds]" << std::endl;

    ASSERT_TRUE(TRACE.size() > 0) << "unable to read timeslice";
  };

  // call function where second arg is number of traces to be read
  std::cout << "PART 2: reading timeslice" << std::endl;
  get_time_slice_and_show_elapsed_time(seis.get(), 0, 100, 0, 1);
  get_time_slice_and_show_elapsed_time(seis.get(), 0, 1000, 0, 1);
  get_time_slice_and_show_elapsed_time(seis.get(), 0, 10000, 0, 1);
  get_time_slice_and_show_elapsed_time(seis.get(), 0, 100000, 0, 1);
  get_time_slice_and_show_elapsed_time(seis.get(), 0, 1000000, 0, 1);
}
