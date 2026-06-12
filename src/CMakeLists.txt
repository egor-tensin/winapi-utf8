file(GLOB_RECURSE winapi_utf8_include CONFIGURE_DEPENDS "../include/*.hpp")
file(GLOB winapi_utf8_src CONFIGURE_DEPENDS "*.cpp")
add_library(winapi_utf8 ${winapi_utf8_include} ${winapi_utf8_src})

target_include_directories(winapi_utf8 PUBLIC ../include)
# Vista is the lower bound (due to WC_ERR_INVALID_CHARS):
if(MINGW)
    target_compile_definitions(winapi_utf8 PRIVATE
        NTDDI_VERSION=NTDDI_VISTA
        _WIN32_WINNT=_WIN32_WINNT_VISTA)
endif()
install(TARGETS winapi_utf8 ARCHIVE DESTINATION lib)
install(DIRECTORY ../include/winapi DESTINATION include)
