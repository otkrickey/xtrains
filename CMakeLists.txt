cmake_minimum_required(VERSION 3.10)
project(xtrains VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the CURL library
find_package(CURL REQUIRED)

# include(CTest)
# enable_testing()

include_directories(src)

add_executable(xtrains.exe
  src/main.cpp
  src/api.cpp
  src/db.cpp
  src/model.cpp
  src/config.cpp
  src/preprocess.cpp
  src/dijkstra.cpp
  src/ndb.cpp
)

target_link_libraries(xtrains.exe CURL::libcurl)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
