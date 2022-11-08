PREFIX ?= /usr/local

CC = gcc
AR = ar

CFLAGS = -std=gnu99 -fPIC -Wall -Wno-unused-parameter -Wno-unused-function
CFLAGS += -I. -O3 -fno-strict-overflow

ROOT := $(shell pwd)
APRILTAG_DIR := $(ROOT)/libs/apriltag
APRILTAG_SRC_DIR := $(APRILTAG_DIR)/src

#APRILTAG_SRCS := $(shell ls *.c common/*.c)
APRILTAG_SRCS := $(APRILTAG_SRC_DIR)/apriltag.c $(APRILTAG_SRC_DIR)/apriltag_pose.c $(APRILTAG_SRC_DIR)/apriltag_quad_thresh.c $(APRILTAG_SRC_DIR)/common/g2d.c $(APRILTAG_SRC_DIR)/common/getopt.c $(APRILTAG_SRC_DIR)/common/homography.c $(APRILTAG_SRC_DIR)/common/image_u8.c $(APRILTAG_SRC_DIR)/common/image_u8x3.c $(APRILTAG_SRC_DIR)/common/image_u8x4.c $(APRILTAG_SRC_DIR)/common/matd.c $(APRILTAG_SRC_DIR)/common/pam.c $(APRILTAG_SRC_DIR)/common/pjpeg.c $(APRILTAG_SRC_DIR)/common/pjpeg-idct.c $(APRILTAG_SRC_DIR)/common/pnm.c $(APRILTAG_SRC_DIR)/common/string_util.c $(APRILTAG_SRC_DIR)/common/svd22.c $(APRILTAG_SRC_DIR)/common/time_util.c $(APRILTAG_SRC_DIR)/common/workerpool.c $(APRILTAG_SRC_DIR)/common/zarray.c $(APRILTAG_SRC_DIR)/common/zhash.c $(APRILTAG_SRC_DIR)/common/zmaxheap.c $(APRILTAG_SRC_DIR)/tag16h5.c $(APRILTAG_SRC_DIR)/tag25h9.c $(APRILTAG_SRC_DIR)/tag36h11.c $(APRILTAG_SRC_DIR)/tagCircle21h7.c $(APRILTAG_SRC_DIR)/tagCircle49h12.c $(APRILTAG_SRC_DIR)/tagCustom48h12.c $(APRILTAG_SRC_DIR)/tagStandard41h12.c $(APRILTAG_SRC_DIR)/tagStandard52h13.c
APRILTAG_HEADERS := $(shell ls $(APRILTAG_DIR)/include/apriltag/*.h $(APRILTAG_DIR)/include/apriltag/common/*.h)
APRILTAG_OBJS := $(APRILTAG_SRCS:%.c=%.o)
TARGETS := libapriltag.a libapriltag.so

.PHONY: all
all: $(TARGETS)
	@$(MAKE) -C example all

.PHONY: install
install: libapriltag.so
	@chmod +x install.sh
	@./install.sh $(PREFIX)/lib libapriltag.so
	@./install.sh $(PREFIX)/ $(APRILTAG_HEADERS)
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
	@rm -rf $(APRILTAG_SRC_DIR)/*.o $(APRILTAG_SRC_DIR)/common/*.o $(TARGETS)
	@$(MAKE) -C example clean
