#include <stdio.h>

#include <iostream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavfilter/avfilter.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/ffversion.h>
#include <libpostproc/postprocess.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
}

int main() {
  AVFormatContext* fmt_ctx = avformat_alloc_context();  // 创建对象并初始化
  int ret = 0;
  // const char* fileName = "../video/juren.mp4";  // 文件地址
  std::string fileName = "../video/juren-5s.mp4";  // 文件地址
  do {
    // 打开文件
    if ((ret = avformat_open_input(&fmt_ctx, fileName.c_str(), NULL, NULL)) < 0)
      break;  // Cannot open video file

    // 查找流信息（音频流和视频流）
    if ((ret = avformat_find_stream_info(fmt_ctx, NULL)) < 0) {
      printf("Cannot find stream information\n");
      break;
    }

    av_dump_format(fmt_ctx, 0, fileName.c_str(), 0);  // 输出视频信息
  } while (0);

  avformat_close_input(&fmt_ctx);  // 关闭文件

  return ret;
}