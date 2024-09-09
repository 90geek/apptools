#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <string.h>
#include <inttypes.h>
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <efi_runtime.h>
#include <runtime.h>

static char *efi_dev_name = "/dev/efi_test";
static char *efi_capsule_loader = "/dev/efi_capsule_loader";
static int fd = -1;
static uint32_t attributes =
        FWTS_UEFI_VAR_NON_VOLATILE |
        FWTS_UEFI_VAR_BOOTSERVICE_ACCESS |
        FWTS_UEFI_VAR_RUNTIME_ACCESS;
static uint16_t variablenametest[] = {'T', 'e', 's', 't', 'v', 'a', 'r', '\0'};
static EFI_GUID gtestguid1 = TEST_GUID1;

static bool compare_guid(const EFI_GUID *guid1, const EFI_GUID *guid2)
{
        bool ident = true;

        if ((guid1->Data1 != guid2->Data1) ||
            (guid1->Data2 != guid2->Data2) ||
            (guid1->Data3 != guid2->Data3))
                ident = false;
        else {
                int i;

                for (i = 0; i < 8; i++) {
                        if (guid1->Data4[i] != guid2->Data4[i])
                                ident = false;
                }
        }
        return ident;
}

static bool compare_name(const uint16_t *name1, const uint16_t *name2)
{
        bool ident = true;
        int i = 0;

        while (true) {
                if ((name1[i] != name2[i])) {
                        ident = false;
                        break;
                } else if (name1[i] == '\0')
                        break;
                i++;
        }
        return ident;
}

static int uefi_runtime_settime(char *optarg)
{       
	long ioret;
	struct efi_settime settime;
	struct efi_gettime gettime;
        EFI_TIME old_time;
        EFI_TIME time;
	EFI_TIME_CAPABILITIES efi_time_cap;
        uint64_t status = ~0ULL;
	char *str;
	char *endptr;

	gettime.Capabilities = &efi_time_cap;
	gettime.Time = &old_time;
	gettime.status = &status;

	ioret = ioctl(fd, EFI_RUNTIME_GET_TIME, &gettime);
  
	if (ioret == -1) {
		if (status == EFI_UNSUPPORTED) {
			printf("gettime runtime service is not supported on this platform.\n");
			return ioret;
		}
		printf("Failed to gettime runtime service,status=%lu\n",status);
		return ioret;
	}

	time = old_time;

	str = optarg;
	time.Year   = (uint16_t)strtoul(str, &endptr, 10);

	str = ++endptr;
	time.Month  = (uint8_t)strtoul(str, &endptr, 10);

	str = ++endptr;
	time.Day    = (uint8_t)strtoul(str, &endptr, 10);

	str = ++endptr;
	time.Hour   = (uint8_t)strtoul(str, &endptr, 10);

	str = ++endptr;
	time.Minute = (uint8_t)strtoul(str, &endptr, 10);

	str = ++endptr;
	time.Second = (uint8_t)strtoul(str, &endptr, 10);

        settime.Time = &time;
        status = ~0ULL;
        settime.status = &status;

	ioret = ioctl(fd, EFI_RUNTIME_SET_TIME, &settime);
	if (ioret == -1) {
		if (status == EFI_UNSUPPORTED) {
			printf("settime runtime service is not supported on this platform.\n");
			return ioret;
		}
		printf("Failed to settime runtime service,status=%lu\n",status);
		return ioret;
	}

	printf("test passed: set new time is %u-%02u-%02u %02u:%02u:%02u\n",time.Year, time.Month, time.Day, time.Hour, time.Minute, time.Second);

	return 0;
}

static int uefi_runtime_setwakeuptime(void)
{
        long ioret;
        struct efi_setwakeuptime setwakeuptime;
        uint64_t status = ~0ULL;
        EFI_TIME oldtime;

        struct efi_gettime gettime;
        EFI_TIME_CAPABILITIES efi_time_cap;

        gettime.Capabilities = &efi_time_cap;
        gettime.Time = &oldtime;
        gettime.status = &status;

again:
        ioret = ioctl(fd, EFI_RUNTIME_GET_TIME, &gettime);
        if (ioret == -1) {
                if (status == EFI_UNSUPPORTED) {
                        printf("GetTime runtime service is not supported on this platform.\n");
                        return -1;
                }
                printf("Failed to get time with UEFI runtime service.\n");
                return -1;
        }

	if (oldtime.Minute != 59)
		oldtime.Minute += 1;
	else{
		printf("wait 1 minute\n");
		sleep(60);
		goto again;
	}

        setwakeuptime.Time = &oldtime;
        status = ~0ULL;
        setwakeuptime.status = &status;
        setwakeuptime.Enabled = 1;

        ioret = ioctl(fd, EFI_RUNTIME_SET_WAKETIME, &setwakeuptime);
        if (ioret == -1) {
                if (status == EFI_UNSUPPORTED) {
                        printf("SetWakeupTime runtime service is not supported on this platform.\n");
                        return -1;
                }
                printf("Failed to set wakeup time with UEFI runtime service.\n");
                return -1;
        }

	printf("test passed: set wakeup time is %u-%02u-%02u %02u:%02u:%02u\n",
		oldtime.Year, oldtime.Month, oldtime.Day, oldtime.Hour, oldtime.Minute, oldtime.Second);

	return 0;
}

static int uefi_runtime_getwakeuptime(void)
{
        long ioret;
        struct efi_getwakeuptime getwakeuptime;
        uint64_t status = ~0ULL;
        uint8_t enabled, pending;
        EFI_TIME efi_time;

        getwakeuptime.Enabled = &enabled;
        getwakeuptime.Pending = &pending;
        getwakeuptime.Time = &efi_time;
        getwakeuptime.status = &status;

        ioret = ioctl(fd, EFI_RUNTIME_GET_WAKETIME, &getwakeuptime);
        if (ioret == -1) {
                if (status == EFI_UNSUPPORTED) {
                        printf("GetWakeupTime runtime service is not supported on this platform.\n");
                        return -1;
                }
                printf("Failed to get wakeup time with UEFI runtime service.\n");
                return -1;
        }

        printf("test passed: enable is %d, alarm time is %u-%02u-%02u %02u:%02u:%02u\n",*getwakeuptime.Enabled,
		efi_time.Year, efi_time.Month, efi_time.Day, efi_time.Hour, efi_time.Minute, efi_time.Second);

        return 0;
}

static int uefi_runtime_gettime(void)
{       
	long ioret;
	struct efi_gettime gettime;     
	EFI_TIME efi_time;
	EFI_TIME_CAPABILITIES efi_time_cap;
        uint64_t status = ~0ULL;

	gettime.Capabilities = &efi_time_cap;
	gettime.Time = &efi_time;
	gettime.status = &status;

	ioret = ioctl(fd, EFI_RUNTIME_GET_TIME, &gettime);
  
	if (ioret == -1) {
		if (status == EFI_UNSUPPORTED) {
			printf("gettime runtime service is not supported on this platform.\n");
			return ioret;
		}
		printf("Failed to gettime with UEFI runtime service,status=%lu\n",status);
		return ioret;
	}
	printf("test passed: current time is %u-%02u-%02u %02u:%02u:%02u\n",efi_time.Year, efi_time.Month, efi_time.Day, efi_time.Hour, efi_time.Minute, efi_time.Second);

	return 0;
}

static int getvariable_test(
        const uint64_t datasize,
        uint16_t *varname,
        const uint32_t multitesttime)
{
        long ioret;
        struct efi_getvariable getvariable;
        struct efi_setvariable setvariable;

        uint64_t status = ~0ULL;
        uint8_t testdata[MAX_DATA_LENGTH];
        uint64_t dataindex;
        uint64_t getdatasize = sizeof(testdata);
        uint32_t attributestest;

        uint8_t data[datasize];
        uint32_t i;

        for (dataindex = 0; dataindex < datasize; dataindex++)
                data[dataindex] = (uint8_t)dataindex;

        setvariable.VariableName = varname;
        setvariable.VendorGuid = &gtestguid1;
        setvariable.Attributes = attributes;
        setvariable.DataSize = datasize;
        setvariable.Data = data;
        setvariable.status = &status;

        ioret = ioctl(fd, EFI_RUNTIME_SET_VARIABLE, &setvariable);

        if (ioret == -1) {
                if (status == EFI_UNSUPPORTED) {
                        printf("SetVariable runtime service is not supported on this platform.\n");
                        return -1;
                }
                if (status == EFI_OUT_OF_RESOURCES) {
                        printf("Run out of resources for SetVariable UEFI runtime interface: cannot test.\n");
                        return -1;
                }
                printf("Failed to set variable with UEFI runtime service.\n");
                return -1;
        }

        getvariable.VariableName = varname;
        getvariable.VendorGuid = &gtestguid1;
        getvariable.Attributes = &attributestest;
        getvariable.DataSize = &getdatasize;
        getvariable.Data = testdata;
        getvariable.status = &status;

        for (i = 0; i < multitesttime; i++) {
                status = ~0ULL;
                ioret = ioctl(fd, EFI_RUNTIME_GET_VARIABLE, &getvariable);
                if (ioret == -1) {
                        if (status == EFI_UNSUPPORTED) {
                                printf("GetVariable runtime service is not supported on this platform.\n");
                                goto err_restore_env; 
                        }
                        printf("Failed to get variable with UEFI runtime service.\n");
                        goto err_restore_env;
                }
        }

        if (*getvariable.status != EFI_SUCCESS) {
                printf("Failed to get variable, return status is not EFI_SUCCESS.\n");
                goto err_restore_env;
        } else if (*getvariable.Attributes != attributes) {
                printf("Failed to get variable with right attributes\n");
                goto err_restore_env;
        } else if (*getvariable.DataSize != datasize) {
                printf("Failed to get variable with correct datasize.\n");
                goto err_restore_env;
        } else {
                for (dataindex = 0; dataindex < datasize; dataindex++) {
                        if (data[dataindex] != (uint8_t)dataindex) {
                                printf("Failed to get variable with correct data.\n");
                                goto err_restore_env;
                        }
                }
        }

        /* delete the variable */
        setvariable.DataSize = 0;
        status = ~0ULL;

        ioret = ioctl(fd, EFI_RUNTIME_SET_VARIABLE, &setvariable);

        if (ioret == -1) {
                printf("Failed to delete variable with UEFI runtime service.\n");
                return -1;
        }

	printf("test passed: UEFI runtime service setVariable and getVariable interface.\n");

        return 0;

err_restore_env:

        setvariable.DataSize = 0;
        status = ~0ULL;

        ioret = ioctl(fd, EFI_RUNTIME_SET_VARIABLE, &setvariable);

        if (ioret == -1) {
               	printf("Failed to delete variable with UEFI runtime service.\n");
                return -1;
        }

        return -1;
}

static int getnextvariable_test(void)
{
        long ioret, i;
        uint64_t status = ~0ULL;

	struct efi_setvariable setvariable;
        uint64_t dataindex, datasize = 10;
        uint8_t data[MAX_DATA_LENGTH];

        struct efi_getnextvariablename getnextvariablename;
        uint64_t variablenamesize = MAX_DATA_LENGTH;
        uint64_t maxvariablenamesize = variablenamesize;
        uint16_t *variablename = NULL;
        EFI_GUID vendorguid;
        bool found_name = 0, found_guid = 0;
	int ret = -1;

        variablename = malloc(sizeof(uint16_t) * variablenamesize);
        if (!variablename) {
                printf("Unable to alloc memory for variable name\n");
                return -1;
        }

        for (dataindex = 0; dataindex < datasize; dataindex++)
                data[dataindex] = (uint8_t)dataindex;

        setvariable.VariableName = variablenametest;
        setvariable.VendorGuid = &gtestguid1;
        setvariable.Attributes = attributes;
        setvariable.DataSize = datasize;
        setvariable.Data = data;
        setvariable.status = &status;

        ioret = ioctl(fd, EFI_RUNTIME_SET_VARIABLE, &setvariable);

        if (ioret == -1) {
                if (status == EFI_UNSUPPORTED) {
                        printf("SetVariable runtime service is not supported on this platform.\n");
                }
                if (status == EFI_OUT_OF_RESOURCES) {
                        printf("Run out of resources for SetVariable UEFI runtime interface: cannot test.\n");
                }
                printf("Failed to set variable with UEFI runtime service.\n");
                ret = -1;
		goto err_restore_env;
        }

        getnextvariablename.VariableNameSize = &variablenamesize;
        getnextvariablename.VariableName = variablename;
        getnextvariablename.VendorGuid = &vendorguid;
        getnextvariablename.status = &status;

        variablename[0] = '\0';

        while (1) {
                variablenamesize = maxvariablenamesize;
                status = ~0ULL;
                ioret = ioctl(fd, EFI_RUNTIME_GET_NEXTVARIABLENAME, &getnextvariablename);
                if (ioret == -1) {
                        if (status == EFI_UNSUPPORTED) {
                                printf("GetNextVariableName runtime service is not supported on this platform.\n");
                                ret = -1;
				goto err_restore_env;
                        }
                        /* no next variable was found*/
                        if (*getnextvariablename.status == EFI_NOT_FOUND)
                                break;

                        printf("Failed to get next variable name with UEFI runtime service.\n");
                        ret = -1;
                        goto err_restore_env;
                }

                if (compare_name(getnextvariablename.VariableName, variablenametest))
                        found_name = true;
                if (compare_guid(getnextvariablename.VendorGuid, &gtestguid1))
                        found_guid = true;
                if (found_name && found_guid)
                        break;

		#if 1
		for (i=0; i<variablenamesize;i++)
			printf("%c",variablename[i]);
		printf("-%x-%x-%x-%x%x%x%x%x%x%x%x\n", 
			       getnextvariablename.VendorGuid->Data1,
                               getnextvariablename.VendorGuid->Data2,
                               getnextvariablename.VendorGuid->Data3,
                               getnextvariablename.VendorGuid->Data4[0],
                               getnextvariablename.VendorGuid->Data4[1],
                               getnextvariablename.VendorGuid->Data4[2],
                               getnextvariablename.VendorGuid->Data4[3],
                               getnextvariablename.VendorGuid->Data4[4],
                               getnextvariablename.VendorGuid->Data4[5],
                               getnextvariablename.VendorGuid->Data4[6],
                               getnextvariablename.VendorGuid->Data4[7]);
		#endif
	}

        if (!found_name) {
                printf("Failed to get next variable name with right name.\n");
		ret = -1;
                goto err_restore_env;
        }
        if (!found_guid) {
                printf("Failed to get next variable name correct guid.\n");
		ret = -1;
                goto err_restore_env;
        }

        /* delete the variable */
        setvariable.DataSize = 0;
        status = ~0ULL;

        ioret = ioctl(fd, EFI_RUNTIME_SET_VARIABLE, &setvariable);

        if (ioret == -1) {
                printf("Failed to delete variable with UEFI runtime service.\n");
		ret = -1;
                goto err_restore_env;
        }

        ret = 0;

err_restore_env:

        if (variablename)
                free(variablename);

	if (ret == 0)
		printf("test passed: UEFI runtime service getnextvariablename interface.\n");

        return ret;
}

static int do_queryvariableinfo(
        uint64_t *status,
        uint64_t *maxvarstoragesize,
        uint64_t *remvarstoragesize,
        uint64_t *maxvariablesize)
{
        long ioret;
        struct efi_queryvariableinfo queryvariableinfo;

        queryvariableinfo.Attributes = FWTS_UEFI_VAR_NON_VOLATILE |
                                        FWTS_UEFI_VAR_BOOTSERVICE_ACCESS |
                                        FWTS_UEFI_VAR_RUNTIME_ACCESS;

        queryvariableinfo.MaximumVariableStorageSize = maxvarstoragesize;
        queryvariableinfo.RemainingVariableStorageSize = remvarstoragesize;
        queryvariableinfo.MaximumVariableSize = maxvariablesize;
        *status = ~0ULL;
        queryvariableinfo.status = status;

        ioret = ioctl(fd, EFI_RUNTIME_QUERY_VARIABLEINFO, &queryvariableinfo);

        if (ioret == -1)
                return -1;

        return 0;
}

static int uefivarinfo_test(void)
{
        uint64_t status;
        uint64_t remvarstoragesize;
        uint64_t maxvariablesize;
        uint64_t maxvarstoragesize;

        if (do_queryvariableinfo(&status, &maxvarstoragesize, &remvarstoragesize, &maxvariablesize) == -1) {
                if (status == EFI_UNSUPPORTED) {
                        printf("QueryVariableInfo UEFI runtime interface not supported on this platform.\n");
                        return -1;
                } else {
                        printf("Failed to query variable info with UEFI runtime service.\n");
                        return -1;
                }
        }

        printf("Maximum storage:       %8" PRIu64 " bytes\n", maxvarstoragesize);
        printf("Remaining storage:     %8" PRIu64 " bytes\n", remvarstoragesize);
        printf("Maximum variable size: %8" PRIu64 " bytes\n", maxvariablesize);

        return 0;
}

static int capsule_update(char *optarg)
{
	int ret;
	int file;
	void *buf = NULL;
	char *p = NULL;
	int size = 0, len = 0, total = 0;
	int fd2 = -1;

	fd2 = open(efi_capsule_loader,O_RDWR);
        if (fd2 < 0) {
                printf("open %s fail, fd=%d,please modprobe capsule_loader\n",efi_capsule_loader, fd2);
                return -1;
        }

	buf = malloc(10*1024*1024); /*10M*/
	if (!buf) {
		printf("malloc 10M mem fail\n");
		close(fd2);
		return -1;
	}

	memset(buf, 0, 10*1024*1024);

	file = open(optarg, O_RDONLY);
	if (file < 0) {
		printf("open %s fail\n",optarg);
		close(fd2);
		free(buf);
		return -1;
	}

	size = read(file, buf, 10*1024*1024);

	if (size > 0)
		printf("read %s data success, size = %d\n",optarg, size);
	else {
		printf("read %s data fail, size = %d\n",optarg, size);
		close(fd2);
                free(buf);
		return -1;
	}

	printf("write data");
	while(size != 0) {
		printf(". ");
		p = buf + total;
		len = write(fd2, p, size);
		if (len < 0){
			printf("fail, len=%d\n",len);
			close(fd2);
        		close(file);
        		free(buf);
			return -1;
		}
		size -= len;
		total += len;
	}
	printf("success, size = %d\n", total);
	
	close(fd2);
	close(file);
	free(buf);

	printf("test passed: please reboot to complete capsule update.\n");

	return 0;
}

int efi_runtime_dev_open(void)
{
	if (!efi_dev_name)
		return -1;

	return open(efi_dev_name, O_WRONLY | O_RDWR);
}

int efi_runtime_dev_close(int fd)
{
	return close(fd);
}

void display_help(void)
{
	printf("Usage:\n");
	printf("-h               display help\n");
	printf("-s TIME          settime, TIME is XXXX-XX-XX-XX-XX-XX\n");
	printf("-g               gettime\n");
	printf("-S               setwakeuptime, 1 minute future\n");
	printf("-w               getwakeuptime\n");
	printf("-v               setvariable and getvariable\n");
	printf("-n               getnextvariablename\n");
	printf("-q               queryvariableinfo\n");
	printf("-u path/file     capsule update\n");
}

static int parse_para_options(int argc, char *argv[])
{
	int ret = 0;
	int c;

	static struct option long_options[] = {
		{"help", no_argument, NULL, 'h'},
		{"settime", required_argument, NULL, 's'},
		{"gettime", no_argument, NULL, 'g'},
		{"setwakeuptime", no_argument, NULL, 'S'},
		{"getwakeuptime", no_argument, NULL, 'w'},
		{"variable", no_argument, NULL, 'v'},
		{"getnextvariablename", no_argument, NULL, 'n'},
		{"queryvariableinfo", no_argument, NULL, 'q'},
		{"capsuleupdate", required_argument, NULL, 'u'},
		{NULL, 0, NULL, 0}
	};
		
	for (;;) {
		c =  getopt_long(argc, argv, "hs:gSwvnqu:", long_options, NULL);

		if (c == -1)
		{
			display_help();
			break;
		}

		switch (c) {
			case 'h':
				display_help();
				break;				
			case 'g':
				ret = uefi_runtime_gettime();
				break;
			case 's':
				ret = uefi_runtime_settime(optarg);
				break;
			case 'S':
				ret = uefi_runtime_setwakeuptime();
				break;
			case 'w':
				ret = uefi_runtime_getwakeuptime();
				break;
			case 'v':
				ret = getvariable_test(10, variablenametest, 1);
				break;
			case 'n':
				ret = getnextvariable_test();
				break;
			case 'q':
				ret = uefivarinfo_test();
				break;
			case 'u':
				ret = capsule_update(optarg);
				break;
			default:
				break;
		};
	}

	return ret;	
}

// int main(int argc, char **argv)
int runtime_test(int argc, char **argv)
{
	int ret;

	fd = efi_runtime_dev_open();
	if (fd < 0) {
		printf("open /dev/efi_test fail, fd=%d\n",fd);
		return -1;
	}
	
	ret = parse_para_options(argc, argv);

	return ret;
}
static void RTShowUsage(void)
{
	puts(
		"Usage:\n"
		"apptool rt [parameter] ... :explain\n"
		"  test : run runtime test api\n"
		"  time : run main_uefitime\n"
		"  getvar : run get variable\n"
		"  setvar : run set variable\n"
		"  getnextvarname : run get next variable name and guid\n"
		"  reset :run uefiresetsystem \n");
}
int runtime_start(int argc, char **argv)
{
	int argv_c = argc - 1, argv_p = 1;
	int Ret = 0;

	if (argc == 1)
	{
    RTShowUsage();
		return Ret;
  }

	if (!strcmp(argv[argv_p], "test"))
	{
		runtime_test (argv_c, argv + argv_p);
		return Ret;
	}
  else if (!strcmp(argv[argv_p], "reset"))
	{
		main_uefiresetsystem (argv_c, argv + argv_p);
		return Ret;
	}
  else if (!strcmp(argv[argv_p], "time"))
	{
		main_uefitime (argv_c, argv + argv_p);
		return Ret;
	}
  else if (!strcmp(argv[argv_p], "getvar"))
	{
		main_uefivarget (argv_c, argv + argv_p);
		return Ret;
	}
  else if (!strcmp(argv[argv_p], "setvar"))
	{
		main_uefivarset (argv_c, argv + argv_p);
		return Ret;
	}
  else if (!strcmp(argv[argv_p], "getnextvarname"))
	{
		main_uefigetnextvarname (argv_c, argv + argv_p);
		return Ret;
	}
  else{
    printf("unsupport %s\n",argv[argv_p]);
    RTShowUsage();
		return Ret;
  }
	return Ret;
}
