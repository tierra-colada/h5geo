#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <h5geo/h5wellcontainer.h>
#include <h5geo/h5well.h>
#include <h5geo/h5core.h>
#include <h5geo/h5devcurve.h>
#include <h5geo/h5logcurve.h>
#include <h5geo/h5points.h>

#include <h5gt/H5File.hpp>
#include <h5gt/H5Group.hpp>
#include <h5gt/H5DataSet.hpp>

#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

class H5WellFixture: public ::testing::Test {
public:

  virtual void SetUp() override{
    // code here will execute just before the test ensues
    static bool trig = false;

    FILE_NAME ="well.h5";
    WELL_NAME ="path/to/well";
    WELL_NAME2 ="path/to/well2";

    DEV_NAME = "my_deviation";

    LOG_TYPE = "GK";
    LOG_NAME = "my_log";

    if (trig){
      h5gt::File file(FILE_NAME, h5gt::File::OpenOrCreate);
      wellContainer = H5WellCnt_ptr(h5geo::createWellContainer(
                                      file, h5geo::CreationType::OPEN_OR_CREATE));
    } else {
      h5gt::File file(FILE_NAME, h5gt::File::OpenOrCreate |
                      h5gt::File::Overwrite);
      wellContainer = H5WellCnt_ptr(h5geo::createWellContainer(
                                      file, h5geo::CreationType::CREATE_OR_OVERWRITE));
    }

    /* TEST_DATA_DIR is a macro defined in CMake and it points to a dir
     * where test data reside */
    MD_X_Y_Z_TVD_DX_DY_AZ_INCL =
        readWellFile(TEST_DATA_DIR"/well_dev", {305, 9}, 11);

    LOG_MD_GR =
        readWellFile(TEST_DATA_DIR"/well_las", {20901, 2}, 72);

    wellParam.headX = 444363;
    wellParam.headY = 7425880;
    wellParam.kb = 50.88;
    wellParam.uwi = "my_uwi";
    wellParam.lengthUnits = "meter";

    devCurveParam.lengthUnits = "meter";
    devCurveParam.temporalUnits = "millisecond";
    devCurveParam.angularUnits = "degree";

    logCurveParam.lengthUnits = "cm";
    logCurveParam.dataUnits = "kg/m2";

    pointsParam.chunkSize = 1;
    pointsParam.domain = h5geo::Domain::TVDSS;
    pointsParam.nPoints = 3;
    pointsParam.lengthUnits = "meter";
    pointsParam.temporalUnits = "ms";

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
  H5WellCnt_ptr wellContainer;
  WellParam wellParam;
  std::string FILE_NAME, WELL_NAME, WELL_NAME2;

  DevCurveParam devCurveParam;
  std::string DEV_NAME;

  LogCurveParam logCurveParam;
  std::string LOG_TYPE, LOG_NAME;

  Eigen::MatrixXd MD_X_Y_Z_TVD_DX_DY_AZ_INCL;
  Eigen::MatrixXd LOG_MD_GR;

  PointsParam pointsParam;
  std::string POINTS_NAME;
};

TEST_F(H5WellFixture, createContainer){
  ASSERT_TRUE(fs::exists(FILE_NAME));
}

TEST_F(H5WellFixture, createWellWithDifferentCreateFlags){
  H5Well_ptr well(
        wellContainer->createWell(
          WELL_NAME, wellParam, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_TRUE(well != nullptr);

  well = H5Well_ptr(
        wellContainer->createWell(
          WELL_NAME, wellParam, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(well != nullptr);

  well = H5Well_ptr(
        wellContainer->createWell(
          WELL_NAME, wellParam, h5geo::CreationType::CREATE_UNDER_NEW_NAME));
  ASSERT_TRUE(well != nullptr);

  well = H5Well_ptr(
        wellContainer->createWell(
          WELL_NAME, wellParam, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_TRUE(well != nullptr);
}

TEST_F(H5WellFixture, equalityOperator){
  H5Well_ptr well1(
        wellContainer->createWell(
          WELL_NAME, wellParam, h5geo::CreationType::OPEN_OR_CREATE));
  H5Well_ptr well2(wellContainer->openWell(WELL_NAME));
  H5Well_ptr well3(
        wellContainer->createWell(
          WELL_NAME2, wellParam, h5geo::CreationType::OPEN_OR_CREATE));
  H5Well_ptr well4(wellContainer->openWell(WELL_NAME2));

  ASSERT_TRUE(*well1 == *well1);
  ASSERT_TRUE(*well1 == *well2);
  ASSERT_TRUE(*well1 != *well3);
  ASSERT_TRUE(*well1 != *well4);
  ASSERT_TRUE(*well3 == *well4);
}

TEST_F(H5WellFixture, openWellByUWI){
  H5Well_ptr well(
        wellContainer->createWell(
          WELL_NAME, wellParam, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_TRUE(well != nullptr);

  H5Well_ptr wellByUwi1(wellContainer->openWellByUWI("noname"));
  ASSERT_TRUE(wellByUwi1 == nullptr);

  H5Well_ptr wellByUwi2(wellContainer->openWellByUWI(wellParam.uwi));
  ASSERT_TRUE(wellByUwi2 != nullptr);
}

TEST_F(H5WellFixture, createDevCurveWithDifferentCreateFlags){
  H5Well_ptr well(
        wellContainer->createWell(
          WELL_NAME, wellParam, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(well != nullptr);

  H5DevCurve_ptr devCurve(
        well->createDevCurve(
          DEV_NAME, devCurveParam, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_TRUE(devCurve != nullptr);

  devCurve = H5DevCurve_ptr(
        well->createDevCurve(
          DEV_NAME, devCurveParam, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(devCurve != nullptr);

  devCurve = H5DevCurve_ptr(
        well->createDevCurve(
          DEV_NAME, devCurveParam, h5geo::CreationType::CREATE_UNDER_NEW_NAME));
  ASSERT_TRUE(devCurve != nullptr);

  devCurve = H5DevCurve_ptr(
        well->createDevCurve(
          DEV_NAME, devCurveParam, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_TRUE(devCurve != nullptr);
}

TEST_F(H5WellFixture, createLogCurveWithDifferentCreateFlags){
  H5Well_ptr well(wellContainer->createWell(
                    WELL_NAME, wellParam, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(well != nullptr);

  H5LogCurve_ptr logCurve(
        well->createLogCurve(
          LOG_TYPE, LOG_NAME, logCurveParam, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_TRUE(logCurve != nullptr);

  logCurve = H5LogCurve_ptr(
        well->createLogCurve(
          LOG_TYPE, LOG_NAME, logCurveParam, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(logCurve != nullptr);

  logCurve = H5LogCurve_ptr(
        well->createLogCurve(
          LOG_TYPE, LOG_NAME, logCurveParam, h5geo::CreationType::CREATE_UNDER_NEW_NAME));
  ASSERT_TRUE(logCurve != nullptr);

  logCurve = H5LogCurve_ptr(
        well->createLogCurve(
          LOG_TYPE, LOG_NAME, logCurveParam, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_TRUE(logCurve != nullptr);
}

TEST_F(H5WellFixture, writeReadDevCurve){
  H5Well_ptr well(
        wellContainer->createWell(
          WELL_NAME, wellParam, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(well != nullptr);

  H5DevCurve_ptr devCurve(
        well->createDevCurve(
          DEV_NAME, devCurveParam, h5geo::CreationType::OPEN_OR_CREATE));

  //  Eigen::MatrixXd MD_X_Y_Z_TVD_DX_DY_AZ_INCL = readDeviation();

  ASSERT_TRUE(devCurve->writeMD(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(0)));
  ASSERT_TRUE(devCurve->writeAZIM(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(7)));
  ASSERT_TRUE(devCurve->writeINCL(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(8)));

  devCurve->updateTvdDxDy();

  auto dx = devCurve->getCurve(h5geo::DevDataType::DX);
  auto dy = devCurve->getCurve(h5geo::DevDataType::DY);
  auto tvd = devCurve->getCurve(h5geo::DevDataType::TVD);
  double dx_norm = (MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(5)-
      devCurve->getCurve(h5geo::DevDataType::DX)).norm();
  double dy_norm = (MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(6)-
      devCurve->getCurve(h5geo::DevDataType::DY)).norm();
  double tvd_norm = (MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(4)-
      devCurve->getCurve(h5geo::DevDataType::TVD)).norm();

  ASSERT_TRUE(dx_norm < MD_X_Y_Z_TVD_DX_DY_AZ_INCL(Eigen::last, 4)*0.005); // less than 0,5 % of max TVD
  ASSERT_TRUE(dy_norm < MD_X_Y_Z_TVD_DX_DY_AZ_INCL(Eigen::last, 4)*0.005); // less than 0,5 % of max TVD
  ASSERT_TRUE(tvd_norm < MD_X_Y_Z_TVD_DX_DY_AZ_INCL(Eigen::last, 4)*0.005); // less than 0,5 % of max TVD
}

TEST_F(H5WellFixture, MdAzIncl2MdXYTvd){
  wellContainer->getH5File().createDataSet<double>(
        "MD_X_Y_Z_TVD_DX_DY_AZ_INCL", h5gt::DataSpace(
          {size_t(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.cols()), size_t(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.rows())})).
      write_raw(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.data());

  Eigen::MatrixXd M = h5geo::MdAzIncl2MdXYTvd(
        MD_X_Y_Z_TVD_DX_DY_AZ_INCL(
          Eigen::all, {0, 7, 8}), wellParam.headX, wellParam.headY,
        "degree", false);

  wellContainer->getH5File().createDataSet<double>(
        "MdAzIncl2MdXYTvd", h5gt::DataSpace({size_t(M.cols()), size_t(M.rows())})).
      write_raw(M.data());

  double std_dev = (MD_X_Y_Z_TVD_DX_DY_AZ_INCL(
                      Eigen::all, {1, 2, 4}) - M(Eigen::all, {1, 2, 3})).norm();
  double MD_max = MD_X_Y_Z_TVD_DX_DY_AZ_INCL(Eigen::last, 0);
  ASSERT_TRUE(std_dev/MD_max < 0.05); // less than 5 %
}

TEST_F(H5WellFixture, MdAzIncl2MdXYTvd_XNorth){
  wellContainer->getH5File().createDataSet<double>(
        "MD_X_Y_Z_TVD_DX_DY_AZ_INCL", h5gt::DataSpace(
          {size_t(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.cols()), size_t(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.rows())})).
      write_raw(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.data());

  Eigen::MatrixXd M = h5geo::MdAzIncl2MdXYTvd(
        MD_X_Y_Z_TVD_DX_DY_AZ_INCL(
          Eigen::all, {0, 7, 8}), wellParam.headY, wellParam.headX,
        "degree", true);

  wellContainer->getH5File().createDataSet<double>(
        "MdAzIncl2MdXYTvd", h5gt::DataSpace({size_t(M.cols()), size_t(M.rows())})).
      write_raw(M.data());

  double std_dev = (MD_X_Y_Z_TVD_DX_DY_AZ_INCL(
                      Eigen::all, {1, 2, 4}) - M(Eigen::all, {2, 1, 3})).norm();
  double MD_max = MD_X_Y_Z_TVD_DX_DY_AZ_INCL(Eigen::last, 0);

  ASSERT_TRUE(std_dev/MD_max < 0.05); // less than 5 %
}

TEST_F(H5WellFixture, TvdXY2MdAzIncl){
  wellContainer->getH5File().createDataSet<double>(
        "MD_X_Y_Z_TVD_DX_DY_AZ_INCL", h5gt::DataSpace(
          {size_t(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.cols()), size_t(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.rows())})).
      write_raw(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.data());

  Eigen::MatrixXd M = h5geo::TvdXY2MdAzIncl(
        MD_X_Y_Z_TVD_DX_DY_AZ_INCL(
          Eigen::all, {4, 1, 2}), wellParam.headX, wellParam.headY, "radian", false);

  Eigen::MatrixXd MM = h5geo::MdAzIncl2MdXYTvd(
        M, wellParam.headX, wellParam.headY, "radian", false);

  wellContainer->getH5File().createDataSet<double>(
        "TvdXY2MdAzIncl", h5gt::DataSpace({size_t(M.cols()), size_t(M.rows())})).
      write_raw(M.data());

  double std_dev = (MD_X_Y_Z_TVD_DX_DY_AZ_INCL(
                      Eigen::all, {1, 2, 4}) - MM(Eigen::all, {1, 2, 3})).norm();
  double MD_max = MD_X_Y_Z_TVD_DX_DY_AZ_INCL(Eigen::last, 0);
  ASSERT_TRUE(std_dev/MD_max < 0.05); // less than 5 %
}

TEST_F(H5WellFixture, TvdXY2MdAzIncl_XNorth){
  wellContainer->getH5File().createDataSet<double>(
        "MD_X_Y_Z_TVD_DX_DY_AZ_INCL", h5gt::DataSpace(
          {size_t(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.cols()), size_t(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.rows())})).
      write_raw(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.data());

  Eigen::MatrixXd M = h5geo::TvdXY2MdAzIncl(
        MD_X_Y_Z_TVD_DX_DY_AZ_INCL(
          Eigen::all, {4, 2, 1}), wellParam.headY, wellParam.headX, "radian", true);

  Eigen::MatrixXd MM = h5geo::MdAzIncl2MdXYTvd(
        M, wellParam.headY, wellParam.headX, "radian", true);

  wellContainer->getH5File().createDataSet<double>(
        "TvdXY2MdAzIncl", h5gt::DataSpace({size_t(M.cols()), size_t(M.rows())})).
      write_raw(M.data());

  double std_dev = (MD_X_Y_Z_TVD_DX_DY_AZ_INCL(
                      Eigen::all, {1, 2, 4}) - MM(Eigen::all, {2, 1, 3})).norm();
  double MD_max = MD_X_Y_Z_TVD_DX_DY_AZ_INCL(Eigen::last, 0);
  ASSERT_TRUE(std_dev/MD_max < 0.05); // less than 5 %
}

TEST_F(H5WellFixture, TvdXY2MdXYTvd){
  Eigen::MatrixXd M = h5geo::TvdXY2MdXYTvd(
        MD_X_Y_Z_TVD_DX_DY_AZ_INCL(
          Eigen::all, {4, 1, 2}), wellParam.headX, wellParam.headY, false);

  wellContainer->getH5File().createDataSet<double>(
        "TvdXY2MdXYTvd", h5gt::DataSpace({size_t(M.cols()), size_t(M.rows())})).
      write_raw(M.data());

  double std_dev = (MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(0) - M.col(0)).norm();
  double MD_max = MD_X_Y_Z_TVD_DX_DY_AZ_INCL(Eigen::last, 0);
  ASSERT_TRUE(std_dev/MD_max < 0.05); // less than 5 %
}

TEST_F(H5WellFixture, TvdXY2MdXYTvd_XNorth){
  Eigen::MatrixXd M = h5geo::TvdXY2MdXYTvd(
        MD_X_Y_Z_TVD_DX_DY_AZ_INCL(
          Eigen::all, {4, 1, 2}), wellParam.headX, wellParam.headY, true);

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
          Eigen::all, {4, 5, 6}), wellParam.headX, wellParam.headY, false);

  wellContainer->getH5File().createDataSet<double>(
        "TvdDxDy2MdXYTvd", h5gt::DataSpace({size_t(M.cols()), size_t(M.rows())})).
      write_raw(M.data());

  double std_dev = (MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(0) - M.col(0)).norm();
  double MD_max = MD_X_Y_Z_TVD_DX_DY_AZ_INCL(Eigen::last, 0);
  ASSERT_TRUE(std_dev/MD_max < 0.05); // less than 5 %
}

TEST_F(H5WellFixture, TvdDxDy2MdXYTvd_XNorth){
  Eigen::MatrixXd M = h5geo::TvdDxDy2MdXYTvd(
        MD_X_Y_Z_TVD_DX_DY_AZ_INCL(
          Eigen::all, {4, 5, 6}), wellParam.headX, wellParam.headY, true);

  wellContainer->getH5File().createDataSet<double>(
        "TvdDxDy2MdXYTvd", h5gt::DataSpace({size_t(M.cols()), size_t(M.rows())})).
      write_raw(M.data());

  double std_dev = (MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(0) - M.col(0)).norm();
  double MD_max = MD_X_Y_Z_TVD_DX_DY_AZ_INCL(Eigen::last, 0);
  ASSERT_TRUE(std_dev/MD_max < 0.05); // less than 5 %
}

TEST_F(H5WellFixture, TvdssXY2MdXYTvd){
  Eigen::MatrixXd TvdssXY(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.rows(), 3);
  TvdssXY.col(0) = MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(4).array() - wellParam.kb;
  TvdssXY.col(1) = MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(1);
  TvdssXY.col(2) = MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(2);

  Eigen::MatrixXd M = h5geo::TvdssXY2MdXYTvd(
        TvdssXY, wellParam.headX, wellParam.headY, wellParam.kb, false);

  wellContainer->getH5File().createDataSet<double>(
        "TvdssXY2MdXYTvd", h5gt::DataSpace({size_t(M.cols()), size_t(M.rows())})).
      write_raw(M.data());

  double std_dev = (MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(0) - M.col(0)).norm();
  double MD_max = MD_X_Y_Z_TVD_DX_DY_AZ_INCL(Eigen::last, 0);
  ASSERT_TRUE(std_dev/MD_max < 0.05); // less than 5 %
}

TEST_F(H5WellFixture, TvdssXY2MdXYTvd_XNorth){
  Eigen::MatrixXd TvdssXY(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.rows(), 3);
  TvdssXY.col(0) = MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(4).array() - wellParam.kb;
  TvdssXY.col(1) = MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(1);
  TvdssXY.col(2) = MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(2);

  Eigen::MatrixXd M = h5geo::TvdssXY2MdXYTvd(
        TvdssXY, wellParam.headX, wellParam.headY, wellParam.kb, true);

  wellContainer->getH5File().createDataSet<double>(
        "TvdssXY2MdXYTvd", h5gt::DataSpace({size_t(M.cols()), size_t(M.rows())})).
      write_raw(M.data());

  double std_dev = (MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(0) - M.col(0)).norm();
  double MD_max = MD_X_Y_Z_TVD_DX_DY_AZ_INCL(Eigen::last, 0);
  ASSERT_TRUE(std_dev/MD_max < 0.05); // less than 5 %
}

TEST_F(H5WellFixture, TvdssDxDy2MdXYTvd){
  Eigen::MatrixXd TvdssDxDy(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.rows(), 3);
  TvdssDxDy.col(0) = MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(4).array() - wellParam.kb;
  TvdssDxDy.col(1) = MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(5);
  TvdssDxDy.col(2) = MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(6);

  Eigen::MatrixXd M = h5geo::TvdssDxDy2MdXYTvd(
        TvdssDxDy, wellParam.headX, wellParam.headY, wellParam.kb, false);

  wellContainer->getH5File().createDataSet<double>(
        "TvdssDxDy2MdXYTvd", h5gt::DataSpace({size_t(M.cols()), size_t(M.rows())})).
      write_raw(M.data());

  double std_dev = (MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(0) - M.col(0)).norm();
  double MD_max = MD_X_Y_Z_TVD_DX_DY_AZ_INCL(Eigen::last, 0);
  ASSERT_TRUE(std_dev/MD_max < 0.05); // less than 5 %
}

TEST_F(H5WellFixture, TvdssDxDy2MdXYTvd_XNorth){
  Eigen::MatrixXd TvdssDxDy(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.rows(), 3);
  TvdssDxDy.col(0) = MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(4).array() - wellParam.kb;
  TvdssDxDy.col(1) = MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(5);
  TvdssDxDy.col(2) = MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(6);

  Eigen::MatrixXd M = h5geo::TvdssDxDy2MdXYTvd(
        TvdssDxDy, wellParam.headX, wellParam.headY, wellParam.kb, true);

  wellContainer->getH5File().createDataSet<double>(
        "TvdssDxDy2MdXYTvd", h5gt::DataSpace({size_t(M.cols()), size_t(M.rows())})).
      write_raw(M.data());

  double std_dev = (MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(0) - M.col(0)).norm();
  double MD_max = MD_X_Y_Z_TVD_DX_DY_AZ_INCL(Eigen::last, 0);
  ASSERT_TRUE(std_dev/MD_max < 0.05); // less than 5 %
}

TEST_F(H5WellFixture, writeReadLogCurve){
  H5Well_ptr well(
        wellContainer->createWell(
          WELL_NAME, wellParam, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(well != nullptr);

  H5LogCurve_ptr logCurve(
        well->createLogCurve(
          LOG_TYPE, LOG_NAME,
          logCurveParam, h5geo::CreationType::OPEN_OR_CREATE));

  ASSERT_TRUE(logCurve != nullptr);

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
  H5Well_ptr well(
        wellContainer->createWell(
          WELL_NAME, wellParam, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(well != nullptr);

  H5LogCurve_ptr logCurve(
        well->createLogCurve(
          LOG_TYPE, LOG_NAME,
          logCurveParam, h5geo::CreationType::OPEN_OR_CREATE));

  H5DevCurve_ptr devCurve(
        well->createDevCurve(DEV_NAME, devCurveParam, h5geo::CreationType::OPEN_OR_CREATE));

  ASSERT_TRUE(devCurve != nullptr);

  ASSERT_TRUE(devCurve->openWell())
      << "Try to find well containing this curve";
  ASSERT_TRUE(logCurve->openWell())
      << "Try to find well containing this curve";

  ASSERT_TRUE(devCurve->openWellContainer())
      << "Get wellcontainer containing this curve";
  ASSERT_TRUE(logCurve->openWellContainer())
      << "Get wellcontainer containing this curve";
}

TEST_F(H5WellFixture, activeDevCurveTest){
  H5Well_ptr well(
        wellContainer->createWell(
          WELL_NAME, wellParam, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(well != nullptr);

  H5DevCurve_ptr devCurve(
        well->createDevCurve(
          DEV_NAME, devCurveParam, h5geo::CreationType::OPEN_OR_CREATE));

  ASSERT_TRUE(devCurve->writeMD(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(0)));
  ASSERT_TRUE(devCurve->writeAZIM(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(7)));
  ASSERT_TRUE(devCurve->writeINCL(MD_X_Y_Z_TVD_DX_DY_AZ_INCL.col(8)));

  devCurve->updateTvdDxDy();

  ASSERT_FALSE(devCurve->isActive());
  ASSERT_TRUE(devCurve->setActive());
  ASSERT_TRUE(devCurve->isActive());

  H5DevCurve_ptr activeCurve(well->openActiveDevCurve());
  ASSERT_TRUE(activeCurve != nullptr);

  ASSERT_TRUE(devCurve->getCurve(h5geo::DevDataType::MD).isApprox(
                activeCurve->getCurve(h5geo::DevDataType::MD)));
  ASSERT_TRUE(devCurve->getCurve(h5geo::DevDataType::DX).isApprox(
                activeCurve->getCurve(h5geo::DevDataType::DX)));
  ASSERT_TRUE(devCurve->getCurve(h5geo::DevDataType::DY).isApprox(
                activeCurve->getCurve(h5geo::DevDataType::DY)));
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

  H5Well_ptr well_1(
        wellContainer->createWell(
          wellName1, wellParam, h5geo::CreationType::CREATE_OR_OVERWRITE));

  H5Well_ptr well_2(
        wellContainer->createWell(
          wellName2, wellParam, h5geo::CreationType::CREATE_OR_OVERWRITE));

  ASSERT_FALSE(well_1->createDevCurve(devCurveName2, devCurveParam, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_FALSE(well_1->createLogCurve(logCurveType2, logCurveName2, logCurveParam, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_FALSE(well_2->createDevCurve(devCurveName1, devCurveParam, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_FALSE(well_2->createLogCurve(logCurveType1, logCurveName1, logCurveParam, h5geo::CreationType::OPEN_OR_CREATE));

  ASSERT_TRUE(well_1->createDevCurve(devCurveName1, devCurveParam, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_TRUE(well_1->createLogCurve(logCurveType1, logCurveName1, logCurveParam, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_TRUE(well_2->createDevCurve(devCurveName2, devCurveParam, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_TRUE(well_2->createLogCurve(logCurveType2, logCurveName2, logCurveParam, h5geo::CreationType::OPEN_OR_CREATE));

  ASSERT_FALSE(well_1->openDevCurve(devCurveName2) != nullptr);
  ASSERT_FALSE(well_1->openLogCurve(logCurveType2, logCurveName2) != nullptr);
  ASSERT_FALSE(well_2->openDevCurve(devCurveName1) != nullptr);
  ASSERT_FALSE(well_2->openLogCurve(logCurveType1, logCurveName1) != nullptr);

  ASSERT_TRUE(well_1->openDevCurve(devCurveName1) != nullptr);
  ASSERT_TRUE(well_1->openLogCurve(logCurveType1, logCurveName1) != nullptr);
  ASSERT_TRUE(well_2->openDevCurve(devCurveName2) != nullptr);
  ASSERT_TRUE(well_2->openLogCurve(logCurveType2, logCurveName2) != nullptr);
}

TEST_F(H5WellFixture, createPointsFromWellTop){
  std::string fileName = "welltops.h5";
  H5WellCnt_ptr wellCnt(
        h5geo::createWellContainerByName(
          fileName, h5geo::CreationType::CREATE_OR_OVERWRITE));
  H5Well_ptr well(
        wellCnt->createWell(
          WELL_NAME, wellParam, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(well != nullptr);

  auto kb = well->getKB();
  auto headXY = well->getHeadCoord();
  std::string pointsName = "well head";
  H5Points_ptr points(
        wellCnt->createPoints(
          pointsName, pointsParam, h5geo::CreationType::CREATE_OR_OVERWRITE));

  h5geo::PointArray pArrIn, pArrOut;
  pArrIn.push_back({headXY(0), headXY(1), kb, "one"});
  pArrIn.push_back({headXY(0), headXY(1), kb, "two"});

  points->writeData(pArrIn);
  pArrOut = points->getData("km");

  ASSERT_TRUE(pArrIn[0].getName() == pArrOut[0].getName());
  ASSERT_EQ(pArrIn[0].x(), pArrOut[0].x()*1000);
  ASSERT_EQ(pArrIn[0].y(), pArrOut[0].y()*1000);
  ASSERT_EQ(pArrIn[0].z(), pArrOut[0].z()*1000);

  ASSERT_TRUE(pArrIn[1].getName() == pArrOut[1].getName());
  ASSERT_EQ(pArrIn[1].x(), pArrOut[1].x()*1000);
  ASSERT_EQ(pArrIn[1].y(), pArrOut[1].y()*1000);
  ASSERT_EQ(pArrIn[1].z(), pArrOut[1].z()*1000);
}

TEST_F(H5WellFixture, getDevCurveGroupList){
  std::string devName1 = "dev1";
  std::string devName2 = "dev2";
  std::string logName1 = "log1";
  std::string logName2 = "log2";

  H5Well_ptr well(
        wellContainer->createWell(
          WELL_NAME, wellParam, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(well != nullptr);

  H5DevCurve_ptr devCurve1(
        well->createDevCurve(
          devName1, devCurveParam, h5geo::CreationType::CREATE_OR_OVERWRITE));

  H5DevCurve_ptr devCurve2(
        well->createDevCurve(
          devName2, devCurveParam, h5geo::CreationType::CREATE_OR_OVERWRITE));

  H5LogCurve_ptr logCurve1(
        well->createLogCurve(
          LOG_TYPE, logName1,
          logCurveParam, h5geo::CreationType::CREATE_OR_OVERWRITE));

  H5LogCurve_ptr logCurve2(
        well->createLogCurve(
          LOG_TYPE, logName2,
          logCurveParam, h5geo::CreationType::CREATE_OR_OVERWRITE));

  std::vector<std::string> devNameList = well->getDevCurveNameList();
  std::vector<std::string> logNameList = well->getLogCurveNameList();
  std::vector<std::string> logTypeList = well->getLogTypeNameList();
  std::vector<h5gt::Group> logGroupList = well->getLogCurveGroupList();

  ASSERT_TRUE(devNameList[0] == devName1);
  ASSERT_TRUE(devNameList[1] == devName2);
  ASSERT_TRUE(logNameList[0] == LOG_TYPE + "/" + logName1);
  ASSERT_TRUE(logNameList[1] == LOG_TYPE + "/" + logName2);
  ASSERT_TRUE(logTypeList[0] == LOG_TYPE);
}

//TEST_F(H5WellFixture, customWell){
//  std::string wellCntName = "/home/kerim/Documents/Colada_prj/default/DATA/wells.h5";
//  std::string wellName = "85-LX-9";

//  H5WellCnt_ptr wellCnt(h5geo::openWellContainerByName(wellCntName));
//  H5Well_ptr well(wellCnt->openWell(wellName));
//  H5DevCurve_ptr devCurve(well->openActiveDevCurve());
//  Eigen::VectorXd x = devCurve->getCurve("X", "m");
//  Eigen::VectorXd dx = devCurve->getCurve("DX", "m");
//  Eigen::VectorXd headXY = well->getHeadCoord("m");
//  double kb = well->getKB("m");
//}
