cmake_minimum_required(VERSION 3.20)
project(BlockchainWork)

set(CMAKE_CXX_STANDARD 14)

## 设置 可执行程序输出目录
set(output_bin  ${PROJECT_SOURCE_DIR}/build/output/bin)
## 设置库文件输出目录
set(output_lib  ${PROJECT_SOURCE_DIR}/build/output/lib)
## 指定可执行程序输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${output_bin})
## 指定 库文件输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${output_lib})

# 添加头文件搜索路径
include_directories(cryptopp/include)
include_directories(blockchain)
include_directories(test)
include_directories(util)
include_directories(./)
link_directories(cryptopp/lib)

# 添加子目录
add_subdirectory(util)
add_subdirectory(blockchain)
add_subdirectory(test)

# 生成制定文件
add_executable(BlockchainWork main.cpp)

# 添加链接库
target_link_libraries(BlockchainWork UTIL_LIBRARY)
target_link_libraries(BlockchainWork BLOCKCHAIN_LIBRARY)
target_link_libraries(BlockchainWork TEST_LIBRARY)
target_link_libraries(BlockchainWork cryptopp)
