#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <h5core/h5core.h>

TEST(H5Core, generateName){
  std::vector<std::string> nameList({"one", "two", "two_1"});
  ASSERT_TRUE("one_1" == h5geo::generateName(nameList, "one"));
  ASSERT_TRUE("two_2" == h5geo::generateName(nameList, "two"));
  ASSERT_TRUE("three" == h5geo::generateName(nameList, "three"));
}

TEST(H5Core, splitString){
  std::vector str = h5geo::splitString("my best sentence!", " ");
  ASSERT_THAT(str, ::testing::ElementsAreArray({"my", "best", "sentence!"}));
}

TEST(H5Core, splitPath){
  std::string path_in = "C://my/path", path_out;
  std::vector<std::string> path_out_vec = h5geo::splitPath(path_in, path_out);
  ASSERT_THAT(path_out_vec, ::testing::ElementsAreArray({"C:", "my", "path"}));
}

TEST(H5Core, splitPathToParentAndObj){
  std::string objName, parentPath;
  parentPath = h5geo::splitPathToParentAndObj("/path/to/data", objName);
  ASSERT_TRUE("data" == objName);
  ASSERT_TRUE("/path/to" == parentPath);
  parentPath = h5geo::splitPathToParentAndObj("path/to/data", objName);
  ASSERT_TRUE("data" == objName);
  ASSERT_TRUE("path/to" == parentPath);
  parentPath = h5geo::splitPathToParentAndObj("/data", objName);
  ASSERT_TRUE("data" == objName);
  ASSERT_TRUE("/" == parentPath);
  parentPath = h5geo::splitPathToParentAndObj("/", objName);
  ASSERT_TRUE("" == objName);
  ASSERT_TRUE("" == parentPath);
}

