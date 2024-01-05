
add_executable(main main.cpp)

target_include_directories(main PRIVATE ${ffmpeg_INCLUDE_DIRS})
target_link_libraries(main PRIVATE  avformat avcodec avutil)
