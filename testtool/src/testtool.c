/*****************************************************************************/
/* COPYRIGHT (C) 2009 STMicroelectronics - All Rights Reserved               */
/* ST makes no warranty express or implied including but not limited to,     */
/* any warranty of                                                           */
/*                                                                           */
/*   (i)  merchantability or fitness for a particular purpose and/or         */
/*   (ii) requirements, for a particular purpose in relation to the LICENSED */
/*        MATERIALS, which is provided AS IS, WITH ALL FAULTS. ST does not   */
/*        represent or warrant that the LICENSED MATERIALS provided here     */
/*        under is free of infringement of any third party patents,          */
/*        copyrights, trade secrets or other intellectual property rights.   */
/*        ALL WARRANTIES, CONDITIONS OR OTHER TERMS IMPLIED BY LAW ARE       */
/*        EXCLUDED TO THE FULLEST EXTENT PERMITTED BY LAW                    */
/*                                                                           */
/*****************************************************************************/
/**
 * @file     testtool.c
 * @brief    This is the testtool component interface.
 * @author   STMicroelectronics
 */

/* Includes */
/* -------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>


#include "testtool/testtool.h"
#include "completion.h"
#include "testtool/clilib.h"
#include "pthread.h"



/* Local defines */
/* ------------- */

/* Local variables */
/* --------------- */

static int module_version = 0;
static int module_ext_version = 1;

static int             UnStarted=1;
static int            overwrite_mode=FALSE;
static pthread_t          Testtool_Task;
static history_t       The_history;
static const           t_key_bind key_binds[] = {{ KEY_BACKSPACE,    "\x08"         , ACTION_BACKSPACE     },
                                                 { KEY_BACKSPACE,    "\x7F"         , ACTION_BACKSPACE     },
                                                 { KEY_BACKSPACE,    "\b"           , ACTION_BACKSPACE     },
                                                 { KEY_RETURN,       "\n"           , ACTION_RETURN        },
                                                 { KEY_RETURN,       "\r"           , ACTION_RETURN        },
                                                 { KEY_END_OF_LINE,  "\x04"         , ACTION_MOVE_EOL      },
                                                 { KEY_END_OF_LINE,  "\x1B[8~"      , ACTION_MOVE_EOL      },
                                                 { KEY_END_OF_LINE,  "\x1B[F"       , ACTION_MOVE_EOL      },
                                                 { KEY_END_OF_LINE,  "\x05"         , ACTION_MOVE_EOL      },
                                                 { KEY_BEGIN_OF_LINE,"\x1B[7~"      , ACTION_MOVE_BOL      },
                                                 { KEY_BEGIN_OF_LINE,"\x1B[H"       , ACTION_MOVE_BOL      },
                                                 { KEY_BEGIN_OF_LINE,"\x1B[1~"      , ACTION_MOVE_BOL      },
                                                 { KEY_BEGIN_OF_LINE,"\x01"         , ACTION_MOVE_BOL      },
                                                 { KEY_ARROW_UP,     "\x1B[A"       , ACTION_HISTORY_UP    },
                                                 { KEY_ARROW_UP,     "\x10"         , ACTION_HISTORY_UP    },
                                                 { KEY_ARROW_DOWN,   "\x1B[B"       , ACTION_HISTORY_DOWN  },
                                                 { KEY_ARROW_DOWN,   "\x0E"         , ACTION_HISTORY_DOWN  },
                                                 { KEY_ARROW_LEFT,   "\x1B[D"       , ACTION_MOVE_LEFT     },
                                                 { KEY_ARROW_LEFT,   "\x02"         , ACTION_MOVE_LEFT     },
                                                 { KEY_ARROW_RIGHT,  "\x1B[C"       , ACTION_MOVE_RIGHT    },
                                                 { KEY_ARROW_RIGHT,  "\x06"         , ACTION_MOVE_RIGHT    },
                                                 { KEY_INSERT,       "\x1B[2~"      , ACTION_TOGGLE_INSERT },
                                                 { KEY_DELETE,       "\x1B[3~"      , ACTION_DELETE        },
                                                 { KEY_DELETE,       "\x1B[4~"      , ACTION_DELETE        },
                                                 { KEY_TAB,          "\t"           , ACTION_COMPLETE      },
                                                 { KEY_CLEAR_LINE,   "\x15"         , ACTION_CLEAR_LINE    },
                                                 { KEY_CLEAR_EOL,    "\x0B"         , ACTION_CLEAR_EOL     },
                                                 { KEY_CTRL_LEFT,    "\x1B[1;5D"    , ACTION_MOVE_BOT      },
                                                 { KEY_CTRL_RIGHT,   "\x1B[1;5C"    , ACTION_MOVE_EOT      }};

/* Local defines */
/* ------------- */
#define DEF_KEY_BINDS_COUNT (sizeof(key_binds)/sizeof(t_key_bind))
#define MAX_HARDKEY_STRLEN  6

#define dev_pts "/dev/pts/0"

static unsigned int get_string(unsigned char *Buffer,unsigned int Size);
static unsigned int get_key(unsigned int *Key);
static unsigned int poll_key(unsigned int *Key);
static unsigned int system_command(char **args);


int get_testtool_mod_ver(void)
{
	return module_version;
}

int get_testtool_mod_ext_ver(void)
{
	return module_ext_version;
}

/* ========================================================================
   Name:        GET_BIND_MATCH
   Description: Macro to match keys
   ======================================================================== */

#define GET_BIND_MATCH(inc,buf,res)                             \
do                                                              \
 {                                                              \
  res=-1;                                                       \
  for (inc=0;inc<DEF_KEY_BINDS_COUNT;inc++)                     \
   {                                                            \
    if (!strcmp((char *)buf,(char *)key_binds[inc].key_string)) \
     {                                                          \
      res=inc;                                                  \
      break;                                                    \
     }                                                          \
   }                                                            \
 } while (0);                                                   \


/* ========================================================================
   Name:		get_string
   Description: Get a string from stdin
   ======================================================================== */

static unsigned int get_string(unsigned char *Buffer,unsigned int Size)
{
  fgets((char *)Buffer,Size,stdin);
  return(strlen((char *)Buffer)+1);
}

/* ========================================================================
   Name:		get_key
   Description: Get a key pressed
   ======================================================================== */

static unsigned int get_key(unsigned int *Key)
{

 /* For LINUX */
 /* ========= */
  struct termio term;
  /* Unblocking the teminal */
  ioctl(0,TCGETA,(void *)&term);
  term.c_lflag &= ~ICANON;
  term.c_lflag &= ~ECHO;
  ioctl(0,TCSETAW,(void *)&term);
  /* Getting the char */
  *Key=(unsigned int)(getchar());
  /* Blocking the teminal */
  ioctl(0,TCGETA,(void *)&term);
  term.c_lflag |= ICANON;
  term.c_lflag |= ECHO;
  ioctl(0,TCSETAW,(void *)&term);
  return(1);
}

/* ========================================================================
   Name:		poll_key
   Description: Check if there is a key pressed
   ======================================================================== */

static unsigned int poll_key(unsigned int *Key)
{
  unsigned int	  ReadChar=0;
  struct pollfd p;
  struct termio term,term_old;
  /* Get control of the terminal */
  ioctl(0,TCGETA,(void *)&term);
  term_old = term;
  term.c_lflag &= ~ICANON;
  term.c_lflag &= ~ECHO;
  ioctl(0,TCSETAW,(void *)&term);
  /* Get event we want to know */
  p.fd	   = 0;
  p.events = POLLIN;
  /* If we receive one thing, get the byte now */
  if (poll(&p,1,-1)>0)
   {
	if (p.revents==POLLIN)
	 {
	  ReadChar=(unsigned int)read(0,Key,1);
	 }
   }
  /* Purge the byte */
  /* tcflush(0,TCIOFLUSH); */
  /* Leave control */
  ioctl(0,TCSETAW,(void *)&term_old);
  return(ReadChar);
}

/* ========================================================================
   Name:		system_command
   Description: Execute an external command in a different process
   ======================================================================== */

static unsigned int system_command(char **args)
{

 int   res=0;
 pid_t son;
 int   status;

 /* Execute a fork to run the system call */
 /* ------------------------------------- */
 if ((args!=NULL)&&(args[0]!=NULL))
  {
   if ((son=fork())==0)
	{
	 execvp(args[0],args);
	 exit(1);
	}
   else
	{
	 waitpid(son,&status,WUNTRACED);
	 if (WIFEXITED(status))
	  {
	   res=WEXITSTATUS(status);
	  }
	}
  }

 /* Return error code */
 /* ----------------- */
 return((unsigned int)res);
}


/* ========================================================================
   Name:        io_read
   Description: Read line command
   ======================================================================== */

int io_read(char *prompt,char *buffer,int buflen)
{
 unsigned char            c;

 unsigned char            cc[3];
 int           cnt,cnt_pos;
 int           i;
 int          eol;
 int          add_in_history;
// e_key_code    key=KEY_NUMBERS;
 unsigned char            hardkey_str[MAX_HARDKEY_STRLEN+1];
 int           ak_str_idx=0;
 int           ak_found_idx;
 e_action_code action_code=ACTION_NOACTION;
 t_completion  completion;

 print((char*)prompt);
 fflush(stdout);

 cnt         = 0;
 cnt_pos     = 0;
 eol         = FALSE;
 buffer[0]   = '\0';
 c           = ' ';
 init_completion(&completion);
 completion.in_completion = FALSE;
 completion.dquote_number = 0;

 /* read characters one by one and perform line editing functions before
    putting them in the read buffer. Characters are echoed as accepted. */
 while ((c != '\0') && (!eol) && (cnt < buflen))
 {

  c = (char)pollkey_console();

  /* Non Printable character : begin of an action key */
  if (!isprint(c))
   {
    do
     {
      hardkey_str[ak_str_idx] = c;
      ak_str_idx++;
      hardkey_str[ak_str_idx] = '\0';
      GET_BIND_MATCH(i, hardkey_str, ak_found_idx);
      /* If we found a match, get the action and break */
      if (ak_found_idx >= 0)
       {
        action_code = key_binds[ak_found_idx].action_code;
      //  key = key_binds[ak_found_idx].key_code;
        ak_str_idx = 0;
        c = ' ';
        break;
       }
      /* At least, check if an action key begins with the chars we have             */
      /* If not, no need to continue and process the printable chars we have readen */
      else
       {
        for (i = 0; i < DEF_KEY_BINDS_COUNT; i++)
        if (!strncmp((char *)hardkey_str,(char *)key_binds[i].key_string, ak_str_idx)) break;
        /* Nothing found */
        if (i == DEF_KEY_BINDS_COUNT) break;
       }
      /* If no action was found AND the string we have so far COULD match but is not complete */
      /* AND we have reached the max char number of a action string : GET OUT                 */
      if (ak_str_idx == MAX_HARDKEY_STRLEN) break;
      /* Else: Read another character */

      c = (char)pollkey_console();

     } while (1);
    /* reinit the hardkey_str for the next possible action key string */
    ak_str_idx = 0;
    hardkey_str[ak_str_idx] = '\0';
    /* if we found no matching action, process the printable characters of hardkey_str */
   }
  /* Printable character */
  else if (isprint(c))
   {
    action_code = ACTION_ADD_CHAR;
   }

  /* Do an action with the specified action code */
  switch(action_code)
   {
    /* Printable character */
    case ACTION_ADD_CHAR:
     if (c == '"') completion.dquote_number++;
     line_insert_buf(buffer, buflen, &cnt, &cnt_pos, overwrite_mode, (char *)&c, 1);
     break;
    /* Return or end of line update the command history */
    case ACTION_RETURN:
     buffer[cnt++] = '\0';
     cc[0] = '\r';
     cc[1] = '\n';
     io_putchars((char *) cc,2);
     eol = TRUE;
     add_in_history = TRUE;
     if ((buffer[0]=='!')&&(buffer[1]!='\0'))
      {
       unsigned int history_id,j;
       if ((buffer[1]=='!') && (buffer[2]=='\0')) history_id=1;
       else sscanf(&buffer[1],"%d",&history_id);
       if ((history_id<=NLINES)&&(history_id>=1))
        {
         j=The_history.write_pointer;
         for (i=NLINES;i!=history_id;i--)
          {
           if (j==NLINES) j=0;
           j++;
          }
         if (j==NLINES) j=0;
         if (The_history.command_history[0][j]!='\0')
          {
           cnt=0;
           while((The_history.command_history[cnt][j]!='\0')&&(cnt<MAX_LINE_LEN-2)) cnt++;
           for (i=0;i<=cnt;i++)
            {
             buffer[i] = The_history.command_history[i][j];
            }
           for (i=cnt;i<MAX_LINE_LEN;i++) buffer[i]='\0';
           add_in_history = FALSE;
          }
        }
      }
     if ((cnt != 1)&&(add_in_history==TRUE))
      {
       /* if chain was not empty */
       for (i = 0; i < cnt + 1; i++)
        {
         The_history.command_history[i][The_history.write_pointer] = buffer[i];
        }
       The_history.write_pointer++;
       if (The_history.write_pointer == NLINES)
        {
         The_history.write_pointer = 0;
        }
       The_history.read_pointer = The_history.write_pointer;
      }
     break;

    /* back_space when possible, note that we may have to adapt the */
    /* buffer if there are some data behind the cursor              */
    case ACTION_BACKSPACE:
     if (cnt_pos > 0 && buffer[cnt_pos - 1] == '"') completion.dquote_number--;
     line_erase_n_chars(buffer, buflen, &cnt, &cnt_pos, 1);
     break;

    /* suppression : remove one character from forward chain ( if */
    /* there are some characters of course                        */
    case ACTION_DELETE:
     if (cnt_pos != cnt)
      {
       for (i = cnt_pos + 1; i < cnt; i++)
        {
         if (buffer[i-1] == '"') completion.dquote_number--;
         buffer[i - 1] = buffer[i];
        }
       buffer[cnt - 1] = ' ';
       io_putchars((char *) (unsigned char *) (buffer + cnt_pos) , cnt - cnt_pos );
       for (i = cnt_pos; i < cnt; i++)
        {
         io_putchars((char *) cc , 1 );
        }
       cnt--;
      }
     break;

    /* tabulation : toggles the overwrite mode */
    case ACTION_TOGGLE_INSERT:
     if (overwrite_mode == FALSE)
      {
       overwrite_mode = TRUE;
      }
     else
      {
       overwrite_mode = FALSE;
      }
     break;

    /* up key : get from command line history */
    case ACTION_HISTORY_UP:
     if (The_history.read_pointer == 0)
      {
       The_history.read_pointer = NLINES - 1;
      }
     else
      {
       The_history.read_pointer--;
      }
     /* erase current line, display previous one */
     cc[0]   = '\b';
     cc[1]   = ' ';
     cc[2]   = '\b';
     /* go forward to delete further text */
     while (cnt_pos < cnt)
      {
       io_putchars((char *) cc+1 , 1 );
       cnt_pos++;
      }
     /* go backward to remove existing text */
     while (cnt > 0)
      {
       io_putchars((char *) cc , 3 );
       cnt--;
      }
     cnt = 0;
     while ((The_history.command_history[cnt][The_history.read_pointer] != '\0')
      && (cnt < MAX_LINE_LEN-2))
     cnt++;
     cnt_pos = cnt;
     for (i = 0; i < cnt; i++)
      {
       buffer[i] = The_history.command_history[i][The_history.read_pointer];
      }
     for (i = cnt; i < MAX_LINE_LEN; i++)
        buffer[i] = '\0';
     io_putchars((char *) (unsigned char*) buffer , cnt );
     break;

    /* down key : get from command line history */
    case ACTION_HISTORY_DOWN:
     The_history.read_pointer++;
     if (The_history.read_pointer==NLINES)
      {
       The_history.read_pointer=0;
      }
     /* erase current line, display previous one */
     cc[0]='\b';
     cc[1]=' ';
     cc[2]='\b';
     /* go forward to delete further text */
     while(cnt_pos<cnt)
      {
       io_putchars((char *)cc+1,1);
       cnt_pos++;
      }
     /* go backward to remove existing text */
     while(cnt>0)
      {
       io_putchars((char *)cc,3);
       cnt--;
      }
     cnt=0;
     while((The_history.command_history[cnt][The_history.read_pointer] != '\0')&&(cnt<MAX_LINE_LEN-2)) cnt++;
     cnt_pos=cnt;
     for (i=0;i<cnt;i++)
      {
       buffer[i]=The_history.command_history[i][The_history.read_pointer];
      }
     for (i=cnt;i<MAX_LINE_LEN;i++) buffer[i]='\0';
     io_putchars((char *)(unsigned char *)buffer,cnt);
     break;

    /* left : go backward */
    case ACTION_MOVE_LEFT:
     if (cnt_pos>0)
      {
       cnt_pos--;
       cc[0] = '\b';
       io_putchars((char *)cc,1);
      }
     break;

    /* right : go forward */
    case ACTION_MOVE_RIGHT:
     if (cnt_pos<cnt)
      {
       cc[0]=buffer[cnt_pos];
       io_putchars((char *)cc,1);
       cnt_pos++;
      }
     break;

    case ACTION_MOVE_EOL:
     while(cnt_pos<cnt)
      {
       cc[0]=buffer[cnt_pos];
       io_putchars((char *)cc,1);
       cnt_pos++;
      }
     break;

    case ACTION_MOVE_BOL:
     cc[0]='\b';
     while(cnt_pos>0)
      {
       io_putchars((char *)cc,1);
       cnt_pos--;
      }
     cnt_pos=0;
    break;

    /* Ctrl right : go to the end of the token */
    case ACTION_MOVE_EOT:
     while (cnt_pos < cnt && (buffer[cnt_pos] == SPACE_CHAR || buffer[cnt_pos] == TAB_CHAR))
      {
       cc[0] = buffer[cnt_pos];
       io_putchars((char *)cc, 1);
       cnt_pos++;
      }
     while (cnt_pos < cnt && buffer[cnt_pos] != SPACE_CHAR && buffer[cnt_pos] != TAB_CHAR)
      {
       cc[0] = buffer[cnt_pos];
       io_putchars((char *)cc, 1);
       cnt_pos++;
      }
     break;

    /* Ctrl left : go to the begining of the token */
    case ACTION_MOVE_BOT:
     cc[0]   = '\b';
     while (cnt_pos > 0 && (buffer[cnt_pos] == SPACE_CHAR || buffer[cnt_pos] == TAB_CHAR))
      {
       io_putchars((char *)cc, 1);
       cnt_pos--;
      }
     while (cnt_pos > 0 && buffer[cnt_pos] != SPACE_CHAR && buffer[cnt_pos] != TAB_CHAR)
      {
       io_putchars((char *)cc, 1);
       cnt_pos--;
      }
     break;

    case ACTION_CLEAR_EOL:
     /* erase current line, display previous one */
     cc[0]   = ' ';
     cc[1]   = '\b';
     /* go forward to delete further text */
     i = cnt_pos;
     while (cnt_pos < cnt)
      {
       io_putchars((char *) cc , 1 );
       if (buffer[cnt_pos] == '"') completion.dquote_number--;
       buffer[cnt_pos] = ' ';
       cnt_pos++;
      }
     /* Go back to the previous position */
     while (cnt_pos > i)
      {
       io_putchars((char *) cc+1 , 1 );
       cnt_pos--;
      }
     cnt = cnt_pos;
     break;

    case ACTION_CLEAR_LINE:
     /* erase current line, display previous one */
     cc[0]   = '\b';
     cc[1]   = ' ';
     cc[2]   = '\b';
     /* go forward to delete further text */
     while (cnt_pos < cnt)
      {
       io_putchars((char *) cc+1 , 1 );
       cnt_pos++;
      }
     /* go backward to remove existing text */
     while (cnt > 0)
      {
       io_putchars((char *)cc, 3);
       cnt--;
      }
     cnt = 0;
     cnt_pos = 0;
     completion.dquote_number = 0;
     break;

    case ACTION_COMPLETE:
     if (cnt_pos == 0) break;
     if (!completion.in_completion)
      {
       init_completion(&completion);
       completion_do_first_tab(&completion, buffer, buflen, &cnt, &cnt_pos);
      }
     else
      {
       completion_do_nth_tab(&completion, buffer, buflen, &cnt, &cnt_pos);
      }
     break;

    default:
     break;
   }
  if (action_code != ACTION_COMPLETE && completion.in_completion) end_completion(&completion);
 }
 return(cnt);
}


/* ========================================================================
   Name:        io_write_console
   Description: I/O write on console
   ======================================================================== */
//static FILE *fp = NULL;
int io_write_console(char *buffer)
{
	
	printf("%s",buffer);
#if 0
	if (access(dev_pts,R_OK) == 0) {			/* judge file exist or not */
		if(fp == NULL)
			fp = fopen("/dev/pts/0", "wb+");
		fwrite(buffer, strlen(buffer), 1, fp);
	} else {
		if(fp !=NULL) {
			fclose(fp);
			fp = NULL;
		}
	}
 #endif
 return(0);
}

/* ========================================================================
   Name:        io_putchars
   Description: I/O putchars
   ======================================================================== */

int io_putchars(char *buffer,unsigned int len)
{
 io_putchars_console(buffer,len);
 return(0);
}

/* ========================================================================
   Name:        io_putchars_console
   Description: I/O putchars on console
   ======================================================================== */

int io_putchars_console(char *buffer,unsigned int len)
{
 write(1,buffer,len);
 return(0);
}

/* ========================================================================
   Name:        io_read_console
   Description: I/O read from console
   ======================================================================== */

int io_read_console(char *prompt,char *buffer,int buflen)
{
 int cnt,i;
 print((char *) prompt);
 fflush(stdout);
 cnt=(int)get_string((unsigned char *)buffer,buflen);
 if (cnt>1)
  {
   for (i=0;i<(cnt+1);i++)
    {
     The_history.command_history[i][The_history.write_pointer]=buffer[i];
    }
   The_history.write_pointer++;
   if (The_history.write_pointer==NLINES)
    {
     The_history.write_pointer=0;
    }
  }
 return(cnt);
}

/* ========================================================================
   Name:        pollkey_console
   Description: Emulate pollkey function from console
   ======================================================================== */

int pollkey_console(void)
{
 unsigned int key,numkeys;
 numkeys=poll_key(&key);
 if (numkeys<1)
  {
   return(-1);
  }
 else
  {
   return((int)key);
  }
}

/* ========================================================================
   Name:        io_setup
   Description: Init the uart port, whatever the host is (console or uart)
   ======================================================================== */

void io_setup(void)
{
 unsigned int i;

 /* Clear unstarted flag */
 /* -------------------- */
 UnStarted=0;

 /* Clear history */
 /* ------------- */
 for (i=0;i<NLINES;i++)
  {
   The_history.command_history[0][i]='\0';
  }
 The_history.read_pointer =0;
 The_history.write_pointer=0;

}

/* ========================================================================
   Name:        readkey
   Description: Readkey from keyboard or console, will determine who is the host
   ======================================================================== */

int readkey(void)
{
 int key;
 /* Under Linux, key won't never be <0 with an infinite timeout */
 key=pollkey_console();

 return((int)key);
}

/* ========================================================================
   Name:        io_pollkey
   Description: Standard io poll key function from uart or console
   ======================================================================== */

int io_pollkey(parse_t *pars_p,char *result_p)
{
 pollkey();
 return(FALSE);
}

/* ========================================================================
   Name:        pollkey
   Description: Standard poll key function from uart or console
   ======================================================================== */

int pollkey(void)
{
   return(pollkey_console());
}

/* ========================================================================
   Name:        io_write_console
   Description: Standard io_write to uart and/or console
   ======================================================================== */

int io_write(char *buffer)
{
   io_write_console(buffer);
 return(0);
}


/* ========================================================================
   Name:        io_history
   Description: Display the history command of the last commands entered
   ======================================================================== */

int io_history (parse_t * pars_p, char *result_p)
{
 int  temp,i,j;
 char tmpbuf[MAX_LINE_LEN];
 print("History of last commands\n");
 temp=The_history.write_pointer;
 for (i=NLINES;i!=0;i--)
  {
   if (temp==NLINES)
    {
     temp=0;
    }
   for (j=0;j<MAX_LINE_LEN;j++)
    {
     tmpbuf[j]=The_history.command_history[j][temp];
    }
   temp++;
   tmpbuf[MAX_LINE_LEN-2] = '\0';
   tmpbuf[MAX_LINE_LEN-1] = '\n';
   print("%2d   -- %s\n",i,tmpbuf);
  }
 return(FALSE);
}

/* ========================================================================
   Name:        io_getchar
   Description: Testtool command that allows to prompt for one character entry
   ======================================================================== */

char io_getchar(char *filter,int case_sensitive)
{
 char  char_got;
 unsigned int   char_console;
 unsigned int   actlen,i;
 int  has_matched=FALSE;
  {
   fflush(stdout);
  }
 while (1)
  {
    {
     get_key(&char_console);
     char_got=(char)char_console;
    }
   /* Now check that char_got matches the filter */
   if (filter==NULL)
    {
     break;
    }
   actlen=strlen(filter);
   for (i=0;i<actlen;i++)
    {
     if (filter[i]==char_got)
      {
       has_matched=TRUE;
      }
    }
   if (has_matched==TRUE)
    {
     break;
    }
  }
 /* Now check if we have to change the case of the character */
 if ((case_sensitive==FALSE)&&(char_got>='A')&&(char_got<= 'Z'))
  {
   char_got+=32; /* Switch to lower case */
  }
 return(char_got);
}

/* ========================================================================
   Name:        display_memory
   Description: Display memory in 32bits style
   ======================================================================== */
#if 0
int display_memory(parse_t *pars_p,char *result_p)
{
 int error;
 int  base;
 int  length;
 int  def_base;
 int  def_range;
 unsigned int  address;
 evaluate_integer("BASEADDRESS" , &def_base  , 10);
 evaluate_integer("RANGE"       , &def_range , 10);
 error=cget_integer(pars_p,def_base,&base);
 if (error)
  {
   tag_current_line(pars_p,"--> Invalid base address !");
  }
 else
  {
   error=(cget_integer(pars_p,def_range,&length)||(length<=0));
   if (error)
    {
     tag_current_line(pars_p,"--> Invalid length !");
    }
   else
    {
     int  cnt,i;
     char contents[17];
     address = (unsigned int)base;
     cnt     = 0;
     while((cnt<=length))
      {
       print("#%08x: %08x %08x %08x %08x",address
                                         ,alt_read_word(get_lwh2f_bus_virtual_base()+address+0x0)
                                         ,alt_read_word(get_lwh2f_bus_virtual_base()+address+0x4)
                                         ,alt_read_word(get_lwh2f_bus_virtual_base()+address+0x8)
                                         ,alt_read_word(get_lwh2f_bus_virtual_base()+address+0xC));
       for (i=0;i<16;)
        {
        	unsigned	int data = alt_read_word(get_lwh2f_bus_virtual_base()+address+i);
         	contents[i]=(unsigned char)(data&0xff);
         	if (isprint((int)contents[i])==0) contents[i]='.';
			i++;
			contents[i]=(unsigned char)((data>>8)&0xff);
			if (isprint((int)contents[i])==0) contents[i]='.';
			i++;
			contents[i]=(unsigned char)((data>>16)&0xff);
			if (isprint((int)contents[i])==0) contents[i]='.';
			i++;
			contents[i]=(unsigned char)((data>>24)&0xff);
			if (isprint((int)contents[i])==0) contents[i]='.';
			i++;
        }
       contents[16]='\0';
       print(" (%s)\n",contents);
       address += 16;
       cnt     += 16;
      }
    }
  }
 return(error||assign_integer(result_p,(int)alt_read_word(get_lwh2f_bus_virtual_base()+base),FALSE));
}
#endif
/* ========================================================================
   Name:        display_memory_bytes
   Description: Display memory in hedit style
   ======================================================================== */
#if 0
int display_memory_bytes(parse_t *pars_p,char *result_p)
{
 int error;
 int  base;
 int  length;
 int  def_base;
 int  def_range;
 unsigned int  address;
 evaluate_integer("BASEADDRESS" , &def_base  , 10);
 evaluate_integer("RANGE"       , &def_range , 10);
 error=cget_integer(pars_p,def_base,&base);
 if (error)
  {
   tag_current_line(pars_p,"--> Invalid base address !");
  }
 else
  {
   error=(cget_integer(pars_p,def_range,&length)||(length<=0));
   if (error)
    {
     tag_current_line(pars_p,"--> Invalid length !");
    }
   else
    {
     int  cnt;
     char contents[17];
     int  i;
     address = (unsigned int)base;
     cnt     = 0;
     while((cnt<=length))
      {
       print("#%08x: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x",address
                                                                                                     ,SYS_ReadRegDev8(address+0x0)
                                                                                                     ,SYS_ReadRegDev8(address+0x1)
                                                                                                     ,SYS_ReadRegDev8(address+0x2)
                                                                                                     ,SYS_ReadRegDev8(address+0x3)
                                                                                                     ,SYS_ReadRegDev8(address+0x4)
                                                                                                     ,SYS_ReadRegDev8(address+0x5)
                                                                                                     ,SYS_ReadRegDev8(address+0x6)
                                                                                                     ,SYS_ReadRegDev8(address+0x7)
                                                                                                     ,SYS_ReadRegDev8(address+0x8)
                                                                                                     ,SYS_ReadRegDev8(address+0x9)
                                                                                                     ,SYS_ReadRegDev8(address+0xA)
                                                                                                     ,SYS_ReadRegDev8(address+0xB)
                                                                                                     ,SYS_ReadRegDev8(address+0xC)
                                                                                                     ,SYS_ReadRegDev8(address+0xD)
                                                                                                     ,SYS_ReadRegDev8(address+0xE)
                                                                                                     ,SYS_ReadRegDev8(address+0xF));
       for (i=0;i<16;i++)
        {
         contents[i]=SYS_ReadRegDev8(address+i);
         if (isprint((int)contents[i])==0) contents[i]='.';
        }
       contents[16]='\0';
       print(" (%s)\n",contents);
       address += 16;
       cnt     += 16;
      }
    }
  }
 return(error||assign_integer(result_p,(int)alt_read_word(base),FALSE));
}
#endif
/* ========================================================================
   Name:        find_string
   Description: Finds string in range of memory
   ======================================================================== */

int find_string(parse_t *pars_p,char *result_p)
{
 int error;
 int  base;
 int  length;
 int  def_base;
 int  def_range;
 char string[MAX_TOK_LEN];
 char def_string[MAX_TOK_LEN];
 int  address;
 int  match=0;
 evaluate_integer ("BASEADDRESS" , &def_base  , 10);
 evaluate_integer ("RANGE"       , &def_range , 10);
 evaluate_string  ("TESTSTRING"  , def_string , MAX_TOK_LEN);
 if (cget_string(pars_p,def_string,string,sizeof(string))||(strlen(string)==0))
  {
   tag_current_line(pars_p,"Illegal search string !");
   error=TRUE;
  }
 else
  {
   error=cget_integer(pars_p,def_base,&base);
   if (error)
    {
     tag_current_line(pars_p,"--> Invalid base address !");
    }
   else
    {
     error=(cget_integer(pars_p,def_range,&length)||(length<=0));
     if (error)
      {
       tag_current_line(pars_p,"--> Invalid length !");
      }
     else
      {
       address=base;
       while(address<(base+length))
        {
         if (*((char *)(uintptr_t)address)==string[0])
          {
           if (memcmp((char *)(uintptr_t)address,string,strlen(string))==0)
            {
             print("Match found at #%lx\n",address);
             match   = address;
             address = address+strlen(string);
            }
           else
            {
             address++;
            }
          }
         else
          {
           address++;
          }
        }
      }
    }
  }
 return(error||assign_integer(result_p,match,FALSE));
}

/* ========================================================================
   Name:        fill_memory
   Description: Fill memory with a pattern
   ======================================================================== */
#if 0
int fill_memory(parse_t *pars_p,char *result_p)
{
 int error;
 int  base;
 int  length;
 int  value;
 int  def_base;
 int  def_range;
 int  def_value;
 evaluate_integer ("BASEADDRESS" , &def_base  , 10);
 evaluate_integer ("RANGE"       , &def_range , 10);
 evaluate_integer ("DATAVALUE"   , &def_value , 10);
 error=cget_integer(pars_p,def_base,&base);
 if (error)
  {
   tag_current_line(pars_p,"--> Invalid base address !");
  }
 else
  {
   error=(cget_integer(pars_p,def_range,&length)||(length<=0));
   if (error)
    {
     tag_current_line(pars_p,"--> Invalid length in bytes, must be a multiple of 4 bytes !");
    }
   else
    {
     error=cget_integer(pars_p,def_value,&value);
     if (error)
      {
       tag_current_line(pars_p,"--> Invalid data value !");
      }
     else
      {
       unsigned int i;
       unsigned int base_inc=(unsigned int)base;
       for (i=0;i<(length/4);i++)
        {
    	  /*to do*/
         //alt_write_word(base_inc,value);
         base_inc+=4;
        }
      }
    }
  }
 return(error||assign_integer(result_p,base,FALSE));
}
#endif
/* ========================================================================
   Name:        copy_memory
   Description: Copy memory content to new address range
   ======================================================================== */
#if 0
int copy_memory(parse_t *pars_p,char *result_p)
{
 int error;
 int  base;
 int  length;
 int  newbase;
 int  def_base;
 int  def_range;
 int  def_newbase;
 evaluate_integer("BASEADDRESS"  , &def_base    , 10);
 evaluate_integer("RANGE"        , &def_range   , 10);
 evaluate_integer("ADDRESSVALUE" , &def_newbase , 10);
 error=cget_integer(pars_p,def_base,&base);
 if (error)
  {
   tag_current_line(pars_p,"--> Invalid base address !");
  }
 else
  {
   error=(cget_integer(pars_p,def_range,&length)||(length<=0));
   if (error)
    {
     tag_current_line(pars_p,"--> Invalid length !");
    }
   else
    {
     error=(cget_integer(pars_p,def_newbase,&newbase)||(newbase==base));
     if (error)
      {
       tag_current_line(pars_p,"--> Invalid new base address !");
      }
     else
      {
       unsigned int i;
       unsigned int base_inc=(unsigned int)base;
       unsigned int newbase_inc=(unsigned int)newbase;
       for (i=0;i<length;i++)
        {
         SYS_WriteRegDev8(newbase_inc,SYS_ReadRegDev8(base_inc));
         newbase_inc++;
         base_inc++;
        }
      }
    }
  }
 return(error||assign_integer(result_p,base,FALSE));
}
#endif
/* ========================================================================
   Name:        peek_byte_memory
   Description: Peek an 8 bit value
   ======================================================================== */
/*
int peek_byte_memory(parse_t *pars_p,char *result_p)
{
 int error;
 int  base;
 int  def_base;
 int  value;
 evaluate_integer("BASEADDRESS",&def_base,10);
 error=cget_integer(pars_p,def_base,&base);
 if (error)
  {
   tag_current_line(pars_p,"--> Expected base address !");
  }
 else
  {
   value=(int)SYS_ReadRegDev8((unsigned int)base);
   print("read 0x%x (hex) = %d (dec)\n",(unsigned char)value,(unsigned char)value);
  }
 return(error||assign_integer(result_p,value,FALSE));
}
*/
/* ========================================================================
   Name:        poke_byte_memory
   Description: Poke an 8 bit value
   ======================================================================== */
/*
int poke_byte_memory(parse_t *pars_p,char *result_p)
{
 int error;
 int  base;
 int  value;
 int  def_base;
 int  def_value;
 evaluate_integer("BASEADDRESS" , &def_base  , 10);
 evaluate_integer("DATAVALUE"   , &def_value , 10);
 error=cget_integer(pars_p,def_base,&base);
 if (error)
  {
   tag_current_line(pars_p,"--> Invalid base address !");
  }
 else
  {
   error=cget_integer(pars_p,def_value,&value);
   if ((value<0)||(value>255))
    {
     error=TRUE;
    }
   if (error)
    {
     tag_current_line(pars_p,"--> Invalid data value !");
    }
   else
    {
     SYS_WriteRegDev8((unsigned int)base,(unsigned char)value);
    }
  }
 return(error||assign_integer(result_p,value,FALSE));
}
*/
/* ========================================================================
   Name:        peek_short_memory
   Description: Peek a 16 bit value
   ======================================================================== */
/*
int peek_short_memory(parse_t *pars_p,char *result_p)
{
 int error;
 int  base;
 int  def_base;
 int  value;
 evaluate_integer("BASEADDRESS",&def_base,10);
 error=cget_integer(pars_p,def_base,&base);
 if (error)
  {
   tag_current_line(pars_p,"--> Expected base address !");
  }
 else
  {
   value=(int)SYS_ReadRegDev16LE((unsigned int)base);
   print("read 0x%x (hex) = %d (dec)\n",(unsigned short)value,(unsigned short)value);
  }
 return(error||assign_integer(result_p,value,FALSE));
}
*/
/* ========================================================================
   Name:        poke_short_memory
   Description: Poke a 16 bit value
   ======================================================================== */
#if 0
int poke_short_memory(parse_t *pars_p,char *result_p)
{
 int error;
 int  base;
 int  value;
 int  def_base;
 int  def_value;
 evaluate_integer("BASEADDRESS" , &def_base  , 10);
 evaluate_integer("DATAVALUE"   , &def_value , 10);
 error=cget_integer(pars_p,def_base,&base);
 if (error)
  {
   tag_current_line(pars_p,"--> Invalid base address !");
  }
 else
  {
   error=cget_integer(pars_p, def_value, &value);
   if ((value<0)||(value>65536))
    {
     error=TRUE;
    }
   if (error)
    {
     tag_current_line(pars_p,"--> Invalid data value !");
    }
   else
    {
     SYS_WriteRegDev16LE((unsigned int)base,(unsigned short)value);
    }
  }
 return(error||assign_integer(result_p,value,FALSE));
}
#endif
/* ========================================================================
   Name:        peek_memory
   Description: Peek a 32 bit value
   ======================================================================== */
#if 0
int peek_memory(parse_t *pars_p,char *result_p)
{
 int error;
 int  base;
 int  def_base;
 int  value;
 evaluate_integer("BASEADDRESS",&def_base,10);
 error=cget_integer(pars_p,def_base,&base);
 if (error)
  {
   tag_current_line (pars_p, "--> Expected base address !");
  }
 else
  {
   print("read 0x%08x (hex) = ", get_lwh2f_bus_virtual_base()+base);
   value=(int)alt_read_word(get_lwh2f_bus_virtual_base()+base);
   print("%u (dec)\n",(int)value);
  }
 return(error || assign_integer(result_p,value,FALSE));
}

/* ========================================================================
   Name:        poke_memory
   Description: Poke a 32 bit value
   ======================================================================== */

int poke_memory(parse_t * pars_p,char *result_p)
{
 int error;
 int  base;
 int  value;
 int  def_base;
 int  def_value;
 evaluate_integer("BASEADDRESS" , &def_base  , 10);
 evaluate_integer("DATAVALUE"   , &def_value , 10);
 error=cget_integer(pars_p,def_base,&base);
 if (error)
  {
   tag_current_line(pars_p,"--> Invalid base address !");
  }
 else
  {
   error=cget_integer(pars_p,def_value,&value);
   if (error)
    {
     tag_current_line(pars_p,"--> Invalid data value !");
    }
   else
    {
     alt_write_word(get_lwh2f_bus_virtual_base()+base,(unsigned int)value);
    }
  }
 return(error||assign_integer(result_p,value,FALSE));
}
#endif
/* ========================================================================
   Name:        wait_time
   Description: Pause and deschedule
   ======================================================================== */

int wait_time(parse_t * pars_p,char *result_p)
{
 int error;
 int  time;
 error=cget_integer(pars_p,1,&time);
 if (error||(time<0) ||(time>5000000))
  {
   error=TRUE;
   tag_current_line(pars_p,"--> Expected number of milli seconds to wait [0...5000000ms] !");
  }
 else
  {
   	usleep(time*1000);
  }
 return(error);
}

/* ========================================================================
   Name:        getakey
   Description: Wait for a key pressed
   ======================================================================== */

int getakey(parse_t *pars_p,char *result_p)
{
 int chain;
 chain=io_getchar(NULL,FALSE);
 if ((chain=='q')||(chain=='Q'))
  {
   return(TRUE);
  }
 return(assign_integer(result_p,chain,FALSE));
}

/* ========================================================================
   Name:        dump_to_file
   Description: Save a file from the memory
   ======================================================================== */
#if 0
int dump_to_file(parse_t * pars_p,char *result_p)
{
 char   Filename[MAX_TOK_LEN];
 int    Address,Size,NbBytesWritten;
 void  *File;

 /* Get filename */
 /* ------------ */
 if (cget_string(pars_p,"c:\file_dump.bin",Filename,MAX_TOK_LEN)==TRUE)
  {
   tag_current_line(pars_p,"--> Illegal file name, should be a string containing the file name !");
   return(TRUE);
  }

 /* Get the address */
 /* --------------- */
 if (cget_integer(pars_p,0xDEADBAFE,&Address)==TRUE)
  {
   tag_current_line(pars_p,"--> Invalid address from which we want to start the dump !");
   return(TRUE);
  }
 if (Address==0xDEADBAFE)
  {
   print("--> Invalid address from which we want to start the dump !\n");
   return(TRUE);
  }

 /* Size to dump */
 /* ------------ */
 if (cget_integer(pars_p,0x00000000,&Size)==TRUE)
  {
   tag_current_line(pars_p,"--> Invalid size to dump !");
   return(TRUE);
  }
 if (Size==0x00000000)
  {
   print("--> Invalid size to dump !\n");
   return(TRUE);
  }

 /* Start the dump now */
 /* ------------------ */
 File=SYS_FOpen(Filename,"wb");
 if (File==NULL)
  {
   print("--> Unable to open the file \"%s\" !\n",Filename);
   return(TRUE);
  }
 NbBytesWritten=SYS_FWrite((char *)Address,1,Size,File);
 SYS_FClose(File);

 /* Return no errors */
 /* ---------------- */
 print("%d bytes have been transfered from memory at @ 0x%08x to the file \"%s\"\n",NbBytesWritten,Address,Filename);
 return(FALSE);
}
#endif
/* ========================================================================
   Name:        dump_from_file
   Description: Load a file into the memory
   ======================================================================== */
#if 0
int dump_from_file(parse_t *pars_p,char *result_p)
{
 char   Filename[MAX_TOK_LEN];
 int    Address,MaxSize,NbBytesRead;
 signed long long    FileSize;
 void  *File;

 /* Get filename */
 /* ------------ */
 if (cget_string(pars_p,"c:\file_dump.bin",Filename,sizeof(Filename))==TRUE)
  {
   tag_current_line(pars_p,"--> Illegal file name, should be a string containing the file name !");
   return(TRUE);
  }

 /* Get the address */
 /* --------------- */
 if (cget_integer(pars_p,0xDEADBAFE,&Address)==TRUE)
  {
   tag_current_line(pars_p,"--> Invalid address from which we want to load the file !");
   return(TRUE);
  }
 if (Address==0xDEADBAFE)
  {
   print("--> Invalid address from which we want to load the file !\n");
   return(TRUE);
  }

 /* Size to read */
 /* ------------ */
 if (cget_integer(pars_p,0x00000000,&MaxSize)==TRUE)
  {
   tag_current_line(pars_p,"--> Invalid size to load !");
   return(TRUE);
  }
 if (MaxSize==0x00000000)
  {
   print("--> Invalid size to load !\n");
   return(TRUE);
  }

 /* Start the read now */
 /* ------------------ */
 File=SYS_FOpen(Filename,"rb");
 if (File==NULL)
  {
   print("--> Unable to open the file \"%s\" !\n",Filename);
   return(TRUE);
  }

 /* Get size of the file */
 /* -------------------- */
 SYS_FSeek(File,0,SEEK_END);
 FileSize=SYS_FTell(File);
 SYS_FSeek(File,0,SEEK_SET);
 if (FileSize==0)
  {
   SYS_FClose(File);
   return(FALSE);
  }
 if (MaxSize>FileSize) MaxSize=(int)FileSize;

 /* Load the file */
 /* ------------- */
 NbBytesRead=SYS_FRead((char *)Address,1,MaxSize,File);
 SYS_FClose(File);

 /* Return no errors */
 /* ---------------- */
 print("%d bytes have been loaded from the file \"%s\" to memory at @ 0x%08x\n",NbBytesRead,Filename,Address);
 return(FALSE);
}
#endif
/* ========================================================================
   Name:        allocate_memory
   Description: Allocate some memory
   ======================================================================== */
#if 0
int allocate_memory(parse_t * pars_p, char *result_p)
{
 int   error;
 int   size;
 char *address_p;
 error=cget_integer(pars_p,-1,&size);
 if ((error)||(size<0))
  {
   tag_current_line(pars_p, "--> Expected valid size to allocate !");
   error=TRUE;
  }
 else
  {
   address_p=(char *)SYS_Memory_Allocate(SYS_MEMORY_NCACHED|SYS_MEMORY_CONTIGUOUS,size,0);
   if (address_p==NULL)
    {
     print("--> Unable to allocate this size");
     return(TRUE);
    }
   else
    {
     print("%d bytes allocated at 0x%08x\n",size,(int)address_p);
    }
  }
 return(error||assign_integer(result_p,(int)address_p,FALSE));
}

/* ========================================================================
   Name:        free_memory
   Description: Deallocate some memory
   ======================================================================== */

int free_memory(parse_t *pars_p,char *result_p)
{
 int error;
 int  address;
 error=cget_integer(pars_p,0,&address);
 if ((error)||(address==0))
  {
   tag_current_line(pars_p,"--> Expected valid address pointer to free !");
   error=TRUE;
  }
 else
  {
   SYS_Memory_Deallocate(SYS_MEMORY_NCACHED|SYS_MEMORY_CONTIGUOUS,(unsigned int)address);
  }
 return(error);
}
#endif
/* ========================================================================
   Name:        execute_shell_command
   Description: Call the above function (LINUX ONLY)
   ======================================================================== */

int execute_shell_command(parse_t *pars_p, char *result)
{
 char   command[256];
 char **args;
 int    i,len;
 int    tokens=0;
 if (cget_string(pars_p,"true",command,sizeof(command))==TRUE)
  {
   tag_current_line(pars_p,"--> Invalid argument, should be a unique string containing the shell command !");
   return(TRUE);
  }
 i=0;
 while((command[i]!=0)&&((command[i]==' ')||(command[i]=='\t'))) i++;
 while(command[i] != 0)
  {
   while((command[i]!=0)&&(command[i]!=' ')&&(command[i] != '\t')) i++;
   tokens++;
   while((command[i]!=0)&&((command[i]==' ')||(command[i] == '\t'))) i++;
  }
 if (tokens<1)
  {
   tag_current_line(pars_p,"--> Invalid argument, should be a unique string containing at least one token !");
   return(TRUE);
  }
 args=(char **)malloc((tokens+1)*sizeof(char *));
 if (args==NULL)
  {
   print("--> Not enough memory to execute this command !\n");
   return(TRUE);
  }
 len    = i;
 i      = 0;
 tokens = 0;
 do
  {
   while ((command[i]!=0)&&((command[i]==' ')||(command[i]=='\t'))) i++;
   args[tokens++]=command+i;
   while ((command[i]!=0)&&((command[i]!=' ')&&(command[i]!='\t'))) i++;
   command[i++]=0;
  } while (i<len);
 args[tokens]=NULL;
 i=system_command(args);
 free(args);
 return(i);
}

/* ========================================================================
   Name:        cli_setup
   Description: Register default routines
   ======================================================================== */

static int cli_setup()
{
 //register_command ("DISPLAY"     , display_memory        , "<Address> <Range> Displays local memory content");
// register_command ("DISPLAY_B"   , display_memory_bytes  , "<Address> <Range> Displays local memory content in byte format");
 register_command ("SEARCH"      , find_string           , "<String> <Address> <Range> Searchs for local memory contents");
// register_command ("FILL"        , fill_memory           , "<Address> <Range> <Value> Fill local memory with value");
// register_command ("COPY"        , copy_memory           , "<Address> <Range> <NewAddress> Copy local memory content");
// register_command ("ALLOC"       , allocate_memory       , "<Size> Allocate some memory in partition");
// register_command ("FREE"        , free_memory           , "<Address> Deallocate the memory");
// register_command ("LOADMEM"     , dump_from_file        , "<Filename> <Address> <Maxsize> Loads file at specified address");
// register_command ("DUMPMEM"     , dump_to_file          , "<Filename> <Address> <Size> Dump memory to binary file");
// register_command ("PEEK"        , peek_memory           , "<Address> Extracts 32 bit memory value");
// register_command ("POKE"        , poke_memory           , "<Address> <Value> Sets 32 bit memory value");
// register_command ("SPEEK"       , peek_short_memory     , "<Address> Extracts 16 bit memory value");
 //register_command ("SPOKE"       , poke_short_memory     , "<Address> <Value> Sets 16 bit memory value");
// register_command ("BPEEK"       , peek_byte_memory      , "<Address> Extracts 8 bit memory value");
 //register_command ("BPOKE"       , poke_byte_memory      , "<Address> <Value> Sets 8 bit memory value");
 register_command ("GETKEY"      , getakey               , "Get a key");
 register_command ("WAIT"        , wait_time             , "<Milliseconds> Pause operation for a time period");
 register_command ("HISTORY"     , io_history            , "History of last commands");
 register_command ("POLLKEY"     , io_pollkey            , "Poll a key");
 register_command ("SHELL"       , execute_shell_command , "<Full Command> Execute an external command, as in a shell");
 assign_integer   ("ZERO"        , 0x00000000            , TRUE);
 assign_integer   ("BASEADDRESS" , 0x00000000            , FALSE);
 assign_integer   ("RANGE"       , 0x00000100            , FALSE);
 assign_integer   ("DATAVALUE"   , 0x00000000            , FALSE);
 assign_integer   ("ADDRESSVALUE", 0x00000000            , FALSE);
 return(FALSE);
}

/* ========================================================================
   Name:        TesttoolTask
   Description: Task starting for testtool prompt
   ======================================================================== */

static void *TesttoolTask(void *Parameters)
{
	 cli_run ("apptool>", NULL );
	 return 0;
}

/* ========================================================================
   Name:        TesttoolInit
   Description: Initialize the command line interpreter
   ======================================================================== */

int TesttoolInit(unsigned int DeviceId)
{

 /* Testtool redirect to fifo stdin if present */
 /* ------------------------------------------ */
#if 0
 {
  FILE *Handle;
  Handle=fopen("/tmp/STAPISDK_STDIN","r");
  if (Handle!=NULL)
   {
    fclose(Handle);
    stdin=freopen("/tmp/STAPISDK_STDIN","w+",stdin);
   }
 }
#endif
 /* Setup CLI */
 /* --------- */
 io_setup();
 cli_init(cli_setup,3000,16);

 /* Return no errors */
 /* ---------------- */
 return(0);
}

/* ========================================================================
   Name:        TesttoolTerm
   Description: Terminate the command line interpreter
   ======================================================================== */

int TesttoolTerm(unsigned int DeviceId)
{
 /* Delete CLI */
 /* ---------- */
 cli_delete();

 /* Return no errors */
 /* ---------------- */
 return(0);
}

/* ========================================================================
   Name:        TesttoolRun
   Description: Function which creates the testtool task
   ======================================================================== */

int TesttoolRun(int Synchronous)
{
	int ret;
 /* Try to start testtool task */
 /* -------------------------- */
 if (Synchronous==FALSE)
  {
	 ret =  pthread_create(&Testtool_Task,NULL,(void *)TesttoolTask, NULL);
   
   if (ret==0)
    {
     return(1);
    }
  }

 /* Otherwise, run it in the same context */
 /* ------------------------------------- */
 else
  {
   TesttoolTask(NULL);
  }

 /* Return no errors */
 /* ---------------- */
 return(0);
}
#if 0

int main(int argc, char ** args) {

	TesttoolInit(0);
	TesttoolRun(TRUE);
	return 0;
}

#endif
