cmake_minimum_required(VERSION 2.8)
cmake_policy(VERSION 2.8)

project(FADTTS)
option(BUILD_TESTING "tests" ON)
option(CREATE_BUNDLE "Create MACOSX_BUNDLE" OFF)

add_subdirectory(src)