cmake_minimum_required (VERSION 2.8)
project(kcm_imchooser)
set(MAJOR_VERSION "0")
set(MINOR_VERSION "1")
set(PATCH_VERSION "0")
set(SUFFIX_VERSION "")

set(VERSION_STRING "${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")
if(NOT SUFFIX_VERSION)
    set(VERSION_STRING_FULL "${VERSION_STRING}")
else(NOT SUFFIX_VERSION)
    set(VERSION_STRING_FULL "${VERSION_STRING}-${SUFFIX_VERSION}")
endif(NOT SUFFIX_VERSION)

find_package(ECM 0.0.11 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(KF5 REQUIRED COMPONENTS KDELibs4Support KIO DBusAddons)

set(CMAKE_C_FLAGS "-Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -fvisibility=hidden ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -fvisibility=hidden ${CMAKE_CXX_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined,--as-needed ${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "-Wl,--no-undefined,--as-needed ${CMAKE_MODULE_LINKER_FLAGS}")

add_subdirectory(src)
add_subdirectory(po)