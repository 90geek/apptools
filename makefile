#===============================================================================
# export variable
#===============================================================================

include $(SDK_DIR)/config/platform.mak
include $(SDK_DIR)/config/app.mak

#===============================================================================
# local variable
#===============================================================================


APP =apptools
OBJS = main.o

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean  install uninstall info subdirs app
all: subdirs app

app:$(OBJS)
	$(AT)$(CC) $(LDFLAGS) -o $(APP) $(OBJS) ${LIB}
subdirs:
	for dir in $(SUBDIRS);\
	do make -C $$dir all || exit 1;\
	done

gdb:
	$(TOOLCHAIN_PREFIX)-gdb $(APP)

strip:

.PHONY:clean 
clean:
	for dir in $(SUBDIRS);do make -C $$dir clean||exit 1;done
	-$(AT)rm -f  $(OBJS)
	-$(AT)rm -f $(APP)
	-$(AT)rm -f *~

install:

uninstall:

info:
	$(SDK_DIR)/config/setenv_info.sh
	
libs:
	$(SDK_DIR)/lib/update_lib.sh

	

	

