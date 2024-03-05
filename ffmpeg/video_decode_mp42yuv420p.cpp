#include <stdio.h>
#include <stdlib.h>

#include <cstddef>
#include <ios>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavcodec/codec.h"
#include "libavcodec/codec_par.h"
#include "libavcodec/packet.h"
#include "libavfilter/avfilter.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/ffversion.h"
#include "libavutil/frame.h"
#include "libavutil/imgutils.h"
#include "libpostproc/postprocess.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
}
// h264 mp4 811k
// yuv420p 351MB
// 4.3 MB -> 904MB

int main() {
  FILE *fp = fopen("result_420p.yuv", "w+b");
  if (fp == NULL) {
    printf("Cannot open result file\n");
    return -1;
  }

  std::string filePath = "../video/test_video.mp4";  // 文件地址
  int videoStreamIndex = -1;                         // 视频流所在的索引
  int ret = 0;

  // 需要的变量名并初始化
  // 封装格式上下文结构体，也是统领全局的结构体，保存了视频文件封装
  AVFormatContext *fmtCtx = nullptr;
  // 存储一帧压缩编码数据。管理压缩后的数据，通常是一帧的数据。
  AVPacket *pkt = nullptr;
  // 编码器上下文结构体，保存了视频（音频）编解码相关信息。
  AVCodecContext *codecCtx = nullptr;
  // 编解码参数，每个AVStream中都含有一个AVCodecParameters，用来存放当前流的编解码参数。
  AVCodecParameters *avCodecPara = nullptr;
  // 每种视频（音频）编解码器(例如H.264解码器)对应一个该结构体。
  const AVCodec *codec = nullptr;
  // 存储一帧解码后像素（采样）数据。
  AVFrame *yuvFrame = av_frame_alloc();

  do {
    //-------------创建AVFormatContext结构体--------------------------------------
    // 分配一个AVFormatContext，FFMPEG所有的操作都要通过这个AVFormatContext来进行
    fmtCtx = avformat_alloc_context();

    // ------------打开文件--------------------------------
    if ((ret = avformat_open_input(&fmtCtx, filePath.c_str(), NULL, NULL)) != 0) {
      printf("cannot open video file: %s\n", filePath.c_str());
      break;
    }

    // 循环查找视频中包含的流信息，直到找到视频类型的流
    // 便将其记录下来 保存到videoStreamIndex变量中
    for (size_t i = 0; i < fmtCtx->nb_streams; i++) {
      if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
        videoStreamIndex = i;
        break;  // 找到视频流就退出
      }
    }

    // 如果videoStream为-1 说明没有找到视频流
    if (videoStreamIndex == -1) {
      printf("can not find video stream\n");
      break;
    }

    // 打印输入和输出信息：长度 比特率 流格式等
    av_dump_format(fmtCtx, 0, filePath.c_str(), 0);

    // 查找解码器
    avCodecPara = fmtCtx->streams[videoStreamIndex]->codecpar;
    codec = avcodec_find_decoder(avCodecPara->codec_id);
    if (codec == NULL) {
      printf("cannot find decoder\n");
      break;
    }
    // 根据解码器参数来创建解码器内容
    codecCtx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecCtx, avCodecPara);
    if (codecCtx == NULL) {
      printf("Cannot alloc context.");
      break;
    }

    //  打开解码器
    // avcodec_open2 函数会根据传入的AVCodecContext
    // 结构体中的参数，选择并打开对应的音视频编解码器。这些参数包括编解码器的ID、解码器选项、输入/输出格式等。函数会检查是否成功打开编解码器，并将编解码器的上下文信息填充到AVCodecContext
    // 结构体中，以供后续的音视频处理使用。
    if ((ret = avcodec_open2(codecCtx, codec, NULL)) < 0) {
      // 具体采用什么解码器ffmpeg经过封装 我们无须知道
      printf("cannot open decoder\n");
      break;
    }
    int w = codecCtx->width;
    int h = codecCtx->height;
    printf("width: %d, height: %d\n", w, h);
    // 分配AVPacket结构体
    pkt = av_packet_alloc();                                 // 分配一个packet
    av_new_packet(pkt, codecCtx->width * codecCtx->height);  // 调整packet的数据

    //===========================  读取视频信息 ===============================//

    int frameCnt = 0;  // 帧数
    // 从 AVFormatContext 容器里面读取一个 AVPacket，需要注意，虽然函数名是 frame，但是读取的是
    // AVPacket。
    while (av_read_frame(fmtCtx, pkt) >= 0) {
      if (pkt->stream_index == videoStreamIndex) {
        // 往 AVCodecContext 解码器 发送一个 AVPacket 。
        //
        if (avcodec_send_packet(codecCtx, pkt) == 0) {
          // 从 AVCodecContext 解码器 读取一个 AVFrame。
          while (avcodec_receive_frame(codecCtx, yuvFrame) == 0) {
            fwrite(yuvFrame->data[0], 1, w * h, fp);      // y
            fwrite(yuvFrame->data[1], 1, w * h / 4, fp);  // u
            fwrite(yuvFrame->data[2], 1, w * h / 4, fp);  // v

            printf("save frame %d to file.\n", frameCnt++);
            fflush(fp);
          }
        }
      }
      av_packet_unref(pkt);  // 重置pkt的内容
    }
  } while (0);
  //===========================释放所有指针===============================//
  av_packet_free(&pkt);
  avcodec_close(codecCtx);
  avformat_close_input(&fmtCtx);
  avformat_free_context(fmtCtx);
  av_frame_free(&yuvFrame);

  return ret;
}