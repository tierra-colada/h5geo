#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <h5geo/h5core.h>

#include <h5gt/H5File.hpp>
#include <h5gt/H5Group.hpp>
#include <h5gt/H5DataSet.hpp>

#include <cmath>
#include <filesystem>
namespace fs = std::filesystem;

class H5CoreFixture: public ::testing::Test {
public:

  virtual void SetUp() override{}
  virtual void TearDown() override{}
};

TEST_F(H5CoreFixture, sort){
  Eigen::VectorXd v(3), v_sorted;
  v << 3, 2, 1;

  auto ind = h5geo::sort(v, v_sorted);
  ASSERT_TRUE(ind(0) == 2);
  ASSERT_TRUE(ind(1) == 1);
  ASSERT_TRUE(ind(2) == 0);

  ASSERT_TRUE(v(0) == v_sorted(2));
  ASSERT_TRUE(v(1) == v_sorted(1));
  ASSERT_TRUE(v(2) == v_sorted(0));
}

TEST_F(H5CoreFixture, sort_matrix){
  Eigen::MatrixXd v(3, 3), v_sorted;
  v << 3,3,3,2,2,2,1,1,1;

  // v =  3 3 3
  //      2 2 2
  //      1 1 1

  auto ind = h5geo::sort(v.col(0), v_sorted);
  ASSERT_TRUE(ind(0) == 2);
  ASSERT_TRUE(ind(1) == 1);
  ASSERT_TRUE(ind(2) == 0);

  ASSERT_TRUE(v(0) == v_sorted(2));
  ASSERT_TRUE(v(1) == v_sorted(1));
  ASSERT_TRUE(v(2) == v_sorted(0));

  // check if there are unresolved external symbols in `h5geo::detail::_sort()`
  h5geo::sort(v.row(0));
  h5geo::sort(v(Eigen::lastN(2), 0));
  h5geo::sort(v(Eigen::seq(1,2), 0));
  h5geo::sort(v(0, Eigen::lastN(2)));
  h5geo::sort(v(0, Eigen::seq(1,2)));
}

TEST_F(H5CoreFixture, sort_vector){
  Eigen::VectorXd v(3), v_sorted;
  v << 3,2,1;

  // v =  3
  //      2
  //      1

  auto ind = h5geo::sort(v, v_sorted);
  ASSERT_TRUE(ind(0) == 2);
  ASSERT_TRUE(ind(1) == 1);
  ASSERT_TRUE(ind(2) == 0);

  ASSERT_TRUE(v(0) == v_sorted(2));
  ASSERT_TRUE(v(1) == v_sorted(1));
  ASSERT_TRUE(v(2) == v_sorted(0));

  // check if there are unresolved external symbols in `h5geo::detail::_sort()`
  h5geo::sort(v.col(0));
  h5geo::sort(v(Eigen::lastN(2)));
  h5geo::sort(v(Eigen::seq(1,2)));
//  h5geo::sort(v(ind));
}

void sort_matrix_ref(
    const Eigen::Ref<const Eigen::MatrixX<double>> v,
    const Eigen::Ref<const Eigen::MatrixX<double>> v_sorted)
{
//  auto ind = h5geo::sort(v.col(0), v_sorted.col(0));
//  ASSERT_TRUE(ind(0) == 2);
//  ASSERT_TRUE(ind(1) == 1);
//  ASSERT_TRUE(ind(2) == 0);

//  ASSERT_TRUE(v(0) == v_sorted(2));
//  ASSERT_TRUE(v(1) == v_sorted(1));
//  ASSERT_TRUE(v(2) == v_sorted(0));
}


TEST_F(H5CoreFixture, sort_matrix_ref){
  Eigen::MatrixXd v(3, 3), v_sorted;
  v << 3,3,3,2,2,2,1,1,1;

  // v =  3 3 3
  //      2 2 2
  //      1 1 1

  sort_matrix_ref(v.col(0), v_sorted);

//  auto ind = h5geo::sort(v.col(0), v_sorted);
//  ASSERT_TRUE(ind(0) == 2);
//  ASSERT_TRUE(ind(1) == 1);
//  ASSERT_TRUE(ind(2) == 0);

//  ASSERT_TRUE(v(0) == v_sorted(2));
//  ASSERT_TRUE(v(1) == v_sorted(1));
//  ASSERT_TRUE(v(2) == v_sorted(0));

//  // check if there are unresolved external symbols in `h5geo::detail::_sort()`
//  h5geo::sort(v.row(0));
//  h5geo::sort(v(Eigen::lastN(2), 0));
//  h5geo::sort(v(Eigen::seq(1,2), 0));
//  h5geo::sort(v(0, Eigen::lastN(2)));
//  h5geo::sort(v(0, Eigen::seq(1,2)));
}

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

TEST_F(H5CoreFixture, interp1MonotonicDecreasing){
  ptrdiff_t n = 5;
  Eigen::VectorXd x = Eigen::VectorXd::LinSpaced(n, 4, 0);
  Eigen::VectorXd y(n);
  y << 1, 2, 1, 5, 3;
  Eigen::VectorXd xnew = Eigen::VectorXd::LinSpaced(2*n+1, 4, -1);

  Eigen::VectorXd ynew = h5geo::interp1Monotonic(x,y,xnew, std::nan("nan"));

  Eigen::VectorXd ynew_expected(2*n-1);
  ynew_expected << 1, 1.5, 2, 1.5, 1, 3, 5, 4, 3;

  ASSERT_TRUE(ynew(Eigen::seq(0, Eigen::last-2)).isApprox(ynew_expected));
  ASSERT_TRUE(std::isnan(ynew(Eigen::last-1)));
  ASSERT_TRUE(std::isnan(ynew(Eigen::last)));
}

TEST_F(H5CoreFixture, interp1MonotonicIncreasing){
  ptrdiff_t n = 5;
  Eigen::VectorXd x = Eigen::VectorXd::LinSpaced(n, 0, 4);
  Eigen::VectorXd y(n);
  y << 1, 2, 1, 5, 3;
  Eigen::VectorXd xnew = Eigen::VectorXd::LinSpaced(2*n+1, 0, 5);

  Eigen::VectorXd ynew = h5geo::interp1Monotonic(x,y,xnew, std::nan("nan"));

  Eigen::VectorXd ynew_expected(2*n-1);
  ynew_expected << 1, 1.5, 2, 1.5, 1, 3, 5, 4, 3;

  ASSERT_TRUE(ynew(Eigen::seq(0, Eigen::last-2)).isApprox(ynew_expected));
  ASSERT_TRUE(std::isnan(ynew(Eigen::last-1)));
  ASSERT_TRUE(std::isnan(ynew(Eigen::last)));
}
