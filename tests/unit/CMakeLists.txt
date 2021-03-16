set(src_files_test
  main.cxx
  core_test.cxx
  h5surf_test.cxx
  h5well_test.cxx
  h5seis_test.cxx
  )

set(resource_files
  data/well.dev
  data/well.las
  )

add_executable(H5GeoTest
  ${src_files_test}
  ${resource_files}
  )

set_target_properties(H5GeoTest PROPERTIES
  RESOURCE "${resource_files}")

file(COPY "data"
  DESTINATION ${CMAKE_BINARY_DIR})

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
