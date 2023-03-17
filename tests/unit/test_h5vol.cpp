#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <h5geo/h5volcontainer.h>
#include <h5geo/h5map.h>
#include <h5geo/h5seis.h>
#include <h5geo/h5well.h>
#include <h5geo/h5vol.h>

#include <h5gt/H5File.hpp>
#include <h5gt/H5Group.hpp>
#include <h5gt/H5DataSet.hpp>

#include <filesystem>
namespace fs = std::filesystem;

class H5VolFixture: public ::testing::Test {
public:

  virtual void SetUp() override{
    // code here will execute just before the test ensues
    static bool trig = false;

    FILE_NAME1 = "vol1.h5";
    FILE_NAME2 = "vol2.h5";
    VOL_NAME1 = "path1/to/vol";
    VOL_NAME2 = "path2/to/vol";

    if (trig){
      h5gt::File file1(FILE_NAME1, h5gt::File::OpenOrCreate);
      volContainer1 = H5VolCnt_ptr(
            h5geo::createVolContainer(
              file1, h5geo::CreationType::OPEN_OR_CREATE));
      h5gt::File file2(FILE_NAME2, h5gt::File::OpenOrCreate);
      volContainer2 = H5VolCnt_ptr(
            h5geo::createVolContainer(
              file2, h5geo::CreationType::OPEN_OR_CREATE));
    } else {
      h5gt::File file1(FILE_NAME1, h5gt::File::OpenOrCreate |
                      h5gt::File::Overwrite);
      volContainer1 = H5VolCnt_ptr(
            h5geo::createVolContainer(
              file1, h5geo::CreationType::CREATE_OR_OVERWRITE));
      h5gt::File file2(FILE_NAME2, h5gt::File::OpenOrCreate |
                      h5gt::File::Overwrite);
      volContainer2 = H5VolCnt_ptr(
            h5geo::createVolContainer(
              file2, h5geo::CreationType::CREATE_OR_OVERWRITE));
    }

    trig = true;

    p.X0 = 0;
    p.Y0 = 0;
    p.Z0 = 0;
    p.dX = 1;
    p.dY = 1;
    p.dZ = 1;
    p.nX = 3;
    p.nY = 4;
    p.nZ = 5;
    p.lengthUnits = "millimeter";
    p.dataUnits = "m/s";
    p.xChunkSize = 64;
    p.yChunkSize = 64;
    p.zChunkSize = 64;
    p.spatialReference = "EPSG:8139";
  }

  virtual void TearDown() override{
    // code here will be called just after the test completes
    // ok to through exceptions from here if need be
    auto h5File = volContainer1->getH5File();
//    h5geo::unlinkContent(h5File);
  }

public:
  H5VolCnt_ptr volContainer1, volContainer2;
  H5VolParam p;
  std::string FILE_NAME1, FILE_NAME2, VOL_NAME1,  VOL_NAME2;
};

TEST_F(H5VolFixture, createContainer){
  ASSERT_TRUE(fs::exists(FILE_NAME1));
}

TEST_F(H5VolFixture, createVolWithDifferentCreateFlags){
  H5Vol_ptr vol(
        volContainer1->createVol(
          VOL_NAME1, p, h5geo::CreationType::OPEN));
  ASSERT_TRUE(vol == nullptr) << "OPEN";

  vol = H5Vol_ptr(volContainer1->createVol(
                      VOL_NAME1, p, h5geo::CreationType::CREATE));
  ASSERT_TRUE(vol != nullptr) << "CREATE";

  vol = H5Vol_ptr(volContainer1->createVol(
                      VOL_NAME1, p, h5geo::CreationType::OPEN));
  ASSERT_TRUE(vol != nullptr) << "OPEN";

  vol = H5Vol_ptr(volContainer1->createVol(
                      VOL_NAME1, p, h5geo::CreationType::CREATE));
  ASSERT_TRUE(vol == nullptr) << "CREATE";

  vol = H5Vol_ptr(volContainer1->createVol(
                      VOL_NAME1, p, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_TRUE(vol != nullptr) << "OPEN_OR_CREATE";

  vol = H5Vol_ptr(volContainer1->createVol(
          VOL_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(vol != nullptr) << "CREATE_OR_OVERWRITE";

  std::string volName_tmp = VOL_NAME1;
  vol = H5Vol_ptr(volContainer1->createVol(
          volName_tmp, p, h5geo::CreationType::CREATE_UNDER_NEW_NAME));
  ASSERT_TRUE(vol != nullptr && volName_tmp.compare(VOL_NAME1)) << "CREATE_UNDER_NEW_NAME";
}

TEST_F(H5VolFixture, clone){
  H5Vol_ptr vol(
        volContainer1->createVol(
          VOL_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(vol != nullptr);

  H5Vol_ptr vol2(dynamic_cast<H5Vol*>(vol->clone()));

  ASSERT_TRUE(*vol.get() == *vol2.get());
}

TEST_F(H5VolFixture, openObject){
  H5Vol_ptr vol(
        volContainer1->createVol(
          VOL_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(vol != nullptr) << "Create vol from container1";

  h5gt::Group group = volContainer1->getH5File().getGroup(VOL_NAME1);
  H5BaseObject* baseObject = h5geo::openObject(group);
  ASSERT_TRUE(baseObject != nullptr) << "Base object is not NULLPTR";
  ASSERT_TRUE(dynamic_cast<H5Seis*>(baseObject) == nullptr) << "Base object is not SEIS";
  ASSERT_TRUE(dynamic_cast<H5Well*>(baseObject) == nullptr) << "Base object is not WELL";
  ASSERT_TRUE(dynamic_cast<H5Vol*>(baseObject) != nullptr) << "Base object is VOL";
}

TEST_F(H5VolFixture, createAndGetVol){
  volContainer1->createVol(VOL_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE);
  H5Vol_ptr vol(volContainer1->openVol(VOL_NAME1));
  ASSERT_TRUE(vol != nullptr);
}

TEST_F(H5VolFixture, createAndGetVolFromGroup){
  h5gt::Group group =
      volContainer1->getH5File().createGroup(VOL_NAME2);
  volContainer1->createVol(group, p, h5geo::CreationType::CREATE_OR_OVERWRITE);
  H5Vol_ptr vol(
        volContainer1->openVol(VOL_NAME2));
  ASSERT_TRUE(vol != nullptr);
}

TEST_F(H5VolFixture, writeAndGetDataFromVol){
  // Eigen::MatrixXf m = Eigen::MatrixXf::Random(p.nX, p.nY*p.nZ);
  Eigen::MatrixXf m(p.nX, p.nY*p.nZ);
  float val = 0;
  for (int j = 0; j < m.cols(); j++){
    for (int i = 0; i < m.rows(); i++){
      m(i,j) = val++;
    }
  }

  H5Vol_ptr vol(
        volContainer1->createVol(
          VOL_NAME2, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(vol != nullptr);
  ASSERT_TRUE(vol->writeData(m,0,0,0,p.nX,p.nY,p.nZ));

  Eigen::MatrixXf M = vol->getData(0,0,0,p.nX,p.nY,p.nZ,"mm/sec");
  ASSERT_TRUE(m.isApprox(M/1000));
}

// prefix `DISABLED_` is to skip test
TEST_F(H5VolFixture, DISABLED_SEGY){
  std::string segyFile = "E:/Teapot Dome/DataSets/Seismic/CD files/3D_Seismic/filt_mig.sgy";

  H5Vol_ptr vol(
        volContainer1->createVol(
          VOL_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(vol != nullptr);

  ASSERT_TRUE(
    vol->readSEGYSTACK(
      segyFile,
      180, 4,
      184, 4,
      188, 4,
      192, 4, 
      -2, 0, 0, // samp rate is negative
      static_cast<h5geo::SegyFormat>(0),
      static_cast<h5geo::Endian>(0),
      [](double progress) { std::cout << "Progress:\t" << progress << std::endl; }));

  vol->setTemporalUnits("microsecond");
  ASSERT_TRUE(vol->exportToSEGY("out.sgy", 
      [](double progress) { std::cout << "Progress:\t" << progress << std::endl; }));
}