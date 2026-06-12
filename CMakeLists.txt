# oldstable as of May '26.
# Plus, I'm pretty sure it's required for things like CONFIGURE_DEPENDS and
# CMAKE_MSVC_RUNTIME_LIBRARY and stuff.
cmake_minimum_required(VERSION 3.25)

project(winapi_utf8 CXX)
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if(MSVC)
    add_compile_options(/MP /W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

add_compile_definitions(WIN32_LEAN_AND_MEAN NOMINMAX)

add_subdirectory(src)
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
    doxygen_add_docs(winapi_utf8_docs
        README.md
        ${winapi_utf8_include}
        ${winapi_utf8_src})
endif()
