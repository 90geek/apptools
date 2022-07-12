#===============================================================================
# export variable
#===============================================================================

include $(SDK_DIR)/config/platform.mak
include $(SDK_DIR)/config/app.mak

#===============================================================================
# local variable
#===============================================================================

LIBS := libinfoui

DRV_LIB_OUT := ../lib/$(LINK_TYPE)

OBJS := loongson-uimain.o loongson-mainboard.o loongson-perf.o loongson-security.o loongson-spec.o loongson-state.o loongson-uimain.o loongson-utils.o loongson-window.o


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
