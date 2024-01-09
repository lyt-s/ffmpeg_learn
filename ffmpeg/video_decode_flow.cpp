#include <stdio.h>

#include <cstddef>
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

#include "libavcodec/codec.h"
#include "libavcodec/codec_par.h"
#include "libavcodec/packet.h"
}

int main() {
  std::string filePath = "../video/juren-5s.mp4";  // 文件地址
  int videoStreamIndex = -1;                       // 视频流所在流序列中的索引
  int ret = 0;                                     // 默认返回值

  // 需要的变量名并初始化
  AVFormatContext *fmtCtx = nullptr;
  AVPacket *pkt = nullptr;
  AVCodecContext *codecCtx = nullptr;
  AVCodecParameters *avCodecPara = nullptr;

  const AVCodec *codec = nullptr;

  do {
    //---------创建AVFormatContext 结构体--------------------------------
    // 分配一个AVFormatContext ，ffmpeg 所有操作都要通过这个AVFormatContext来进行
    fmtCtx = avformat_alloc_context();
    //----------------打开文件--------------------------------
    if ((ret = avformat_open_input(&fmtCtx, filePath.c_str(), NULL, NULL)) != 0) {
      printf("can not open video file %s\n", filePath.c_str());
      break;
    }

    // 循环查找视频中包含的流信息，直到找到视频类型的流
    // 便将其记录下来，保存到videoStreamIndex变量中
    for (size_t i = 0; i < fmtCtx->nb_streams; i++) {
      if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
        videoStreamIndex = i;
        break;  // 找到视频流就退出
      }
    }

    // 如果 videoStream 为-1, 说明没有找到视频流
    if (videoStreamIndex == -1) {
      printf("cannot find video stream\n");
      break;
    }

    // 打印输入和输出的信息， 长度， 比特率，流格式 等
    av_dump_format(fmtCtx, 0, filePath.c_str(), 0);

    //------------------查找解码器---------------------------
    avCodecPara = fmtCtx->streams[videoStreamIndex]->codecpar;
    //
    codec = avcodec_find_decoder(avCodecPara->codec_id);

    if (codec == NULL) {
      printf("cannot find decoder\n");
      break;
    }
    // 根据解码器参数来创建解码器内容
    codecCtx = avcodec_alloc_context3(codec);

    avcodec_parameters_to_context(codecCtx, avCodecPara);
    if (codecCtx == NULL) {
      printf("cannot alloc context\n");
      break;
    }

    //----------------打开解码器---------------------------
    if ((ret = avcodec_open2(codecCtx, codec, NULL)) < 0) {
      printf("cannot open decoder\n");
      break;
    }

    //---------------- 分配AVPacket结构体---------------------
    int i = 0;                                               // 用于帧计数
    pkt = av_packet_alloc();                                 // 分配一个packet
    av_new_packet(pkt, codecCtx->width * codecCtx->height);  // 调整packet数据

    // ----------------读取视频信息-------------------------------------
    while (av_read_frame(fmtCtx, pkt) >= 0) {
      if (pkt->stream_index == videoStreamIndex) {
        i++;  // 只计算视频帧
      }
      av_packet_unref(pkt);
    }
    printf("there are %d frames int total.\n", i);

  } while (0);
  //-----------释放所有指针-----------------------------------
  av_packet_free(&pkt);
  avcodec_close(codecCtx);
  avformat_close_input(&fmtCtx);
  avformat_free_context(fmtCtx);
  return ret;
}