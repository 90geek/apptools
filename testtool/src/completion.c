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
 * @file     completion.c
 * @brief    This is the completion toolkit available in testtool.
 * @author   STMicroelectronics
 */

/* Includes */
/* -------- */
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "completion.h"


/*
 * Complares 2 characters, ignoring the cass.
 *
 */
int charcmp_nocass(char a,char b)
{
 if (isalpha((int)a) && isalpha((int)b))
  {
   if (isupper((int)a)) a = tolower((int)a);
   if (isupper((int)b)) b = tolower((int)b);
  }
 return a == b ? TRUE : FALSE;
}

/*
 * Complares 2 strings, ignoring the cass.
 *
 */
int strcmp_nocass(const char *a, const char *b)
{
 while(*a != '\0' && *b != '\0' && charcmp_nocass(*a, *b) == TRUE)
  {
   a++;
   b++;
  }
 return(*a == '\0' && *b == '\0') ? TRUE : FALSE;
}

/*
 * Complares the n first chars of 2 strings, ignoring the cass.
 *
 */
int strncmp_nocass(const char *a, const char *b, int n)
{
 while(*a != '\0' && *b != '\0' && charcmp_nocass(*a, *b) == TRUE && n != 0)
  {
   a++;
   b++;
   n--;
  }
 return (n == 0) ? TRUE : FALSE;
}

/*
 * Return a newly allocated buffer containing the first n chars of str
 *
 */
char *strndupl(const char *str, int n)
{
 char   *res;
 int  len;
 len=strlen(str);
 if (n<len) len=n;
 if ((res=malloc(sizeof(char)*(len+1)))==NULL)
  {
   printf("[%s] Unable to allocate memory!\n", __FUNCTION__);
   return(NULL);
  }
 strncpy(res, str, len);
 res[len] = 0;
 return(res);
}

/*
 * Get the common root between str1 and str2
 * and put a '\0' in str1 at the end of this root
 */
int atomic_get_common_root(char *str1,char *str2)
{
 int j;
 for (j=0;str1[j]!='\0' && str2[j]!='\0';j++)
  {
   if (charcmp_nocass(str1[j],str2[j])==FALSE)
    {
     str1[j]='\0';
     break;
    }
  }
 if (str1[j]!='\0' && str2[j]!='\0') return(j);
 return(0);
}

/*
 * Allocate and initialize a match_list object
 *
 */
t_match_list *new_match_list(e_match_list_type match_list_type, const char *title)
{
 t_match_list  *res;

 if ((res = malloc(sizeof (t_match_list))) == NULL)
  {
   printf("[%s] Cannot create new completion object: no memory\n", __FUNCTION__);
   return NULL;
  }
 res->match_list_type = match_list_type;
 res->title           = strdup(title);
 res->str_list        = NULL;
 res->common_root     = NULL;
 res->common_root_len = 0;
 res->common_chars    = 0;
 res->match_number    = 0;
 return(res);
}


/*
 * De-allocate a match_list object
 *
 */
void free_match_list(t_match_list *match_list)
{
 int i;

 free(match_list->title);
 if (match_list->common_root != NULL)
  {
   free(match_list->common_root);
  }
 /* Do not free elts of str_list because they are just references     */
 /* to symtab when match_list is an COMPL_COMMAND or COMPL_MACRO type */
 if (match_list->match_list_type == COMPL_FILE)
  {
   for (i = 0; i < match_list->match_number; i++)
    {
     free(match_list->str_list[i]);
    }
  }
 if (match_list->str_list)
  {
   free(match_list->str_list);
  }
 free(match_list);
}


/*
 * Get the common root of all the strings in str_list of match_list
 *
 */
int match_list_get_common_root(t_match_list *match_list)
{
 int   i;
 char  *res;
 if (match_list->match_number == 0)
  {
   match_list->common_root = strdup("");
   match_list->common_root_len = 0;
   return(FALSE);
  }
 res=strdup(match_list->str_list[0]);
 for (i=1;i<match_list->match_number;i++)
  {
   if (!atomic_get_common_root(res, match_list->str_list[i]))
   break;
  }
 match_list->common_root = res;
 match_list->common_root_len = strlen(res);
 return(TRUE);
}

/*
 * Return a newly allocated match_list object, containing all the valid completions
 * for a given command pattern
 */
t_match_list *look_for_cmd_matches(symtab_t **symtab, int symbol_cnt, char *pattern)
{
 t_match_list *res;
 int           i,ncmp,j=0;

 if ((res = new_match_list(COMPL_COMMAND, "commands:")) == NULL) return(NULL);
 ncmp = strlen(pattern);
 /* As we use a table, we are forced to process 2 passes, the first to count, the second to collect */
 for (i=0;i<symbol_cnt;i++)
  {
   if (symtab[i]->type == COM_SYMBOL && TRUE == strncmp_nocass(pattern, symtab[i]->name_p, ncmp))
   res->match_number++;
  }
 if (res->match_number == 0) return(res);
 if ((res->str_list = malloc(sizeof(char *) * res->match_number)) == NULL)
  {
   printf("[%s] Cannot allocate memory !\n", __FUNCTION__);
   return(res);
  }
 for (i=0;i<symbol_cnt;i++)
  {
   if (symtab[i]->type == COM_SYMBOL && TRUE == strncmp_nocass(pattern, symtab[i]->name_p, ncmp)) res->str_list[j++]=symtab[i]->name_p;
  }
 res->common_chars=ncmp;
 match_list_get_common_root(res);
 return(res);
}

/*
 * Return a newly allocated match_list object, containing all the valid completions
 * for a given macro pattern
 */
t_match_list *look_for_macro_matches(symtab_t **symtab, int symbol_cnt, char *pattern)
{
 t_match_list *res;
 int           i,ncmp,j=0;

 if ((res = new_match_list(COMPL_MACRO, "macros:")) == NULL) return NULL;
 ncmp = strlen(pattern);
 /* As we use a table, we are forced to process 2 passes, the first to count, the second to collect */
 for (i=0;i<symbol_cnt;i++)
  {
   if (symtab[i]->type == MAC_SYMBOL && TRUE == strncmp_nocass(pattern, symtab[i]->name_p, ncmp))
   res->match_number++;
  }
 if (res->match_number == 0) return res;
 if ((res->str_list = malloc(sizeof(char *) * res->match_number)) == NULL)
  {
   printf("[%s] Cannot allocate memory !\n", __FUNCTION__);
   return res;
  }
 for (i=0;i<symbol_cnt;i++)
  {
   if (symtab[i]->type == MAC_SYMBOL && TRUE == strncmp_nocass(pattern, symtab[i]->name_p, ncmp)) res->str_list[j++] = symtab[i]->name_p;
  }
 res->common_chars=ncmp;
 match_list_get_common_root(res);
 return(res);
}

/*
 * From any string looking like a path, get the parent directory path
 * Ex: /m -> /
 * /mnt/pouet -> /mnt/
 * /mnt/p -> /mnt/
*/
static char *path_to_dir(const char *path)
{
 char  *dir;
 int    plen,i;
 plen = strlen(path);
 if (path[plen-1]=='/') dir=strdup(path);
 else
  {
   for (i=plen-1;i>=0;i--)
    {
     if (path[i]=='/') break;
    }
   dir=strndupl(path, i+1);
 }
 return(dir);
}

/*
 * From any string looking like a path, get the pattern of the file being searched
 * Ex: /m -> m
 * /mnt/pouet -> pouet
 * /mnt/p -> p
 * /mnt/ -> NULL
 */
static char *path_to_file(const char *path)
{
 char  *f;
 int   plen, i;
 plen = strlen(path);
 if (path[plen - 1] == '/') f = NULL;
 else
  {
   for (i = plen - 1; i >= 0; i--)
   if (path[i] == '/') break;
   f = strndupl(path + i + 1, plen - i - 1);
  }
 return(f);
}

/*
 * Return a newly allocated match_list object, containing all the valid completions
 * for a given file pattern
 * [1] with "/ata" : OSPLUS -> list mount points begining by "ata"
 *       LINUX  -> list files in / begining by "ata"
 * [2] With "/ata0/o : OSPLUS -> list files in /ata0/ begining by "o"
 *         LINUX  -> idem
 * [3] With "ata" : OSPLUS -> do nothing, no possibility
 *      LINUX  -> list files in PWD begining by ata
 */
t_match_list *look_for_files_matches(const char *pattern)
{
  t_match_list  *res;
  char          *dir, *file, *tmp;
  struct  dirent  *dent;
  DIR          *d;
  int            i;
  int            ncmp;

 if ((res = new_match_list(COMPL_FILE, "files:")) == NULL) return(NULL);

 /* Under OS2x, we need a mount point after the initial '/' to be able to call ReadDir */
 if (pattern[0] == '/')
  {
   dir = path_to_dir(pattern);
   file = path_to_file(pattern);
  }
 else
  {
   if ((tmp = getenv("PWD")) == NULL) dir = strdup("./");
   else
    {
     dir = malloc((strlen(tmp) + 2) * sizeof (char));
     strcpy(dir, tmp);
     strcat(dir, "/");
    }
   file = (char *)pattern;
  }
 ncmp = file == NULL ? 0 : strlen(file);
 if ((d = opendir(dir)) == NULL)
  {
   free(dir);
   if (file && file != pattern) free(file);
   return(res);
  }
 /* First pass to count */
 while ((dent = readdir(d)) != 0)
  {
   if ((file == NULL || strncmp(file, dent->d_name, ncmp) == 0) && strcmp(dent->d_name, ".") != 0 && strcmp(dent->d_name, "..") != 0) res->match_number++;
  }
 closedir(d);
 if (res->match_number==0)
  {
   free(dir);
   if (file && file != pattern) free(file);
   return(res);
  }
 if ((res->str_list = malloc(sizeof(char *) * res->match_number)) == NULL)
  {
   free(dir);
   if (file && file != pattern) free(file);
   return(res);
  }
 /* Second pass to collect */
 i=0;
 d = opendir(dir);
  while ((dent = readdir(d)) != 0)
  {
   if ((file == NULL || strncmp(file, dent->d_name, ncmp) == 0) && strcmp(dent->d_name, ".") != 0 && strcmp(dent->d_name, "..") != 0)
    {
     struct stat info;
     tmp = malloc((strlen(dir) + strlen(dent->d_name) + 1) * sizeof (char));
     strcpy(tmp,dir);
     strcat(tmp,dent->d_name);
     if (stat(tmp, &info) == 0)
      {
       char *tmp2;
       /* If we have a directory, append a '/' at its end */
       if (S_ISDIR(info.st_mode))
        {
         tmp2 = malloc((strlen(dent->d_name) + 2) * sizeof (char));
         strcpy(tmp2, dent->d_name);
         strcat(tmp2, "/");
        }
       else
        {
         tmp2 = strdup(dent->d_name);
        }
       res->str_list[i++] = tmp2;
      }
     else
      {
       res->match_number--;
      }
     free(tmp);
    }
  }
 /* End : clean, free and finalize res */
  closedir(d);
 free(dir);
 if (file && file != pattern) free(file);
 res->common_chars = ncmp;
 match_list_get_common_root(res);
 return(res);
}

/*
 * Insert a string (buf_to_add) of n_chars characters, at the position cnt_pos
 * in buffer. Update cnt and cnt_pos.
 */
void line_insert_buf(char *buffer, int buflen, int *cnt, int *cnt_pos,int overwrite_mode, char *buf_to_add, int n_chars)
{
 int i;
 unsigned char  cc;
 if (n_chars == 0) return;
 if (*cnt < (buflen-n_chars))
  {
   if (*cnt_pos == *cnt)
    {
     /* add characters at end of line */
     for (i = 0; i < n_chars; i++)
      {
       buffer[*cnt_pos] = buf_to_add[i];
       (*cnt_pos)++;
       *cnt = *cnt_pos;
      }
     io_putchars(buf_to_add, n_chars);
    }
   else
    {
     /* we are in the middle of the chain */
     /* first case is we are in overwrite mode, just change the character */
     if (overwrite_mode == TRUE)
      {
       for (i=0;i<n_chars;i++)
        {
         buffer[*cnt_pos] = buf_to_add[i];
         (*cnt_pos)++;
        }
       io_putchars(buf_to_add, n_chars);
      }
     else
      {
       /* we have characters forward, we shift the buffer */
       for (i=*cnt;i>(*cnt_pos-1);i--) buffer[i+n_chars] = buffer[i];
       /* we copy the buffer into the line buffer */
       for (i=0;i<n_chars;i++) buffer[*cnt_pos+i] = buf_to_add[i];
       /* now write the end of buffer and go back to cursor */
       io_putchars((char *) (buffer + *cnt_pos), *cnt - *cnt_pos + n_chars);
       cc = '\b';
       for (i=*cnt_pos;i<*cnt;i++) io_putchars((char *)&cc, 1);
       (*cnt_pos) += n_chars;
       (*cnt)     += n_chars;
      }/* end of we are not in overwrite mode */
    }
  }
}

/*
 * Erase n_chars characters, at the position cnt_pos
 * in buffer. Update cnt and cnt_pos.
 */
void line_erase_n_chars(char *buffer, int buflen, int *cnt, int *cnt_pos, int n_chars)
{
 int i;
 unsigned char  cc[3];
 if (n_chars==0) return;
 if (*cnt>0 && *cnt_pos - n_chars >= 0)
  {
   if (*cnt_pos<*cnt)
    {
     for (i=*cnt_pos;i<*cnt;i++)     buffer[i-n_chars] = buffer[i];
     for (i=*cnt-n_chars;i<*cnt;i++) buffer[i]         = ' ';
     cc[0]='\b';
     for (i=0;i<n_chars;i++) io_putchars((char *)cc,1);
     io_putchars((char *)(buffer+*cnt_pos-n_chars),*cnt-*cnt_pos+n_chars);
     *cnt_pos-=n_chars;
     for (i=*cnt_pos;i<*cnt;i++) io_putchars((char *)cc,1);
     *cnt-=n_chars;
    }
   else
    {
     cc[0] = '\b';
     cc[1] = ' ';
     cc[2] = '\b';
     for (i=0;i<n_chars;i++) io_putchars((char *)cc,3);
     *cnt     -= n_chars;
     *cnt_pos -= n_chars;
    }
  }
}

/*
 * Get the token (whole word) just before the cursor
 */
char *get_current_token(t_completion *compl, char *buf, int buflen, int cnt, int *cnt_pos)
{
 int i=*cnt_pos;
 /*If we are in the middle of a token, move to the end */
 /* TODO : If we are at the end of a token, and we have space under the cursor, move to the begining */
 if (buf[i] == SPACE_CHAR)
  {
   while (buf[i] == SPACE_CHAR && i > 0) i--;
  }
 if (compl->dquote_number % 2 != 0)
   while (i > 0 && buf[i-1] != '"') i--;
 else
   while (i > 0 && buf[i-1] != SPACE_CHAR && buf[i-1] != TAB_CHAR && buf[i-1] != '"') i--;
 return(strndupl(buf+i,*cnt_pos-i));
}

/*
 * Call this to init completion (first time TAB is pushed)
 *
 */
void init_completion(t_completion *compl)
{
 /* By default we first try commands */
 compl->current_compl_type  = COMPL_COMMAND;
 compl->current_compl_idx   = 0;
 compl->previous_compl_type = COMPL_TYPE_NUMBER;
 compl->previous_compl_idx  = -1;
 compl->pattern             = NULL;
 compl->cmd_matches         = NULL;
 compl->macro_matches       = NULL;
 compl->common_root         = NULL;
 compl->common_root_len     = 0;
 compl->files_matches       = NULL;
 compl->in_completion       = TRUE;
}

/*
 * Call this function when another action is called
 *
 */
void end_completion(t_completion *compl)
{
 if (compl->pattern       != NULL) free(compl->pattern);
 if (compl->cmd_matches   != NULL) free_match_list(compl->cmd_matches);
 if (compl->macro_matches != NULL) free_match_list(compl->macro_matches);
 if (compl->common_root   != NULL) free(compl->common_root);
 if (compl->files_matches != NULL) free_match_list(compl->files_matches);
 compl->pattern         = NULL;
 compl->cmd_matches     = NULL;
 compl->macro_matches   = NULL;
 compl->common_root     = NULL;
 compl->common_root_len = 0;
 compl->files_matches   = NULL;
 compl->in_completion   = FALSE;
}

/*
 * Return the right match list pointer according to the current completion type
 * if old is TRUE, return the match list object of the previous type state.
 */
__inline t_match_list *get_match_list(t_completion *compl,int old)
{
 e_completion_type ct;
 ct = old == TRUE ? compl->previous_compl_type : compl->current_compl_type;
 switch(ct)
  {
   case COMPL_COMMAND : return(compl->cmd_matches);
   case COMPL_MACRO   : return(compl->macro_matches);
   case COMPL_FILE    : return(compl->files_matches);
   default            : return(NULL);
  }
}

/*
 * Increment completion type, and set it to the next non-empty completion category
 * Return true if the type has changed
*/
__inline int increment_compl_type(t_completion *compl)
{
 t_match_list  *curr_ml,*ml=get_match_list(compl, FALSE);
 /* Increment the completion index */
 compl->previous_compl_idx  = compl->current_compl_idx;
 compl->previous_compl_type = compl->current_compl_type;
 if (compl->current_compl_type!=COMPL_TYPE_NUMBER) compl->current_compl_idx+=1;
 /* We are not yet at the end of the possible completions for this type */
 if (compl->current_compl_type!=COMPL_TYPE_NUMBER && compl->current_compl_idx<ml->match_number) return(FALSE);
 /* We have to move to the next type */
 if (compl->current_compl_type==COMPL_TYPE_NUMBER) compl->previous_compl_idx=-1;
 compl->current_compl_idx=0;
 do
  {
   compl->current_compl_type=(compl->current_compl_type+1)%COMPL_TYPE_NUMBER;
   curr_ml = get_match_list(compl,FALSE);
  } while (curr_ml->match_number==0);
 return(TRUE);
}

/*
 * Complete the word with the next available proposition
 *
 */
__inline void completion_do_complete(t_completion *compl,char *buffer, int buflen, int *cnt, int *cnt_pos)
{
 t_match_list *ml=get_match_list(compl,FALSE);
 line_insert_buf(buffer,buflen,cnt,cnt_pos,FALSE,ml->str_list[compl->current_compl_idx],strlen(ml->str_list[compl->current_compl_idx]));
}

/*
 * Get the common root between common roots of all types
 *
 */
void completion_get_common_root(t_completion *compl)
{
 if (compl->cmd_matches->match_number>0)
  {
   compl->common_root = strdup(compl->cmd_matches->common_root);
   if (compl->macro_matches->match_number>0) atomic_get_common_root(compl->common_root, compl->macro_matches->common_root);
   if (compl->files_matches->match_number>0) atomic_get_common_root(compl->common_root, compl->files_matches->common_root);
  }
 else
  {
   if (compl->macro_matches->match_number>0)
    {
     compl->common_root = strdup(compl->macro_matches->common_root);
     if (compl->files_matches->match_number>0) atomic_get_common_root(compl->common_root, compl->files_matches->common_root);
    }
   else if (compl->files_matches->match_number>0) compl->common_root = strdup(compl->files_matches->common_root);
   else compl->common_root = strdup("");
  }
 compl->common_root_len = strlen(compl->common_root);
}

/*
 * At the first tab, we init the completion
 * If the total of matches is greater than 1, then complete to the common root
 * If the total of matches is  == 1, then complete to this matches and end completion
 *
 */
void completion_do_first_tab(t_completion *compl, char *buffer, int buflen, int *cnt, int *cnt_pos)
{
 int total_matches;

 init_completion(compl);
 if ((compl->pattern = get_current_token(compl, buffer, buflen, *cnt, cnt_pos)) == NULL)
  {
   end_completion(compl);
   return ;
  }
 compl->cmd_matches   = look_for_cmd_matches(symtab_p, symbol_cnt, compl->pattern);
 compl->macro_matches = look_for_macro_matches(symtab_p, symbol_cnt, compl->pattern);
 compl->files_matches = look_for_files_matches(compl->pattern);

 total_matches        = compl->cmd_matches->match_number + compl->macro_matches->match_number;
 total_matches       += compl->files_matches->match_number;
 /* No matches : we end completion and quit */
 if (total_matches == 0)
  {
   end_completion(compl);
   return;
  }
 if (compl->macro_matches->match_number != 0) compl->current_compl_type = COMPL_MACRO;
 if (compl->files_matches->match_number != 0) compl->current_compl_type = COMPL_FILE;
 /* Only one match : we complete the line and quit */
 if (total_matches == 1)
  {
   if (compl->current_compl_type == COMPL_FILE) line_erase_n_chars(buffer, buflen, cnt, cnt_pos, compl->files_matches->common_chars);
   else line_erase_n_chars(buffer, buflen, cnt, cnt_pos, strlen(compl->pattern));
   completion_do_complete(compl, buffer, buflen, cnt, cnt_pos);
   if (compl->current_compl_type == COMPL_FILE && compl->files_matches->str_list[compl->current_compl_idx][strlen(compl->files_matches->str_list[compl->current_compl_idx]) - 1] != '/')
    {
     line_insert_buf(buffer, buflen, cnt, cnt_pos, FALSE, "\" ", 2);
    }
   else if (compl->current_compl_type != COMPL_FILE || compl->files_matches->str_list[compl->current_compl_idx][strlen(compl->files_matches->str_list[compl->current_compl_idx]) - 1] != '/')
    {
     line_insert_buf(buffer, buflen, cnt, cnt_pos, FALSE, " ", 1);
    }
   end_completion(compl);
   return;
  }
 /* Several matches : we get the common root, and print it */
 completion_get_common_root(compl);
 if (compl->current_compl_type == COMPL_FILE) line_erase_n_chars(buffer, buflen, cnt, cnt_pos, compl->files_matches->common_chars);
 else line_erase_n_chars(buffer, buflen, cnt, cnt_pos, strlen(compl->pattern));
 line_insert_buf(buffer, buflen, cnt, cnt_pos, FALSE, compl->common_root, compl->common_root_len);
 if (compl->cmd_matches->match_number == 0)
  {
   if (compl->macro_matches->match_number > 0) compl->current_compl_type = COMPL_MACRO;
   else compl->current_compl_type = COMPL_FILE;
  }
 compl->current_compl_idx = 0;
 compl->previous_compl_type = COMPL_TYPE_NUMBER;
 compl->previous_compl_idx = -1;
}

/*
 * Do Nth tab: Completion mode is already initialized
 * - Erase the current proposition
 * - Increment the proposition counter
 * - if proposition counter has reached the proposition number of the category,
 *      go to the next category
 * - if we have reached the last category, print the common root again, we'll loop from
 *   the begining at the next tab
 * - Erase the last proposition
 * - Print the new one
 */
void completion_do_nth_tab(t_completion *compl, char *buffer, int buflen, int *cnt, int *cnt_pos)
{
 t_match_list  *prev_ml;
 t_match_list  *curr_ml;
 int           change_type = FALSE;

 prev_ml = get_match_list(compl, TRUE);
 /* 2nd tab only, or relooping after a full completion cycle */
 if (compl->previous_compl_type == COMPL_TYPE_NUMBER) line_erase_n_chars(buffer, buflen, cnt, cnt_pos, compl->common_root_len);
 else line_erase_n_chars(buffer, buflen, cnt, cnt_pos, strlen(prev_ml->str_list[compl->previous_compl_idx]));
 /* Ending of a completion cycle : we print the common root */
 if (compl->current_compl_type == COMPL_TYPE_NUMBER)
  {
   line_insert_buf(buffer, buflen, cnt, cnt_pos, FALSE, compl->common_root, compl->common_root_len);
   increment_compl_type(compl);
   return;
  }
 completion_do_complete(compl, buffer, buflen, cnt, cnt_pos);
 change_type = increment_compl_type(compl);
 curr_ml     = get_match_list(compl, FALSE);
 if (change_type == TRUE && curr_ml == prev_ml)
  {
   compl->current_compl_type = COMPL_TYPE_NUMBER;
   compl->current_compl_idx = 0;
   change_type = FALSE;
  }
}


