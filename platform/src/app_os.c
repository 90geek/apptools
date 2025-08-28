#include "platform/app_os.h"


#define OS_DEBUG

#ifdef OS_DEBUG
#define OS_DBG(fmt, args...)		MODULE_PRINT(0, fmt, "APP_OS",	##args)
#else 
#define OS_DBG(fmt, args...)

#endif
#define OS_INF(fmt, args...)	MODULE_PRINT(YELLOW, fmt, "APP_OS",  ##args)
#define OS_ERR(fmt, args...)	MODULE_PRINT(RED, fmt, "APP_OS",	##args)


/**
* @ingroup		 
* @brief			 Allocate memory.
*
* @param			 size			Size to allocate.
* @return			 Pointer to allocated memory, NULL if no more memory available.
*/
void*  app_malloc(S32 size)
{
	 return malloc(size);
}

/*
* @ingroup		 
* @brief			 Reallocate memory.
* 
* @param			 pData		Pointer to current allocated memory.
* @param			 i32Size	Size to allocate.
* @return			 Pointer to allocated memory, NULL if no more memory available.
*/
void* app_realloc(void* pData, S32 size)
{
	 return realloc(pData, size);
}

/*
* @ingroup		 
* @brief			 Free memory.
*
* @param			 pData		Pointer to memory to free.
* @return			 none
*/
void app_free(void* pData)
{
	 free(pData);
}

/*
* @ingroup		 
* @brief			 Create mutex.
*
* @param			 none
* @return			 Pointer to created mutex, NULL if creation failed.
*/
app_mutex_t* app_mutex_create(void)
{
	 pthread_mutexattr_t attr;
	 pthread_mutex_t *pMutex = NULL;
	 
	 pMutex = malloc(sizeof(pthread_mutex_t));
	 if (pMutex == NULL)
	 {
			OS_ERR("%s: malloc failed.\n", __FUNCTION__);
			return NULL;
	 }
	 
	 pthread_mutexattr_init (&attr);
	 // pthread_mutexattr_settype (&attr, PTHREAD_MUTEX_RECURSIVE_NP);
	 if (pthread_mutex_init (pMutex, &attr) != 0)
	 {
			OS_ERR("%s: mutex init failed.\n", __FUNCTION__);
			return NULL;
	 }
	 
	 pthread_mutexattr_destroy (&attr);

	 return pMutex;
}

/*
* @ingroup		 
* @brief			 Delete mutex.
*
* @param			 pMutex		Pointer to mutex to delete.
* @return			 none
*/
void app_mutex_delete(app_mutex_t* pMutex)
{
	 pthread_mutex_destroy (pMutex);
	 free(pMutex);
}

/*
* @ingroup		 
* @brief			 Lock mutex.
*
* @param			 pMutex			 Pointer to mutex to lock.
* @return			 none
*/
void app_mutex_lock(app_mutex_t* pMutex)
{
	 pthread_mutex_lock (pMutex);
}

/*
* @ingroup		 
* @brief			 Unlock mutex.
*
* @param			 pMutex			 Pointer to mutex to unlock.
* @return			 none
*/
void app_mutex_unlock(app_mutex_t* pMutex)
{
	 pthread_mutex_unlock (pMutex);
}

/*
* @ingroup		 
* @brief			 Create semaphore.
*
* @param			 none
* @return			 Pointer to created semaphore, NULL if creation failed.
*/
app_semaphore_t* app_semaphore_create(void)
{
	 S32 retVal;
	 sem_t *hSemaphore = NULL;
	 
	 hSemaphore = malloc(sizeof(sem_t));
	 if (hSemaphore == NULL)
	 {
			OS_ERR("%s: malloc failed.\n", __FUNCTION__);
			return NULL;
	 }

	 retVal = sem_init(hSemaphore, 0, 0);
	 
	if (retVal < 0)
	{
		return NULL;
	}

	 return hSemaphore;
}

/*
* @ingroup		 
* @brief			 Wait for semaphore.
*
* @param			 pSemaphore			Pointer to semaphore.
* @param			 ms							Number of miliseconds to wait.
*
* @return			 returns 0 if successfull, otherwise -1.
*/
S32 app_semaphore_wait_timeout(app_semaphore_t* pSemaphore, S32 ms)
{
	 S32 retVal;
	 struct timespec sTs;

	 clock_gettime(CLOCK_REALTIME, &sTs);
	 sTs.tv_sec += ms / 1000;
	 ms = ms % 1000;
	 if ((sTs.tv_nsec + ms * 1000000) >= 1000000000){
			sTs.tv_nsec = (sTs.tv_nsec + ms * 1000000) % 1000000000;
			sTs.tv_sec += 1;
	 } else {
			sTs.tv_nsec += ms * 1000000;
	 }

	 retVal = sem_timedwait(pSemaphore, &sTs);

	if (retVal < 0)
	{
		return -1;
	}

	 return 0;
}

S32 app_semaphore_wait(app_semaphore_t* pSemaphore)
{
	 S32 retVal;

	 retVal = sem_wait(pSemaphore);

	if (retVal < 0)
	{
		return -1;
	}

	 return 0;
}


/*
* @ingroup		 
* @brief			 Signal semaphore.
*
* @param			 pSemaphore			Pointer to semaphore.
* @return			 returns 0 if successfull, otherwise -1.
*/
S32 app_semaphore_signal(app_semaphore_t* pSemaphore)
{
	 S32 retVal;

	 retVal = sem_post(pSemaphore);
	 
	if (retVal < 0)
	{
		return -1;
	}

	 return 0;
}

/*
* @ingroup		 
* @brief			 Delete semaphore.
*
* @param			 pSemaphore  Pointer to semaphore to delete.
* @return			 none
*/
void app_semaphore_delete(app_semaphore_t* pSemaphore)
{
	 sem_destroy(pSemaphore);
	 free(pSemaphore);
}

/*
* @ingroup		 
* @brief			 Wait an amount of time in milliseconds.
*
* @param			 ms				Wait time in milliseconds.
* @return			 none
*/
void app_sleep(S32 ms)
{
	 usleep(ms*1000);
}


/*
* @ingroup		 
* @brief			 Wait an amount of time in milliseconds.
*
* @param			 ms				Wait time in milliseconds.
* @return			 none
*/
void app_sleep_us(S32 us)
{
	 usleep(us);
}

/*
* @ingroup		 
* @brief			 Open file.
*
* @param			 pcFilename  NULL-terminated filename.
* @param			 pcMode			 NULL-terminated opening arguments. "r", "w", "a", "b" supported.
* @return			 Pointer to open file, NULL if failure.
*/
app_file_t* app_file_open(const char* pcFilename, const char* pcMode)
{
	 return (app_file_t*)fopen(pcFilename, pcMode);
}

/*
* @ingroup		 
* @brief			 Close file.
*
* @param			 pFile		 Pointer to file.
* @return			 Zero value if successfull, nonzero value if failure.
*/
S32 app_file_close(app_file_t* pFile)
{
	 return (S32)fclose((FILE*)pFile);
}

/*
* @ingroup		 
* @brief			 Read bytes from file.
*
* @param			 pData		Pointer to a block of memory.
* @param			 size			Size in bytes to read.
* @param			 pFile		Pointer to file.
* @return			 Number of successfully read bytes.
*/
S32 app_file_read(void* pData, S32 size, app_file_t* pFile)
{
	 return (S32)fread(pData, 1, size, (FILE*)pFile);
}

/*
* @ingroup		 
* @brief			 Write bytes to file.
*
* @param			 pData		Pointer to data to write.
* @param			 size			Size in bytes to write.
* @param			 pFile		Pointer to file.
* @return			 Number of successfully written bytes.
*/
S32 app_file_write(const void* pData, S32 size, app_file_t* pFile)
{
	 return (S32)fwrite(pData, 1, size , (FILE*)pFile);
}

/*
* @ingroup		 
* @brief			 Sets the file position indicator to a new position.
*
* @param			 pFile					Pointer to file.
* @param			 offset					Number of bytes to offset from origin.
* @param			 origin					Position from where offset is added. It is specified by one of the
*															following constant:
*															APP_SEEK_CUR	 Beginning of file
*															APP_SEEK_END	 Current position of the file pointer
*															APP_SEEK_SET	 End of file
* @return			 Zero value if successfull, nonzero value if failure.
*/
S32 app_file_seek(app_file_t* pFile, S32 offset, S32 origin)
{
	 return (S32)fseek((FILE*)pFile, (long)offset, origin);
}

/*
* @ingroup		 
* @brief			 Gets the file position indicator.
*
* @param			 pFile			 Pointer to file.
* @return			 File position indicator if successfull, -1 if failure.
*/
S32 app_file_tell(app_file_t* pFile)
{
	 return (S32)ftell((FILE*)pFile);
}

/*
* @ingroup		 
* @brief			 Create thread.
*
* @param			 pstFunction			 A pointer to the application-defined function to be executed by the thread.
*																 This pointer represents the starting address of the thread.
* @param			 pParam						 A pointer to a variable to be passed to the thread.
* @param			 stackSize				 Size of the stack, in bytes.
* @return			 Pointer to created thread, NULL if creation failed.
*/
app_thread_t* app_thread_create(S32 (*pstFunction)(void*), void* pParam,
																	 S32			 stackSize)
{
	 pthread_t *pThreadId = NULL;																																
	// void *ret;																																		 
	 
	 pThreadId = malloc (sizeof(pthread_t));
	 if (pThreadId == NULL)
	 {
			OS_ERR("%s: malloc failed.\n", __FUNCTION__);
			return NULL;
	 }
																																								
	 if (pthread_create(pThreadId, NULL, (void*)pstFunction, pParam) != 0)
	 {
			OS_ERR("%s: creating thread failed.\n", __FUNCTION__);
			return NULL;
	 }
	 return pThreadId;
}

app_thread_t* app_thread_create_ext(S32 (*pstFunction)(void*), void* pParam,	app_thread_attr_t *attr, S32	stackSize)
{
	 pthread_t *pThreadId = NULL;																																
	// void *ret;																																		 
	 
	 pThreadId = malloc (sizeof(pthread_t));
	 if (pThreadId == NULL)
	 {
			OS_ERR("%s: malloc failed.\n", __FUNCTION__);
			return NULL;
	 }
																																								
	 if (pthread_create(pThreadId, attr, (void*)pstFunction, pParam) != 0)
	 {
			OS_ERR("%s: creating thread failed.\n", __FUNCTION__);
			return NULL;
	 }
	 
	 return pThreadId;
}


/*
* @ingroup		 
* @brief			 Delete thread.
*
* @param			 pstThread			A pointer to the thread to delete.
* @return			 Zero value if successfull, nonzero value if failure.
*/
S32 app_thread_delete(app_thread_t* pstThread)
{
	 free(pstThread);
	 return 0;
}

int app_msg_queue_create(int	key) 
{
	int msg_id;
//	key_t key = ftok(".", proj_id);
	
//	if (key<0) {
//		perror("ftok()");
//		return	-1;
//	}

	msg_id = msgget(key,IPC_EXCL);	/*检查消息队列是否存在*/
	if(msg_id>=0)
	{
		msgctl(msg_id, IPC_RMID, NULL); 
	}

	msg_id = msgget(key, 0666|IPC_CREAT);/*创建消息队列*/
			if(msg_id <0)
	{
				OS_ERR("failed to create msq | errno=%d [%s]\n",errno,strerror(errno));
	 }
	OS_INF("app_msg_queue_create, msg_key = 0x%x,msg_id = 0x%x\n",key,msg_id);
	return msg_id;
}

int app_msg_queue_delete(int msg_id)
{
	return msgctl(msg_id, IPC_RMID, NULL);	
}

int app_msg_send_no_wait(int msg_id,msg_buf_t  *msg_buf, int len)
{
	int status;

	if(msg_id < 0)
		return -1;		

	status = msgsnd(msg_id, msg_buf, len, IPC_NOWAIT);

	return status;
} 

/*flag = 1, block 
 flag = 0 , no wait*/
int  app_msg_receive(int msg_id,msg_buf_t  *msg_buf,int len,int flag)
{
	int status;

	if(flag == 0)
		status = msgrcv(msg_id, msg_buf, len, msg_buf->mtype, IPC_NOWAIT );
	else
		status = msgrcv(msg_id, msg_buf, len, msg_buf->mtype, 0);

	return status;		
}
// ASCII码数组转字符串
void app_ascii_to_str(const char* codes, int len, char* output) {
    for (int i = 0; i < len; i++) {
        output[i] = (char)(codes[i] & 0x7F); // 只保留7位ASCII
    }
    output[len] = '\0';
}

// 字符串转ASCII码数组
void app_str_to_ascii(const char* str, char* output) {
    for (int i = 0; str[i] != '\0'; i++) {
        output[i] = (char)str[i];
    }
}
void app_print_data(unsigned char *buf,int size)
{
	int tmp=0;
	
	printf("\n\n");
	for(tmp=0;tmp<size;tmp++)
	{
		printf("%02x ", buf[tmp]);
		if(0==((tmp+1)%16))
			printf("*******\n");
	}
	printf("\n\n");
}

//
//rate of progress output
//
void app_ProgressInit(void)
{
	char prog[] = "[                                                                        ]   0%";

	printf("%s", prog);
}

void app_ProgressShow(int percentage)
{
	char prog[] = "[                                                                        ]   0%";
	char per[5];
	int count;
	int i;

	if (percentage > 100) percentage = 100;
	if (percentage < 0) percentage = 0;

	count = percentage * 72 / 100 - 1;

	for (i = 1; i <= count; i++)
		prog[i] = '=';
	prog[i] = '>';

	sprintf(per, "%u%%", percentage);

	memcpy(prog + strlen(prog) - strlen(per), per, strlen(per));

	printf("\r%s", prog);
}

void app_ProgressDone(void)
{
	char prog[] = "[========================================================================] 100%";

	printf("\r%s\n", prog);
}

char* app_system(const char *cmd)
{
	char result[102400] = {0};
	char buf[1024] = {0};
	FILE *fp = NULL;
	char *data=NULL;
	int count=0;

	if( (fp = popen(cmd, "r")) == NULL ) {
			printf("popen error!\n");
			return NULL;
	}

	while (fgets(buf, sizeof(buf), fp)) {
			strcat(result, buf);
			count++;
			if(count*1024==102400)
			{
				printf("result buffer is overflow!!!\n");
				break;
			}
	}
	pclose(fp);
	// printf("result: %s\n", result);
	// printf("resultlen: %d\n", strlen(result));
	if(strlen(result)!=0)
	{
		data=app_malloc(strlen(result));
		if(data==NULL)
			printf("app_molloc failed!!!\n");
		strcpy(data,result);
	}
	return data;
}

U64 app_get_pcie_region(const char *dev)
{
	char cmd[1024] = {0};
	char *data=NULL;
	char * leftover=NULL;
	U64 base=0;

	if(dev==NULL)
		return 0;
	
	sprintf(cmd,"lspci -nnvv -s %s | grep 'Region 0' | awk -F' ' '{print $5}'",dev);
	printf("cmd: %s\n", cmd);
	data=app_system(cmd);
	if(data==NULL)
	{
		printf("app_system failed!!!\n");
		return 0;
	}

	base=strtoul(data,&leftover,16);
	printf("base=0x%llx\n",base);
	if(data!=NULL)
		app_free(data);
	return base;
}

U64 app_get_time_s(void)
{
	 struct timeval tp;
	 struct timezone tz;
	 gettimeofday(&tp, &tz);
	 // printf("second:%ld\n", tp.tv_sec); // 秒
	 // printf("Microseconds:%ld\n", tp.tv_usec); // 微妙
	 return tp.tv_sec;
}

U64 app_get_time_us(void)
{
	 struct timeval tp;
	 struct timezone tz;
	 U64 value=0;
	 gettimeofday(&tp, &tz);
	 // printf("second:%ld\n", tp.tv_sec); // 秒
	 // printf("Microseconds:%ld\n", tp.tv_usec+tp.tv_sec*1000*1000); // 微妙
	 value = (tp.tv_sec&0xff)*1000*1000;
	 value += tp.tv_usec;
	 return value;
}

U64 app_get_time_ns(void)
{
	 U64 value=0;
	 // 需要#include <time.h>头文件
	 struct timespec timestamp;
	 clock_gettime(CLOCK_REALTIME, &timestamp);
	 // printf("second:%ld\n", timestamp.tv_sec); // 秒
	 // printf("nanosecond:%ld\n", timestamp.tv_nsec); // 纳秒
	 value = (timestamp.tv_sec&0xff)*1000*1000*1000;
	 value += timestamp.tv_nsec;
	 return value;
}

