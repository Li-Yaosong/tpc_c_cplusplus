# tests/CMakeLists.txt
add_executable(test_ipv4 test_ipv4.cc)  # 修改目标名称

# 使用现代方式设置标准
target_compile_features(test_ipv4 PRIVATE cxx_std_20)

# 添加静态链接
target_link_options(test_ipv4 PRIVATE -static)

# 如果有需要，添加宽松选项
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(test_ipv4 PRIVATE -fpermissive)
endif()

# 确保正确链接库
target_link_libraries(test_ipv4 PRIVATE xnet.headers)

add_test(NAME ipv4_all_tests COMMAND test_ipv4)  # 更新命令名称