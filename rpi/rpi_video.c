/*
Copyright (c) 2018, Raspberry Pi (Trading) Ltd.
Copyright (c) 2014, DSP Group Ltd.
Copyright (c) 2014, James Hughes
Copyright (c) 2013, Broadcom Europe Ltd.

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holder nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * \file RaspiVidYUV.c
 * Command line program to capture a camera video stream and save file
 * as uncompressed YUV420 data
 * Also optionally display a preview/viewfinder of current camera input.
 *
 * Description
 *
  * 2 components are created; camera and preview.
 * Camera component has three ports, preview, video and stills.
 * Preview is connected using standard mmal connections, the video output
 * is written straight to the file in YUV 420 format via the requisite buffer
 * callback. Still port is not used
 *
 * We use the RaspiCamControl code to handle the specific camera settings.
 * We use the RaspiPreview code to handle the generic preview
*/

// We use some GNU extensions (basename)
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <sysexits.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "bcm_host.h"
#include "interface/vcos/vcos.h"

#include "interface/mmal/mmal.h"
#include "interface/mmal/mmal_logging.h"
#include "interface/mmal/mmal_buffer.h"
#include "interface/mmal/util/mmal_util.h"
#include "interface/mmal/util/mmal_util_params.h"
#include "interface/mmal/util/mmal_default_components.h"
#include "interface/mmal/util/mmal_connection.h"

#include "RaspiCommonSettings.h"
#include "RaspiCamControl.h"
#include "RaspiPreview.h"
#include "RaspiCLI.h"
#include "RaspiHelpers.h"

#include "mtqueue.h"
#include "rpi_video.h"

struct rpi_video
{
  char *args;
  mtqueue_t *mtq;
};

#include <semaphore.h>

// Standard port setting for the camera component
#define MMAL_CAMERA_PREVIEW_PORT 0
#define MMAL_CAMERA_VIDEO_PORT 1
#define MMAL_CAMERA_CAPTURE_PORT 2

// Video format information
// 0 implies variable
#define VIDEO_FRAME_RATE_NUM 30
#define VIDEO_FRAME_RATE_DEN 1

/// Video render needs at least 2 buffers.
#define VIDEO_OUTPUT_BUFFERS_NUM 3

/// Interval at which we check for an failure abort during capture
const int ABORT_INTERVAL = 100; // ms


// Forward
typedef struct RASPIVIDYUV_STATE_S RASPIVIDYUV_STATE;

/** Struct used to pass information in camera video port userdata to callback
 */
typedef struct
{
   RASPIVIDYUV_STATE *pstate;           /// pointer to our state in case required in callback
   int abort;                           /// Set to 1 in callback if an error occurs to attempt to abort the capture
   int frame;
   int64_t starttime;
   int64_t lasttime;
} PORT_USERDATA;

/** Structure containing all state information for the current run
 */
struct RASPIVIDYUV_STATE_S
{
  rpi_video_t *rpivid;
  
   RASPICOMMONSETTINGS_PARAMETERS common_settings;

  int output_format; // 0 =YUV, 1 = Y800, 2 = RGB8
  
   int framerate;                      /// Requested frame rate (fps)
  
   RASPIPREVIEW_PARAMETERS preview_parameters;   /// Preview setup parameters
   RASPICAM_CAMERA_PARAMETERS camera_parameters; /// Camera setup parameters

   MMAL_COMPONENT_T *camera_component;    /// Pointer to the camera component
   MMAL_CONNECTION_T *preview_connection; /// Pointer to the connection from camera to preview

   MMAL_POOL_T *camera_pool;            /// Pointer to the pool of buffers used by camera video port

   PORT_USERDATA callback_data;         /// Used to move data to the camera callback

   int frame;

   int64_t starttime;
   int64_t lasttime;
};

/// Command ID's and Structure defining our command line options
enum
{
   CommandFramerate,
   CommandUseRGB,
   CommandUseYUV,
   CommandUseY8
};

static COMMAND_LIST cmdline_commands[] =
{
   { CommandFramerate,     "-fps",  "fps","Specify the frames per second to record", 1},
   { CommandUseRGB,        "-rgb",        "rgb","use RGB888 format", 0},
   { CommandUseYUV,        "-yuv",        "yuv","use YUV420 format", 0},
   { CommandUseY8,        "-y8",        "y8","use Y8 format", 0},
};

static int cmdline_commands_size = sizeof(cmdline_commands) / sizeof(cmdline_commands[0]);

/**
 * Dump image state parameters to stderr.
 *
 * @param state Pointer to state structure to assign defaults to
 */
static void dump_status(RASPIVIDYUV_STATE *state)
{
   int size, ystride, yheight;

   if (!state)
   {
      vcos_assert(0);
      return;
   }

   raspicommonsettings_dump_parameters(&state->common_settings);

   fprintf(stderr, "framerate %d\n", state->framerate);

   // Calculate the individual image size
   // Y stride rounded to multiple of 32. U&V stride is Y stride/2 (ie multiple of 16).
   // Y height is padded to a 16. U/V height is Y height/2 (ie multiple of 8).

   // Y plane
   ystride = ((state->common_settings.width + 31) & ~31);
   yheight = ((state->common_settings.height + 15) & ~15);

   size = ystride * yheight;

   // U and V plane
   size += 2 * ystride/2 * yheight/2;

   fprintf(stderr, "Sub-image size %d bytes in total.\n  Y pitch %d, Y height %d, UV pitch %d, UV Height %d\n", size, ystride, yheight, ystride/2,yheight/2);



   raspipreview_dump_parameters(&state->preview_parameters);
   raspicamcontrol_dump_parameters(&state->camera_parameters);
}

/**
 * Display usage information for the application to stdout
 *
 * @param app_name String to display as the application name
 */
static void application_help_message(char *app_name)
{
   fprintf(stdout, "Display camera output to display, and optionally saves an uncompressed YUV420 or RGB file \n\n");
   fprintf(stdout, "NOTE: High resolutions and/or frame rates may exceed the bandwidth of the system due\n");
   fprintf(stdout, "to the large amounts of data being moved to the SD card. This will result in undefined\n");
   fprintf(stdout, "results in the subsequent file.\n");
   fprintf(stdout, "The single raw file produced contains all the images. Each image in the files will be of size\n");
   fprintf(stdout, "width*height*1.5 for YUV or width*height*3 for RGB, unless width and/or height are not divisible by 16.");
   fprintf(stdout, "Use the image size displayed during the run (in verbose mode) for an accurate value\n");

   fprintf(stdout, "The Linux split command can be used to split up the file to individual frames\n");

   fprintf(stdout, "\nusage: %s [options]\n\n", app_name);

   fprintf(stdout, "Image parameter commands\n\n");

   raspicli_display_help(cmdline_commands, cmdline_commands_size);

   fprintf(stdout, "\n");

   return;
}

/**
 * Parse the incoming command line and put resulting parameters in to the state
 *
 * @param argc Number of arguments in command line
 * @param argv Array of pointers to strings from command line
 * @param state Pointer to state structure to assign any discovered parameters to
 * @return Non-0 if failed for some reason, 0 otherwise
 */
static int parse_cmdline(int argc, const char **argv, RASPIVIDYUV_STATE *state)
{
   // Parse the command line arguments.
   // We are looking for --<something> or -<abbreviation of something>

   int valid = 1;
   int i;

   for (i = 1; i < argc && valid; i++)
   {
      int command_id, num_parameters;

      if (!argv[i])
         continue;

      if (argv[i][0] != '-')
      {
         valid = 0;
         continue;
      }

      // Assume parameter is valid until proven otherwise
      valid = 1;

      command_id = raspicli_get_command_id(cmdline_commands, cmdline_commands_size, &argv[i][1], &num_parameters);

      // If we found a command but are missing a parameter, continue (and we will drop out of the loop)
      if (command_id != -1 && num_parameters > 0 && (i + 1 >= argc) )
         continue;

      //  We are now dealing with a command line option
      switch (command_id)
      {
      case CommandFramerate: // fps to record
      {
         if (sscanf(argv[i + 1], "%u", &state->framerate) == 1)
         {
            // TODO : What limits do we need for fps 1 - 30 - 120??
            i++;
         }
         else
            valid = 0;
         break;
      }

      case CommandUseRGB:
	state->output_format = 2;
	break;

      case CommandUseYUV:
	state->output_format = 0;
	break;

      case CommandUseY8:
	state->output_format = 1;
	break;

      default:
      {
         // Try parsing for any image specific parameters
         // result indicates how many parameters were used up, 0,1,2
         // but we adjust by -1 as we have used one already
         const char *second_arg = (i + 1 < argc) ? argv[i + 1] : NULL;
         int parms_used = (raspicamcontrol_parse_cmdline(&state->camera_parameters, &argv[i][1], second_arg));

         // Still unused, try common settings
         if (!parms_used)
            parms_used = raspicommonsettings_parse_cmdline(&state->common_settings, &argv[i][1], second_arg, &application_help_message);

         // Still unused, try preview options
         if (!parms_used)
            parms_used = raspipreview_parse_cmdline(&state->preview_parameters, &argv[i][1], second_arg);

         // If no parms were used, this must be a bad parameters
         if (!parms_used)
            valid = 0;
         else
            i += parms_used - 1;

         break;
      }
      }
   }

   if (!valid)
   {
      fprintf(stderr, "Invalid command line option (%s)\n", argv[i-1]);
      return 1;
   }

   return 0;
}

/**
 *  buffer header callback function for camera
 *
 *  Callback will dump buffer data to internal buffer
 *
 * @param port Pointer to port from which callback originated
 * @param buffer mmal buffer header pointer
 */
static void camera_buffer_callback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer)
{
   MMAL_BUFFER_HEADER_T *new_buffer;

   // We pass our file handle and other stuff in via the userdata field.

   PORT_USERDATA *pData = (PORT_USERDATA *)port->userdata;
   RASPIVIDYUV_STATE *pstate = pData->pstate;

   if (!pData) {
      vcos_log_error("Received a camera buffer callback with no state");
      return;
   }
   
   struct rpi_video_frame *frame = calloc(1, sizeof(struct rpi_video_frame));

   int bytes_to_write = buffer->length;
   int64_t current_time = get_microseconds64();
      
   // only write luma component
   if (pstate->output_format == 1) 
	bytes_to_write = vcos_min(buffer->length, port->format->es->video.width * port->format->es->video.height);

   
   frame->data = malloc(bytes_to_write);
   frame->width = pstate->common_settings.width;
   frame->height = pstate->common_settings.height;
   frame->format = pstate->output_format;
   frame->length = bytes_to_write;
   frame->utime = current_time;
   
   // XXX could we skip the memcopy and just leave the memory locked
   // until put_frame?
   mmal_buffer_header_mem_lock(buffer);
   memcpy(frame->data, buffer->data, bytes_to_write);
   mmal_buffer_header_mem_unlock(buffer);
   mmal_buffer_header_release(buffer);

   // and send one back to the port (if still open)
   if (port->is_enabled)
   {
      MMAL_STATUS_T status;

      new_buffer = mmal_queue_get(pData->pstate->camera_pool->queue);

      if (new_buffer)
         status = mmal_port_send_buffer(port, new_buffer);

      if (!new_buffer || status != MMAL_SUCCESS)
         vcos_log_error("Unable to return a buffer to the camera port");
   }

   mtqueue_push(pstate->rpivid->mtq, frame);
}

int rpi_video_get_frames_pending(rpi_video_t *rpivid)
{
  return mtqueue_size(rpivid->mtq);
}

struct rpi_video_frame *rpi_video_get_frame(rpi_video_t *rpivid)
{
  return mtqueue_pop_blocking(rpivid->mtq);
}

void rpi_video_put_frame(rpi_video_t *rpivid, struct rpi_video_frame *frame)
{
  free(frame->data);
  free(frame);
}

rpi_video_t *rpi_video_create(const char *args)
{
  rpi_video_t *rpivid = calloc(1, sizeof(rpi_video_t));
  rpivid->args = strdup(args);
  rpivid->mtq = mtqueue_create();
  
  return rpivid;
}

/**
 * Create the camera component, set up its ports
 *
 * @param state Pointer to state control struct
 *
 * @return MMAL_SUCCESS if all OK, something else otherwise
 *
 */
static MMAL_STATUS_T create_camera_component(RASPIVIDYUV_STATE *state)
{
   MMAL_COMPONENT_T *camera = 0;
   MMAL_ES_FORMAT_T *format;
   MMAL_PORT_T *preview_port = NULL, *video_port = NULL, *still_port = NULL;
   MMAL_STATUS_T status;
   MMAL_POOL_T *pool;

   /* Create the component */
   status = mmal_component_create(MMAL_COMPONENT_DEFAULT_CAMERA, &camera);

   if (status != MMAL_SUCCESS)
   {
      vcos_log_error("Failed to create camera component");
      goto error;
   }

   status = raspicamcontrol_set_stereo_mode(camera->output[0], &state->camera_parameters.stereo_mode);
   status += raspicamcontrol_set_stereo_mode(camera->output[1], &state->camera_parameters.stereo_mode);
   status += raspicamcontrol_set_stereo_mode(camera->output[2], &state->camera_parameters.stereo_mode);

   if (status != MMAL_SUCCESS)
   {
      vcos_log_error("Could not set stereo mode : error %d", status);
      goto error;
   }

   MMAL_PARAMETER_INT32_T camera_num =
   {{MMAL_PARAMETER_CAMERA_NUM, sizeof(camera_num)}, state->common_settings.cameraNum};

   status = mmal_port_parameter_set(camera->control, &camera_num.hdr);

   if (status != MMAL_SUCCESS)
   {
      vcos_log_error("Could not select camera : error %d", status);
      goto error;
   }

   if (!camera->output_num)
   {
      status = MMAL_ENOSYS;
      vcos_log_error("Camera doesn't have output ports");
      goto error;
   }

   status = mmal_port_parameter_set_uint32(camera->control, MMAL_PARAMETER_CAMERA_CUSTOM_SENSOR_CONFIG, state->common_settings.sensor_mode);

   if (status != MMAL_SUCCESS)
   {
      vcos_log_error("Could not set sensor mode : error %d", status);
      goto error;
   }

   preview_port = camera->output[MMAL_CAMERA_PREVIEW_PORT];
   video_port = camera->output[MMAL_CAMERA_VIDEO_PORT];
   still_port = camera->output[MMAL_CAMERA_CAPTURE_PORT];

   // Enable the camera, and tell it its control callback function
   status = mmal_port_enable(camera->control, default_camera_control_callback);

   if (status != MMAL_SUCCESS)
   {
      vcos_log_error("Unable to enable control port : error %d", status);
      goto error;
   }

   //  set up the camera configuration
   {
      MMAL_PARAMETER_CAMERA_CONFIG_T cam_config =
      {
         { MMAL_PARAMETER_CAMERA_CONFIG, sizeof(cam_config) },
         .max_stills_w = state->common_settings.width,
         .max_stills_h = state->common_settings.height,
         .stills_yuv422 = 0,
         .one_shot_stills = 0,
         .max_preview_video_w = state->common_settings.width,
         .max_preview_video_h = state->common_settings.height,
         .num_preview_video_frames = 3,
         .stills_capture_circular_buffer_height = 0,
         .fast_preview_resume = 0,
         .use_stc_timestamp = MMAL_PARAM_TIMESTAMP_MODE_RESET_STC
      };
      mmal_port_parameter_set(camera->control, &cam_config.hdr);
   }

   // Now set up the port formats

   // Set the encode format on the Preview port
   // HW limitations mean we need the preview to be the same size as the required recorded output

   format = preview_port->format;

   if(state->camera_parameters.shutter_speed > 6000000)
   {
      MMAL_PARAMETER_FPS_RANGE_T fps_range = {{MMAL_PARAMETER_FPS_RANGE, sizeof(fps_range)},
         { 50, 1000 }, {166, 1000}
      };
      mmal_port_parameter_set(preview_port, &fps_range.hdr);
   }
   else if(state->camera_parameters.shutter_speed > 1000000)
   {
      MMAL_PARAMETER_FPS_RANGE_T fps_range = {{MMAL_PARAMETER_FPS_RANGE, sizeof(fps_range)},
         { 166, 1000 }, {999, 1000}
      };
      mmal_port_parameter_set(preview_port, &fps_range.hdr);
   }

   //enable dynamic framerate if necessary
   if (state->camera_parameters.shutter_speed)
   {
      if (state->framerate > 1000000./state->camera_parameters.shutter_speed)
      {
         state->framerate=0;
         if (state->common_settings.verbose)
            fprintf(stderr, "Enable dynamic frame rate to fulfil shutter speed requirement\n");
      }
   }

   format->encoding = MMAL_ENCODING_OPAQUE;
   format->es->video.width = VCOS_ALIGN_UP(state->common_settings.width, 32);
   format->es->video.height = VCOS_ALIGN_UP(state->common_settings.height, 16);
   format->es->video.crop.x = 0;
   format->es->video.crop.y = 0;
   format->es->video.crop.width = state->common_settings.width;
   format->es->video.crop.height = state->common_settings.height;
   format->es->video.frame_rate.num = state->framerate;
   format->es->video.frame_rate.den = VIDEO_FRAME_RATE_DEN;

   status = mmal_port_format_commit(preview_port);

   if (status != MMAL_SUCCESS)
   {
      vcos_log_error("camera viewfinder format couldn't be set");
      goto error;
   }

   // Set the encode format on the video  port

   format = video_port->format;

   if(state->camera_parameters.shutter_speed > 6000000)
   {
      MMAL_PARAMETER_FPS_RANGE_T fps_range = {{MMAL_PARAMETER_FPS_RANGE, sizeof(fps_range)},
         { 50, 1000 }, {166, 1000}
      };
      mmal_port_parameter_set(video_port, &fps_range.hdr);
   }
   else if(state->camera_parameters.shutter_speed > 1000000)
   {
      MMAL_PARAMETER_FPS_RANGE_T fps_range = {{MMAL_PARAMETER_FPS_RANGE, sizeof(fps_range)},
         { 167, 1000 }, {999, 1000}
      };
      mmal_port_parameter_set(video_port, &fps_range.hdr);
   }

   switch (state->output_format) {
     // YUV formats
   case 0:
   case 1:
     format->encoding = MMAL_ENCODING_I420;
     format->encoding_variant = MMAL_ENCODING_I420;
     break;

   case 2:
     format->encoding = mmal_util_rgb_order_fixed(still_port) ? MMAL_ENCODING_RGB24 : MMAL_ENCODING_BGR24;
     format->encoding_variant = 0;  //Irrelevant when not in opaque mode
     break;
   }


   format->es->video.width = VCOS_ALIGN_UP(state->common_settings.width, 32);
   format->es->video.height = VCOS_ALIGN_UP(state->common_settings.height, 16);
   format->es->video.crop.x = 0;
   format->es->video.crop.y = 0;
   format->es->video.crop.width = state->common_settings.width;
   format->es->video.crop.height = state->common_settings.height;
   format->es->video.frame_rate.num = state->framerate;
   format->es->video.frame_rate.den = VIDEO_FRAME_RATE_DEN;

   status = mmal_port_format_commit(video_port);

   if (status != MMAL_SUCCESS)
   {
      vcos_log_error("camera video format couldn't be set");
      goto error;
   }

   // Ensure there are enough buffers to avoid dropping frames
   if (video_port->buffer_num < VIDEO_OUTPUT_BUFFERS_NUM)
      video_port->buffer_num = VIDEO_OUTPUT_BUFFERS_NUM;

   status = mmal_port_parameter_set_boolean(video_port, MMAL_PARAMETER_ZERO_COPY, MMAL_TRUE);
   if (status != MMAL_SUCCESS)
   {
      vcos_log_error("Failed to select zero copy");
      goto error;
   }

   // Set the encode format on the still  port

   format = still_port->format;

   format->encoding = MMAL_ENCODING_OPAQUE;
   format->encoding_variant = MMAL_ENCODING_I420;

   format->es->video.width = VCOS_ALIGN_UP(state->common_settings.width, 32);
   format->es->video.height = VCOS_ALIGN_UP(state->common_settings.height, 16);
   format->es->video.crop.x = 0;
   format->es->video.crop.y = 0;
   format->es->video.crop.width = state->common_settings.width;
   format->es->video.crop.height = state->common_settings.height;
   format->es->video.frame_rate.num = 0;
   format->es->video.frame_rate.den = 1;

   status = mmal_port_format_commit(still_port);

   if (status != MMAL_SUCCESS)
   {
      vcos_log_error("camera still format couldn't be set");
      goto error;
   }

   /* Ensure there are enough buffers to avoid dropping frames */
   if (still_port->buffer_num < VIDEO_OUTPUT_BUFFERS_NUM)
      still_port->buffer_num = VIDEO_OUTPUT_BUFFERS_NUM;

   /* Enable component */
   status = mmal_component_enable(camera);

   if (status != MMAL_SUCCESS)
   {
      vcos_log_error("camera component couldn't be enabled");
      goto error;
   }

   raspicamcontrol_set_all_parameters(camera, &state->camera_parameters);

   /* Create pool of buffer headers for the output port to consume */
   pool = mmal_port_pool_create(video_port, video_port->buffer_num, video_port->buffer_size);

   if (!pool)
   {
      vcos_log_error("Failed to create buffer header pool for camera still port %s", still_port->name);
   }

   state->camera_pool = pool;
   state->camera_component = camera;

   if (state->common_settings.verbose)
      fprintf(stderr, "Camera component done\n");

   return status;

error:

   if (camera)
      mmal_component_destroy(camera);

   return status;
}

/**
 * Destroy the camera component
 *
 * @param state Pointer to state control struct
 *
 */
static void destroy_camera_component(RASPIVIDYUV_STATE *state)
{
   if (state->camera_component)
   {
      mmal_component_destroy(state->camera_component);
      state->camera_component = NULL;
   }
}

/**
 * main
 */
int rpi_video_start(rpi_video_t *rpivid)
{
  int argc = 1;
  const char **argv = calloc(sizeof(char*), 1);
  argv[0] = strdup("rpi_video_internal");

  if (1) {
    char *p = rpivid->args;
    while (1) {
      // add another arg
      argv = realloc(argv, sizeof(char*) * (argc + 1));
      argv[argc] = strdup(p);
      argc++;
      char *s = strstr(argv[argc-1], " ");
      if (!s)
	break;
      
      *s = 0;
      p = &s[1];
    }
  }
  
  // Our main data storage vessel..
  RASPIVIDYUV_STATE *state = calloc(1, sizeof(RASPIVIDYUV_STATE));
  state->common_settings.width = 1920;
  state->common_settings.height = 1080;
  state->framerate = VIDEO_FRAME_RATE_NUM;
  // Setup preview window defaults
  raspipreview_set_defaults(&state->preview_parameters);
  // Set up the camera_parameters to default
  raspicamcontrol_set_defaults(&state->camera_parameters);
  
   int exit_code = EX_OK;

   state->rpivid = rpivid;
   
   MMAL_STATUS_T status = MMAL_SUCCESS;
   MMAL_PORT_T *camera_preview_port = NULL;
   MMAL_PORT_T *camera_video_port = NULL;
   MMAL_PORT_T *preview_input_port = NULL;

   bcm_host_init();

   // Register our application with the logging system
   vcos_log_register("RaspiVid", VCOS_LOG_CATEGORY);

   signal(SIGINT, default_signal_handler);

   // Disable USR1 for the moment - may be reenabled if go in to signal capture mode
   signal(SIGUSR1, SIG_IGN);

   set_app_name(argv[0]);

   // Parse the command line and put options in to our status structure
   if (parse_cmdline(argc, argv, state))
   {
      status = -1;
      exit(EX_USAGE);
   }

   if ((state->common_settings.width % 16) || state->common_settings.height %16) {
     printf("WARNING: width or height is not a multiple of 16\n");
   }
     
   // Setup for sensor specific parameters, only set W/H settings if zero on entry
   get_sensor_defaults(state->common_settings.cameraNum, state->common_settings.camera_name,
                       &state->common_settings.width, &state->common_settings.height);

   if (state->common_settings.verbose)
   {
      print_app_details(stderr);
      dump_status(state);
   }

   // OK, we have a nice set of parameters. Now set up our components
   // We have two components. Camera, Preview

   if ((status = create_camera_component(state)) != MMAL_SUCCESS)
   {
      vcos_log_error("%s: Failed to create camera component", __func__);
      exit_code = EX_SOFTWARE;
      goto error;
   }

   
   if ((status = raspipreview_create(&state->preview_parameters)) != MMAL_SUCCESS)
   {
      vcos_log_error("%s: Failed to create preview component", __func__);
      destroy_camera_component(state);
      exit_code = EX_SOFTWARE;
      goto error;
   }

   if (state->common_settings.verbose)
     fprintf(stderr, "Starting component connection stage\n");
   
   camera_preview_port = state->camera_component->output[MMAL_CAMERA_PREVIEW_PORT];
   camera_video_port   = state->camera_component->output[MMAL_CAMERA_VIDEO_PORT];
   //   camera_still_port   = state->camera_component->output[MMAL_CAMERA_CAPTURE_PORT];
   preview_input_port  = state->preview_parameters.preview_component->input[0];
   
   if (state->preview_parameters.wantPreview )
     {
       if (state->common_settings.verbose)
         {
	   fprintf(stderr, "Connecting camera preview port to preview input port\n");
	   fprintf(stderr, "Starting video preview\n");
         }
       
       // Connect camera to preview
       status = connect_ports(camera_preview_port, preview_input_port, &state->preview_connection);
       
       if (status != MMAL_SUCCESS) {
	 state->preview_connection = NULL;
	 goto error;
       }
     }
   
   // Set up our userdata - this is passed though to the callback where we need the information.
   state->callback_data.pstate = state;
   state->callback_data.abort = 0;
   
   camera_video_port->userdata = (struct MMAL_PORT_USERDATA_T *) &state->callback_data;
	 
   if (state->common_settings.verbose)
     fprintf(stderr, "Enabling camera video port\n");
	   
   // Enable the camera video port and tell it its callback function
   status = mmal_port_enable(camera_video_port, camera_buffer_callback);
   
   if (status != MMAL_SUCCESS) {
     vcos_log_error("Failed to setup camera output");
     goto error;
   }
   
   // Send all the buffers to the camera video port
   {
     int num = mmal_queue_length(state->camera_pool->queue);

     for (int q=0; q<num; q++)
       {
	 MMAL_BUFFER_HEADER_T *buffer = mmal_queue_get(state->camera_pool->queue);

	 if (!buffer)
	   vcos_log_error("Unable to get a required buffer %d from pool queue", q);
	 
	 if (mmal_port_send_buffer(camera_video_port, buffer)!= MMAL_SUCCESS)
	   vcos_log_error("Unable to send a buffer to camera video port (%d)", q);
       }
   }
   
   if (mmal_port_parameter_set_boolean(camera_video_port, MMAL_PARAMETER_CAPTURE, 1) != MMAL_SUCCESS) {
     printf("start failed\n");
     exit(1);
   }
   
   return 0;
   
   //         mmal_status_to_int(status);
   //         vcos_log_error("%s: Failed to connect camera to preview", __func__);
      
error:
   
   mmal_status_to_int(status);
   
   if (state->common_settings.verbose)
     fprintf(stderr, "Closing down\n");
   
   // Disable all our ports that are not handled by connections
   check_disable_port(camera_video_port);
   
   if (state->preview_parameters.wantPreview && state->preview_connection)
     mmal_connection_destroy(state->preview_connection);
   
   if (state->preview_parameters.preview_component)
     mmal_component_disable(state->preview_parameters.preview_component);
   
   if (state->camera_component)
     mmal_component_disable(state->camera_component);
   
   raspipreview_destroy(&state->preview_parameters);
   destroy_camera_component(state);
   
   if (state->common_settings.verbose)
     fprintf(stderr, "Close down completed, all components disconnected, disabled and destroyed\n\n");
   
   if (status != MMAL_SUCCESS)
     raspicamcontrol_check_configuration(128);
   
   return exit_code;
}


