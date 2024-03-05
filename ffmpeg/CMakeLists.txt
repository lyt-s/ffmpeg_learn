# 这里要注意 GLOB 和 GLOB_RECURSE 的区别
file(GLOB sources CONFIGURE_DEPENDS *.cpp) 
foreach (source IN ITEMS ${sources})
    get_filename_component(name "${source}" NAME_WLE)
    add_executable(${name} ${name}.cpp)
    target_include_directories(${name} PRIVATE ${ffmpeg_INCLUDE_DIRS})
    target_link_libraries(${name} PRIVATE  avformat avcodec avutil swscale)
endforeach()


