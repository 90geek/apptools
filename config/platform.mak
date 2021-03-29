
ifeq ($(PLATFORM), hisi)
TOOLCHAIN_DIR = $(SDK_DIR)/../toolchain/hisi/arm-histbv310-linux
export TOOLCHAIN_PREFIX = ${TOOLCHAIN_DIR}/bin/arm-histbv310-linux-
else ifeq ($(PLATFORM), local)
TOOLCHAIN_DIR = /usr
export TOOLCHAIN_PREFIX = ${TOOLCHAIN_DIR}/bin/
else ifeq ($(PLATFORM), loongarch64)
TOOLCHAIN_DIR = /home/zhubo/00_loongson/toolchain/la_toolchain/loongarch64-linux-gnu-2021-01-02
export TOOLCHAIN_PREFIX = ${TOOLCHAIN_DIR}/bin/loongarch64-linux-gnu-
else 
$(error PLATFORM not define) 
endif
CC := $(TOOLCHAIN_PREFIX)gcc
CXX := $(TOOLCHAIN_PREFIX)g++
AR=$(TOOLCHAIN_PREFIX)ar
AS=$(TOOLCHAIN_PREFIX)as
LD=$(TOOLCHAIN_PREFIX)ld
CPP=$(TOOLCHAIN_PREFIX)cpp
CC=$(TOOLCHAIN_PREFIX)gcc
NM=$(TOOLCHAIN_PREFIX)nm
STRIP=$(TOOLCHAIN_PREFIX)strip
OBJCOPY=$(TOOLCHAIN_PREFIX)objcopy
OBJDUMP=$(TOOLCHAIN_PREFIX)objdump




