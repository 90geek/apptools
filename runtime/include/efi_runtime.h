/*
 * Copyright(C) 2016-2020 Canonical Ltd.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef _FWTS_EFI_RUNTIME_H_
#define _FWTS_EFI_RUNTIME_H_

typedef enum {
	EfiResetCold,
	EfiResetWarm,
	EfiResetShutdown,
	EfiResetPlatformSpecific
} EFI_RESET_TYPE;

typedef struct {
	uint32_t	Data1;
	uint16_t	Data2;
	uint16_t	Data3;
	uint8_t		Data4[8];
} __attribute__ ((packed)) EFI_GUID;

typedef struct {
	uint16_t	Year;		/* 1900 – 9999 */
	uint8_t		Month;		/* 1 – 12 */
	uint8_t		Day;		/* 1 – 31 */
	uint8_t		Hour;		/* 0 – 23 */
	uint8_t		Minute;		/* 0 – 59 */
	uint8_t		Second;		/* 0 – 59 */
	uint8_t		Pad1;
	uint32_t	Nanosecond;	/* 0 – 999,999,999 */
	int16_t		TimeZone;	/* -1440 to 1440 or 2047 */
	uint8_t		Daylight;
	uint8_t		Pad2;
} __attribute__ ((packed)) EFI_TIME;

typedef struct {
	uint32_t	Resolution;
	uint32_t	Accuracy;
	uint8_t		SetsToZero;
} __attribute__ ((packed)) EFI_TIME_CAPABILITIES;

typedef struct {
	EFI_GUID CapsuleGuid;
	uint32_t HeaderSize;
	uint32_t Flags;
	uint32_t CapsuleImageSize;
} __attribute__ ((packed)) EFI_CAPSULE_HEADER;

struct efi_getvariable {
	uint16_t	*VariableName;
	EFI_GUID	*VendorGuid;
	uint32_t	*Attributes;
	uint64_t	*DataSize;
	void		*Data;
	uint64_t	*status;
} __attribute__ ((packed));

struct efi_setvariable {
	uint16_t	*VariableName;
	EFI_GUID	*VendorGuid;
	uint32_t	Attributes;
	uint64_t	DataSize;
	void		*Data;
	uint64_t	*status;
} __attribute__ ((packed));

struct efi_getnextvariablename {
	uint64_t	*VariableNameSize;
	uint16_t	*VariableName;
	EFI_GUID	*VendorGuid;
	uint64_t	*status;
} __attribute__ ((packed));

struct efi_queryvariableinfo {
	uint32_t	Attributes;
	uint64_t	*MaximumVariableStorageSize;
	uint64_t	*RemainingVariableStorageSize;
	uint64_t	*MaximumVariableSize;
	uint64_t	*status;
} __attribute__ ((packed));

struct efi_gettime {
	EFI_TIME		*Time;
	EFI_TIME_CAPABILITIES	*Capabilities;
	uint64_t		*status;
} __attribute__ ((packed));

struct efi_settime {
	EFI_TIME		*Time;
	uint64_t		*status;
} __attribute__ ((packed));

struct efi_getwakeuptime {
	uint8_t		*Enabled;
	uint8_t		*Pending;
	EFI_TIME	*Time;
	uint64_t	*status;
} __attribute__ ((packed));

struct efi_setwakeuptime {
	uint8_t		Enabled;
	EFI_TIME	*Time;
	uint64_t	*status;
} __attribute__ ((packed));

struct efi_getnexthighmonotoniccount {
	uint32_t	*HighCount;
	uint64_t	*status;
} __attribute__ ((packed));

struct efi_querycapsulecapabilities {
	EFI_CAPSULE_HEADER	**CapsuleHeaderArray;
	uint64_t		CapsuleCount;
	uint64_t		*MaximumCapsuleSize;
	EFI_RESET_TYPE		*ResetType;
	uint64_t		*status;
} __attribute__ ((packed));

struct efi_resetsystem {
	int			reset_type;
	uint64_t		status;
	unsigned long		data_size;
	uint16_t		*data;
} __attribute__ ((packed));

/* ioctl calls that are permitted to the /dev/efi_runtime interface. */
#define EFI_RUNTIME_GET_VARIABLE \
	_IOWR('p', 0x01, struct efi_getvariable)
#define EFI_RUNTIME_SET_VARIABLE \
	_IOW('p', 0x02, struct efi_setvariable)

#define EFI_RUNTIME_GET_TIME \
	_IOR('p', 0x03, struct efi_gettime)
#define EFI_RUNTIME_SET_TIME \
	_IOW('p', 0x04, struct efi_settime)

#define EFI_RUNTIME_GET_WAKETIME \
	_IOR('p', 0x05, struct efi_getwakeuptime)
#define EFI_RUNTIME_SET_WAKETIME \
	_IOW('p', 0x06, struct efi_setwakeuptime)

#define EFI_RUNTIME_GET_NEXTVARIABLENAME \
	_IOWR('p', 0x07, struct efi_getnextvariablename)

#define EFI_RUNTIME_QUERY_VARIABLEINFO \
	_IOR('p', 0x08, struct efi_queryvariableinfo)

#define EFI_RUNTIME_GET_NEXTHIGHMONOTONICCOUNT \
	_IOR('p', 0x09, struct efi_getnexthighmonotoniccount)

#define EFI_RUNTIME_QUERY_CAPSULECAPABILITIES \
	_IOR('p', 0x0A, struct efi_querycapsulecapabilities)

#define EFI_RUNTIME_RESET_SYSTEM \
	_IOW('p', 0x0B, struct efi_resetsystem)

#define BITS_PER_LONG   (sizeof(long) * 8)
        
#define HIGH_BIT_SET    (1UL << (BITS_PER_LONG-1))

#define EFI_SUCCESS                     0
#define EFI_LOAD_ERROR                  (1 | HIGH_BIT_SET)
#define EFI_INVALID_PARAMETER           (2 | HIGH_BIT_SET)
#define EFI_UNSUPPORTED                 (3 | HIGH_BIT_SET)
#define EFI_BAD_BUFFER_SIZE             (4 | HIGH_BIT_SET)
#define EFI_BUFFER_TOO_SMALL            (5 | HIGH_BIT_SET)
#define EFI_NOT_READY                   (6 | HIGH_BIT_SET)
#define EFI_DEVICE_ERROR                (7 | HIGH_BIT_SET)
#define EFI_WRITE_PROTECTED             (8 | HIGH_BIT_SET)
#define EFI_OUT_OF_RESOURCES            (9 | HIGH_BIT_SET)
#define EFI_VOLUME_CORRUPTED            (10 | HIGH_BIT_SET)
#define EFI_VOLUME_FULL                 (11 | HIGH_BIT_SET)
#define EFI_NO_MEDIA                    (12 | HIGH_BIT_SET)
#define EFI_MEDIA_CHANGED               (13 | HIGH_BIT_SET)
#define EFI_NOT_FOUND                   (14 | HIGH_BIT_SET)
#define EFI_ACCESS_DENIED               (15 | HIGH_BIT_SET)
#define EFI_NO_RESPONSE                 (16 | HIGH_BIT_SET)
#define EFI_NO_MAPPING                  (17 | HIGH_BIT_SET)
#define EFI_TIMEOUT                     (18 | HIGH_BIT_SET)
#define EFI_NOT_STARTED                 (19 | HIGH_BIT_SET)
#define EFI_ALREADY_STARTED             (20 | HIGH_BIT_SET)
#define EFI_ABORTED                     (21 | HIGH_BIT_SET)
#define EFI_ICMP_ERROR                  (22 | HIGH_BIT_SET)
#define EFI_TFTP_ERROR                  (23 | HIGH_BIT_SET)
#define EFI_PROTOCOL_ERROR              (24 | HIGH_BIT_SET)
#define EFI_INCOMPATIBLE_VERSION        (25 | HIGH_BIT_SET)
#define EFI_SECURITY_VIOLATION          (26 | HIGH_BIT_SET)
#define EFI_CRC_ERROR                   (27 | HIGH_BIT_SET)
#define EFI_END_OF_MEDIA                (28 | HIGH_BIT_SET)
#define EFI_END_OF_FILE                 (31 | HIGH_BIT_SET)
#define EFI_INVALID_LANGUAGE            (32 | HIGH_BIT_SET)
#define EFI_COMPROMISED_DATA            (33 | HIGH_BIT_SET)
#define EFI_IP_ADDRESS_CONFLICT         (34 | HIGH_BIT_SET)
#define EFI_HTTP_ERROR                  (35 | HIGH_BIT_SET)

#define TEST_GUID1 \
{ \
        0xF6FAB04F, 0xACAF, 0x4af3, {0xB9, 0xFA, 0xDC, \
                                                0xF9, 0x7F, 0xB4, 0x42, 0x6F} \
}

#define MAX_DATA_LENGTH         1024

enum {
        FWTS_UEFI_VAR_NON_VOLATILE =                                    0x00000001,
        FWTS_UEFI_VAR_BOOTSERVICE_ACCESS =                              0x00000002,
        FWTS_UEFI_VAR_RUNTIME_ACCESS =                                  0x00000004,
        FWTS_UEFI_VARIABLE_HARDWARE_ERROR_RECORD =                      0x00000008,
        FWTS_UEFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS =                 0x00000010,
        FWTS_UEFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS =      0x00000020,
        FWTS_UEFI_VARIABLE_APPEND_WRITE =                               0x00000040,
        FWTS_UEFI_VARIABLE_ENHANCED_AUTHENTICATED_ACCESS =              0x00000080
};

#endif /* _FWTS_EFI_RUNTIME_H_ */

