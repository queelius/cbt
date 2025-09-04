add_executable(test_cbt test_cbt.cpp)
target_link_libraries(test_cbt PRIVATE cbt::cbt)
target_compile_features(test_cbt PRIVATE cxx_std_17)

# Comprehensive test suite
add_executable(test_cbt_comprehensive test_cbt_comprehensive.cpp)
target_link_libraries(test_cbt_comprehensive PRIVATE cbt::cbt)
target_compile_features(test_cbt_comprehensive PRIVATE cxx_std_17)

# Enable code coverage if requested
option(ENABLE_COVERAGE "Enable code coverage" OFF)
if(ENABLE_COVERAGE)
    target_compile_options(test_cbt PRIVATE --coverage -g -O0)
    target_link_options(test_cbt PRIVATE --coverage)
    target_compile_options(test_cbt_comprehensive PRIVATE --coverage -g -O0)
    target_link_options(test_cbt_comprehensive PRIVATE --coverage)
endif()

add_test(NAME cbt_tests COMMAND test_cbt)
add_test(NAME cbt_comprehensive_tests COMMAND test_cbt_comprehensive)