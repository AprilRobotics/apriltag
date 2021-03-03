PREFIX ?= /usr/local

CC = gcc
AR = ar

CFLAGS = -std=gnu99 -fPIC -Wall -Wno-unused-parameter -Wno-unused-function
CFLAGS += -I. -O3 -fno-strict-overflow

#APRILTAG_SRCS := $(shell ls *.c common/*.c)
APRILTAG_SRCS := apriltag.c apriltag_pose.c apriltag_quad_thresh.c common/g2d.c common/getopt.c common/homography.c common/image_u8.c common/image_u8x3.c common/image_u8x4.c common/matd.c common/pam.c common/pjpeg.c common/pjpeg-idct.c common/pnm.c common/string_util.c common/svd22.c common/time_util.c common/unionfind.c common/workerpool.c common/zarray.c common/zhash.c common/zmaxheap.c tag16h5.c tag25h9.c tag36h11.c tagCircle21h7.c tagCircle49h12.c tagCustom48h12.c tagStandard41h12.c tagStandard52h13.c
APRILTAG_HEADERS := $(shell ls *.h common/*.h)
APRILTAG_OBJS := $(APRILTAG_SRCS:%.c=%.o)
TARGETS := libapriltag.a libapriltag.so

.PHONY: all
all: $(TARGETS)
	@$(MAKE) -C example all

.PHONY: install
install: libapriltag.so
	@chmod +x install.sh
	@./install.sh $(PREFIX)/lib libapriltag.so
	@./install.sh $(PREFIX)/include/apriltag $(APRILTAG_HEADERS)
	@ldconfig

libapriltag.a: $(APRILTAG_OBJS)
	@echo "   [$@]"
	@$(AR) -cq $@ $(APRILTAG_OBJS)

libapriltag.so: $(APRILTAG_OBJS)
	@echo "   [$@]"
	@$(CC) -fPIC -shared -o $@ $^

%.o: %.c
	@echo "   $@"
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean
clean:
	@rm -rf *.o common/*.o $(TARGETS)
	@$(MAKE) -C example clean
