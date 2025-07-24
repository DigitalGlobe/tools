set(VERSION 1.7.5)
#
# Modify REF to latest commit id from https://github.com/apache/apr
# Update SHA512 with actual SHA512
#
vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO apache/apr
    REF 6445e8804008922f8018aa238aa4d6bba608c49a
    SHA512 0
    HEAD_REF 1.7.x
)

if (VCPKG_TARGET_IS_WINDOWS)
    vcpkg_check_features(OUT_FEATURE_OPTIONS FEATURE_OPTIONS
        FEATURES
            private-headers INSTALL_PRIVATE_H
    )

    string(COMPARE EQUAL "${VCPKG_LIBRARY_LINKAGE}" "static" APR_BUILD_STATIC)
    string(COMPARE EQUAL "${VCPKG_LIBRARY_LINKAGE}" "dynamic" APR_BUILD_SHARED)

    vcpkg_cmake_configure(
        SOURCE_PATH "${SOURCE_PATH}"
        OPTIONS
            -DAPR_BUILD_STATIC=${APR_BUILD_STATIC}
            -DAPR_BUILD_SHARED=${APR_BUILD_SHARED}
            -DAPR_BUILD_TESTAPR=OFF
            -DINSTALL_PDB=OFF
            -DAPR_INSTALL_PRIVATE_H=${INSTALL_PRIVATE_H}
            -DAPR_INSTALL_INCLUDE_DIR=include/apr-1
    )

    vcpkg_cmake_install()
    vcpkg_copy_pdbs()
    vcpkg_cmake_config_fixup(PACKAGE_NAME "apr"
                             CONFIG_PATH "lib/cmake/apr")

    file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")
else()
    # In development
endif()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

# Handle copyright
file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)

