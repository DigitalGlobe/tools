#ifndef ffmpegApi_HEADER
#define ffmpegApi_HEADER

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdio.h>
#if defined(FMPEG_SINGLE_INCLUDE)
#include <ffmpeg/avcodec.h>
#include <ffmpeg/avformat.h>
#include <ffmpeg/avdevice.h>
#include <ffmpeg/swscale.h>
#else
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#endif
#ifdef __cplusplus
}
#endif

   
#endif
