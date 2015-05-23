#
# CMake listfile to specify the build process, see:
# http://www.cmake.org/cmake/help/documentation.html
#
project(zxing)
cmake_minimum_required(VERSION 2.8.0)

# Adjust CMake's module path.
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)

# Add libzxing library.
file(GLOB_RECURSE LIBZXING_FILES
    "./core/src/*.cpp"
    "./core/src/*.h"
    "./core/src/*.cc"
    "./core/src/*.hh"
)

include_directories("./core/src/")
add_library(libzxing STATIC ${LIBZXING_FILES})
set_target_properties(libzxing PROPERTIES PREFIX "")
find_package(Iconv)
if(ICONV_FOUND)
    include_directories(${ICONV_INCLUDE_DIR})
    target_link_libraries(libzxing ${ICONV_LIBRARIES})
else()
    add_definitions(-DNO_ICONV=1)
endif()
add_definitions(-DNDEBUG)