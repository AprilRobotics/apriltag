OBJS=RaspiVidYUV.o RaspiCamControl.o RaspiCLI.o RaspiCommonSettings.o RaspiHelpers.o RaspiPreview.o 

all: raspividyuv

%.o: %.c
	gcc -c $^ -std=gnu99 -I/opt/vc/include/

raspividyuv: $(OBJS)
	gcc -o $< $^ -o raspividyuv -lmmal -lmmal_core -lmmal_util -lbcm_host   -L/opt/vc/lib/ -lpthread -lvcos -ldl

clean:
	rm -rf *.o raspividyuv
