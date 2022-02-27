#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <h5geo/h5mapcontainer.h>
#include <h5geo/h5map.h>
#include <h5geo/h5seis.h>
#include <h5geo/h5well.h>
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

    FILE_NAME1 = "map1.h5";
    FILE_NAME2 = "map2.h5";
    MAP_NAME1 = "path1/to/map";
    MAP_NAME2 = "path2/to/map";

    if (trig){
      h5gt::File file1(FILE_NAME1, h5gt::File::OpenOrCreate);
      mapContainer1 = H5MapCnt_ptr(
            h5geo::createMapContainer(
              file1, h5geo::CreationType::OPEN_OR_CREATE));
      h5gt::File file2(FILE_NAME2, h5gt::File::OpenOrCreate);
      mapContainer2 = H5MapCnt_ptr(
            h5geo::createMapContainer(
              file2, h5geo::CreationType::OPEN_OR_CREATE));
    } else {
      h5gt::File file1(FILE_NAME1, h5gt::File::OpenOrCreate |
                      h5gt::File::Overwrite);
      mapContainer1 = H5MapCnt_ptr(
            h5geo::createMapContainer(
              file1, h5geo::CreationType::CREATE_OR_OVERWRITE));
      h5gt::File file2(FILE_NAME2, h5gt::File::OpenOrCreate |
                      h5gt::File::Overwrite);
      mapContainer2 = H5MapCnt_ptr(
            h5geo::createMapContainer(
              file2, h5geo::CreationType::CREATE_OR_OVERWRITE));
    }

    trig = true;

    p.X0 = 0;
    p.Y0 = 0;
    p.X1 = 100;
    p.Y1 = 0;
    p.X2 = 100;
    p.Y2 = 0;
    p.nX = 10;
    p.nY = 20;
    p.domain = h5geo::Domain::TVD;
    p.lengthUnits = "millimeter";
    p.dataUnits = "m/s";
    p.xChunkSize = 5;
    p.yChunkSize = 5;
    p.spatialReference = "EPSG:8139";
  }

  virtual void TearDown() override{
    // code here will be called just after the test completes
    // ok to through exceptions from here if need be
    auto h5File = mapContainer1->getH5File();
//    h5geo::unlinkContent(h5File);
  }

public:
  H5MapCnt_ptr mapContainer1, mapContainer2;
  MapParam p;
  std::string FILE_NAME1, FILE_NAME2, MAP_NAME1,  MAP_NAME2;
};

TEST_F(H5MapFixture, createContainer){
  ASSERT_TRUE(fs::exists(FILE_NAME1));

  H5MapCnt_ptr mapCnt(h5geo::openMapContainerByName("/home/kerim/Documents/Colada_prj/default/DATA/horizons/Seismic horizon 1.h5"));
  H5Map_ptr map(mapCnt->openMap("Seismic horizon 1"));
  Eigen::VectorXd origin = map->getOrigin("m", true);
  std::cout << origin << std::endl;

  Eigen::MatrixXd M = map->getData("ms");
  std::cout << "M mean: " << M.mean() << std::endl;
}

TEST_F(H5MapFixture, createMapWithDifferentCreateFlags){
  H5Map_ptr map(
        mapContainer1->createMap(
          MAP_NAME1, p, h5geo::CreationType::OPEN));
  ASSERT_TRUE(map == nullptr) << "OPEN";

  map = H5Map_ptr(mapContainer1->createMap(
                      MAP_NAME1, p, h5geo::CreationType::CREATE));
  ASSERT_TRUE(map != nullptr) << "CREATE";

  map = H5Map_ptr(mapContainer1->createMap(
                      MAP_NAME1, p, h5geo::CreationType::OPEN));
  ASSERT_TRUE(map != nullptr) << "OPEN";

  map = H5Map_ptr(mapContainer1->createMap(
                      MAP_NAME1, p, h5geo::CreationType::CREATE));
  ASSERT_TRUE(map == nullptr) << "CREATE";

  map = H5Map_ptr(mapContainer1->createMap(
                      MAP_NAME1, p, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_TRUE(map != nullptr) << "OPEN_OR_CREATE";

  map = H5Map_ptr(mapContainer1->createMap(
          MAP_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(map != nullptr) << "CREATE_OR_OVERWRITE";

  std::string seisName_tmp = MAP_NAME1;
  map = H5Map_ptr(mapContainer1->createMap(
          seisName_tmp, p, h5geo::CreationType::CREATE_UNDER_NEW_NAME));
  ASSERT_TRUE(map != nullptr && seisName_tmp.compare(MAP_NAME1)) << "CREATE_UNDER_NEW_NAME";
}

TEST_F(H5MapFixture, clone){
  H5Map_ptr map(
        mapContainer1->createMap(
          MAP_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(map != nullptr);

  H5Map_ptr map2(dynamic_cast<H5Map*>(map->clone()));

  ASSERT_TRUE(*map.get() == *map2.get());
}

TEST_F(H5MapFixture, openObject){
  H5Map_ptr map(
        mapContainer1->createMap(
          MAP_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(map != nullptr) << "Create map from container1";

  h5gt::Group group = mapContainer1->getH5File().getGroup(MAP_NAME1);
  H5BaseObject* baseObject = h5geo::openObject(group);
  ASSERT_TRUE(baseObject != nullptr) << "Base object is not NULLPTR";
  ASSERT_TRUE(dynamic_cast<H5Seis*>(baseObject) == nullptr) << "Base object is not SEIS";
  ASSERT_TRUE(dynamic_cast<H5Well*>(baseObject) == nullptr) << "Base object is not WELL";
  ASSERT_TRUE(dynamic_cast<H5Map*>(baseObject) != nullptr) << "Base object is MAP";
}

TEST_F(H5MapFixture, createAndGetMap){
  mapContainer1->createMap(MAP_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE);
  H5Map_ptr map(mapContainer1->openMap(MAP_NAME1));
  ASSERT_TRUE(map != nullptr);
}

TEST_F(H5MapFixture, createAndGetMapFromGroup){
  h5gt::Group group =
      mapContainer1->getH5File().createGroup(MAP_NAME2);
  mapContainer1->createMap(group, p, h5geo::CreationType::CREATE_OR_OVERWRITE);
  H5Map_ptr map(
        mapContainer1->openMap(MAP_NAME2));
  ASSERT_TRUE(map != nullptr);
}

TEST_F(H5MapFixture, writeAndGetDataFromMap){
  Eigen::MatrixXd m = Eigen::MatrixXd::Random(p.nY, p.nX);

  H5Map_ptr map(
        mapContainer1->createMap(
          MAP_NAME2, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(map != nullptr);
  ASSERT_TRUE(map->writeData(m));
  Eigen::MatrixXd M = map->getData("mm/sec");
  ASSERT_TRUE(m.isApprox(M/1000));
}

TEST_F(H5MapFixture, addAndGetAttribute){
  H5Map_ptr map1(mapContainer1->createMap(MAP_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(map1 != nullptr);

  H5Map* map2 = mapContainer1->createMap(MAP_NAME2, p, h5geo::CreationType::CREATE_OR_OVERWRITE);
  ASSERT_TRUE(map2 != nullptr);

  ASSERT_TRUE(map1->addAttribute(map2, "myAttr"));
  ASSERT_TRUE(map1->getAttribute("myAttr") != nullptr);
  ASSERT_TRUE(map1->removeAttribute("myAttr"));
}

TEST_F(H5MapFixture, addAndGetExternalAttribute){
  H5Map_ptr map1(mapContainer1->createMap(MAP_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(map1 != nullptr);

  H5Map* map2 = mapContainer2->createMap(MAP_NAME2, p, h5geo::CreationType::CREATE_OR_OVERWRITE);
  ASSERT_TRUE(map2 != nullptr);

  ASSERT_TRUE(map1->addExternalAttribute(map2, "myAttr"));
  ASSERT_TRUE(map1->getAttribute("myAttr") != nullptr);
  ASSERT_TRUE(map1->removeAttribute("myAttr"));
}

//#include <hdf5.h>

//TEST_F(H5MapFixture, virtualDset){
//  std::string cntName = "virtual.h5";
//  std::string dsetName = "vdset";
//  std::string segyName = "/home/kerim/.julia/dev/JUDI/overthrust_model_2D.h5";
//  H5MapCnt_ptr cnt(h5geo::createMapContainerByName(cntName, h5geo::CreationType::CREATE_OR_OVERWRITE));
//  ASSERT_TRUE(cnt);

//  h5gt::File file = cnt->getH5File();
//  h5gt::DataSetCreateProps dsetP;
//  DATASET;
//}
