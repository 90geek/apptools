#ifndef __EDK_API_H__
#define __EDK_API_H__

#define UEFI_TYPE 1
#if UEFI_TYPE

#ifndef NULL
#define NULL (void *)0
#endif

/* BOOL type constant values */
#ifndef TRUE
    #define TRUE (1 == 1)
#endif

#ifndef FALSE
    #define FALSE (!TRUE)
#endif

#define EFIAPI
///
/// Datum is read-only.
///
#define CONST     const

///
/// Datum is scoped to the current file or function.
///
#define STATIC    static

///
/// Undeclared type.
///
#define VOID      void

//
// Modifiers for Data Types used to self document code.
// This concept is borrowed for UEFI specification.
//

///
/// Datum is passed to the function.
///
#define IN

///
/// Datum is returned from the function.
///
#define OUT

///
/// Passing the datum to the function is optional, and a NULL
/// is passed if the value is not supplied.
///
#define OPTIONAL

//
//  UEFI specification claims 1 and 0. We are concerned about the
//  compiler portability so we did it this way.
//

//
// Null character
//
#define CHAR_NULL             0x0000

  ///
  /// 8-byte unsigned value
  ///
  typedef unsigned long long  UINT64;
  ///
  /// 8-byte signed value
  ///
  typedef long long           INT64;
  ///
  /// 4-byte unsigned value
  ///
  typedef unsigned int        UINT32;
  ///
  /// 4-byte signed value
  ///
  typedef int                 INT32;
  ///
  /// 2-byte unsigned value
  ///
  typedef unsigned short      UINT16;
  ///
  /// 2-byte Character.  Unless otherwise specified all strings are stored in the
  /// UTF-16 encoding format as defined by Unicode 2.1 and ISO/IEC 10646 standards.
  ///
  typedef unsigned short      CHAR16;
  ///
  /// 2-byte signed value
  ///
  typedef short               INT16;
  ///
  /// Logical Boolean.  1-byte value containing 0 for FALSE or a 1 for TRUE.  Other
  /// values are undefined.
  ///
  typedef unsigned char       BOOLEAN;
  ///
  /// 1-byte unsigned value
  ///
  typedef unsigned char       UINT8;
  ///
  /// 1-byte Character
  ///
  typedef char                CHAR8;
  ///
  /// 1-byte signed value
  ///
  typedef signed char         INT8;

///
/// Unsigned value of native width.  (4 bytes on supported 32-bit processor instructions,
/// 8 bytes on supported 64-bit processor instructions)
///
typedef UINT64  UINTN;
///
/// Signed value of native width.  (4 bytes on supported 32-bit processor instructions,
/// 8 bytes on supported 64-bit processor instructions)
///
typedef INT64   INTN;

#define Write64(addr, data)     (*(volatile UINT64*)(addr) = (data))
#define Write32(addr, data)     (*(volatile UINT32*)(addr) = (data))
#define Write16(addr, data)     (*(volatile UINT16*)(addr) = (data))
#define Writel(addr, data)      (*(volatile UINT32*)(addr) = (data))
#define Writew(addr, data)      (*(volatile UINT16*)(addr) = (data))
#define Writeb(addr, data)      (*(volatile UINT8*)(addr) = (data))
#define Read64(addr)            (*(volatile UINT64*)(addr))
#define Read32(addr)            (*(volatile UINT32*)(addr))
#define Read16(addr)            (*(volatile UINT16*)(addr))
#define Readl(addr)             (*(volatile UINT32*)(addr))
#define Readw(addr)             (*(volatile UINT16*)(addr))
#define Readb(addr)             (*(volatile UINT8*)(addr))

//
// Status codes common to all execution phases
//
typedef UINTN RETURN_STATUS;

typedef RETURN_STATUS             EFI_STATUS;
typedef VOID                      *EFI_HANDLE;
typedef VOID                      *EFI_EVENT;
typedef UINTN                     EFI_TPL;
typedef UINT64                    EFI_LBA;
typedef UINT16                    STRING_REF;
typedef UINT64                    EFI_PHYSICAL_ADDRESS;
typedef UINT64                    EFI_VIRTUAL_ADDRESS;

#define MAX_BIT     0//0x8000000000000000ULL
/**
  Produces a RETURN_STATUS code with the highest bit set.

  @param  StatusCode    The status code value to convert into a warning code.
                        StatusCode must be in the range 0x00000000..0x7FFFFFFF.

  @return The value specified by StatusCode with the highest bit set.

**/
#define ENCODE_ERROR(StatusCode)     ((RETURN_STATUS)(MAX_BIT | (StatusCode)))

/**
  Produces a RETURN_STATUS code with the highest bit clear.

  @param  StatusCode    The status code value to convert into a warning code.
                        StatusCode must be in the range 0x00000000..0x7FFFFFFF.

  @return The value specified by StatusCode with the highest bit clear.

**/
#define ENCODE_WARNING(StatusCode)   ((RETURN_STATUS)(StatusCode))

/**
  Returns TRUE if a specified RETURN_STATUS code is an error code.

  This function returns TRUE if StatusCode has the high bit set.  Otherwise, FALSE is returned.

  @param  StatusCode    The status code value to evaluate.

  @retval TRUE          The high bit of StatusCode is set.
  @retval FALSE         The high bit of StatusCode is clear.

**/
#define RETURN_ERROR(StatusCode)     (((INTN)(RETURN_STATUS)(StatusCode)) < 0)

///
/// The operation completed successfully.
///
#define RETURN_SUCCESS               0

///
/// The image failed to load.
///
#define RETURN_LOAD_ERROR            ENCODE_ERROR (1)

///
/// The parameter was incorrect.
///
#define RETURN_INVALID_PARAMETER     ENCODE_ERROR (2)

///
/// The operation is not supported.
///
#define RETURN_UNSUPPORTED           ENCODE_ERROR (3)

///
/// The buffer was not the proper size for the request.
///
#define RETURN_BAD_BUFFER_SIZE       ENCODE_ERROR (4)

///
/// The buffer was not large enough to hold the requested data.
/// The required buffer size is returned in the appropriate
/// parameter when this error occurs.
///
#define RETURN_BUFFER_TOO_SMALL      ENCODE_ERROR (5)

///
/// There is no data pending upon return.
///
#define RETURN_NOT_READY             ENCODE_ERROR (6)

///
/// The physical device reported an error while attempting the
/// operation.
///
#define RETURN_DEVICE_ERROR          ENCODE_ERROR (7)

///
/// The device can not be written to.
///
#define RETURN_WRITE_PROTECTED       ENCODE_ERROR (8)

///
/// The resource has run out.
///
#define RETURN_OUT_OF_RESOURCES      ENCODE_ERROR (9)

///
/// An inconsistency was detected on the file system causing the
/// operation to fail.
///
#define RETURN_VOLUME_CORRUPTED      ENCODE_ERROR (10)

///
/// There is no more space on the file system.
///
#define RETURN_VOLUME_FULL           ENCODE_ERROR (11)

///
/// The device does not contain any medium to perform the
/// operation.
///
#define RETURN_NO_MEDIA              ENCODE_ERROR (12)

///
/// The medium in the device has changed since the last
/// access.
///
#define RETURN_MEDIA_CHANGED         ENCODE_ERROR (13)

///
/// The item was not found.
///
#define RETURN_NOT_FOUND             ENCODE_ERROR (14)

///
/// Access was denied.
///
#define RETURN_ACCESS_DENIED         ENCODE_ERROR (15)

///
/// The server was not found or did not respond to the request.
///
#define RETURN_NO_RESPONSE           ENCODE_ERROR (16)

///
/// A mapping to the device does not exist.
///
#define RETURN_NO_MAPPING            ENCODE_ERROR (17)

///
/// A timeout time expired.
///
#define RETURN_TIMEOUT               ENCODE_ERROR (18)

///
/// The protocol has not been started.
///
#define RETURN_NOT_STARTED           ENCODE_ERROR (19)

///
/// The protocol has already been started.
///
#define RETURN_ALREADY_STARTED       ENCODE_ERROR (20)

///
/// The operation was aborted.
///
#define RETURN_ABORTED               ENCODE_ERROR (21)

///
/// An ICMP error occurred during the network operation.
///
#define RETURN_ICMP_ERROR            ENCODE_ERROR (22)

///
/// A TFTP error occurred during the network operation.
///
#define RETURN_TFTP_ERROR            ENCODE_ERROR (23)

///
/// A protocol error occurred during the network operation.
///
#define RETURN_PROTOCOL_ERROR        ENCODE_ERROR (24)

///
/// A function encountered an internal version that was
/// incompatible with a version requested by the caller.
///
#define RETURN_INCOMPATIBLE_VERSION  ENCODE_ERROR (25)

///
/// The function was not performed due to a security violation.
///
#define RETURN_SECURITY_VIOLATION    ENCODE_ERROR (26)

///
/// A CRC error was detected.
///
#define RETURN_CRC_ERROR             ENCODE_ERROR (27)

///
/// The beginning or end of media was reached.
///
#define RETURN_END_OF_MEDIA          ENCODE_ERROR (28)

///
/// The end of the file was reached.
///
#define RETURN_END_OF_FILE           ENCODE_ERROR (31)

///
/// The language specified was invalid.
///
#define RETURN_INVALID_LANGUAGE      ENCODE_ERROR (32)

///
/// The security status of the data is unknown or compromised
/// and the data must be updated or replaced to restore a valid
/// security status.
///
#define RETURN_COMPROMISED_DATA      ENCODE_ERROR (33)

///
/// A HTTP error occurred during the network operation.
///
#define RETURN_HTTP_ERROR            ENCODE_ERROR (35)

///
/// The string contained one or more characters that
/// the device could not render and were skipped.
///
#define RETURN_WARN_UNKNOWN_GLYPH    ENCODE_WARNING (1)

///
/// The handle was closed, but the file was not deleted.
///
#define RETURN_WARN_DELETE_FAILURE   ENCODE_WARNING (2)

///
/// The handle was closed, but the data to the file was not
/// flushed properly.
///
#define RETURN_WARN_WRITE_FAILURE    ENCODE_WARNING (3)

///
/// The resulting buffer was too small, and the data was
/// truncated to the buffer size.
///
#define RETURN_WARN_BUFFER_TOO_SMALL ENCODE_WARNING (4)

///
/// The data has not been updated within the timeframe set by
/// local policy for this type of data.
///
#define RETURN_WARN_STALE_DATA       ENCODE_WARNING (5)

///
/// The resulting buffer contains UEFI-compliant file system.
///
#define RETURN_WARN_FILE_SYSTEM      ENCODE_WARNING (6)

#define EFI_SUCCESS               RETURN_SUCCESS
#define EFI_LOAD_ERROR            RETURN_LOAD_ERROR
#define EFI_INVALID_PARAMETER     RETURN_INVALID_PARAMETER
#define EFI_UNSUPPORTED           RETURN_UNSUPPORTED
#define EFI_BAD_BUFFER_SIZE       RETURN_BAD_BUFFER_SIZE
#define EFI_BUFFER_TOO_SMALL      RETURN_BUFFER_TOO_SMALL
#define EFI_NOT_READY             RETURN_NOT_READY
#define EFI_DEVICE_ERROR          RETURN_DEVICE_ERROR
#define EFI_WRITE_PROTECTED       RETURN_WRITE_PROTECTED
#define EFI_OUT_OF_RESOURCES      RETURN_OUT_OF_RESOURCES
#define EFI_VOLUME_CORRUPTED      RETURN_VOLUME_CORRUPTED
#define EFI_VOLUME_FULL           RETURN_VOLUME_FULL
#define EFI_NO_MEDIA              RETURN_NO_MEDIA
#define EFI_MEDIA_CHANGED         RETURN_MEDIA_CHANGED
#define EFI_NOT_FOUND             RETURN_NOT_FOUND
#define EFI_ACCESS_DENIED         RETURN_ACCESS_DENIED
#define EFI_NO_RESPONSE           RETURN_NO_RESPONSE
#define EFI_NO_MAPPING            RETURN_NO_MAPPING
#define EFI_TIMEOUT               RETURN_TIMEOUT
#define EFI_NOT_STARTED           RETURN_NOT_STARTED
#define EFI_ALREADY_STARTED       RETURN_ALREADY_STARTED
#define EFI_ABORTED               RETURN_ABORTED
#define EFI_ICMP_ERROR            RETURN_ICMP_ERROR
#define EFI_TFTP_ERROR            RETURN_TFTP_ERROR
#define EFI_PROTOCOL_ERROR        RETURN_PROTOCOL_ERROR
#define EFI_INCOMPATIBLE_VERSION  RETURN_INCOMPATIBLE_VERSION
#define EFI_SECURITY_VIOLATION    RETURN_SECURITY_VIOLATION
#define EFI_CRC_ERROR             RETURN_CRC_ERROR
#define EFI_END_OF_MEDIA          RETURN_END_OF_MEDIA
#define EFI_END_OF_FILE           RETURN_END_OF_FILE
#define EFI_INVALID_LANGUAGE      RETURN_INVALID_LANGUAGE
#define EFI_COMPROMISED_DATA      RETURN_COMPROMISED_DATA
#define EFI_HTTP_ERROR            RETURN_HTTP_ERROR

#define EFI_WARN_UNKNOWN_GLYPH    RETURN_WARN_UNKNOWN_GLYPH
#define EFI_WARN_DELETE_FAILURE   RETURN_WARN_DELETE_FAILURE
#define EFI_WARN_WRITE_FAILURE    RETURN_WARN_WRITE_FAILURE
#define EFI_WARN_BUFFER_TOO_SMALL RETURN_WARN_BUFFER_TOO_SMALL
#define EFI_WARN_STALE_DATA       RETURN_WARN_STALE_DATA
#define EFI_WARN_FILE_SYSTEM      RETURN_WARN_FILE_SYSTEM

///
/// Last Attempt Status Values
///
#define LAST_ATTEMPT_STATUS_SUCCESS                       0x00000000
#define LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL            0x00000001
#define LAST_ATTEMPT_STATUS_ERROR_INSUFFICIENT_RESOURCES  0x00000002
#define LAST_ATTEMPT_STATUS_ERROR_INCORRECT_VERSION       0x00000003
#define LAST_ATTEMPT_STATUS_ERROR_INVALID_FORMAT          0x00000004
#define LAST_ATTEMPT_STATUS_ERROR_AUTH_ERROR              0x00000005
#define LAST_ATTEMPT_STATUS_ERROR_PWR_EVT_AC              0x00000006
#define LAST_ATTEMPT_STATUS_ERROR_PWR_EVT_BATT            0x00000007

typedef struct {
	UINT32	Data1;
	UINT16	Data2;
	UINT16	Data3;
	UINT8		Data4[8];
} __attribute__ ((packed)) EFI_GUID;

#define EFI_ERROR(A)              RETURN_ERROR(A)
#define GUID_STRING_LENGTH  36
#define PcdMaximumUnicodeStringLength 1000000
#define PcdMaximumAsciiStringLength 1000000
#define PcdGet32(x) x
#endif

VOID *
EFIAPI
AllocatePool (
  IN UINTN  AllocationSize
  );

VOID *
EFIAPI
AllocateZeroPool (
  IN UINTN  AllocationSize
  );

VOID
EFIAPI
FreePool (
  IN VOID   *Buffer
  );

VOID *
EFIAPI
CopyMem (
  OUT VOID       *DestinationBuffer,
  IN CONST VOID  *SourceBuffer,
  IN UINTN       Length
  );

VOID *
EFIAPI
SetMem (
  OUT VOID  *Buffer,
  IN UINTN  Length,
  IN UINT8  Value
  );

void Print(const char *format,...);
BOOLEAN CompareGuid(const EFI_GUID *guid1, const EFI_GUID *guid2);

#endif
