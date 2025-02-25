#===============================================================================
# export variable
#===============================================================================

include $(SDK_DIR)/config/platform.mak
include $(SDK_DIR)/config/app.mak

#===============================================================================
# local variable
#===============================================================================

LIBS := libplatform

DRV_LIB_OUT := ../lib/$(LINK_TYPE)

OBJS := app_os.o app_platform.o platform_debug.o app_spi.o app_mac_addr.o app_mm.o
OBJS += loongson/edk_api.o loongson/mem.o loongson/pwm.o  loongson/spi.o 
OBJS += loongson/temperature.o loongson/cpu.o loongson/ls7a.o loongson/i2c.o
OBJS += loongson/mps.o loongson/ChipsetLpc.o loongson/Ls7aGpio.o loongson/Beep.o loongson/Gpio.o
OBJS += loongson/AvsLib.o
#===============================================================================
# rules
#===============================================================================
.PHONY: all clean install uninstall

all:	$(LIBS)
	
clean:
	$(AT)rm -rf $(OBJS)
	$(AT)rm -rf $(LIBS).a
	$(AT)rm -rf $(LIBS).so
	$(AT)rm -rf *~

$(LIBS) : $(OBJS)
ifeq ($(LINK_TYPE), static)
	$(AT)$(AR) -rs $@.a $^
	$(AT)test -d $(DRV_LIB_OUT) || mkdir -p $(DRV_LIB_OUT)
	$(AT)mv $@.a $(DRV_LIB_OUT)
else
	$(AT)$(CC) $^ $(CFLAGS) -shared -o $@.so 
	$(AT)test -d $(DRV_LIB_OUT) || mkdir -p $(DRV_LIB_OUT)
	$(AT)mv $@.so $(DRV_LIB_OUT)
endif

install:

uninstall:
