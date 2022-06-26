#include <gtest/gtest.h>

#ifdef H5GEO_USE_GDAL
#include <gdal_priv.h>
#endif

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  #ifdef H5GEO_USE_GDAL
  // must be called before reading files to initialize data readers
  GDALAllRegister();
  #endif
  return RUN_ALL_TESTS();
}
