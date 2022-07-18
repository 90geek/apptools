#include "platform/app_platform.h"
#include "platform/app_os.h"
#include "LsRegDef.h"


#define PWM_REG_BASE			( LS7A_PWM0_REG_BASE )	/*LW HPS to FPGA bridge*/

#define GPIO_REGS_BASE		( LS7A_GPIO_REG_BASE )	/*GPIO Address Map*/

#define PWM_REG_SPAN (0x400)	 /*0x400byte*/
#define PWM_REG_MASK (LWH2F_REG_SPAN -1)

#define GPIO_REGS_SPAN ( 0x3000 )			/*12K*/
#define GPIO_REGS_MASK ( GPIO_REGS_SPAN - 1 )


static int fd = 0;
void *gpio_mem_base = NULL;
void *pwm_mem_base = NULL;

static void open_mem_fd(void) ;
static void close_mem_fd(void);

static int pwm_mem_mapping(void);
static void pwm_mem_clean(void);

static int gpio_mem_mapping(void);
static void gpio_mem_clean(void);

void memory_mapping(void)
{
	open_mem_fd() ;
	pwm_mem_mapping();
	gpio_mem_mapping();
}

void memory_release(void)
{
	pwm_mem_clean();
	gpio_mem_clean();
	close_mem_fd();
}

int get_memdev_fd(void)
{
	return fd;
}
void * get_gpio_mem_base(void) 
{
	return gpio_mem_base;
}

void * get_pwm_mem_base(void) 
{
	return pwm_mem_base;
}

/*********/
static void open_mem_fd(void) 
{
	// map the address space for the Peripheral Region Address Map into user space so we can interact with them.
	// we'll actually map in the entire CSR span of the HPS since we want to access various registers within that span
	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
		printf( "ERROR: could not open	\"/dev/mem\"...\n" );
	}
}

static void close_mem_fd(void)
{
	if(fd == 0)
		return;
	close( fd );
}

static int pwm_mem_mapping(void)
{

	pwm_mem_base = mmap( NULL, PWM_REG_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, PWM_REG_BASE );

	if( pwm_mem_base == MAP_FAILED ) {
		printf( "ERROR: pwm_mem_mapping, mmap()  failed...\n" );
		return( 1 );
	}
	return 0;
}

static void pwm_mem_clean(void)
{

	if( pwm_mem_base == NULL)
		return;
	// clean up our memory mapping and exit
	if( munmap( pwm_mem_base, PWM_REG_SPAN ) != 0 )
	{
		printf( "ERROR:pwm_mem_clean,  munmap() failed...\n" );
	}
}

/*********/
static int gpio_mem_mapping(void)
{

	gpio_mem_base = mmap( NULL, GPIO_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, GPIO_REGS_BASE );

	if( gpio_mem_base == MAP_FAILED ) {
		printf( "ERROR:gpio_mem_mapping, mmap()  failed...\n" );
		return( 1 );
	}
	return 0;
}

static void gpio_mem_clean(void)
{

	if( gpio_mem_base == NULL)
		return;
	// clean up our memory mapping and exit
	if( munmap( gpio_mem_base, GPIO_REGS_SPAN ) != 0 )
	{
		printf( "ERROR:gpio_mem_clean,	munmap() failed...\n" );
	}

}

int memoffset;
static int needclose;
void *p2v_mem_mapping(unsigned long long paddr,int size)
{
	unsigned long long memmask;
	void *vaddr = NULL;

	if(fd==0 || fd==-1)
	{
		open_mem_fd() ;
		if(fd==-1)
			return NULL;
		else
			needclose=1;
	}
	memoffset = paddr % getpagesize();
	vaddr = (void*)mmap(NULL,size+memoffset, PROT_READ|PROT_WRITE,MAP_SHARED,fd,paddr-memoffset);
	vaddr = vaddr + memoffset;
	printf("mmap addr start : %p \n",vaddr);
	return vaddr;
}

int p2v_mem_clean(void *vaddr)
{
	int status ;

	if( vaddr != NULL)
	{
		status = munmap(vaddr-memoffset, 1);
		if(status != 0){
			printf("ERROR: p2v_mem_clean , munmap() failed...\n");
		}
		if(needclose)
		{
			close_mem_fd();
			needclose=0;
			fd=0;
		}
		return 0;
	}
	return -1;
}
static int read_buffer(int fd, U8 *buf, U32 count, const char *prefix)
{
	int r = 1;
	U32 r2 = 0;

	while (r2 != count && r != 0)
	{
		r = read(fd, buf + r2, count - r2);
		if (r == -1)
		{
			if (errno != EINTR)
			{
				perror(prefix);
				return -1;
			}
		}
		else
			r2 += r;
	}

	if (r2 != count)
	{
		fprintf(stderr, "%s: Unexpected end of file\n", prefix);
		return -1;
	}

	return 0;
}


/*
 * Copy a physical memory chunk into a memory buffer.
 * This function allocates memory.
 */
void *ls_mem_chunk(U64 base, U32 len, const char *devmem)
{
		void *p;
		int fd;
		struct stat statbuf;
		U64 mmoffset;
		void *mmp;

		if ((fd = open(devmem, O_RDONLY | O_SYNC)) == -1)
		{
				perror(devmem);
				return NULL;
		}

		if ((p = malloc(len)) == NULL)
		{
				perror("malloc");
				goto out;
		}

		if (fstat(fd, &statbuf) == -1)
		{
				fprintf(stderr, "%s: ", devmem);
				perror("stat");
				goto err_free;
		}

		/*
		 * mmap() will fail with SIGBUS if trying to map beyond the end of
		 * the file.
		 */
		if (S_ISREG(statbuf.st_mode) && base + (U64)len > statbuf.st_size)
		{
				fprintf(stderr, "mmap: Can't map beyond end of file %s\n",
						devmem);
				goto err_free;
		}

		mmoffset = base % getpagesize();
		/*
		 * Please note that we don't use mmap() for performance reasons here,
		 * but to workaround problems many people encountered when trying
		 * to read from /dev/mem using regular read() calls.
		 */
		mmp = mmap(NULL, mmoffset + len, PROT_READ, MAP_SHARED, fd, base - mmoffset);
		if (mmp == MAP_FAILED)
				goto try_read;

		memcpy(p, (U8 *)mmp + mmoffset, len);

		if (munmap(mmp, mmoffset + len) == -1)
		{
				fprintf(stderr, "%s: ", devmem);
				perror("munmap");
		}

		goto out;

try_read:
		if (lseek(fd, base, SEEK_SET) == -1)
		{
				fprintf(stderr, "%s: ", devmem);
				perror("lseek");
				goto err_free;
		}

		if (read_buffer(fd, p, len, devmem) == 0)
				goto out;

err_free:
		free(p);
		p = NULL;

out:
		if (close(fd) == -1)
				perror(devmem);

		return p;
}

