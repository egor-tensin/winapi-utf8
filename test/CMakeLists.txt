find_package(Boost REQUIRED COMPONENTS unit_test_framework)

file(GLOB unit_tests_src "*.cpp")
add_executable(unit_tests ${unit_tests_src})
set_target_properties(unit_tests PROPERTIES OUTPUT_NAME winapi-utf8-unit-tests)
target_link_libraries(unit_tests PRIVATE winapi_utf8)
target_link_libraries(unit_tests PRIVATE Boost::disable_autolinking Boost::unit_test_framework)
install(TARGETS unit_tests RUNTIME DESTINATION bin)
install_pdbs(TARGETS unit_tests DESTINATION bin)

add_test(NAME unit_tests COMMAND unit_tests)
set_tests_properties(unit_tests PROPERTIES
                     PASS_REGULAR_EXPRESSION "No errors detected")
