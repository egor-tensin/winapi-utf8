find_package(Boost REQUIRED COMPONENTS unit_test_framework)

add_executable(unit_tests main.cpp convert.cpp string.cpp)
target_link_libraries(unit_tests PRIVATE winapi_utf8)
target_link_libraries(unit_tests PRIVATE Boost::disable_autolinking Boost::unit_test_framework)

install(TARGETS unit_tests RUNTIME DESTINATION bin/test)
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    install(FILES "$<TARGET_PDB_FILE:unit_tests>" DESTINATION bin/test OPTIONAL)
endif()
