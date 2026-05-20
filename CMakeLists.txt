cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 选项：是否构建为共享库，共享库设为 ON；静态库设为 OFF
option(BUILD_SHARED_LIBS "Build libraries as shared" OFF)

# 获取SFML源码并构建（版本 3.0.1）
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main src/main.cpp src/Game.cpp src/Tetris.cpp)
target_include_directories(main PRIVATE include)
target_compile_features(main PRIVATE cxx_std_17)

# 添加优化标志，减少杀毒软件误报
if(MSVC)
  target_compile_options(main PRIVATE /O2 /W4)
else()
  target_compile_options(main PRIVATE -O2 -Wall -Wextra)
endif()

target_link_libraries(main PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network)

# 在Windows系统中（使用 MinGW-w64）构建时
# 静态链接需要将libstdc++、libgcc编译进可执行文件中，避免运行时依赖外部 DLL
if(WIN32 AND (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")))
  target_link_libraries(main PRIVATE
      -static-libgcc
      -static-libstdc++
    )  
endif()

enable_testing()
add_test(NAME SmokeTest
         COMMAND main
         WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

install(TARGETS main RUNTIME DESTINATION .)
install(DIRECTORY "data/"
        DESTINATION "data"
        FILES_MATCHING PATTERN "*"
)

set(CPACK_PACKAGE_NAME "CMakeSFMLTest")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VENDOR "HDU")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "CMake-SFML-Test")
set(CPACK_PACKAGE_CONTACT "contact@example.com")
include(CPack)