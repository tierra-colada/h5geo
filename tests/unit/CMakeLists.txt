set(src_files_test
  main.cpp
  test_h5map.cpp
  test_h5well.cpp
  test_h5seis.cpp
  test_h5core.cpp
  )

set(resource_files
  ../pytest/data/well_dev
  ../pytest/data/well_las
  )

add_executable(H5GeoTest
  ${src_files_test}
  ${resource_files}
  )

set_target_properties(H5GeoTest PROPERTIES
  RESOURCE "${resource_files}")

file(COPY "../pytest/data"
  DESTINATION ${CMAKE_BINARY_DIR})

add_compile_definitions(TEST_DATA_DIR="${CMAKE_BINARY_DIR}/data")

target_link_libraries(H5GeoTest 
  PRIVATE ${GMOCK_BOTH_LIBRARIES}
  PRIVATE gtest_main
  PRIVATE gmock_main
  PRIVATE h5geo
  )

target_link_libraries(H5GeoTest PRIVATE magic_enum::magic_enum)
target_link_libraries(H5GeoTest PRIVATE mio::mio)
target_link_libraries(H5GeoTest PRIVATE units::units)

if(H5GEO_USE_THREADS)
  target_include_directories(H5GeoTest PRIVATE ${TBB_INCLUDE_DIRS})
  target_link_libraries(H5GeoTest PRIVATE ${TBB_LIBRARIES_RELEASE})
  target_link_libraries(H5GeoTest PRIVATE OpenMP::OpenMP_CXX)
endif()

if(H5GEO_USE_GDAL)
  target_link_libraries(H5GeoTest PRIVATE GDAL::GDAL)
  target_link_libraries(H5GeoTest PRIVATE ${GDAL_LIBS})  # must be linked or undef ref to GEOS
  target_include_directories(H5GeoTest PRIVATE ${GDAL_TOP_LEVEL_INCLUDE_DIR})
endif()

add_test(
  NAME H5GeoTest
  COMMAND H5GeoTest
  )
