#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "rpi_video.h"

int main(int argc, char *argv[])
{
  // You're constructing a command-line for raspividyuv; see that
  // program for documentation. You can also addd --help to the string
  // below and the supported options will print out here.
  // Note rpi cam 2.0 is capable of 3280x2464, 1920x1232
  //
  //  rpi_video_t *rpivid = rpi_video_create("--help");
  //  rpi_video_t *rpivid = rpi_video_create("--width 1920 --height 1232 --fps 5 --y8");
  rpi_video_t *rpivid = rpi_video_create("--width 1280 --height 720 --fps 10 --y8");

  // You can record a "movie" of the raw images. The resulting file
  // can be played with vlc, by customizing the line below with the correct resolution.
  //
  // vlc --demux rawvideo --rawvid-fps 5 --rawvid-width 1280
  // --rawvid-height 720 --rawvid-chroma Y800 <input file>
  //
  // Note that depending on whether you use --rgb, --yuv, or --y8, you
  // need to specify a different --rawvid-chroma flag:
  //
  // rpi-video-format     vlc --rawvid-chroma
  // yuv (0)           I420
  // y8  (1)           Y800
  // rgb (2)           RGB8
  FILE *f = NULL; // fopen("/tmp/out.yuv", "w");

  printf("initializing...\n ");
  rpi_video_start(rpivid);
  
  for (int framecount = 0; 1; framecount++) {

    struct rpi_video_frame *frame = rpi_video_get_frame(rpivid);

    // minimize latency by discarding any old frames that are pending.
    if (rpi_video_get_frames_pending(rpivid) > 1) {
      printf("dropping frame\n");
      rpi_video_put_frame(rpivid, frame);
      continue;
    }

    // process the frame
    printf("[frame %4d] %d %d, %d bytes, format %d\n",
	   framecount, frame->width, frame->height, frame->length, frame->format);

    // write the image
    if (f && frame->length != fwrite(frame->data, 1, frame->length, f)) {
      perror("write");
    }

    rpi_video_put_frame(rpivid, frame);
  }
}
