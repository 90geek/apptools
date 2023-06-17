#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "platform/app_types.h"
#include "platform/app_os.h"
#include "edk_api.h"

VOID *
EFIAPI
AllocatePool (
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
VOID *
EFIAPI
SetMem (
  OUT VOID  *Buffer,
  IN UINTN  Length,
  IN UINT8  Value
  )
{
 if (Length == 0) {
    return Buffer;
  }

  // ASSERT ((Length - 1) <= (MAX_ADDRESS - (UINTN)Buffer));

 return memset(Buffer, Value, Length);
}

void Print(const char *format,...)
{
	va_list args;
		va_start(args, format);
		vfprintf(stdout, format, args);
		va_end(args);
}

// VOID DebugPrint (
//   IN  UINTN        ErrorLevel,
//   IN  CONST CHAR8  *Format,
//   ...
//   )
// {
// 	va_list args;
// 		va_start(args, Format);
// 		vfprintf(stdout, Format, args);
// 		va_end(args);
// }
BOOLEAN CompareGuid(const EFI_GUID *guid1, const EFI_GUID *guid2)
{
        BOOLEAN ident = TRUE;

        if ((guid1->Data1 != guid2->Data1) ||
            (guid1->Data2 != guid2->Data2) ||
            (guid1->Data3 != guid2->Data3))
                ident = FALSE;
        else {
                int i;

                for (i = 0; i < 8; i++) {
                        if (guid1->Data4[i] != guid2->Data4[i])
                                ident = FALSE;
                }
        }
        return ident;
}
