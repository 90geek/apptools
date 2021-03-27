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
 * @file     completion.h
 * @brief    This is the completion toolkit file header.
 * @author   STMicroelectronics
 */

/* Includes */
/* -------- */
#ifndef __COMPLETION_H
#define __COMPLETION_H
#include "testtool/clilib.h"


/* New types */
/* --------- */
typedef enum e_match_list_type
{
 COMPL_COMMAND,
 COMPL_MACRO,
 COMPL_FILE,
 COMPL_TYPE_NUMBER
} e_match_list_type;
typedef e_match_list_type e_completion_type;

/* Definition of match list structure */
/* ---------------------------------- */
typedef struct s_match_list
{
 e_completion_type   match_list_type;   /* The kind of matches                 */
 char               *title;             /* The title of match category         */
 char              **str_list;          /* The list of matchess                */
 char               *common_root;       /* The common root between all matches */
 int                 common_root_len;   /* Length of the common root           */
 int                 common_chars;      /* Number of head char in common       */
 int                 match_number;      /* The number of matches               */
} t_match_list;

/* Definition of completion structure */
/* ---------------------------------- */
typedef struct s_completion
{
 e_completion_type  previous_compl_type;
 int                previous_compl_idx;
 e_completion_type  current_compl_type;
 int                current_compl_idx;
 char              *pattern;
 t_match_list      *cmd_matches;
 t_match_list      *macro_matches;
 char              *common_root;        /* The common root between all matches */
 int                common_root_len;    /* Length of the common root           */
 t_match_list      *files_matches;
 int               in_completion;
 char               dquote_number;
} t_completion;

/* Function prototypes */
/* ------------------- */
int                   charcmp_nocass            (char a,char b);
int                   strcmp_nocass             (const char *a,const char *b);
int                   strncmp_nocass            (const char *a,const char *b,int n);
int                   match_list_get_common_root(t_match_list *match_list);
char* strndupl(const char *str, int n);
char                  *get_current_token         (t_completion *compl, char  *buf,int buflen,int cnt,int *cnt_pos);
int                    atomic_get_common_root    (char *str1,char *str2);
t_match_list          *new_match_list            (e_match_list_type match_list_type,const char *title);
t_match_list          *look_for_cmd_matches      (symtab_t **symtab,int symbol_cnt,char *pattern);
t_match_list          *look_for_macro_matches    (symtab_t **symtab,int symbol_cnt,char *pattern);
t_match_list          *look_for_files_matches    (const char *pattern);
void                   free_match_list           (t_match_list *compl);
void                   line_insert_buf           (char *buffer,int buflen,int *cnt,int *cnt_pos,int overwrite_mode,char *buf_to_add,int n_chars);
void                   line_erase_n_chars        (char *buffer,int buflen,int *cnt,int *cnt_pos,int n_chars);
void                   init_completion           (t_completion *compl);
void                   end_completion            (t_completion *compl);
void                   completion_do_first_tab   (t_completion *compl, char *buffer,int buflen,int *cnt,int *cnt_pos);
void                   completion_do_nth_tab     (t_completion *compl, char *buffer,int buflen,int *cnt,int *cnt_pos);
__inline t_match_list *get_match_list            (t_completion *compl,int old);
__inline int          increment_compl_type      (t_completion *compl);
__inline void          completion_do_complete    (t_completion *compl,char *buffer,int buflen,int *cnt,int *cnt_pos);
#endif
