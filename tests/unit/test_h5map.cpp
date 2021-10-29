#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <h5geo/h5mapcontainer.h>
#include <h5geo/h5map.h>
#include <h5geo/h5core.h>

#include <h5gt/H5File.hpp>
#include <h5gt/H5Group.hpp>
#include <h5gt/H5DataSet.hpp>

#include <filesystem>
namespace fs = std::filesystem;

class H5MapFixture: public ::testing::Test {
public:

  virtual void SetUp() override{
    // code here will execute just before the test ensues
    static bool trig = false;

    FILE_NAME = "map.h5";
    MAP_NAME1 = "path1/to/map";
    MAP_NAME2 = "path2/to/map";

    if (trig){
      h5gt::File file(FILE_NAME, h5gt::File::OpenOrCreate);
      mapContainer = H5MapCnt_ptr(
            h5geo::createMapContainer(
              file, h5geo::CreationType::OPEN_OR_CREATE));
    } else {
      h5gt::File file(FILE_NAME, h5gt::File::OpenOrCreate |
                      h5gt::File::Overwrite);
      mapContainer = H5MapCnt_ptr(
            h5geo::createMapContainer(
              file, h5geo::CreationType::CREATE_OR_OVERWRITE));
    }

    trig = true;

    p.X0 = 0;
    p.Y0 = 10;
    p.dX = 2;
    p.dY = 4;
    p.nX = 10;
    p.nY = 20;
    p.domain = h5geo::Domain::TVD;
    p.spatialUnits = "millimeter";
    p.dataUnits = "m/s";
  }

  virtual void TearDown() override{
    // code here will be called just after the test completes
    // ok to through exceptions from here if need be
    auto h5File = mapContainer->getH5File();
//    h5geo::unlinkContent(h5File);
  }

public:
  H5MapCnt_ptr mapContainer;
  MapParam p;
  std::string FILE_NAME, MAP_NAME1,  MAP_NAME2;
};

TEST_F(H5MapFixture, createContainer){
  ASSERT_TRUE(fs::exists(FILE_NAME));
}

TEST_F(H5MapFixture, createMapWithDifferentCreateFlags){
  H5Map_ptr map(
        mapContainer->createMap(
          MAP_NAME1, p, h5geo::CreationType::OPEN));
  ASSERT_TRUE(map == nullptr) << "OPEN";

  map = H5Map_ptr(mapContainer->createMap(
                      MAP_NAME1, p, h5geo::CreationType::CREATE));
  ASSERT_TRUE(map != nullptr) << "CREATE";

  map = H5Map_ptr(mapContainer->createMap(
                      MAP_NAME1, p, h5geo::CreationType::OPEN));
  ASSERT_TRUE(map != nullptr) << "OPEN";

  map = H5Map_ptr(mapContainer->createMap(
                      MAP_NAME1, p, h5geo::CreationType::CREATE));
  ASSERT_TRUE(map == nullptr) << "CREATE";

  map = H5Map_ptr(mapContainer->createMap(
                      MAP_NAME1, p, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_TRUE(map != nullptr) << "OPEN_OR_CREATE";

  map = H5Map_ptr(mapContainer->createMap(
          MAP_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(map != nullptr) << "CREATE_OR_OVERWRITE";

  std::string seisName_tmp = MAP_NAME1;
  map = H5Map_ptr(mapContainer->createMap(
          seisName_tmp, p, h5geo::CreationType::CREATE_UNDER_NEW_NAME));
  ASSERT_TRUE(map != nullptr && seisName_tmp.compare(MAP_NAME1)) << "CREATE_UNDER_NEW_NAME";
}

TEST_F(H5MapFixture, createAndGetMap){
  mapContainer->createMap(MAP_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE);
  H5Map_ptr map(mapContainer->getMap(MAP_NAME1));
  ASSERT_TRUE(map != nullptr);
}

TEST_F(H5MapFixture, createAndGetMapFromGroup){
  h5gt::Group group =
      mapContainer->getH5File().createGroup(MAP_NAME2);
  mapContainer->createMap(group, p, h5geo::CreationType::CREATE_OR_OVERWRITE);
  H5Map_ptr map(
        mapContainer->getMap(MAP_NAME2));
  ASSERT_TRUE(map != nullptr);
}

TEST_F(H5MapFixture, writeAndGetDataFromMap){
  Eigen::MatrixXd m = Eigen::MatrixXd::Random(p.nY, p.nX);

  H5Map_ptr map(
        mapContainer->createMap(
          MAP_NAME2, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(map != nullptr);
  ASSERT_TRUE(map->writeData(m));
  Eigen::MatrixXd M = map->getData("mm/sec");
  ASSERT_TRUE(m.isApprox(M/1000));
}
