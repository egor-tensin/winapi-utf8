cmake_minimum_required(VERSION 3.5) # for Boost::* imported targets

project(winapi_utf8 CXX)

enable_testing()

include(cmake/common.cmake)

file(GLOB_RECURSE winapi_utf8_include "include/*.hpp")
file(GLOB winapi_utf8_src "src/*.cpp")
add_library(winapi_utf8 ${winapi_utf8_include} ${winapi_utf8_src})
target_include_directories(winapi_utf8 PUBLIC include)

add_subdirectory(3rdparty/microsoft/SafeInt EXCLUDE_FROM_ALL)
target_link_libraries(winapi_utf8 PRIVATE SafeInt)

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
