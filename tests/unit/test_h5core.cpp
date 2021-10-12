#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <h5geo/h5core.h>

#include <h5gt/H5File.hpp>
#include <h5gt/H5Group.hpp>
#include <h5gt/H5DataSet.hpp>

#include <filesystem>
namespace fs = std::filesystem;

class H5CoreFixture: public ::testing::Test {
public:

  virtual void SetUp() override{}
  virtual void TearDown() override{}
};

TEST_F(H5CoreFixture, getTraceHeaderNames){
  std::vector<std::string> fullHeaderNames, shortHeaderNames;
  h5geo::getTraceHeaderNames(fullHeaderNames, shortHeaderNames);
  ASSERT_TRUE(fullHeaderNames.size() == h5geo::getTraceHeaderCount());
  ASSERT_TRUE(shortHeaderNames.size() == h5geo::getTraceHeaderCount());

  std::cout << "Seis full TRACE header names:" << std::endl;
  for(const auto& name : fullHeaderNames){
    std::cout << name << std::endl;
  }

  std::cout << "Seis short TRACE header names:" << std::endl;
  for(const auto& name : shortHeaderNames){
    std::cout << name << std::endl;
  }
}

TEST_F(H5CoreFixture, getBinHeaderNames){
  std::vector<std::string> fullHeaderNames, shortHeaderNames;
  h5geo::getBinHeaderNames(fullHeaderNames, shortHeaderNames);
  ASSERT_TRUE(fullHeaderNames.size() == h5geo::getBinHeaderCount());
  ASSERT_TRUE(shortHeaderNames.size() == h5geo::getBinHeaderCount());

  std::cout << "Seis full BIN header names:" << std::endl;
  for(const auto& name : fullHeaderNames){
    std::cout << name << std::endl;
  }

  std::cout << "Seis short BIN header names:" << std::endl;
  for(const auto& name : shortHeaderNames){
    std::cout << name << std::endl;
  }
}

TEST_F(H5CoreFixture, getTraceHeaderBytes){
  std::vector<int> bytesStart, nBytes;
  h5geo::getTraceHeaderBytes(bytesStart, nBytes);
  ASSERT_TRUE(bytesStart.size() == h5geo::getTraceHeaderCount());
  ASSERT_TRUE(nBytes.size() == h5geo::getTraceHeaderCount());

  std::cout << "Seis TRACE header bytes START:" << std::endl;
  for(const auto& val : bytesStart){
    std::cout << val << std::endl;
  }

  std::cout << "Seis TRACE header bytes N:" << std::endl;
  for(const auto& val : nBytes){
    std::cout << val << std::endl;
  }
}

TEST_F(H5CoreFixture, getBinHeaderBytes){
  std::vector<int> bytesStart, nBytes;
  h5geo::getBinHeaderBytes(bytesStart, nBytes);
  ASSERT_TRUE(bytesStart.size() == h5geo::getBinHeaderCount());
  ASSERT_TRUE(nBytes.size() == h5geo::getBinHeaderCount());

  std::cout << "Seis BIN header bytes START:" << std::endl;
  for(const auto& val : bytesStart){
    std::cout << val << std::endl;
  }

  std::cout << "Seis BIN header bytes N:" << std::endl;
  for(const auto& val : nBytes){
    std::cout << val << std::endl;
  }
}
