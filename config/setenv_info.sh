#!/bin/bash
#--------

# Define color and macros
# -----------------------
cDEF='\e[0m'
cBOLD='\e[1m'
cUNDERL='\e[4m'
cBLACK='\e[30m'
cRED='\e[31m'
cGREEN='\e[32m'
cYELLOW='\e[33m'
cBLUE='\e[34m'
cPINK='\e[35m'
cCYAN='\e[36m'
cWHITE='\e[37m'
WARNING_NF="${cBOLD}${cYELLOW}/!\ Not found /!\ ${cDEF}"
WARNING_RO="${cBOLD}${cYELLOW}/!\ Read only /!\ ${cDEF}"

# Define all string variables
# ---------------------------
if [ -d "$STSDKROOT" ]; then
 if [ ! -w "$STSDKROOT" ]; then
  STSDKROOT_STRING="${cBOLD}${cRED} $STSDKROOT ${cDEF} ${WARNING_RO}"
 else
  STSDKROOT_STRING=${cGREEN}$STSDKROOT${cDEF}
 fi
else
 STSDKROOT_STRING="${cBOLD}${cRED}$STSDKROOT${cDEF} ${WARNING_NF}"
fi
if [ -d "$ST40ROOT" ]; then
 ST40ROOT_STRING=$cGREEN$ST40ROOT${cDEF}
else
 ST40ROOT_STRING="${cBOLD}${cRED}$ST40ROOT${cDEF} ${WARNING_NF}"
fi
if [ -d "$OSPLUSROOT" ]; then
 OSPLUSROOT_STRING=$cGREEN$OSPLUSROOT${cDEF}
else
 OSPLUSROOT_STRING="${cBOLD}${cRED}$OSPLUSROOT${cDEF} ${WARNING_NF}"
fi
if [ -d "$KDIR" ]; then
 if [ ! -w "$KDIR" ]; then
  KDIR_STRING="${cBOLD}${cRED}$KDIR${cDEF} ${WARNING_RO}"
 else
  KDIR_STRING=$cGREEN$KDIR${cDEF}
 fi
else
 KDIR_STRING="${cBOLD}${cRED}$KDIR${cDEF} ${WARNING_NF}"
fi
if [ -d "$LINUX_SERVERDIR" ]; then
 LINUX_SERVERDIR_STRING=$cGREEN$LINUX_SERVERDIR${cDEF}
else
 LINUX_SERVERDIR_STRING="${cBOLD}${cRED}$LINUX_SERVERDIR${cDEF} ${WARNING_NF}"
fi
if [ -d "$KTARGET" ]; then
 if [ ! -w "$KTARGET" ] || [ ! -x "$KTARGET" ]; then
  KTARGET_STRING="${cBOLD}${cRED}$KTARGET${cDEF} ${WARNING_RO}"
 else
  KTARGET_STRING=$cGREEN$KTARGET_SOURCE${cDEF}
 fi
 KTARGET_STRING=$cGREEN$KTARGET${cDEF}
else
 KTARGET_STRING="${cBOLD}${cRED}$KTARGET${cDEF} ${WARNING_NF}"
fi
if [ -d "$MULTICOM_SOURCE" ]; then
 if [ ! -w "$MULTICOM_SOURCE" ]; then
  MULTICOM_SOURCE_STRING="${cBOLD}${cRED}$MULTICOM_SOURCE${cDEF} ${WARNING_RO}"
 else
  MULTICOM_SOURCE_STRING=$cGREEN$MULTICOM_SOURCE${cDEF}
 fi
else
 MULTICOM_SOURCE_STRING="${cBOLD}${cRED}$MULTICOM_SOURCE${cDEF} ${WARNING_NF}"
fi

# Platform settings
# -----------------
echo -e "${cYELLOW}${cBOLD}_________________________________________________________________________________${cDEF}"
echo -e " ${cBLUE}${cBOLD}${cUNDERL} Platform ${cDEF}"
echo -e " - ${cBOLD} Platform ${cDEF}       : ${cDEF}${cWHITE} ${PLATFORM} ${cDEF}"
if [ "${CHIP}" != "" ]; then
 echo -e " - ${cBOLD} Chipset ${cDEF}        : ${cDEF}${cWHITE} ${CHIP} ${cDEF}"
else
 echo -e " - ${cBOLD} Chipset ${cDEF}        : ${cDEF}${cYELLOW} none ${cDEF}"
fi


# System settings
# ---------------
echo -e "${cYELLOW}${cBOLD}_________________________________________________________________________________${cDEF}"
echo -e " ${cBLUE}${cBOLD}${cUNDERL}System${cDEF}"
echo -e " - ${cBOLD}Device${cDEF}          : ${cWHITE}${DEVICE_TYPE}${cDEF}"
if [ "${DVD_OS}" = "LINUX" ]; then
echo -e " - ${cBOLD}Operating System${cDEF}	: ${cWHITE}${DVD_OS}${cDEF} revision ${cWHITE}${LINUX_VERSION}${cDEF}"
else
echo -e " - ${cBOLD}Operating System${cDEF}	: ${cWHITE}LINUX${cDEF}"
fi

# Tools settings
# --------------
echo -e "${cYELLOW}${cBOLD}_________________________________________________________________________________${cDEF}"
echo -e " ${cBLUE}${cBOLD}${cUNDERL}Tools${cDEF}"
echo -e  " - ${cBOLD}Toolchain${cDEF}    : $cWHITE${TOOLCHAIN_PREFIX}${cDEF}"

#Support
echo -e "${cYELLOW}${cBOLD}_________________________________________________________________________________${cDEF}"
echo -e " ${cBLUE}${cBOLD}${cUNDERL}Support ${cDEF}"
export |grep SUPPORT
echo -e "${cYELLOW}${cBOLD}_________________________________________________________________________________${cDEF}"

