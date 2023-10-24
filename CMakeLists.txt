cmake_minimum_required(VERSION 3.0.0)
project(xtrains VERSION 0.1.0 LANGUAGES C CXX)

# Find the CURL library
find_package(CURL REQUIRED)

# include(CTest)
# enable_testing()

add_executable(xtrains.exe main.cpp)

target_link_libraries(xtrains.exe CURL::libcurl)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
