OBJS=rpi_video.o RaspiCamControl.o RaspiCLI.o RaspiCommonSettings.o RaspiHelpers.o RaspiPreview.o mtqueue.o zqueue.o 

all: rpi_video_test rpi_apriltag_demo

%.o: %.c
	gcc -c $^ -std=gnu99 -g -Wall -O2 -I/opt/vc/include/ -I../

rpi_apriltag_demo: rpi_apriltag_demo.o $(OBJS) 
	gcc -o $@ $^  ../lib/libapriltag.a -lm -lmmal -lmmal_core -lmmal_util -lbcm_host -L/opt/vc/lib/ -lpthread -lvcos -ldl 

rpi_video_test: rpi_video_test.o $(OBJS) 
	gcc -o $@ $^  -lmmal -lmmal_core -lmmal_util -lbcm_host -L/opt/vc/lib/ -lpthread -lvcos -ldl 

clean:
	rm -rf *.o *~ rpi_apriltag_demo
