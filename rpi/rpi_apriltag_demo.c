#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "rpi_video.h"
#include "apriltag.h"
#include "tag36h11.h"
#include "tagStandard41h12.h"

/*
  Build the apriltag distribution first...

  vlc --demux rawvideo --rawvid-fps 5 --rawvid-width 320 --rawvid-height 240 --rawvid-chroma I420 out.yuv

  rpi-video-format     vlc --rawvid-chroma
  yuv (0)           I420
  y8  (1)           Y800
  rgb (2)           RGB8


  Note rpi cam 2.0 is capable of 3280x2464, 1920x1232
*/


int main(int argc, char *argv[])
{
  // You're constructing a command-line for raspividyuv; see that
  // program for documentation. You can also addd --help to the string
  // below and the supported options will print out here.
  // Note rpi cam 2.0 is capable of 3280x2464, 1920x1232
  //
  // ** For correct operation with apriltag, MUST HAVE --y8 **
  //
  //  rpi_video_t *rpivid = rpi_video_create("--width 1920 --height 1232 --fps 30 --y8");
  rpi_video_t *rpivid = rpi_video_create("--width 1280 --height 720 --fps 40 --y8");

  // You can record a "movie" of the raw images. The resulting file
  // can be played with vlc, by customizing the line below with the correct resolution.
  //
  // vlc --demux rawvideo --rawvid-fps 5 --rawvid-width 1280
  // --rawvid-height 720 --rawvid-chroma Y800 <input file>

    FILE *f = NULL; // fopen("/tmp/out.yuv", "w");
  
  printf("initializing...\n");
  rpi_video_start(rpivid);

  apriltag_detector_t *det = apriltag_detector_create();
  det->nthreads = 4;
  det->quad_decimate = 2;
  
  apriltag_detector_add_family_bits(det, tag36h11_create(), 0);
  apriltag_detector_add_family_bits(det, tagStandard41h12_create(), 0);
  
  for (int framecount = 0; 1; framecount++) {

    struct rpi_video_frame *frame = rpi_video_get_frame(rpivid);

    if (rpi_video_get_frames_pending(rpivid) > 1) {
      printf("dropping frame\n");
      rpi_video_put_frame(rpivid, frame);
      continue;
    }

    // do something
    printf("[frame %4d] %d %d, %d bytes, format %d\n",
	   framecount, frame->width, frame->height, frame->length, frame->format);

    
    if (f && frame->length != fwrite(frame->data, 1, frame->length, f)) {
      perror("write");
    }

    image_u8_t im = { .width = frame->width,
		      .height = frame->height,
		      .stride = frame->width,
		      .buf = frame->data };
    
    zarray_t *detections = apriltag_detector_detect(det, &im);

    int quiet = 0;

    for (int i = 0; i < zarray_size(detections); i++) {
      apriltag_detection_t *det;
      zarray_get(detections, i, &det);
      
      if (!quiet)
	printf("   %3d: id (%2dx%2d)-%-4d, hamming %d, margin %8.3f\n",
	       i, det->family->nbits, det->family->h, det->id, det->hamming, det->decision_margin);
    }


    /*    if (!quiet) {
      timeprofile_display(det->tp);
      } */

    double t =  timeprofile_total_utime(det->tp) / 1.0E3;
    printf("detected %3d tags from %4d quads in %8.1f ms\n",
	   zarray_size(detections), det->nquads, t);

    apriltag_detections_destroy(detections);

    rpi_video_put_frame(rpivid, frame);
    printf("\n");
  }
}
