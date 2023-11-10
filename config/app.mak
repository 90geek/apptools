
SUBDIRS :=

#########################################################################
#module driver 
#########################################################################
#header include

INCLUDEFLAGS :=

SDK_INCLUDE = 

# Link Type: static / dynamic
LINK_TYPE = static

##########################################################################
#platform
##########################################################################
#PLATFORM_INCLUDE = $(SDK_DIR)/lib/platform/include
#LIBPATH += -L$(SDK_DIR)/lib/platform/lib/$(PLATFORM)_$(CHIP)/$(LINK_TYPE)
PLATFORM_INCLUDE = $(SDK_DIR)/platform/include
LIBPATH += -L$(SDK_DIR)/platform/lib/$(LINK_TYPE)
PLATFORM_LIB = platform
SUBDIRS += platform/src 

##########################################################################
#testtool
##########################################################################
TESTTOOL_INCLUDE =  $(SDK_DIR)/testtool/include
LIBPATH += -L$(SDK_DIR)/testtool/lib/$(LINK_TYPE)
TESTTOOL_LIB = testtool
SUBDIRS += testtool/src

##########################################################################
ifeq ($(CAPSULE_SUPPORT), 1) 
CAPSULE_INCLUDE =  $(SDK_DIR)/capsule/include
INCLUDEFLAGS += -I$(CAPSULE_INCLUDE)
CFLAGS += -DCAPSULE_SUPPORT

LIBPATH += -L$(SDK_DIR)/capsule/lib/$(LINK_TYPE)
LIB += -lcapsule

SUBDIRS +=  capsule/src

else

endif
##########################################################################
ifeq ($(HARDINFO_SUPPORT), 1) 
HARDINFO_INCLUDE =  $(SDK_DIR)/hardinfo/include
INCLUDEFLAGS += -I$(HARDINFO_INCLUDE)
CFLAGS += -DHARDINFO_SUPPORT

LIBPATH += -L$(SDK_DIR)/hardinfo/lib/$(LINK_TYPE)
LIB += -lhardinfo

SUBDIRS +=  hardinfo/src

else

endif

##########################################################################
ifeq ($(DMIDECODE_SUPPORT), 1) 
DMIDECODE_INCLUDE =  $(SDK_DIR)/dmidecode/include
# DMIDECODE_INCLUDE +=  $(SDK_DIR)/dmidecode/src
INCLUDEFLAGS += -I$(DMIDECODE_INCLUDE)
CFLAGS += -DDMIDECODE_SUPPORT

LIBPATH += -L$(SDK_DIR)/dmidecode/lib/$(LINK_TYPE)
LIB += -ldmidecode

SUBDIRS += dmidecode/src
# CFLAGS += -W -Wall -Wshadow -Wstrict-prototypes -Wpointer-arith -Wcast-qual \
          -Wcast-align -Wwrite-strings -Wmissing-prototypes -Winline -Wundef
else

endif
##########################################################################
ifeq ($(INFOUI_SUPPORT), 1) 
INFOUI_INCLUDE =  $(SDK_DIR)/infoui/include
INCLUDEFLAGS += -I$(INFOUI_INCLUDE)
INCLUDEFLAGS +=-I/usr/include/gtk-3.0 -I/usr/include/pango-1.0 -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/include/fribidi -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/uuid -I/usr/include/harfbuzz -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/gio-unix-2.0/ -I/usr/include/atk-1.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/at-spi-2.0 -I/usr/include/dbus-1.0 -I/usr/lib64/dbus-1.0/include
CFLAGS += -DINFOUI_SUPPORT

LIBPATH += -L$(SDK_DIR)/infoui/lib/$(LINK_TYPE)
LIB += -linfoui

SUBDIRS += infoui/src

CFLAGS += -g -Wall `pkg-config --cflags gtk+-3.0`
LDFLAGS += `pkg-config --libs gtk+-3.0`
else

endif
##########################################################################
ifeq ($(DEVMEM_SUPPORT), 1) 
DMIDECODE_INCLUDE =  $(SDK_DIR)/devmem/include
INCLUDEFLAGS += -I$(DMIDECODE_INCLUDE)
CFLAGS += -DDEVMEM_SUPPORT

LIBPATH += -L$(SDK_DIR)/devmem/lib/$(LINK_TYPE)
LIB += -ldevmem

SUBDIRS += devmem/src
# CFLAGS += -W -Wall -Wshadow -Wstrict-prototypes -Wpointer-arith -Wcast-qual \
          -Wcast-align -Wwrite-strings -Wmissing-prototypes -Winline -Wundef
else

endif

##########################################################################
ifeq ($(RUNTIME_SUPPORT), 1) 
RUNTIME_INCLUDE =  $(SDK_DIR)/runtime/include
INCLUDEFLAGS += -I$(RUNTIME_INCLUDE)
CFLAGS += -DRUNTIME_SUPPORT

LIBPATH += -L$(SDK_DIR)/runtime/lib/$(LINK_TYPE)
LIB += -lruntime

SUBDIRS +=  runtime/src

else

endif
##########################################################################
ifeq ($(UPFLASH_SUPPORT), 1) 
UPFLASH_INCLUDE =  $(SDK_DIR)/upflash/include
INCLUDEFLAGS += -I$(UPFLASH_INCLUDE)
CFLAGS += -DUPFLASH_SUPPORT

LIBPATH += -L$(SDK_DIR)/upflash/lib/$(LINK_TYPE)
LIB += -lupflash

SUBDIRS +=  upflash/src

else

endif

##########################################################################
INCLUDEFLAGS += -I${PLATFORM_INCLUDE} -I${TESTTOOL_INCLUDE}
LIB += -l${TESTTOOL_LIB} -l${PLATFORM_LIB}

#########################################################################
#platform 
#########################################################################

ifeq ($(PLATFORM), host)

INCLUDEFLAGS += -I$(SDK_DIR)/platform/src
LIB +=
CFLAGS += -fgnu89-inline

ARCH := $(shell uname -m)
ifeq ($(ARCH),x86_64)
CFLAGS += -DPLATFORM_X86
else ifeq ($(ARCH),loongarch64)
CFLAGS += -fgnu89-inline -DPLATFORM_LA64
else ifeq ($(ARCH),mips64)
CFLAGS += -fgnu89-inline -march=mips64r2 -mabi=64 -DPLATFORM_MIPS64
LDFLAGS += -march=mips64r2 -mabi=64
else
    $(error Unsupported architecture: $(ARCH))
endif
#########################################################################
else ifeq ($(PLATFORM), loongarch64)

INCLUDEFLAGS += -I$(SDK_DIR)/platform/src

MSTAR_LIB +=

LIBPATH +=

LIB +=

CFLAGS += -fgnu89-inline -DPLATFORM_LA64
LDFLAGS +=
#########################################################################
else ifeq ($(PLATFORM), mips64)

INCLUDEFLAGS += -I$(SDK_DIR)/platform/src

MSTAR_LIB +=

LIBPATH +=

LIB +=

CFLAGS += -fgnu89-inline -march=mips64r2 -mabi=64 -DPLATFORM_MIPS64
LDFLAGS += -march=mips64r2 -mabi=64

#########################################################################
else 
$(error PLATFORM not define) 
endif


#########################################################################
# 
#########################################################################
LIB += -lrt -lm -lpthread -lstdc++ -ldl

CFLAGS += $(INCLUDEFLAGS)

CFLAGS += -Wl,--gc-sections  -O0 -g 

#compile error report
# CFLAGS += -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function  -Wno-unused-but-set-variable -Wno-sign-compare -Wno-unused-but-set-parameter -Wno-unused-label -Wno-empty-body -Wno-undef 

ifeq ($(LINK_TYPE), static)
CFLAGS += -static
else
CFLAGS += -fPIC
endif	


LDFLAGS += -Wl,--gc-sections -g -Wall ${LIBPATH}

#########################################################################
# 
########################################################################
%.o: %.c
	$(AT)$(CC) $(CFLAGS)  -c -o $@ $^

%.o: %.cxx
	$(AT)$(CXX) $(CFLAGS) -c -o $@ $^
