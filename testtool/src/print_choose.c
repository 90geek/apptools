
#include "testtool/print_choose.h"

#include "platform/app_os.h"
//#include "system/system_db.h"

static int Log_Print_Flag = 0;


void get_file_name(char *name,int *mark);

int  get_log_flag(void)
{
	return Log_Print_Flag;
}


void set_log_flag(int value)
{
	Log_Print_Flag = value;
	//Update_System_Int_Table(LOG,value);
}


void create_file_name(char *name,int mark)
{
	time_t timep;
    struct tm *p;
    time(&timep);
    p=localtime(&timep); /*取得当地时间*/
	
	sprintf(name,"/usb/sda1/%04d-%02d-%02d-(%02d)",(1900+p->tm_year),( 1+p->tm_mon),p->tm_mday,mark+1);
}

static void *log_task(void *arg)
{
	do{
		if(get_log_flag() == 1)
		{
			struct stat statbuf;
			char file_name[30]= "";
			int mark = 0;
			int iStatus = -1;
			get_file_name(file_name,&mark);
			stat(file_name,&statbuf);
			int size=statbuf.st_size;
			iStatus = stat("/dev/sda1",&statbuf);
			if(size >= 10000000 && iStatus == 0){
				create_file_name(file_name,mark);				
				FILE * stream = NULL;
				stream = freopen(file_name,"ab",stdout);
				setvbuf(stdout, NULL, _IONBF, 0);
			}
			else if(iStatus != 0){
					set_log_flag(0);
					freopen("/dev/console","ab",stdout);
			}
		}
		app_sleep(1000);
	}while(1);

	return 0;
}

void Log_Print_Init(void)
{
	int value;
	app_thread_t*  log_task_t;
	int iStatus = -1;
	struct stat buf;
	
	//Load_Log_Flag(&value);
	//set_log_flag(value);
	if(get_log_flag() == 0)
		freopen("/dev/console","ab",stdout);
	else if(get_log_flag() == 1){
		char file_name[30];
		int mark = 0;
		get_file_name(file_name,&mark);
		iStatus = stat("/dev/sda1",&buf);
		if(iStatus == 0){
			FILE * stream = NULL;
			stream = freopen(file_name,"ab",stdout);
			setvbuf(stdout, NULL, _IONBF, 0);
		}
	}

	log_task_t = app_thread_create((void *)log_task,NULL , 0);
	if(log_task_t == NULL)
	{
		printf("create log task failed\n");
	}
}


void get_file_name(char *name,int *mark)
{
    time_t timep;
    struct tm *p;
	int file_status;
	int count = 0;
    time(&timep);
    p=localtime(&timep); /*取得当地时间*/
	
	char path[40]="";
	
	sprintf(path,"/usb/sda1/%04d-%02d-%02d-(%02d)",(1900+p->tm_year),( 1+p->tm_mon),p->tm_mday,count);
	
	file_status = access(path, 0);
	if(file_status != 0){
		sprintf(name,"/usb/sda1/%04d-%02d-%02d-(%02d)",(1900+p->tm_year),( 1+p->tm_mon),p->tm_mday,count);
		*mark = count ;
	}
	else{
		
		do{
		 count ++;
		 sprintf(path,"/usb/sda1/%04d-%02d-%02d-(%02d)",(1900+p->tm_year),( 1+p->tm_mon),p->tm_mday,count);
		 file_status = access(path, 0);
		}while(file_status == 0);
		sprintf(name,"/usb/sda1/%04d-%02d-%02d-(%02d)",(1900+p->tm_year),( 1+p->tm_mon),p->tm_mday,count-1);
		*mark = count-1;
	}
}

void set_print_path(int path)
{
	int iStatus = -1;
	int file_status;
	struct stat buf;
	
	if(path == 0){
		freopen("/dev/console","ab",stdout);
		set_log_flag(0);
		sleep(2);
	}
	else if(path == 1){
		char file_name[30];
		int mark = 0;
		get_file_name(file_name,&mark);
		iStatus = stat("/dev/sda1",&buf);
		if(iStatus == 0){
			printf("\n==========found USb in!!!!!!\n");
			file_status = access("/usb/sda1", 0);
			if(file_status == 0){
				FILE * stream = NULL;
				stream = freopen(file_name,"ab",stdout);
				setvbuf(stdout, NULL, _IONBF, 0);//实时清除缓存
				set_log_flag(1);
				sleep(2);
			}
		}
		else{
			//Lcd_Display(SECOND_LINE,  "No Usb Insert");
			printf("\n==========/dev/sda1 no USb\n");
			sleep(5);
		}
	}

}

