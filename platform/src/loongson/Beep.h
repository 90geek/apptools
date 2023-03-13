
#ifndef __BEEP_H__
#define __BEEP_H__

#include "edk_api.h"
//
// BEEP
//
typedef struct _LS_BEEP_OPS LS_BEEP_OPS;

typedef
EFI_STATUS
(EFIAPI *EFI_PEI_LS_BEEP_MULOPS) (
  UINT32   KeepMs,
  UINT32   PwmFreq
  );

typedef
EFI_STATUS
(EFIAPI *EFI_PEI_LS_BEEP_OPS) (
  VOID
  );

struct  _LS_BEEP_OPS {
  EFI_PEI_LS_BEEP_MULOPS   BeepPwmOn;
  EFI_PEI_LS_BEEP_OPS      BeepOn;
  EFI_PEI_LS_BEEP_OPS      BeepOff;
};

EFI_STATUS
BeepOn (
  VOID
  );

EFI_STATUS
BeepOff (
  VOID
  );
EFI_STATUS
BeepPwmOn (
  UINT32   KeepMs,
  UINT32   PwmFreq
  );

EFI_STATUS
LsCpuBeepPwmOn (
  UINT32   KeepMs,
  UINT32   PwmFreq
  );
#endif
