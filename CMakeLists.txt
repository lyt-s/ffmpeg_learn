
# cmake -G Ninja -S . -B ./build -DUSE_AVX_INSTRUCTIONS=1 -DCMAKE_INSTALL_PREFIX=$THIRD_PARTY_DIR/dlib
# cmake --build ./build || RET=$?
# cmake --build ./build --target install || RET=$?

   
cmake_minimum_required(VERSION 3.18)

if (NOT CMAKE_BUILD_TYPE)
    # set(CMAKE_BUILD_TYPE Release)
    set(CMAKE_BUILD_TYPE Debug)

endif()
set(CMAKE_CXX_STANDARD 20)

##设置 生成compile_commands.json 提供跳转功能 
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(ffmpeg_learn LANGUAGES CXX)
#----------------------------------------------------------------

include(cmake/FindPackages.cmake)
# 有问题
# include(cmake/FindFFmpeg.cmake) 
#----------------------------------------------------------------
add_subdirectory(ffmpeg)