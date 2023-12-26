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
  src/api.cpp
  src/config.cpp
  src/db.cpp
  src/database.cpp
  src/dijkstra.cpp
  src/main.cpp
  src/model.cpp
  src/preprocess.cpp
  src/serialization.cpp
)

target_link_libraries(xtrains.exe CURL::libcurl)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
