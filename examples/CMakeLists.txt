add_executable(cbt_demo cbt_demo.cpp)
target_link_libraries(cbt_demo PRIVATE cbt::cbt)
target_compile_features(cbt_demo PRIVATE cxx_std_17)

add_executable(mapping_demo mapping_demo.cpp)
target_link_libraries(mapping_demo PRIVATE cbt::cbt)
target_compile_features(mapping_demo PRIVATE cxx_std_17)