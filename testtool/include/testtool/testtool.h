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
 * @file     testtool.h
 * @brief    This is the testtool header file.
 * @author   STMicroelectronics
 */

#ifndef _DEVELOP_H_
#define _DEVELOP_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
/* -------- */
#include "clilib.h"
/* Key definition */
/* -------------- */
typedef enum e_key_code
{
 KEY_ESC,
 KEY_BACKSPACE,
 KEY_RETURN,
 KEY_END_OF_LINE,
 KEY_BEGIN_OF_LINE,
 KEY_ARROW_UP,
 KEY_ARROW_DOWN,
 KEY_ARROW_LEFT,
 KEY_ARROW_RIGHT,
 KEY_INSERT,
 KEY_DELETE,
 KEY_TAB,
 KEY_CLEAR_LINE,
 KEY_CLEAR_EOL,
 KEY_CTRL_LEFT,
 KEY_CTRL_RIGHT,
 KEY_NUMBERS
} e_key_code;

/* Action definition */
/* ----------------- */
typedef enum e_action_code
{
 ACTION_BACKSPACE,
 ACTION_RETURN,
 ACTION_MOVE_EOL,
 ACTION_MOVE_BOL,
 ACTION_HISTORY_UP,
 ACTION_HISTORY_DOWN,
 ACTION_MOVE_LEFT,
 ACTION_MOVE_RIGHT,
 ACTION_TOGGLE_INSERT,
 ACTION_DELETE,
 ACTION_COMPLETE,
 ACTION_CLEAR_LINE,
 ACTION_CLEAR_EOL,
 ACTION_ADD_CHAR,
 ACTION_MOVE_BOT,
 ACTION_MOVE_EOT,
 ACTION_NOACTION
} e_action_code;

/* Key bind definition */
/* ------------------- */
typedef struct s_key_bind
{
 e_key_code     key_code;
 char          *key_string;
 e_action_code  action_code;
} t_key_bind;

/* History definition */
/* ------------------ */
#define NLINES 20
typedef struct
{
 unsigned int write_pointer;
 unsigned int read_pointer;
 unsigned char  command_history[MAX_LINE_LEN][NLINES];
} history_t;

/* Prototypes */
/* ---------- */
//void           print                           (const char *format,...);
void           report                          (int Error,const char *format,...);
int            scan                            (const char *format,void *Variable);
char           io_getchar                      (char *filter,int case_sensitive);
int            pollkey                         (void);
void           tag_current_line                (parse_t * pars_p,char *message_p);
int           assign_integer                  (char *token_p,int value,int constant);
int           assign_float                    (char *token_p,double value,int constant);
int           assign_string                   (char *token_p,char *value,int constant);
int           register_command                (char *token_p,int (*action) (parse_t *,char *),char *help_p);
int           cget_string                     (parse_t *pars_p,char *default_p,char *result_p,short max_len);
int           cget_integer                    (parse_t *pars_p,int def_int,int *result_p);
int           cget_float                      (parse_t *pars_p,double def_flt,double *result_p);
int           cget_item                       (parse_t *pars_p,char *default_p,char *result_p, short max_len);
int           cget_token_count                (parse_t *pars_p,short *result_p);
int           evaluate_integer                (char *token_p,int *value_p,short default_base);
int           evaluate_float                  (char *token_p,double *value_p);
int           evaluate_comparison             (char *token_p,int *result_p,short default_base);
int           evaluate_string                 (char *token_p,char *string_p,short max_len);
int 		TesttoolInit                    (unsigned int DeviceId);
int 		TesttoolRun                     (int Synchronous);

int            exec_external_command           (char **args);

int get_testtool_mod_ver(void);
int get_testtool_mod_ext_ver(void);


/* C++ support */
/* ----------- */
#ifdef __cplusplus
}
#endif
#endif

