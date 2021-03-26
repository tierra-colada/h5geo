# Helper for finding magic_enum
find_package(magic_enum CONFIG QUIET)

if(NOT magic_enum_FOUND)
    find_path(MAGIC_ENUM_INCLUDE_DIR magic_enum.hpp 
        HINTS 
        ENV magic_enum_ROOT 
        ENV magic_enum_ROOT_DIR 
        "${CMAKE_CURRENT_SOURCE_DIR}/magic_enum/include/"
        "${magic_enum_ROOT}/include/"
        )

    mark_as_advanced(MAGIC_ENUM_INCLUDE_DIR)

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(magic_enum
        REQUIRED_VARS MAGIC_ENUM_INCLUDE_DIR
        )

    if(magic_enum_FOUND AND NOT TARGET magic_enum::magic_enum)
        add_library(magic_enum::magic_enum INTERFACE IMPORTED)
        set_target_properties(magic_enum::magic_enum PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${MAGIC_ENUM_INCLUDE_DIR}")

        target_compile_features(magic_enum::magic_enum INTERFACE)
    endif()
endif()
