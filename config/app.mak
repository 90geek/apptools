
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
CFLAGS += -DCAPSULE_INCLUDE

LIBPATH += -L$(SDK_DIR)/capsule/lib/$(LINK_TYPE)
LIB += -lcapsule

SUBDIRS +=  capsule/src

else

endif
##########################################################################
ifeq ($(HARDINFO_SUPPORT), 1) 
HARDINFO_INCLUDE =  $(SDK_DIR)/hardinfo/include
INCLUDEFLAGS += -I$(HARDINFO_INCLUDE)
CFLAGS += -DHARDINFO_INCLUDE

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
CFLAGS += -DDMIDECODE_INCLUDE

LIBPATH += -L$(SDK_DIR)/dmidecode/lib/$(LINK_TYPE)
LIB += -ldmidecode

SUBDIRS += dmidecode/src
# CFLAGS += -W -Wall -Wshadow -Wstrict-prototypes -Wpointer-arith -Wcast-qual \
          -Wcast-align -Wwrite-strings -Wmissing-prototypes -Winline -Wundef
else

endif

##########################################################################
INCLUDEFLAGS += -I${PLATFORM_INCLUDE} -I${TESTTOOL_INCLUDE}
LIB += -l${TESTTOOL_LIB} -l${PLATFORM_LIB}

#########################################################################
#platform 
#########################################################################

ifeq ($(PLATFORM), local)

INCLUDEFLAGS += -I$(SDK_DIR)/platform/src
LIB +=
CFLAGS += -fgnu89-inline

#########################################################################
else ifeq ($(PLATFORM), loongarch64)

INCLUDEFLAGS += -I$(SDK_DIR)/platform/src

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
