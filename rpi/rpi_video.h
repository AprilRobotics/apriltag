#ifndef RPI_VIDEO_H
#define RPI_VIDEO_H

#include <stdint.h>

struct rpi_video_frame
{
  int width;
  int height;
  int format; // 0 = yuv, 1 = y8, 2 = rgb8
  
  uint8_t *data;
  int length;

  uint64_t utime;
};
  
typedef struct rpi_video rpi_video_t;

rpi_video_t *rpi_video_create(const char *cmdline);

int rpi_video_start(rpi_video_t *rpivid);

// can be used to get the number of queued up frames-- useful if
// processing is falling behind real time, user can rapidly call
// get_frame/put_frame to catch up.
int rpi_video_get_frames_pending(rpi_video_t *rpivid);

// blocks until a frame is available. The memory belongs to the
// rpi_video driver and is only safe to use up until put_frame is
// called.
struct rpi_video_frame *rpi_video_get_frame(rpi_video_t *rpivid);

// caller MUST return the frame when they're done. 
void rpi_video_put_frame(rpi_video_t *rpivid, struct rpi_video_frame *frame);


#endif
