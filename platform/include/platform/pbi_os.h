#ifndef __PBI_OS_H__
#define __PBI_OS_H__

#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <stdarg.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <errno.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/time.h>

//#include "misc/llist.h"

#include "pbi_types.h"


#include "pbi_debug.h"

/**
 ***/

#define MSG_BUF_MAX_LEN              100
typedef struct _msg_buf_t{
	long mtype;
	unsigned char mtext[MSG_BUF_MAX_LEN];
} msg_buf_t;

#define MSG_QUE_BLOCK  1
#define MSG_QUE_NON_BLOCK 0

typedef void  pbi_mutex_t;
typedef void  pbi_semaphore_t;
typedef void  pbi_file_t;
typedef void  pbi_thread_t;

typedef pthread_attr_t pbi_thread_attr_t;

/**
* @ingroup     
* @brief       Allocate memory.
*
* @param       size     Size to allocate.
* @return      Pointer to allocated memory, NULL if no more memory available.
*/
void* pbi_malloc(S32 size);

/*
* @ingroup     
* @brief       Reallocate memory.
* 
* @param       pData    Pointer to current allocated memory.
* @param       i32Size  Size to allocate.
* @return      Pointer to allocated memory, NULL if no more memory available.
*/
void* pbi_realloc(void* pData, S32 size);
/*
* @ingroup     
* @brief       Free memory.
*
* @param       pData    Pointer to memory to free.
* @return      none
*/
void pbi_free(void* pData);

/*
* @ingroup     
* @brief       Create mutex.
*
* @param       none
* @return      Pointer to created mutex, NULL if creation failed.
*/
pbi_mutex_t* pbi_mutex_create(void);
/*
* @ingroup     
* @brief       Delete mutex.
*
* @param       pMutex   Pointer to mutex to delete.
* @return      none
*/
void pbi_mutex_delete(pbi_mutex_t* pMutex);

/*
* @ingroup     
* @brief       Lock mutex.
*
* @param       pMutex      Pointer to mutex to lock.
* @return      none
*/
void pbi_mutex_lock(pbi_mutex_t* pMutex);

/*
* @ingroup     
* @brief       Unlock mutex.
*
* @param       pMutex      Pointer to mutex to unlock.
* @return      none
*/
void pbi_mutex_unlock(pbi_mutex_t* pMutex);

/*
* @ingroup     
* @brief       Create semaphore.
*
* @param       none
* @return      Pointer to created semaphore, NULL if creation failed.
*/
pbi_semaphore_t* pbi_semaphore_create(void);



S32 pbi_semaphore_wait(pbi_semaphore_t* pSemaphore);

/*
* @ingroup     
* @brief       Wait for semaphore.
*
* @param       pSemaphore     Pointer to semaphore.
* @param       ms             Number of miliseconds to wait.
*
* @return      returns 0 if successfull, otherwise -1.

*/
S32 pbi_semaphore_wait_timeout(pbi_semaphore_t* pSemaphore, S32 ms);


/*
* @ingroup     
* @brief       Signal semaphore.
*
* @param       pSemaphore     Pointer to semaphore.
* @return      returns 0 if successfull, otherwise -1.
*/
S32 pbi_semaphore_signal(pbi_semaphore_t* pSemaphore);

/*
* @ingroup     
* @brief       Delete semaphore.
*
* @param       pSemaphore  Pointer to semaphore to delete.
* @return      none
*/
void pbi_semaphore_delete(pbi_semaphore_t* pSemaphore);

/*
* @ingroup     
* @brief       Wait an amount of time in milliseconds.
*
* @param       ms       Wait time in milliseconds.
* @return      none
*/
void pbi_sleep(S32 ms);

void pbi_sleep_us(S32 us);


/*
* @ingroup     
* @brief       Open file.
*
* @param       pcFilename  NULL-terminated filename.
* @param       pcMode      NULL-terminated opening arguments. "r", "w", "a", "b" supported.
* @return      Pointer to open file, NULL if failure.
*/
pbi_file_t* pbi_file_open(const char* pcFilename, const char* pcMode);
/*
* @ingroup     
* @brief       Close file.
*
* @param       pFile     Pointer to file.
* @return      Zero value if successfull, nonzero value if failure.
*/
S32 pbi_file_close(pbi_file_t* pFile);
/*
* @ingroup     
* @brief       Read bytes from file.
*
* @param       pData    Pointer to a block of memory.
* @param       size     Size in bytes to read.
* @param       pFile    Pointer to file.
* @return      Number of successfully read bytes.
*/
S32 pbi_file_read(void* pData, S32 size, pbi_file_t* pFile);

/*
* @ingroup     
* @brief       Write bytes to file.
*
* @param       pData    Pointer to data to write.
* @param       size     Size in bytes to write.
* @param       pFile    Pointer to file.
* @return      Number of successfully written bytes.
*/
S32 pbi_file_write(const void* pData, S32 size, pbi_file_t* pFile);

/*
* @ingroup     
* @brief       Sets the file position indicator to a new position.
*
* @param       pFile          Pointer to file.
* @param       offset         Number of bytes to offset from origin.
* @param       origin         Position from where offset is added. It is specified by one of the
*                             following constant:
*                             OS_SEEK_CUR   Beginning of file
*                             OS_SEEK_END   Current position of the file pointer
*                             OS_SEEK_SET   End of file
* @return      Zero value if successfull, nonzero value if failure.
*/
S32 pbi_file_seek(pbi_file_t* pFile, S32 offset, S32 origin);
/*
* @ingroup     
* @brief       Gets the file position indicator.
*
* @param       pFile       Pointer to file.
* @return      File position indicator if successfull, -1 if failure.
*/
S32 pbi_file_tell(pbi_file_t* pFile);

/*
* @ingroup     
* @brief       Create thread.
*
* @param       pstFunction       A pointer to the application-defined function to be executed by the thread.
*                                This pointer represents the starting address of the thread.
* @param       pParam            A pointer to a variable to be passed to the thread.
* @param       stackSize         Size of the stack, in bytes.
* @return      Pointer to created thread, NULL if creation failed.
*/
pbi_thread_t* pbi_thread_create(S32 (*pstFunction)(void*), void* pParam,
                                   S32       stackSize);

pbi_thread_t* pbi_thread_create_ext(S32 (*pstFunction)(void*), void* pParam, pbi_thread_attr_t *attr,
                                   S32       stackSize);

/*
* @ingroup     
* @brief       Delete thread.
*
* @param       pstThread      A pointer to the thread to delete.
* @return      Zero value if successfull, nonzero value if failure.
*/
S32 pbi_thread_delete(pbi_thread_t* pstThread);


int pbi_msg_queue_create(int  key) ;
int pbi_msg_queue_delete(int msg_id);
int pbi_msg_send_no_wait(int msg_id,msg_buf_t  *msg_buf, int len);
/*flag = 1, block 
 flag = 0 , no wait*/
int  pbi_msg_receive(int msg_id,msg_buf_t  *msg_buf,int len,int flag);

void pbi_print_data(unsigned char *buf,int size);

#endif
