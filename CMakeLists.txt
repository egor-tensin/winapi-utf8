cmake_minimum_required(VERSION 3.5) # for Boost::* imported targets

project(winapi_utf8 CXX)

include(cmake/common.cmake)

file(GLOB_RECURSE winapi_utf8_include "include/*.hpp")
file(GLOB winapi_utf8_src "src/*.cpp")
add_library(winapi_utf8 ${winapi_utf8_include} ${winapi_utf8_src})
target_include_directories(winapi_utf8 PUBLIC include/)

install(TARGETS winapi_utf8 ARCHIVE DESTINATION lib)
install(DIRECTORY include/winapi DESTINATION include)

if(WINAPI_UTF8_ENABLE_TESTS)
    add_subdirectory(test)
endif()

install(FILES LICENSE.txt DESTINATION share)
