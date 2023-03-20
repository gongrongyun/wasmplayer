#ifndef __PLAYER__
#define __PLAYER__

#include <stdio.h>
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>

typedef void receive(uint8_t **data, int y_size);
};

#include <emscripten/emscripten.h>

class Player {
private:
  char *url;
  AVFormatContext *pFormatCtx;
  AVCodecContext *pCodecCtx;
  AVCodec const *pCodec;
  AVFrame *pFrame, *pFrameYUV;
  unsigned char *out_buffer;
  AVPacket *packet;
  AVStream *vStream;
  struct SwsContext *img_convert_ctx;

  bool shoudplay;
  int cb;

public:
  Player();
  ~Player();
  bool prepare(char *url, int cb);
  void play();
  void stop();
};

#endif