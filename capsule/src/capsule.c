#include "platform/app_os.h"
#include "testtool/testtool.h"
#include "capsule.h"
#include <sys/ioctl.h>
#include <linux/ioctl.h>

#define FPGA  0xEF
#define FPGA_SELECT   (_IOW(FPGA, 0x1, int))

int get_file_to_buffer(char * bin_file, unsigned char **buf)
{
   int from_fd=0,file_len=0,count=0;
	 unsigned char  *ptr1 = NULL;
	 int Ret = 0;

  printf("open file %s\n",bin_file);
	if((from_fd=open(bin_file,O_RDONLY))==-1)
	{
		printf("Open %s Error\n",bin_file);
		return -1;
	}

	file_len=lseek(from_fd,0,SEEK_END);
	lseek(from_fd,0,SEEK_SET);
	count=file_len;
	ptr1=app_malloc(file_len*sizeof(char));
	memset(ptr1,0x0,file_len*sizeof(char));
	buf=ptr1;

	Ret=read(from_fd,ptr1,file_len);
	if(Ret==-1)
	{
		printf("Load File Error\n");
		close(from_fd);
		return -1;
	}
	printf("Load File, Size is %d\n",Ret);

  ptr1 = NULL;
	close(from_fd);

  return count;
}

int capsule_update( char* bin_file)
{
  unsigned char  *ptr1 = NULL,*buffer = NULL;
  int fd = 0;
  int ret = 0;
  int len=0;

  len = get_file_to_buffer(bin_file, &buffer);
  if( ret < 0)
  {
    printf("get file buffer failed!!\n");
  }

  ret = system("lsmod |grep efi_capsule_loader");
  if((ret >>8) != 0)
  {
    printf("os no supper capsule update!!!");
  }

	fd = open("/dev/efi_capsule_loader",O_RDWR);
	if( fd == -1 )
  {
		printf("open /dev/efi_capsule_loader error, LINE : %d\n",__LINE__ );	
  }
	ret = write(fd, buffer, len);

	close(fd);
	app_free(buffer);

}

int capsule_update_debug(parse_t * pars_p,char *result_p)
{
	char path[32]={0};

	/* --------------- */
	if (cget_string(pars_p,"", path, sizeof(path))==TRUE)
	{
	  tag_current_line(pars_p,"Invalid path");
	  return 1;
	}
	printf("file path:%s\n",path);

  capsule_update(path);
	return 0;
}

void capsule_debug_register(void)
{
	register_command ("CAPSULE_UPDATE" 			, capsule_update_debug , "file_path");
}

int capsule_init()
{
  capsule_debug_register();
}

