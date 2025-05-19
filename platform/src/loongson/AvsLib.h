
#ifndef __AVS_LIB_H__
#define __AVS_LIB_H__

#include "edk_api.h"

VOID
AvsSetVol (
  UINT64 TotNode,
  INTN RailSel,
  INTN Vol,
  INTN RxDelay,
  INTN ClkDiv
  );

RETURN_STATUS
AvsGetVol (
  UINT64 TotNode,
  INTN RailSel,
  INTN RxDelay,
  INTN ClkDiv
  );

RETURN_STATUS
AvsGet (
  UINT64 TotNode,
  INTN RailSel,
  INTN CmdType
  );

VOID
AvsAdjustVol (
  UINT64 TotNode,
	INTN Vddn,
	INTN Vddp
  );
#endif
