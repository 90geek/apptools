
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
ifeq ($(TSIP_SUPPORT), 0) 
INCLUDEFLAGS +=
CFLAGS +=

LIBPATH += -L$(SDK_DIR)/tsip/lib/$(LINK_TYPE)
LIB += -ltsip_app

SUBDIRS +=  tsip/src

INCLUDEFLAGS +=

else

endif

##########################################################################
INCLUDEFLAGS += -I${PLATFORM_INCLUDE} -I${TESTTOOL_INCLUDE}
LIB += -l${TESTTOOL_LIB} -l${PLATFORM_LIB}

#########################################################################
#platform 
#########################################################################

ifeq ($(PLATFORM), local)

INCLUDEFLAGS +=
LIB +=
CFLAGS += -fgnu89-inline

#########################################################################
else ifeq ($(PLATFORM), loongarch64)

INCLUDEFLAGS += 

MSTAR_LIB +=

LIBPATH +=

LIB +=

CFLAGS += -fgnu89-inline
LDFLAGS +=
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
