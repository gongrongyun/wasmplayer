#include "player.h"

Player::Player()
{
}

Player::~Player()
{
  avformat_close_input(&pFormatCtx);
  av_frame_free(&pFrame);
  av_frame_free(&pFrameYUV);
  avcodec_close(pCodecCtx);
  sws_freeContext(img_convert_ctx);
}

bool Player::prepare(char *url, int cb)
{
  printf("url %s\n", url);
  EM_ASM(console.log("prepare"));
  cb = cb;
  avformat_network_init();
  pFormatCtx = avformat_alloc_context();
  if (avformat_open_input(&pFormatCtx, url, nullptr, nullptr) != 0)
  {
    EM_ASM(console.log('open url error'));
    return false;
  }
  printf("open url success");
  if (avformat_find_stream_info(pFormatCtx, nullptr) < 0)
  {
    EM_ASM(console.log("do not found stream info"));
    return false;
  }

  for (int i = 0; i < pFormatCtx->nb_streams; i++)
  {
    if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
    {
      vStream = pFormatCtx->streams[i];
    }
  }
  if (vStream == nullptr)
  {
    EM_ASM(console.log("video stream not found"));
    return false;
  }
  EM_ASM(console.log("find stream"));
  pCodec = avcodec_find_decoder(vStream->codecpar->codec_id);
  if (pCodec == nullptr)
  {
    EM_ASM(console.log("do not found codec"));
    return false;
  }

  pCodecCtx = avcodec_alloc_context3(pCodec);

  if (avcodec_open2(pCodecCtx, pCodec, nullptr) < 0)
  {
    EM_ASM(console.log("can not open codecContext"));
    return false;
  }
EM_ASM(console.log("start to malloc"));
  pFrame = av_frame_alloc();
  pFrameYUV = av_frame_alloc();
  out_buffer = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1));
  av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1);

  packet = (AVPacket *)av_malloc(sizeof(AVPacket));
  img_convert_ctx = sws_getContext(
      pCodecCtx->width,
      pCodecCtx->height,
      pCodecCtx->pix_fmt,
      pCodecCtx->width,
      pCodecCtx->height,
      AV_PIX_FMT_YUV420P,
      SWS_BICUBIC,
      nullptr,
      nullptr,
      nullptr);
  EM_ASM(console.log("finish"));
  return true;
}

void Player::play()
{
  shoudplay = true;
  while (shoudplay && av_read_frame(pFormatCtx, packet) >= 0)
  {
    if (packet->stream_index == vStream->index)
    {
      avcodec_send_packet(pCodecCtx, packet);
      int ret = avcodec_receive_frame(pCodecCtx, pFrame);
      if (ret == 0)
      {
        sws_scale(
            img_convert_ctx,
            (const unsigned char *const *)pFrame->data,
            pFrame->linesize,
            0,
            pCodecCtx->height,
            pFrameYUV->data,
            pFrameYUV->linesize);
        ((receive *)cb)(pFrameYUV->data, pCodecCtx->width * pCodecCtx->height);
      }
      else if (ret = EAGAIN)
      {
        continue;
      }
      else
      {
        EM_ASM(console.error("decode error", ret));
        break;
      }
    }
    av_packet_unref(packet);
  }
}

void Player::stop()
{
  shoudplay = false;
}