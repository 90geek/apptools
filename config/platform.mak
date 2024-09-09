
ifeq ($(PLATFORM), host)
TOOLCHAIN_DIR = /usr
export TOOLCHAIN_PREFIX = ${TOOLCHAIN_DIR}/bin/
else ifeq ($(PLATFORM), loongarch64)
# TOOLCHAIN_DIR = /home/zhubo/loongson/toolchain/LARCH_toolchain_root
# export TOOLCHAIN_PREFIX = ${TOOLCHAIN_DIR}/bin/loongarch64-linux-gnu-
export TOOLCHAIN_PREFIX = loongarch64-linux-gnu-
else ifeq ($(PLATFORM), lagcc14)
export TOOLCHAIN_PREFIX = loongarch64-unknown-linux-gnu-
else ifeq ($(PLATFORM), mips64)
export TOOLCHAIN_PREFIX = mips-linux-gnu-
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




