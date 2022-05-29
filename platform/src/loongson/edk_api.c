#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "platform/app_types.h"
#include "edk_api.h"

VOID *
EFIAPI
AllocateZeroPool (
  IN UINTN  AllocationSize
  )
{
  VOID  *Memory;

  Memory = malloc (AllocationSize);
  if (Memory != NULL) {
    memset(Memory, 0,AllocationSize);
  }
  return Memory;
}

VOID
EFIAPI
FreePool (
  IN VOID   *Buffer
  )
{
  EFI_STATUS        Status;
  free (Buffer);
}


VOID *
EFIAPI
CopyMem (
  OUT VOID       *DestinationBuffer,
  IN CONST VOID  *SourceBuffer,
  IN UINTN       Length
  )
{
  if (Length == 0) {
    return DestinationBuffer;
  }
  // ASSERT ((Length - 1) <= (MAX_ADDRESS - (UINTN)DestinationBuffer));
  // ASSERT ((Length - 1) <= (MAX_ADDRESS - (UINTN)SourceBuffer));

  if (DestinationBuffer == SourceBuffer) {
    return DestinationBuffer;
  }
  return memcpy (DestinationBuffer, SourceBuffer, Length);
}

void Print(const char *format,...)
{
	va_list args;
		va_start(args, format);
		vfprintf(stdout, format, args);
		va_end(args);
}
