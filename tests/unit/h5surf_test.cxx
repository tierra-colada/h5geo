#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <h5surf/h5surfcontainer.h>

class H5SurfFixture: public ::testing::Test {
public:

  virtual void SetUp() override{
    // code here will execute just before the test ensues
    static bool trig = false;

    if (trig){
      surfContainer = H5SurfContainer::create(
            FILE_NAME,
            h5gt::File::OpenOrCreate);
    } else {
      surfContainer = H5SurfContainer::create(
            FILE_NAME,
            h5gt::File::OpenOrCreate |
            h5gt::File::Overwrite);
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
  std::optional<H5SurfContainer> surfContainer;
  SurfParam p;
  std::string FILE_NAME = "surf.h5";
  std::string SURF_NAME1 = "path1/to/surface";
  std::string SURF_NAME2 = "path2/to/surface";
};

TEST_F(H5SurfFixture, createContainer){
  ASSERT_TRUE(h5geo::isFileExist(FILE_NAME));
}

TEST_F(H5SurfFixture, createSurfWithDifferentCreateFlags){
  std::optional<H5Surf> surf =
      surfContainer->createSurf(
        SURF_NAME1, p, h5geo::CreationType::OPEN_OR_CREATE);
  ASSERT_TRUE(surf.has_value());
  surf =
      surfContainer->createSurf(
        SURF_NAME1, p, h5geo::CreationType::CREATE_OR_OVERWRITE);
  ASSERT_TRUE(surf.has_value());
  surf =
      surfContainer->createSurf(
        SURF_NAME1, p, h5geo::CreationType::CREATE_UNDER_NEW_NAME);
  ASSERT_TRUE(surf.has_value());
  surf =
      surfContainer->createSurf(
        SURF_NAME1, p, h5geo::CreationType::OPEN_OR_CREATE);
  ASSERT_TRUE(surf.has_value());
}

TEST_F(H5SurfFixture, createAndGetSurf){
  surfContainer->createSurf(SURF_NAME1, p);
  std::optional<H5Surf> surf = surfContainer->getSurf(SURF_NAME1);
  ASSERT_TRUE(surf.has_value());
}

TEST_F(H5SurfFixture, createAndGetSurfFromGroup){
  h5gt::Group group =
      surfContainer->getH5File().createGroup(SURF_NAME2);
  surfContainer->createSurf(group, p);
  std::optional<H5Surf> surf = surfContainer->getSurf(SURF_NAME2);
  ASSERT_TRUE(surf.has_value());
}

TEST_F(H5SurfFixture, writeAndGetDataFromSurf){
  Eigen::MatrixXd m = Eigen::MatrixXd::Random(p.nY, p.nX);

  std::optional<H5Surf> surf =
      surfContainer->createSurf(
        SURF_NAME2, p);
  ASSERT_TRUE(surf.has_value());
  ASSERT_TRUE(surf->writeData(m));
  Eigen::MatrixXd M = surf->getData();
  ASSERT_TRUE(m.isApprox(M));
}
