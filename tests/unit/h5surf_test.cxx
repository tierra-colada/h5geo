#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <h5geo/h5surfcontainer.h>
#include <h5geo/h5surf.h>
#include <h5geo/h5core.h>

#include <h5gt/H5File.hpp>
#include <h5gt/H5Group.hpp>
#include <h5gt/H5DataSet.hpp>

#include <filesystem>
namespace fs = std::filesystem;

class H5SurfFixture: public ::testing::Test {
public:

  virtual void SetUp() override{
    // code here will execute just before the test ensues
    static bool trig = false;

    if (trig){
      h5gt::File file(FILE_NAME, h5gt::File::OpenOrCreate);
      surfContainer = H5SurfCnt_ptr(
            h5geo::createSurfContainer(
              file, h5geo::CreationType::OPEN_OR_CREATE));
    } else {
      h5gt::File file(FILE_NAME, h5gt::File::OpenOrCreate |
                      h5gt::File::Overwrite);
      surfContainer = H5SurfCnt_ptr(
            h5geo::createSurfContainer(
              file, h5geo::CreationType::CREATE_OR_OVERWRITE));
    }

    p.X0 = 0;
    p.Y0 = 10;
    p.dX = 2;
    p.dY = 4;
    p.nX = 10;
    p.nY = 20;

    trig = true;
  }

  virtual void TearDown() override{
    // code here will be called just after the test completes
    // ok to through exceptions from here if need be
    auto h5File = surfContainer->getH5File();
    h5geo::unlinkContent(h5File);
  }

public:
  H5SurfCnt_ptr surfContainer;
  SurfParam p;
  std::string FILE_NAME = "surf.h5";
  std::string SURF_NAME1 = "path1/to/surface";
  std::string SURF_NAME2 = "path2/to/surface";
};

TEST_F(H5SurfFixture, createContainer){
  ASSERT_TRUE(fs::exists(FILE_NAME));
}

TEST_F(H5SurfFixture, createSurfWithDifferentCreateFlags){
  H5Surf_ptr surf(
        surfContainer->createSurf(
          SURF_NAME1, p, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_TRUE(surf != nullptr);

  surf = H5Surf_ptr(
        surfContainer->createSurf(
          SURF_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(surf != nullptr);

  surf = H5Surf_ptr(
        surfContainer->createSurf(
          SURF_NAME1, p, h5geo::CreationType::CREATE_UNDER_NEW_NAME));
  ASSERT_TRUE(surf != nullptr);

  surf = H5Surf_ptr(
        surfContainer->createSurf(
          SURF_NAME1, p, h5geo::CreationType::OPEN_OR_CREATE));
  ASSERT_TRUE(surf != nullptr);

}

TEST_F(H5SurfFixture, createAndGetSurf){
  surfContainer->createSurf(SURF_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE);
  H5Surf_ptr surf(surfContainer->getSurf(SURF_NAME1));
  ASSERT_TRUE(surf != nullptr);
}

TEST_F(H5SurfFixture, createAndGetSurfFromGroup){
  h5gt::Group group =
      surfContainer->getH5File().createGroup(SURF_NAME2);
  surfContainer->createSurf(group, p, h5geo::CreationType::CREATE_OR_OVERWRITE);
  H5Surf_ptr surf(
        surfContainer->getSurf(SURF_NAME2));
  ASSERT_TRUE(surf != nullptr);
}

TEST_F(H5SurfFixture, writeAndGetDataFromSurf){
  Eigen::MatrixXd m = Eigen::MatrixXd::Random(p.nY, p.nX);

  H5Surf_ptr surf(
        surfContainer->createSurf(
          SURF_NAME2, p, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(surf != nullptr);
  ASSERT_TRUE(surf->writeData(m));
  Eigen::MatrixXd M = surf->getData();
  ASSERT_TRUE(m.isApprox(M));
}
