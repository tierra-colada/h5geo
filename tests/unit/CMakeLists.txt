set(src_files_test
  main.cpp
  h5surf_test.cpp
  h5well_test.cpp
  h5seis_test.cpp
  )

set(resource_files
  data/well_dev
  data/well_las
  )

add_executable(H5GeoTest
  ${src_files_test}
  ${resource_files}
  )

set_target_properties(H5GeoTest PROPERTIES
  RESOURCE "${resource_files}")

file(COPY "data"
  DESTINATION ${CMAKE_BINARY_DIR})

add_compile_definitions(TEST_DATA_DIR="${CMAKE_BINARY_DIR}/data")

target_link_libraries(H5GeoTest 
  PRIVATE ${GMOCK_BOTH_LIBRARIES}
  PRIVATE gtest_main
  PRIVATE gmock_main
  PRIVATE h5geo
  )

add_test(
  NAME H5GeoTest
  COMMAND H5GeoTest
  )
