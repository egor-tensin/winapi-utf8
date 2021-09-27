cmake_minimum_required(VERSION 3.5) # for Boost::* imported targets

project(winapi_utf8 CXX)
enable_testing()

include(cmake/common.cmake)

file(GLOB_RECURSE winapi_utf8_include "include/*.hpp")
file(GLOB winapi_utf8_src "src/*.cpp")
add_library(winapi_utf8 ${winapi_utf8_include} ${winapi_utf8_src})
target_include_directories(winapi_utf8 PUBLIC include)
# Vista is the lower bound (due to WC_ERR_INVALID_CHARS):
if(MINGW)
    target_compile_definitions(winapi_utf8 PRIVATE
        NTDDI_VERSION=NTDDI_VISTA
        _WIN32_WINNT=_WIN32_WINNT_VISTA)
endif()
install(TARGETS winapi_utf8 ARCHIVE DESTINATION lib)
install(DIRECTORY include/winapi DESTINATION include)

if(WINAPI_UTF8_TESTS)
    add_subdirectory(test)
endif()

install(FILES LICENSE.txt DESTINATION share)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_INCLUDE_GRAPH NO)
    set(DOXYGEN_INCLUDED_BY_GRAPH NO)
    set(DOXYGEN_SHOW_INCLUDE_FILES NO)
    set(DOXYGEN_SORT_MEMBER_DOCS NO)
    set(DOXYGEN_SOURCE_BROWSER YES)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)
    doxygen_add_docs(docs README.md ${winapi_utf8_include} ${winapi_utf8_src})
endif()
