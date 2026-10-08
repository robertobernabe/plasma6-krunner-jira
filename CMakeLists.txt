cmake_minimum_required(VERSION 3.16)

project(jirarunner)

set(QT_MIN_VERSION "6.5.0")
set(KF6_MIN_VERSION "6.0.0")

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)
include(CTest)

find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Gui Widgets)
find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS Runner I18n CoreAddons KCMUtils Config)

add_subdirectory(src)

if(BUILD_TESTING)
    find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Test)
    add_subdirectory(autotests)
endif()

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
