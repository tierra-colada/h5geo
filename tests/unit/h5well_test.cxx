#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <h5well/h5wellcontainer.h>

#include <fstream>

class H5WellFixture: public ::testing::Test {
public:

  virtual void SetUp() override{
    // code here will execute just before the test ensues
    static bool trig = false;

    if (trig){
      wellContainer = H5WellContainer::create(
            FILE_NAME,
            h5gt::File::OpenOrCreate);
    } else {
      wellContainer = H5WellContainer::create(
            FILE_NAME,
            h5gt::File::OpenOrCreate |
            h5gt::File::Overwrite);
    }

    if (h5geo::isFileExist("../data/well.dev")){
      MD_X_Y_Z_TVD_DX_DY_AZ_INCL =
          readWellFile("../data/well.dev", {305, 9}, 11);

      LOG_MD_GR =
          readWellFile("../data/well.las", {20901, 2}, 72);

    } else if (h5geo::isFileExist("data/well.dev")){
      MD_X_Y_Z_TVD_DX_DY_AZ_INCL =
          readWellFile("data/well.dev", {305, 9}, 11);

      LOG_MD_GR =
          readWellFile("data/well.las", {20901, 2}, 72);
    }

    wellParam.headX = 0;
    wellParam.headY = 10;
    wellParam.kb = 2;
    wellParam.uwi = "my_uwi";

    trig = true;
  }

  virtual void TearDown() override{
    // code here will be called just after the test completes
    // ok to through exceptions from here if need be
    auto h5File = wellContainer->getH5File();
    h5geo::unlinkContent(h5File);
  }

  Eigen::MatrixXd readWellFile(
      const std::string& file,
      const std::vector<size_t>& dims,
      size_t n_rows2skip){
    Eigen::MatrixXd X = Eigen::MatrixXd::Zero(dims[0], dims[1]);
    std::ifstream fin;
    fin.open(file);

    if (fin.is_open()){
      for (size_t i = 0; i < n_rows2skip; i++)
        fin.ignore(1000, '\n');

      for (size_t row = 0; row < dims[0]; row++)
        for (size_t col = 0; col < dims[1]; col++)
        {
          double item = 0.0;
          fin >> item;
          X(row, col) = item;
        }
      fin.close();
    }
    return X;
  }

public:
  std::optional<H5WellContainer> wellContainer;
  WellParam wellParam;
  std::string FILE_NAME ="well.h5";
  std::string WELL_NAME ="path/to/well";
  std::string WELL_NAME2 ="path/to/wel2l";

  DevCurveParam devCurveParam;
  std::string DEV_NAME = "my_deviation";

  LogCurveParam logCurveParam;
  std::string LOG_TYPE = "GK";
  std::string LOG_NAME = "my_log";

  Eigen::MatrixXd MD_X_Y_Z_TVD_DX_DY_AZ_INCL;
  Eigen::MatrixXd LOG_MD_GR;

  double x0 = 444363;
  double y0 = 7425880;
  double kb = 50.88;
};

TEST_F(H5WellFixture, createContainer){
  ASSERT_TRUE(h5geo::isFileExist(FILE_NAME));
}

TEST_F(H5WellFixture, createWellWithDifferentCreateFlags){
  std::optional<H5Well> well =
      wellContainer->createWell(
        WELL_NAME, wellParam, h5geo::CreationType::OPEN_OR_CREATE);
  ASSERT_TRUE(well.has_value());
  well =
      wellContainer->createWell(
        WELL_NAME, wellParam, h5geo::CreationType::CREATE_OR_OVERWRITE);
  ASSERT_TRUE(well.has_value());
  well =
      wellContainer->createWell(
        WELL_NAME, wellParam, h5geo::CreationType::CREATE_UNDER_NEW_NAME);
  ASSERT_TRUE(well.has_value());
  well =
      wellContainer->createWell(
        WELL_NAME, wellParam, h5geo::CreationType::OPEN_OR_CREATE);
  ASSERT_TRUE(well.has_value());
}

TEST_F(H5WellFixture, createDevCurveWithDifferentCreateFlags){
  std::optional<H5Well> well =
      wellContainer->createWell(
        WELL_NAME, wellParam, h5geo::CreationType::CREATE_OR_OVERWRITE);
  ASSERT_TRUE(well.has_value());

  std::optional<H5DevCurve> devCurve =
      well->createDevCurve(DEV_NAME, devCurveParam, h5geo::CreationType::OPEN_OR_CREATE);
  ASSERT_TRUE(devCurve.has_value());
  devCurve =
      well->createDevCurve(DEV_NAME, devCurveParam, h5geo::CreationType::CREATE_OR_OVERWRITE);
  ASSERT_TRUE(devCurve.has_value());
  devCurve =
      well->createDevCurve(DEV_NAME, devCurveParam, h5geo::CreationType::CREATE_UNDER_NEW_NAME);
  ASSERT_TRUE(devCurve.has_value());
  devCurve =
      well->createDevCurve(DEV_NAME, devCurveParam, h5geo::CreationType::OPEN_OR_CREATE);
  ASSERT_TRUE(devCurve.has_value());
}

TEST_F(H5WellFixture, createLogCurveWithDifferentCreateFlags){
  std::optional<H5Well> well =
      wellContainer->createWell(
        WELL_NAME, wellParam, h5geo::CreationType::CREATE_OR_OVERWRITE);
  ASSERT_TRUE(well.has_value());

  std::optional<H5LogCurve> logCurve =
      well->createLogCurve(LOG_TYPE, LOG_NAME, logCurveParam, h5geo::CreationType::OPEN_OR_CREATE);
  ASSERT_TRUE(logCurve.has_value());
  logCurve =
      well->createLogCurve(LOG_TYPE, LOG_NAME, logCurveParam, h5geo::CreationType::CREATE_OR_OVERWRITE);
  ASSERT_TRUE(logCurve.has_value());
  logCurve =
      well->createLogCurve(LOG_TYPE, LOG_NAME, logCurveParam, h5geo::CreationType::CREATE_UNDER_NEW_NAME);
  ASSERT_TRUE(logCurve.has_value());
  logCurve =
      well->createLogCurve(LOG_TYPE, LOG_NAME, logCurveParam, h5geo::CreationType::OPEN_OR_CREATE);
  ASSERT_TRUE(logCurve.has_value());
}

TEST_F(H5WellFixture, writeReadDevCurve){
  std::optional<H5Well> well =
      wellContainer->createWell(
        WELL_NAME, wellParam, h5geo::CreationType::CREATE_OR_OVERWRITE);
  ASSERT_TRUE(well.has_value());

  std::optional<H5DevCurve> devCurve =
      well->createDevCurve(DEV_NAME, devCurveParam, h5geo::CreationType::OPEN_OR_CREATE);

  //  Eigen::MatrixXd MD_X_Y_Z_TVD_DX_DY_AZ_INCL = readDeviation();

  ASSERT_TRUE(devCurve->writeCurve(h5geo::DevDataType::MD,
                                   MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(0)));
  ASSERT_TRUE(devCurve->writeCurve(h5geo::DevDataType::X,
                                   MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(1)));
  ASSERT_TRUE(devCurve->writeCurve(h5geo::DevDataType::Y,
                                   MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(2)));
  ASSERT_TRUE(devCurve->writeCurve(h5geo::DevDataType::TVD,
                                   MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(4)));

  ASSERT_TRUE(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(0).isApprox(
                devCurve->getCurve(h5geo::DevDataType::MD)));
  ASSERT_TRUE(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(1).isApprox(
                devCurve->getCurve(h5geo::DevDataType::X)));
  ASSERT_TRUE(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(2).isApprox(
                devCurve->getCurve(h5geo::DevDataType::Y)));
  ASSERT_TRUE(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(4).isApprox(
                devCurve->getCurve(h5geo::DevDataType::TVD)));
}

TEST_F(H5WellFixture, MdAzIncl2MdXYTvd){
  wellContainer->getH5File().createDataSet<double>(
        "MD_X_Y_Z_TVD_DX_DY_AZ_INCL", h5gt::DataSpace(
          {size_t(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.cols()), size_t(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.rows())})).
      write_raw(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.data());

  Eigen::MatrixXd M = h5geo::MdAzIncl2MdXYTvd(
        MD_X_Y_Z_TVD_DX_DY_AZ_INCL(
          Eigen::all, {0, 7, 8}), x0, y0,
        h5geo::AngleUnits::DEGREE);

  wellContainer->getH5File().createDataSet<double>(
        "MdAzIncl2MdXYTvd", h5gt::DataSpace({size_t(M.cols()), size_t(M.rows())})).
      write_raw(M.data());

  double std_dev = (MD_X_Y_Z_TVD_DX_DY_AZ_INCL(
                      Eigen::all, {1, 2, 4}) - M(Eigen::all, {1, 2, 3})).norm();
  double MD_max = MD_X_Y_Z_TVD_DX_DY_AZ_INCL(Eigen::last, 0);
  ASSERT_TRUE(std_dev/MD_max < 0.05); // less than 5 %
}

TEST_F(H5WellFixture, TvdXY2MdXYTvd){
  Eigen::MatrixXd M = h5geo::TvdXY2MdXYTvd(
        MD_X_Y_Z_TVD_DX_DY_AZ_INCL(
          Eigen::all, {4, 1, 2}), x0, y0);

  wellContainer->getH5File().createDataSet<double>(
        "TvdXY2MdXYTvd", h5gt::DataSpace({size_t(M.cols()), size_t(M.rows())})).
      write_raw(M.data());

  double std_dev = (MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(0) - M.col(0)).norm();
  double MD_max = MD_X_Y_Z_TVD_DX_DY_AZ_INCL(Eigen::last, 0);
  ASSERT_TRUE(std_dev/MD_max < 0.05); // less than 5 %
}

TEST_F(H5WellFixture, TvdDxDy2MdXYTvd){
  Eigen::MatrixXd M = h5geo::TvdDxDy2MdXYTvd(
        MD_X_Y_Z_TVD_DX_DY_AZ_INCL(
          Eigen::all, {4, 5, 6}), x0, y0);

  wellContainer->getH5File().createDataSet<double>(
        "TvdDxDy2MdXYTvd", h5gt::DataSpace({size_t(M.cols()), size_t(M.rows())})).
      write_raw(M.data());

  double std_dev = (MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(0) - M.col(0)).norm();
  double MD_max = MD_X_Y_Z_TVD_DX_DY_AZ_INCL(Eigen::last, 0);
  ASSERT_TRUE(std_dev/MD_max < 0.05); // less than 5 %
}

TEST_F(H5WellFixture, TvdssXY2MdXYTvd){
  Eigen::MatrixXd TvdssXY(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.rows(), 3);
  TvdssXY.col(0) = MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(4).array() + kb;
  TvdssXY.col(1) = MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(1);
  TvdssXY.col(2) = MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(2);

  Eigen::MatrixXd M = h5geo::TvdssXY2MdXYTvd(
        TvdssXY, x0, y0, kb);

  wellContainer->getH5File().createDataSet<double>(
        "TvdssXY2MdXYTvd", h5gt::DataSpace({size_t(M.cols()), size_t(M.rows())})).
      write_raw(M.data());

  double std_dev = (MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(0) - M.col(0)).norm();
  double MD_max = MD_X_Y_Z_TVD_DX_DY_AZ_INCL(Eigen::last, 0);
  ASSERT_TRUE(std_dev/MD_max < 0.05); // less than 5 %
}

TEST_F(H5WellFixture, TvdssDxDy2MdXYTvd){
  Eigen::MatrixXd TvdssDxDy(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.rows(), 3);
  TvdssDxDy.col(0) = MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(4).array() + kb;
  TvdssDxDy.col(1) = MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(5);
  TvdssDxDy.col(2) = MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(6);

  Eigen::MatrixXd M = h5geo::TvdssDxDy2MdXYTvd(
        TvdssDxDy, x0, y0, kb);

  wellContainer->getH5File().createDataSet<double>(
        "TvdssDxDy2MdXYTvd", h5gt::DataSpace({size_t(M.cols()), size_t(M.rows())})).
      write_raw(M.data());

  double std_dev = (MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(0) - M.col(0)).norm();
  double MD_max = MD_X_Y_Z_TVD_DX_DY_AZ_INCL(Eigen::last, 0);
  ASSERT_TRUE(std_dev/MD_max < 0.05); // less than 5 %
}

TEST_F(H5WellFixture, writeReadLogCurve){
  std::optional<H5Well> well =
      wellContainer->createWell(
        WELL_NAME, wellParam, h5geo::CreationType::CREATE_OR_OVERWRITE);
  ASSERT_TRUE(well.has_value());

  std::optional<H5LogCurve> logCurve =
      well->createLogCurve(
        LOG_TYPE, LOG_NAME,
        logCurveParam, h5geo::CreationType::OPEN_OR_CREATE);

  ASSERT_TRUE(logCurve.has_value());

  ASSERT_TRUE(logCurve->writeCurve(h5geo::LogDataType::MD,
                                   LOG_MD_GR.col(0)));
  ASSERT_TRUE(logCurve->writeCurve(h5geo::LogDataType::VAL,
                                   LOG_MD_GR.col(1)));

  ASSERT_TRUE(LOG_MD_GR.col(0).isApprox(
                logCurve->getCurve(h5geo::LogDataType::MD)));
  ASSERT_TRUE(LOG_MD_GR.col(1).isApprox(
                logCurve->getCurve(h5geo::LogDataType::VAL)));
}

TEST_F(H5WellFixture, getWellFromCurve){
  std::optional<H5Well> well =
      wellContainer->createWell(
        WELL_NAME, wellParam, h5geo::CreationType::CREATE_OR_OVERWRITE);
  ASSERT_TRUE(well.has_value());

  std::optional<H5LogCurve> logCurve =
      well->createLogCurve(
        LOG_TYPE, LOG_NAME,
        logCurveParam, h5geo::CreationType::OPEN_OR_CREATE);

  std::optional<H5DevCurve> devCurve =
      well->createDevCurve(DEV_NAME, devCurveParam, h5geo::CreationType::OPEN_OR_CREATE);


  ASSERT_TRUE(devCurve.has_value());

  ASSERT_TRUE(devCurve->getWell())
      << "Try to find well containing this curve";
  ASSERT_TRUE(logCurve->getWell())
      << "Try to find well containing this curve";

  ASSERT_TRUE(devCurve->getWellContainer())
      << "Get wellcontainer containing this curve";
  ASSERT_TRUE(logCurve->getWellContainer())
      << "Get wellcontainer containing this curve";
}

TEST_F(H5WellFixture, activeDevCurveTest){
  std::optional<H5Well> well =
      wellContainer->createWell(
        WELL_NAME, wellParam, h5geo::CreationType::CREATE_OR_OVERWRITE);
  ASSERT_TRUE(well.has_value());

  std::optional<H5DevCurve> devCurve =
      well->createDevCurve(DEV_NAME, devCurveParam, h5geo::CreationType::OPEN_OR_CREATE);

  ASSERT_TRUE(devCurve->writeCurve(h5geo::DevDataType::MD,
                                   MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(0)));
  ASSERT_TRUE(devCurve->writeCurve(h5geo::DevDataType::X,
                                   MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(1)));
  ASSERT_TRUE(devCurve->writeCurve(h5geo::DevDataType::Y,
                                   MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(2)));
  ASSERT_TRUE(devCurve->writeCurve(h5geo::DevDataType::TVD,
                                   MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(4)));

  ASSERT_FALSE(devCurve->isActive());
  ASSERT_TRUE(devCurve->setActive());
  ASSERT_TRUE(devCurve->isActive());

  std::optional<H5DevCurve> activeCurve = well->getActiveDevCurve();
  ASSERT_TRUE(activeCurve.has_value());

  ASSERT_TRUE(devCurve->getCurve(h5geo::DevDataType::MD).isApprox(
                activeCurve->getCurve(h5geo::DevDataType::MD)));
  ASSERT_TRUE(devCurve->getCurve(h5geo::DevDataType::X).isApprox(
                activeCurve->getCurve(h5geo::DevDataType::X)));
  ASSERT_TRUE(devCurve->getCurve(h5geo::DevDataType::Y).isApprox(
                activeCurve->getCurve(h5geo::DevDataType::Y)));
  ASSERT_TRUE(devCurve->getCurve(h5geo::DevDataType::TVD).isApprox(
                activeCurve->getCurve(h5geo::DevDataType::TVD)));
}

TEST_F(H5WellFixture, getCurveFromDifferentWell){
  std::string wellName1 = "1R";
  std::string wellName2 = "2R";
  std::string devCurveName1 = "/1R/DEV/my_dev";
  std::string logCurveType1 = "/1R/LOG/my_log_type";
  std::string logCurveName1 = "/1R/LOG/my_log_type/my_log_name";
  std::string devCurveName2 = "/2R/DEV/my_dev";
  std::string logCurveType2 = "/2R/LOG/my_log_type";
  std::string logCurveName2 = "/2R/LOG/my_log_type/my_log_name";

  std::optional<H5Well> well_1 =
      wellContainer->createWell(
        wellName1, wellParam, h5geo::CreationType::CREATE_OR_OVERWRITE);

  std::optional<H5Well> well_2 =
      wellContainer->createWell(
        wellName2, wellParam, h5geo::CreationType::CREATE_OR_OVERWRITE);

  ASSERT_FALSE(well_1->createDevCurve(devCurveName2, devCurveParam, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_FALSE(well_1->createLogCurve(logCurveType2, logCurveName2, logCurveParam, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_FALSE(well_2->createDevCurve(devCurveName1, devCurveParam, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_FALSE(well_2->createLogCurve(logCurveType1, logCurveName1, logCurveParam, h5geo::CreationType::OPEN_OR_CREATE));

  ASSERT_TRUE(well_1->createDevCurve(devCurveName1, devCurveParam, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_TRUE(well_1->createLogCurve(logCurveType1, logCurveName1, logCurveParam, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_TRUE(well_2->createDevCurve(devCurveName2, devCurveParam, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_TRUE(well_2->createLogCurve(logCurveType2, logCurveName2, logCurveParam, h5geo::CreationType::OPEN_OR_CREATE));

  ASSERT_FALSE(well_1->getDevCurve(devCurveName2).has_value());
  ASSERT_FALSE(well_1->getLogCurve(logCurveType2, logCurveName2).has_value());
  ASSERT_FALSE(well_2->getDevCurve(devCurveName1).has_value());
  ASSERT_FALSE(well_2->getLogCurve(logCurveType1, logCurveName1).has_value());

  ASSERT_TRUE(well_1->getDevCurve(devCurveName1).has_value());
  ASSERT_TRUE(well_1->getLogCurve(logCurveType1, logCurveName1).has_value());
  ASSERT_TRUE(well_2->getDevCurve(devCurveName2).has_value());
  ASSERT_TRUE(well_2->getLogCurve(logCurveType2, logCurveName2).has_value());
}
