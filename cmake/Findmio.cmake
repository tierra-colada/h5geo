# Helper for finding mio
find_package(mio CONFIG QUIET)

if(NOT mio_FOUND)
    find_path(mio_INCLUDE_DIR mio/mmap.hpp
        HINTS
        ENV mio_ROOT
        ENV mio_ROOT_DIR
        "${CMAKE_CURRENT_SOURCE_DIR}/mio/include/"
        "${mio_ROOT}/include/"
        )

    mark_as_advanced(mio_INCLUDE_DIR)

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(mio
        REQUIRED_VARS mio_INCLUDE_DIR
        )

    if(mio_FOUND AND NOT TARGET mio::mio)
        add_library(mio::mio INTERFACE IMPORTED)
        set_target_properties(mio::mio PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${mio_INCLUDE_DIR}")

        target_compile_features(mio::mio INTERFACE)
    endif()
endif()
