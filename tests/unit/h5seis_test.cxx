#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <h5seis/h5seiscontainer.h>

class H5SeisFixture: public ::testing::Test {
public:

  virtual void SetUp() override{
    // code here will execute just before the test ensues
    static bool trig = false;

    if (trig){
      seisContainer = H5SeisContainer::create(
            FILE_NAME,
            h5gt::File::OpenOrCreate);
    } else {
      seisContainer = H5SeisContainer::create(
            FILE_NAME,
            h5gt::File::OpenOrCreate |
            h5gt::File::Overwrite);
    }

    p.domain = h5geo::Domain::OWT;
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
    //    h5geo::unlinkContent(seisContainer->h5File);
  }

public:
  std::optional<H5SeisContainer> seisContainer;
  SeisParam p;
  std::string FILE_NAME = "seis.h5";
  std::string SEIS_NAME1 = "path1/to/seis";
  std::string SEIS_NAME2 = "path2/to/seis";
};

TEST_F(H5SeisFixture, createContainer){
  ASSERT_TRUE(h5geo::isFileExist(FILE_NAME));
}

TEST_F(H5SeisFixture, createSeisWithDifferentCreateFlags){
  std::optional<H5Seis> seis =
      seisContainer->createSeis(
        SEIS_NAME1, p, h5geo::CreationType::OPEN_OR_CREATE);
  bool v = seis.has_value();
  ASSERT_TRUE(seis.has_value()) << "OPEN_OR_CREATE";
  seis =
      seisContainer->createSeis(
        SEIS_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE);
  ASSERT_TRUE(seis.has_value()) << "CREATE_OR_OVERWRITE";
  seis =
      seisContainer->createSeis(
        SEIS_NAME1, p, h5geo::CreationType::CREATE_UNDER_NEW_NAME);
  ASSERT_TRUE(seis.has_value())  << "CREATE_UNDER_NEW_NAME";
  seis =
      seisContainer->createSeis(
        SEIS_NAME1, p, h5geo::CreationType::OPEN_OR_CREATE);
  ASSERT_TRUE(seis.has_value())  << "OPEN_OR_CREATE";
}

TEST_F(H5SeisFixture, createAndGetSeis){
  seisContainer->createSeis(SEIS_NAME1, p);
  std::optional<H5Seis> seis = seisContainer->getSeis(SEIS_NAME1);
  ASSERT_TRUE(seis.has_value());
}

TEST_F(H5SeisFixture, createAndGetSeisFromGroup){
  h5gt::Group group =
      seisContainer->getH5File().createGroup(SEIS_NAME2);
  seisContainer->createSeis(group, p);
  std::optional<H5Seis> seis = seisContainer->getSeis(SEIS_NAME2);
  ASSERT_TRUE(seis.has_value());
}

TEST_F(H5SeisFixture, writeAndGetTextHeader){
  std::optional<H5Seis> seis =
      seisContainer->createSeis(
        SEIS_NAME1, p, h5geo::CreationType::OPEN_OR_CREATE);
  ASSERT_TRUE(seis.has_value()) << "OPEN_OR_CREATE";

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
  std::optional<H5Seis> seis =
      seisContainer->createSeis(
        SEIS_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE);
  ASSERT_TRUE(seis.has_value()) << "OPEN_OR_CREATE";

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
  std::optional<H5Seis> seis =
      seisContainer->createSeis(
        SEIS_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE);
  ASSERT_TRUE(seis.has_value()) << "OPEN_OR_CREATE";

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

TEST_F(H5SeisFixture, writeAndGetTraceHeader){
  std::optional<H5Seis> seis =
      seisContainer->createSeis(
        SEIS_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE);
  ASSERT_TRUE(seis.has_value()) << "OPEN_OR_CREATE";

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
  std::optional<H5Seis> seis =
      seisContainer->createSeis(
        SEIS_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE);
  ASSERT_TRUE(seis.has_value()) << "OPEN_OR_CREATE";

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
