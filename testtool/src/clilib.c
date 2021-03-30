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
 * @file     clilib.c
 * @brief    This is the command line interpreter component.
 * @author   STMicroelectronics
 */

/* Includes */
/* -------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <signal.h>
#include "testtool/clilib.h"
#include "testtool/testtool.h"

#include "platform/app_os.h"


/* Global constants */
/* ---------------- */
#define STRING_TABLE_INIT_SIZE   40  /* if not enough, table is extended with memory allocations  */
#define TOKEN_TABLE_INIT_SIZE    170 /* if not enough, table is extended with memory allocations  */
#define PARSE_TABLE_INIT_SIZE    40  /* if not enough, table is extended with memory allocations  */
#define CONTROL_NAME_LENGTH      10  /* e.g : WHILE23                                             */
#define TMP_CHAR_MAX_LINE_LENGTH 16

/* Global defines */
/* -------------- */
int      symbol_cnt;  /* Number of symbols declared */
int      command_cnt; /* Number of cmd declared */
int      macro_cnt;   /* Number of macro declared */
symtab_t **symtab_p;  /* Symbol table pointer       */

/* Definition of slot */
/* ------------------ */
typedef struct
{
 char Slot[1]; /* Just to say it's a table, memory allocation is done with greater size! */
} SlotTable_t;

/* Local defines */
/* ------------- */
static int           max_symbol_cnt;             /* maximum number of symbols        */
static short           macro_depth;                /* macro invocation depth           */
static short           number_base;                /* default inp/output base for ints */
static void         *cur_stream_p;               /* current file ip stream           */
static macro_t      *cur_macro_p;                /* current macro position           */
static char         *prompt_p;                   /* input prompt                     */
static int          cur_echo;                   /* status of input echo             */
static char          format_buf[MAX_LINE_LEN*8]; /* limited output length            */
static void         *logfiled;
//static int          log_output;
static char          CommandLineDelimiter[] = "= \\\t";
static char          delim_set[] = " ,\\\t";
static char          LogicalOps[]           = "><=!";
static char          ComparisonOps[]       = "|&";
static SlotTable_t **StringTable_p;
static short           StringTableIndex;
static unsigned short           MaxSimultaneousStringReached;
static SlotTable_t **TokenTable_p;
static short           TokenTableIndex;
static unsigned short           MaxSimultaneousTokenReached;
static parse_t     **ParseTable_p;
static short           ParseTableIndex;
static unsigned short           MaxSimultaneousParseReached;
static unsigned char            ElifNameCount;         /* used to define 'Elif' macro with unique name       */
static int           match_len=2;
static int           return_result=FALSE;
static int           ignore_error_if_macro=FALSE;
static int          cli_running=FALSE;
static int          ForceDelete=TRUE;

/* Local prototypes */
/* ---------------- */
int command_loop         (macro_t * macro_p,void *file_p,char *rtn_exp_p,int echo);
int uncommand_loop       (macro_t *macro_p ,void *file_p,char *rtn_exp_p,int echo);
int evaluate_integer_expr(parse_t *pars_p,int *value_p,short default_base);
int evaluate_float_expr  (parse_t *pars_p,double *value_p);

/* ========================================================================
   Name:        cli_statistics
   Description: Display cli statistics
   ======================================================================== */

static int cli_statistics (parse_t *parse_p, char *result_sym_p)
{
 int       loop_cpt=0;
 int       mac_cpt=0;
 int       com_cpt=0;
 int       str_cpt=0;
 int       int_cpt=0;
 int       flt_cpt=0;
 symtab_t *sym_pt;

 /* Symbols counting */
 /* ---------------- */
 while(loop_cpt<symbol_cnt)
  {
   sym_pt=symtab_p[loop_cpt];
   if (sym_pt->type==MAC_SYMBOL) mac_cpt++;
   if (sym_pt->type==COM_SYMBOL) com_cpt++;
   if (sym_pt->type==STR_SYMBOL) str_cpt++;
   if (sym_pt->type==INT_SYMBOL) int_cpt++;
   if (sym_pt->type==FLT_SYMBOL) flt_cpt++;
   loop_cpt++;
  }

 /* Print informations */
 /* ------------------ */
 print("Testtool statistics :\n");
 print("---------------------\n");
 print("Declared symbols count       : %d/%d\n",symbol_cnt,max_symbol_cnt);
 print("Number of macros             : %d\n",mac_cpt);
 print("Number of commands           : %d\n",com_cpt);
 print("Number of strings            : %d\n",str_cpt);
 print("Number of integers           : %d\n",int_cpt);
 print("Number of floats             : %d\n",flt_cpt);
 print("MaxSimultaneousTokenReached  : %d\n",MaxSimultaneousTokenReached);
 print("MaxSimultaneousStringReached : %d\n",MaxSimultaneousStringReached);
 print("MaxSimultaneousParseReached  : %d\n",MaxSimultaneousParseReached);
 print("Current TokenTableIndex      : %d\n",TokenTableIndex);
 print("Current StringTableIndex     : %d\n",StringTableIndex);
 print("Current ParseTableIndex      : %d\n",ParseTableIndex);
 return(FALSE);
}

/* ========================================================================
   Name:        alloc_string_table_slot
   Description: Give new memory slot in line table
   ======================================================================== */

static char *alloc_string_table_slot(unsigned short LineSize)
{
 SlotTable_t **Tmp_p = StringTable_p; /* make it not NULL */
 StringTableIndex++;
 if (StringTableIndex<STRING_TABLE_INIT_SIZE)
  {
   /* Index = 0 => 1 slot took, index=1 => 2 slots ... */
   if ((StringTableIndex+1)>MaxSimultaneousStringReached)
    {
     MaxSimultaneousStringReached=StringTableIndex+1;
    }
  }
 else
  {
   if ((StringTableIndex+1)>MaxSimultaneousStringReached)
    {
     MaxSimultaneousStringReached = StringTableIndex+10;
     /* Re-allocate with greater size print table and copy content to new @ */
     Tmp_p=(SlotTable_t **)malloc(sizeof(SlotTable_t *)*(MaxSimultaneousStringReached));
     if (Tmp_p!=NULL)
      {
       /* Copy content from StringTable_p to Tmp_p : keep (StringTableIndex+1)-1 old values, -1 because as been incremented yet */
       memcpy(Tmp_p,StringTable_p,sizeof(SlotTable_t *)*(StringTableIndex));
       free(StringTable_p);
       StringTable_p = Tmp_p;
      }
     else
      {
       /* Allocation failed : use last slot, crunch it ...*/
       print("%s():**ERROR** !!! Not enough memory,last slot crunched !!!\n",__FUNCTION__);
       StringTableIndex--;
      }
    }
   if (Tmp_p!=NULL)
    {
     StringTable_p[StringTableIndex]=(SlotTable_t *)malloc(LineSize);
    }
  }
 return(StringTable_p[StringTableIndex]->Slot);
}

/* ========================================================================
   Name:        free_string_table_slot
   Description: Free memory slot for print
   ======================================================================== */

static void free_string_table_slot(void)
{
 if (StringTableIndex>=STRING_TABLE_INIT_SIZE)
  {
   free(StringTable_p[StringTableIndex]);
  }
 StringTableIndex--;
}


/* ========================================================================
   Name:        alloc_token_table_slot
   Description: Give new memory slot in token table
   ======================================================================== */

static char *alloc_token_table_slot(unsigned short TokenSize)
{
 SlotTable_t **Tmp_p=TokenTable_p; /* make it not NULL */
 TokenTableIndex++;
 if (TokenTableIndex<TOKEN_TABLE_INIT_SIZE)
  {
   /* Index = 0 => 1 slot took, index=1 => 2 slots ... */
   if ((TokenTableIndex+1)>MaxSimultaneousTokenReached)
    {
     MaxSimultaneousTokenReached=TokenTableIndex+1;
    }
  }
 else
  {
   if ((TokenTableIndex+1)>MaxSimultaneousTokenReached)
    {
     MaxSimultaneousTokenReached=TokenTableIndex+10;
     /* Re-allocate with greater size print table and copy content to new @ */
     Tmp_p=(SlotTable_t **)malloc(sizeof(SlotTable_t *)*(MaxSimultaneousTokenReached));
     if (Tmp_p!=NULL)
      {
       /* Copy content from TokenTable_p to Tmp_p : keep (TokenTableIndex+1)-1 old values, -1 because as been incremented yet */
       memcpy(Tmp_p,TokenTable_p,sizeof(SlotTable_t *)*(TokenTableIndex));
       free(TokenTable_p);
       TokenTable_p=Tmp_p;
      }
     else
      {
       /* Allocation failed : use last slot, crunch it ...*/
       print("%s():**ERROR** !!! Not enough memory,last slot crunched !!!\n",__FUNCTION__);
       TokenTableIndex--;
      }
    }
   if (Tmp_p!=NULL)
    {
     TokenTable_p[TokenTableIndex]=(SlotTable_t *)malloc(TokenSize);
    }
  }
 return(TokenTable_p[TokenTableIndex]->Slot);
}

/* ========================================================================
   Name:        free_token_table_slot
   Description: Free memory slot for print
   ======================================================================== */

static void free_token_table_slot(void)
{
 if (TokenTableIndex>=TOKEN_TABLE_INIT_SIZE)
  {
   free(TokenTable_p[TokenTableIndex]);
  }
 TokenTableIndex--;
}

/* ========================================================================
   Name:        alloc_parse_table_slot
   Description: Give new memory slot in token table
   ======================================================================== */

static parse_t * alloc_parse_table_slot(unsigned short TokenSize)
{
 parse_t **Tmp_p = ParseTable_p; /* make it not NULL */
 ParseTableIndex++;
 if (ParseTableIndex<PARSE_TABLE_INIT_SIZE)
  {
   /* Index = 0 => 1 slot took, index=1 => 2 slots ... */
   if ((ParseTableIndex+1)>MaxSimultaneousParseReached)
    {
     MaxSimultaneousParseReached=ParseTableIndex+1;
    }
  }
 else
  {
   if ((ParseTableIndex+1)>MaxSimultaneousParseReached)
    {
     MaxSimultaneousParseReached=ParseTableIndex+10;
     /* Re-allocate with greater size print table and copy content to new @ */
     Tmp_p=(parse_t **)malloc(sizeof(parse_t *)*(MaxSimultaneousParseReached));
     if (Tmp_p!=NULL)
      {
       /* Copy content from ParseTable_p to Tmp_p : keep (ParseTableIndex+1)-1 old values, -1 because as been incremented yet */
       memcpy(Tmp_p,ParseTable_p,sizeof(parse_t *)*(ParseTableIndex));
       free(ParseTable_p);
       ParseTable_p=Tmp_p;
      }
     else
      {
       /* Allocation failed : use last slot, crunch it ...*/
       print("%s():**ERROR** !!! Not enough memory,last slot crunched !!!\n",__FUNCTION__);
       ParseTableIndex--;
      }
    }
   if (Tmp_p!=NULL)
    {
     unsigned short AllocSize;
     AllocSize=sizeof(parse_t)+TokenSize;
     ParseTable_p[ParseTableIndex]=(parse_t *)malloc(AllocSize);
    }
  }
 return(ParseTable_p[ParseTableIndex]);
}

/* ========================================================================
   Name:        free_parse_table_slot
   Description: Free memory slot for print
   ======================================================================== */

static void free_parse_table_slot(void)
{
 if (ParseTableIndex>=PARSE_TABLE_INIT_SIZE)
  {
   free(ParseTable_p[ParseTableIndex]);
  }
 ParseTableIndex--;
}

/* ========================================================================
   Name:        print
   Description: Print method using variable arg format to invoke formatting
   ======================================================================== */

void print(const char *format,...)
{
 va_list list;
 va_start(list,format);
 vsnprintf(format_buf,sizeof(format_buf),format,list);
 io_write(format_buf);
 va_end(list);
}

/* ========================================================================
   Name:        scan
   Description: Indirect 'scanf' function from uart or console
   ======================================================================== */

int scan(const char *format,void *Variable)
{
 int  i;
 char ip_char[MAX_LINE_LEN+1];
 for (i=0;i<(MAX_LINE_LEN+1);i++) ip_char[i]=0;
 read_input(ip_char,"");
 sscanf(ip_char,format,Variable);
 return(1);
}

/* ========================================================================
   tests the character for equality with a set of delimiter characters
   ======================================================================== */
int is_delim (char character, char *delim_p)
{
 short delim_count = 0;

 while ((delim_p[delim_count] != '\0') &&
        (character != delim_p[delim_count]))
   delim_count++;
 return ((character == delim_p[delim_count]) ||
        (character == NL_CHAR) ||
        (character == CR_CHAR) ||
        (character == '\0'));
}
/* ========================================================================
   tests strings for equality, but not in an exact way. Trailing space is OK
   Comparison will succeed if the tested string matches the definition string
   but is shorter (i.e is an abbreviation). It will not match if the tested
   string is longer or less than minlen chars are present in the tested
   string. Comparison is also case insensitive.
   ======================================================================== */
int is_matched (char *tested_p, char *definition_p, short minlen)
{
 int match = TRUE;
 short  cnt = 0;

 while (((tested_p[cnt] == definition_p[cnt]) ||
            ((tested_p[cnt] & 0xdf) == (definition_p[cnt] & 0xdf))) &&
           (tested_p[cnt] != '\0') &&
           (definition_p[cnt] != '\0'))
  cnt++;

 /* if we found the end of the tested string before we found a mis-match
 then strings are matched. If the definition string is shorter than
 minumum length requirements, then match can succeed. */
 if ((tested_p[cnt] != '\0') ||
    ((cnt < minlen) && (definition_p[cnt] != '\0')))
    match = FALSE;

 /* Exact Match was requested*/
 if(minlen==-1)
  {
   if(cnt<strlen(definition_p))
    {
     match=FALSE;
    }
  }
 return (match);
}

/* ========================================================================
   tests a token against a set of control redirection primitives
   and returns an identifier for the construct found
   ======================================================================== */
int is_control (char *token_p, short * construct_p)
{
 int found = TRUE;

 if (is_matched (token_p, "DEFINE", 2))
  *construct_p = DEFINE_CONST;
 else if (is_matched (token_p, "IF", 2))
  *construct_p = IF_CONST;
 else if (is_matched(token_p, "ELIF", 2))
  *construct_p = ELIF_CONST;
 else if (is_matched (token_p, "ELSE", 2))
  *construct_p = ELSE_CONST;
 else if (is_matched (token_p, "WHILE", 2))
  *construct_p = WHILE_CONST;
 else if (is_matched (token_p, "FOR", 2))
  *construct_p = FOR_CONST;
 else
  found = FALSE;

 return (found);
}

/* ========================================================================
   returns, if possible, an integer value. The default base
   is used for conversion in the absence of other information.
   If a '#' character preceeds the number hex base is assumed
   If a '$' is used as a prefix, then a binary representation is assumed
   If an 'o' or 'O' character is used then octal is assumed.
   Any sign makes the number a decimal representation
   ======================================================================== */

int conv_int(char *token_p,short default_base)
{
 int             value;
 short             base, cnt;
 int            negative;
 static char    *conv = "0123456789ABCDEF";

 negative = FALSE;
 switch (token_p[0])
  {
   case '#' : /* keep old syntax for backward compatibility */
   case 'h' :
   case 'H' :
    base = 16;
    token_p++;
   break;

   case 'b' :
   case 'B' :
    if(strlen(token_p) > 2) /*Fix for GNBvd82554*/
     {
      base = 2;
      token_p++;
     }
    else
     base = default_base;
   break;

   case '$' : /* keep old syntax for backward compatibility */
    base = 2;
    token_p++;
   break;

   case 'o' :
   case 'O' :
    base = 8;
    token_p++;
   break;

   case '-' :
    negative = TRUE;
    /* no break */
   case '+' :
    base = 10;
    token_p++;
   break;

   default :
    base = default_base;
   break;
  }

 /* convert by comparison to ordered string array of numeric characters */
 value = 0;
 cnt = 0;
 while ((token_p[cnt] != '\0') && ( value != BAD_INT_VAL) )
  {
   short             i = 0;
   while ((conv[i] != (char) toupper ((int)token_p[cnt])) && (i < base))
    i++;
   /*        if ((i >= base) || (value >= (int) 0x7fffffff) ||
            ((value >= 214748364) && (i >= 8))) */
   if ( i >= base )
    value = BAD_INT_VAL;
   else
    value = (value * base) + i;
   cnt++;
  }

 if (( negative ) && ( value != BAD_INT_VAL) )
  value = -value;

 return (value);
}

/* ========================================================================
   returns, if possible, an floating point value. Scanf seems not to
   detect any errors in its input format, and so we have to do a check
   on number validity prior to calling it. This is rather complicated
   to try and reject float expressions, but validate pure float numbers
   ======================================================================== */

int conv_flt(char *token_p,double *value_p)
{
 short             cnt = 0;
 int            error = FALSE;
 int            seen_dp = FALSE;

 while ((token_p[cnt] != '\0') && !error)
  {
   /* check for silly characters */
   if ((token_p[cnt] != '-') &&
       (token_p[cnt] != '+') &&
       (token_p[cnt] != '.') &&
       (token_p[cnt] != 'E') &&
       (token_p[cnt] != 'e') &&
       ((token_p[cnt] > '9') || (token_p[cnt] < '0')))
    error = TRUE;
   /* check for more than one decimal point */
   if (token_p[cnt] == '.')
    {
     if (seen_dp)
      error = TRUE;
     else
      seen_dp = TRUE;
    }

   /* check for sign after decimal point not associated with exponent */
   if (((token_p[cnt] == '+') || (token_p[cnt] == '-')) && seen_dp)
    {
     if ((token_p[cnt - 1] != 'E') && (token_p[cnt - 1] != 'e'))
      error = TRUE;
    }
   /* check for sign before a decimal point but not at start of token */
   if (((token_p[cnt] == '+') || (token_p[cnt] == '-')) && !seen_dp)
    {
     if (cnt > 0)
      error = TRUE;
    }
   cnt++;
  }

 if (error || (sscanf (token_p, "%lg", value_p) == 0))
  *value_p = BAD_INT_VAL;

 return (error);
}

/* ========================================================================
   looks for a symbol type/name within the table. We look from
   most recent to oldest symbol to accomodate scope.
   We also scan for the shortest defined symbol that matches
   the input token - this resolves order dependent declaration problems
   ======================================================================== */

symtab_t *look_for(char *token_p,short type)
{
 short             cnt;
 int            found = FALSE;
 symtab_t       *symbol_p;
 symtab_t       *shortest_p;
 short             short_len;

 short_len = MAX_TOK_LEN;
 shortest_p = (symtab_t *) NULL;

 /* point to last symbol in the table */
 if (symbol_cnt != 0)
  {
   cnt = 1;
   while (cnt <= symbol_cnt)
    {
     symbol_p = symtab_p[symbol_cnt - cnt];
     /* protect against deleted symbols */
     if (symbol_p->name_p != NULL)
      {
       /* look for a name match of at least two characters and
       shortest matching definition of search type */
       found = ((is_matched (token_p, symbol_p->name_p, match_len) &&
                ((symbol_p->type & type) > 0)));
       if (found && (symbol_p->name_len < short_len))
        {
         shortest_p = symbol_p;
         short_len = symbol_p->name_len;
        }
      }
     cnt++;
    }
  }

 return (shortest_p);
}

/* ========================================================================
   displays current tokeniser string and tags the position of last token.
   An optional message is displayed
   ======================================================================== */

void tag_current_line(parse_t *pars_p,char *message_p)
{
 char            tag[MAX_LINE_LEN+2];
 short             i;

 print ("%s\n", pars_p->line_p);
 for (i = 0; i < pars_p->par_pos; i++)
  tag[i] = SPACE_CHAR;
 tag[pars_p->par_pos] = '^';
 tag[pars_p->par_sta] = '^';
 tag[(pars_p->par_pos) + 1] = '\0';
 print ("%s\n%s\n", tag, message_p);
}

/* ========================================================================
   included for tokeniser testing
   ======================================================================== */

void pars_debug(parse_t *pars_p)
{
 tag_current_line (pars_p, "debug");
 print ("Tok = \"%s\", delim = %x, toklen = %d \n",
    pars_p->token,
    pars_p->tok_del,
    pars_p->tok_len);
}

/* ========================================================================
   start up a new environment
   ======================================================================== */

void init_pars(parse_t *pars_p,char *new_line_p)
{
 pars_p->line_p = new_line_p;
 pars_p->par_pos = 0;
 pars_p->par_sta = 0;
 pars_p->tok_del = '\0';
 pars_p->token[0] = '\0';
 pars_p->tok_len = 0;
}

/* ========================================================================
   copies parsing status
   ======================================================================== */

void cp_pars(parse_t *dest_p,parse_t *source_p)
{
 *dest_p = *source_p;
}

/* ========================================================================
   implements a tokeniser with a 'soft' approach to end of line conditions
   repeated calls will eventually leave parse position at the null
   terminating the input string.
   ======================================================================== */

short get_tok(parse_t *pars_p,char *delim_p)
{
 short             par_sta = pars_p->par_pos;
 short             par_pos = par_sta;
 short             tok_len = 0;
 short             quotes = 0;

 /* check that we are not already at the end of a line due to a previous
    call (or a null input) - if so return a null token  End of line now
    includes finding comment character! */

 if ((pars_p->line_p[par_pos] == '\0') ||
     (pars_p->line_p[par_pos] == COMMENT_CHAR))
  {
   pars_p->token[0] = '\0';
   pars_p->tok_del = '\0';
  }
 else
  {
   /* attempt to find start of a token, noting special case of first
      call, incrementing past last delimiter and checking for end of
      line on the way */
   if (par_pos != 0)
    par_pos++;
   while (((pars_p->line_p[par_pos] == SPACE_CHAR) ||
        (pars_p->line_p[par_pos] == TAB_CHAR)) &&
        (pars_p->line_p[par_pos] != '\0') &&
        (pars_p->line_p[par_pos] != COMMENT_CHAR))
    par_pos++;

   /* if we find a delimiter before anything else, return a null token
      also deal with special case of a comment character ending a line */
   if (is_delim (pars_p->line_p[par_pos], delim_p) ||
      (pars_p->line_p[par_pos] == COMMENT_CHAR))
    {
     pars_p->token[0] = '\0';
      if (pars_p->line_p[par_pos] != COMMENT_CHAR)
       pars_p->tok_del = pars_p->line_p[par_pos];
      else
       pars_p->tok_del = '\0';
    }
   else
    {
     /* copy token from line into token string until next delimiter
        found. Note that delimiters found within pairs of DOUBLE
        quotes will not be considered significant. Quotes can be
        embedded within strings using '\' however. Note also that we
        have to copy the '\' escape char where it is used, it can be
        taken out when the string is evaluated */
     while ((!is_delim (pars_p->line_p[par_pos], delim_p) || (quotes > 0)) &&
            (tok_len < MAX_TOK_LEN) && (pars_p->line_p[par_pos] != '\0'))
      {
       pars_p->token[tok_len] = pars_p->line_p[par_pos++];
        if ((pars_p->token[tok_len] == '"') && (tok_len == 0))
         quotes++;
        if ((pars_p->token[tok_len] == '"') && (tok_len > 0))
         {
          if (pars_p->token[tok_len - 1] != ESCAPE_CHAR)
           {
            if (quotes > 0)
             quotes--;
            else
             quotes++;
           }
         }
        tok_len++;
      }
     /* if we ran out of token space before copy ended, move up to
        delimiter */
     while (!is_delim (pars_p->line_p[par_pos], delim_p))
      par_pos++;

     /* tidy up the rest of the data */
     pars_p->tok_del = pars_p->line_p[par_pos];
     pars_p->token[tok_len] = '\0';
    }
  }

 pars_p->par_pos = par_pos;
 pars_p->par_sta = par_sta;
 pars_p->tok_len = tok_len;

 return (tok_len);
}

/* ========================================================================
   Name:        init_sym_table
   Description: Sets up a simple symbol table as an array of elements
   ======================================================================== */

void init_sym_table(int elements)
{
 symbol_cnt     = 0;
 command_cnt    = 0;
 macro_cnt      = 0;
 max_symbol_cnt = elements; /* based on mimimum size required */
 symtab_p       = (symtab_t **)malloc((size_t)(sizeof(symtab_t *)*max_symbol_cnt));
 if (symtab_p==NULL)
  {
   print("%s():**ERROR** !!! Symbol table initialization : not enough memory !!!\n",__FUNCTION__);
  }
}

/* ========================================================================
   Name:        init_string_table
   Description: Allocate memory for line table
   ======================================================================== */

static void init_string_table(void)
{
 /* Reset variables */
 /* --------------- */
 StringTableIndex             = -1;
 MaxSimultaneousStringReached = 0;

 /* Alloc line table */
 /* ---------------- */
 StringTable_p=(SlotTable_t **)malloc(sizeof(SlotTable_t*)*STRING_TABLE_INIT_SIZE);
 if (StringTable_p==NULL)
  {
   print("%s():**ERROR** !!! Line table initialization : not enough memory !!!\n",__FUNCTION__);
  }
 else
  {
   unsigned char i;
   for (i=0;i<STRING_TABLE_INIT_SIZE;i++)
    {
     StringTable_p[i]=(SlotTable_t *)malloc(MAX_LINE_LEN);
     if (StringTable_p[i]==NULL)
      {
       print("%s():**ERROR** !!! Line table initialization : not enough memory !!!\n",__FUNCTION__);
       break;
      }
    }
  }
}

/* ========================================================================
   Name:        init_token_table
   Description: Allocate memory for token table
   ======================================================================== */

static void init_token_table(void)
{
 /* Reset variables */
 /* --------------- */
 TokenTableIndex             = -1;
 MaxSimultaneousTokenReached = 0;

 /* Alloc token table */
 /* ----------------- */
 TokenTable_p=(SlotTable_t **)malloc(sizeof(SlotTable_t*)*TOKEN_TABLE_INIT_SIZE);
 if (TokenTable_p==NULL)
  {
   print("%s():**ERROR** !!! Token table initialization : not enough memory !!!\n",__FUNCTION__);
  }
 else
  {
   unsigned int i;
   for (i=0;i<TOKEN_TABLE_INIT_SIZE;i++)
    {
     TokenTable_p[i]=(SlotTable_t *)malloc(MAX_LINE_LEN);
     if (TokenTable_p[i]==NULL)
      {
       print("%s():**ERROR** !!! Token table initialization : not enough memory !!!\n",__FUNCTION__);
       break;
      }
    }
  }
}

/* ========================================================================
   Name:        init_parse_table
   Description: Allocate memory for parse table
   ======================================================================== */

static void init_parse_table(void)
{
 /* Reset variables */
 /* --------------- */
 ParseTableIndex             = -1;
 MaxSimultaneousParseReached = 0;

 /* Alloc parse table */
 /* ----------------- */
 ParseTable_p=(parse_t **)malloc(sizeof(parse_t *)*PARSE_TABLE_INIT_SIZE);
 if (ParseTable_p==NULL)
  {
   print("%s():**ERROR** !!! Parse table initialization : not enough memory !!!\n",__FUNCTION__);
  }
 else
  {
   unsigned char i;
   unsigned short AllocSize;
   AllocSize=sizeof(parse_t)+MAX_LINE_LEN;
   for (i=0;i<PARSE_TABLE_INIT_SIZE;i++)
    {
     ParseTable_p[i]=(parse_t *)malloc(AllocSize);
     if (ParseTable_p[i]==NULL)
      {
       print("%s():**ERROR** !!! Parse table initialization : not enough memory !!!\n",__FUNCTION__);
       break;
      }
    }
  }
}

/* ========================================================================
   deletes all symbol entries in table created above a given nest level.
   if the last entry is deleted, the table store is deallocated.
   ======================================================================== */

void purge_symbols(short level)
{
 int             cnt;
 symtab_t       *symbol_p;
 int            exit = FALSE;

 cnt = 1;
 while (!exit && (cnt <= symbol_cnt))
  {
   symbol_p = symtab_p[symbol_cnt - cnt];
   if ((exit = (symbol_p->depth <= level)) == FALSE)
    {
     if (symbol_p->name_p != NULL)
      free (symbol_p->name_p);
     if (symbol_p->type == STR_SYMBOL)
      free (symbol_p->value.str_val);
     if (symbol_p->type == MAC_SYMBOL)
      {
       macro_t        *l_p, *nl_p;

       l_p = symbol_p->value.mac_val;
       while (l_p != (macro_t *) NULL)
        {
         nl_p = l_p->line_p;
         free ((char *) l_p);
         l_p = nl_p;
        }
      }
     free ((char *) symbol_p);
     cnt++;
    }
  }
 symbol_cnt = symbol_cnt - (cnt - 1);
 if (symbol_cnt == 0)
  free ((char *) symtab_p);
}

/* ========================================================================
   adds symbol structure to table and checks for name clashes, scope
   clashes, invalid identifiers and lack of table space. A valid name
   must contain alphanumerics only, plus '_', with
   the first character alphabetic.
   ======================================================================== */

symtab_t *insert_symbol(char *token_p,short type)
{
 symtab_t       *symbol_p;
 symtab_t       *oldsym_p;
 short             cnt;
 int            valid;

 /* check that symbol table has a spare slot and issue a warning if close */
 symbol_p = (symtab_t *) NULL;
 if (symbol_cnt >= (max_symbol_cnt - 1))
  print ("Cannot insert \"%s\" in symbol table - table is full!\n", token_p);
 else
  {
   valid = TRUE;
   cnt = 0;
   while (token_p[cnt] != '\0')
    {
     if (((token_p[cnt] < 'A') || (token_p[cnt] > 'Z')) &&
        ((token_p[cnt] < 'a') || (token_p[cnt] > 'z')) &&
        ((token_p[cnt] < '0') || (token_p[cnt] > '9') || (cnt == 0)) &&
        ((token_p[cnt] != '_') || (cnt == 0)))
      valid = FALSE;
     cnt++;
    }
   if (!valid)
    print ("Cannot insert \"%s\" in symbol table - invalid symbol name\n",
                   token_p);
   else
    {
     /* carry on with insertion process, checking for scoped name
        clashes */
     /* look for a symbol of the same type and matching name. This can
        be ok if it was declared in a macro level less than the current one */
     oldsym_p = look_for (token_p, ANY_SYMBOL);
     if ((oldsym_p != (symtab_t *) NULL) && (oldsym_p->depth >= macro_depth))
      print ("Cannot insert \"%s\" in symbol table - name clash within current scope\n", token_p);
     else
      {
       symbol_p = (symtab_t *) malloc (sizeof (symtab_t));
       if (symbol_p == NULL)
        {
         print ("Cannot insert \"%s\" in symbol table - no memory available\n", token_p);
        }
       else
        {
         symbol_p->name_p = malloc (MAX_TOK_LEN);
         if (symbol_p->name_p == NULL)
          {
           print ("Cannot insert \"%s\" in symbol table - no memory available\n", token_p);
           free(symbol_p);
           symbol_p = NULL;
          }
         else
          {
           /* print("Inserted symbol \"%s\" using space at %x\n",
              token_p, symbol_p); */
           strcpy (symbol_p->name_p, token_p);
           symbol_p->name_len = strlen (token_p);
           symbol_p->type = type;
           symbol_p->depth = macro_depth;

           /* insert new structure in table and warn if near full */
           symtab_p[symbol_cnt] = symbol_p;
           symbol_cnt++;
           if(symbol_p->type==COM_SYMBOL)
            command_cnt++;
           if(symbol_p->type==MAC_SYMBOL)
            macro_cnt++;
           if (symbol_cnt >= (max_symbol_cnt - 10))
            print ("Warning: Symbol table nearly full - (%ld of %ld entries)\n",
                                  symbol_cnt, max_symbol_cnt);
          }
        }
      }
    }
  }

 return (symbol_p);
}

/* ========================================================================
   allows deletion of a symbol from table, providing its been declared
   at the current macro_depth
   ======================================================================== */

int delete_symbol(char *token_p)
{
 symtab_t *symbol_p;
 int     error = FALSE;
 unsigned short      Index;

 symbol_p = look_for(token_p, ANY_SYMBOL); /* look for any type */
 if (symbol_p == (symtab_t*)NULL)
  {
   error = TRUE;
  }
 else
  {
   if (((symbol_p->fixed)||(symbol_p->depth != macro_depth))&&(!ForceDelete))
    {
     error = TRUE;
    }
   else
    {
     /* free symbol name storage */
     free (symbol_p->name_p);
     symbol_p->name_p = NULL;

     /* delete string storage */
     if (symbol_p->type == STR_SYMBOL)
      {
       free ( symbol_p->value.str_val);
      }

     /* delete any macro line buffers */
     if (symbol_p->type == MAC_SYMBOL)
      {
       macro_t  *l_p,*nl_p;

       l_p = symbol_p->value.mac_val;
       while (l_p != (macro_t*)NULL)
        {
         nl_p = l_p->line_p;
         free ((char*)l_p);
         l_p = nl_p;
        }
      }

     if(symbol_p->type==COM_SYMBOL)
      command_cnt--;

     if(symbol_p->type==MAC_SYMBOL)
      macro_cnt--;

     /* mark symbol as unused, ready for purge when nest level is stripped */
     symbol_p->type = NO_SYMBOL;

     /* defragment symbol table */
     /* find deleted symbol index in table */
     Index = 0;
     while (symtab_p[Index]->type != NO_SYMBOL)
      {
       Index++;
      }
     /* move next symbols backward from 1 slot in table  */
     while(Index < (symbol_cnt-1))
      {
       symtab_p[Index] = symtab_p[Index+1] ;
       Index++;
      }

     symtab_p[symbol_cnt-1] = NULL;
     symbol_cnt--;

     /* Actually de allocate the deleted symbol */
     free ((char*)symbol_p);
    }
  }

 return(error);
}

/* ========================================================================
   creates or updates an integer symbol table entry
   ======================================================================== */

int assign_integer(char *token_p,int value,int constant)
{
 symtab_t       *symbol_p;
 int            error = FALSE;

 if (strlen (token_p) != 0)
  {
   symbol_p = look_for (token_p, INT_SYMBOL);
   if ((symbol_p == (symtab_t *) NULL) && (token_p[0] != '\0'))
    {
     symbol_p = insert_symbol (token_p, INT_SYMBOL);
     if (symbol_p == (symtab_t *) NULL)
      error = TRUE;
     else
      {
       symbol_p->fixed = constant;
       if (symbol_p->fixed)
        symbol_p->info_p = "integer constant";
       else
        symbol_p->info_p = "integer variable";
                symbol_p->value.int_val = value;
      }
    }
   else
    {
     if (symbol_p->fixed)
      error = TRUE;
     else
      symbol_p->value.int_val = value;
    }
  }

 return (error);
}

/* ========================================================================
   creates an integer symbol table entry without looking for existing symbol
   with a name match
   ======================================================================== */

int create_integer(char *token_p,int value,int constant)
{
 symtab_t       *symbol_p;
 int            error = FALSE;

 if (strlen (token_p) != 0)
  {
   symbol_p = insert_symbol (token_p, INT_SYMBOL);
   if (symbol_p == (symtab_t *) NULL)
    error = TRUE;
   else
    {
     symbol_p->fixed = constant;
     if (symbol_p->fixed)
      symbol_p->info_p = "integer constant";
     else
      symbol_p->info_p = "integer variable";
     symbol_p->value.int_val = value;
    }
  }

 return (error);
}

/* ========================================================================
   creates or updates a floating point symbol table entry
   ======================================================================== */

int assign_float(char *token_p,double value,int constant)
{
 symtab_t       *symbol_p;
 int            error = FALSE;

 if (strlen (token_p) != 0)
  {
   symbol_p = look_for (token_p, FLT_SYMBOL);
   if ((symbol_p == (symtab_t *) NULL) && (token_p[0] != '\0'))
    {
     symbol_p = insert_symbol (token_p, FLT_SYMBOL);
     if (symbol_p == (symtab_t *) NULL)
      error = TRUE;
     else
      {
       symbol_p->fixed = constant;
       if (symbol_p->fixed)
        symbol_p->info_p = "floating point constant";
       else
        symbol_p->info_p = "floating point variable";
       symbol_p->value.flt_val = value;
      }
    }
   else
    {
     if (symbol_p->fixed)
      error = TRUE;
     else
      symbol_p->value.flt_val = value;
    }
  }

 return (error);
}

/* ========================================================================
   creates a floating point symbol table entry
   ======================================================================== */

int create_float(char *token_p,double value,int constant)
{
 symtab_t       *symbol_p;
 int            error = FALSE;

 if (strlen (token_p) != 0)
  {
   symbol_p = insert_symbol (token_p, FLT_SYMBOL);
   if (symbol_p == (symtab_t *) NULL)
    error = TRUE;
   else
    {
     symbol_p->fixed = constant;
     if (symbol_p->fixed)
      symbol_p->info_p = "floating point constant";
     else
      symbol_p->info_p = "floating point variable";
     symbol_p->value.flt_val = value;
    }
  }

 return (error);
}

/* ========================================================================
   creates or updates a string symbol table entry
   ======================================================================== */

int assign_string(char *token_p,char *value,int constant)
{
 symtab_t       *symbol_p;
 int            error = FALSE;

 if (strlen (token_p) != 0)
  {
   symbol_p = look_for (token_p, STR_SYMBOL);
    if (symbol_p == (symtab_t *) NULL)
     {
      symbol_p = insert_symbol (token_p, STR_SYMBOL);
      if (symbol_p == (symtab_t *) NULL)
       error = TRUE;
      else
       {
        symbol_p->fixed = constant;
        if (symbol_p->fixed)
         symbol_p->info_p = "string constant";
        else
         symbol_p->info_p = "string variable";
        symbol_p->value.str_val = malloc (MAX_TOK_LEN);
       if (symbol_p->value.str_val == NULL)
        error = TRUE;
       else
        strcpy (symbol_p->value.str_val, value);
      }
    }
   else
    {
     if (symbol_p->fixed)
      error = TRUE;
     else
      strcpy (symbol_p->value.str_val, value);
    }
  }

 return (error);
}

/* ========================================================================
   creates a string symbol table entry
   ======================================================================== */

int create_string(char *token_p,char *value,int constant)
{
 symtab_t       *symbol_p;
 int            error = FALSE;

 if (strlen (token_p) != 0)
  {
   symbol_p = insert_symbol (token_p, STR_SYMBOL);
   if (symbol_p == (symtab_t *) NULL)
    error = TRUE;
   else
    {
     symbol_p->fixed = constant;
     if (symbol_p->fixed)
      symbol_p->info_p = "string constant";
     else
      symbol_p->info_p = "string variable";
     symbol_p->value.str_val = malloc (MAX_TOK_LEN);
     if (symbol_p->value.str_val == NULL)
      error = TRUE;
     else
      strcpy (symbol_p->value.str_val, value);
    }
  }

 return (error);
}

/* ========================================================================
   establishes the existence of a command procedure
   ======================================================================== */

int register_command (char *token_p,int (*action)(parse_t *,char *),char *help_p)
{
 symtab_t       *symbol_p;
 int            error = FALSE;

 symbol_p = look_for (token_p, (COM_SYMBOL | MAC_SYMBOL));
 if (symbol_p != (symtab_t *) NULL)
  {
   error = TRUE;
   print ("Name clash when registering command \"%s\"\n", token_p);
  }
 else
  {
   symbol_p = insert_symbol (token_p, COM_SYMBOL);
   if (symbol_p == (symtab_t *) NULL)
    error = TRUE;
   else
    {
     symbol_p->fixed = TRUE;
     symbol_p->info_p = help_p;
     symbol_p->value.com_val = action;
    }
  }

 return (error);
}

/* ========================================================================
   attempts a series of operations to extract an integer value from a token
   ======================================================================== */

int evaluate_integer(char *token_p,int *value_p,short default_base)
{
 parse_t         pars;
 symtab_t       *symbol_p;
 int            error = FALSE;

 if (token_p[0] == '\0')
  error = TRUE;
 else if (token_p[0] == '-')
  {                           /* unary negative */
   token_p++;
   error = evaluate_integer (token_p, value_p, default_base);
   if (!error)
    *value_p = -(*value_p);
  }
 else if (token_p[0] == '~')
  {                           /* unary bitflip */
   token_p++;
   error = evaluate_integer (token_p, value_p, default_base);
   if (!error)
    *value_p = ~(*value_p);
  }
 else if (token_p[0] == '!')
  {                           /* unary NOT */
   token_p++;
   error = evaluate_integer (token_p, value_p, default_base);
   if (!error)
    *value_p = !(*value_p);
  }
 else
  {
   /* First look for a simple number, then try a symbol reference. If
      this all fails, the value may be due to an expression, so try and
      evaluate it */
   *value_p = conv_int (token_p, default_base);
   if (*value_p == BAD_INT_VAL)
    {
     symbol_p = look_for (token_p, INT_SYMBOL);
     if (symbol_p == (symtab_t *) NULL)
      {
       init_pars (&pars, token_p);
       error = evaluate_integer_expr (&pars, value_p, default_base);
      }
     else
      {
       error = FALSE;
       *value_p = symbol_p->value.int_val;
      }
    }
  }

 return (error);
}

/* ========================================================================
   generalised recursive evaluate of a possibly bracketed integer expression.
   No leading spaces are allowed in the passed string, and the parsing
   structure is assumed to be initialised but not yet used
   ======================================================================== */

int evaluate_integer_expr(parse_t *pars_p,int *value_p,short default_base)
{
 int            error = FALSE;
 short             brackets = 0;
 int             value1, value2;
 short             index, i;
 char            sub_expr[MAX_TOK_LEN];
 char            operation;

 static char    *delim = "*/+-|&^%";     /* recognised arithmetic operators */

 /* pair up a leading bracket */
 if (pars_p->line_p[0] == '(')
  {
   brackets = 1;
   index = 1;
   while ((pars_p->line_p[index] != '\0') && (brackets > 0))
    {
     if (pars_p->line_p[index] == ')')
      brackets--;
     if (pars_p->line_p[index] == '(')
      brackets++;
     index++;
    }
   if (brackets != 0)
    error = TRUE;
   else
    {
     /* copy substring without enclosing brackets and evaluate it. if
        this is ok, then update parsing start position */
     for (i = 1; i < index; i++)
      sub_expr[i - 1] = pars_p->line_p[i];
     sub_expr[index - 2] = '\0';
     error = evaluate_integer (sub_expr, &value1, default_base);
     if (!error)
      pars_p->par_pos = index - 1;
    }
  }
 if (!error)
  {
   /* look for a token and check for significance */
      get_tok (pars_p, delim);
   if (pars_p->tok_del == '\0')
    {                       /* no operator seen */
     if (pars_p->par_sta > 0)    /* bracket removal code used */
      *value_p = value1;
     else
      error = TRUE;   /* not a valid expression! */
    }
   else
    {
     /* have found an operator. If we stripped brackets in the first
        half of this code, then the interesting part of the line is
        now after the operator. If we did not, then evaluate both
        sides of operator */
     operation = pars_p->tok_del;
     if (pars_p->par_sta == 0)
      error = evaluate_integer (pars_p->token, &value1, default_base);
     if (!error)
      {
       get_tok (pars_p, "");   /* all the way to the end of this
                                           line */
       error = evaluate_integer (pars_p->token, &value2, default_base);
      }
     if (!error)
      {
       switch (operation)
        {
          case '+':
           *value_p = value1 + value2;
          break;
          case '-':
           *value_p = value1 - value2;
          break;
          case '*':
           *value_p = value1 * value2;
          break;
          case '/':
           *value_p = value1 / value2;
          break;
          case '&':
           *value_p = value1 & value2;
          break;
          case '|':
           *value_p = value1 | value2;
          break;
          case '^':
           *value_p = value1 ^ value2;
          break;
          case '%':
           *value_p = value1 % value2;
          break;
          default:
           error = TRUE;
          break;
        }
      }
    }
  }

 return (error);
}

/* ========================================================================
   attempts a series of operations to extract a floating pt value from a tok
   ======================================================================== */

int evaluate_float(char *token_p,double *value_p)
{
 parse_t         pars;
 symtab_t       *symbol_p;
 int            error = FALSE;

 /* this will only be relevant for non null strings w/o leading white
    space. First look for a simple number, then try a symbol reference. If
    this all fails, the value may be due to an expression, so try and
    evaluate it */

 if (token_p[0] == '\0')
  error = TRUE;
 else
  {
   error = conv_flt (token_p, value_p);
   if (error)
    {
     /* a symbol would be legal if it were an integer, but have to float it */
     symbol_p = look_for (token_p, FLT_SYMBOL | INT_SYMBOL);
     if (symbol_p == (symtab_t *) NULL)
      {
       init_pars (&pars, token_p);
       error = evaluate_float_expr (&pars, value_p);
      }
     else
      {
       error = FALSE;
       if (symbol_p->type == FLT_SYMBOL)
        *value_p = symbol_p->value.flt_val;
       else
        *value_p = (double) symbol_p->value.int_val;
      }
    }
  }

 return (error);
}

/* ========================================================================
   generalised recursive evaluate of a, possibly bracketed, float expression
   No leading spaces are allowed in the passed string, and the parsing
   structure is assumed to be initialised but not yet used
   ======================================================================== */

int evaluate_float_expr(parse_t *pars_p,double *value_p)
{
 int            error = FALSE;
 short             brackets = 0;
 double          value1, value2;
 short             index, i;
 char            sub_expr[MAX_TOK_LEN];
 char            operation;
 static char    *delim = "*/+-";     /* recognised arithmetic operators */

 if (pars_p->line_p[0] == '(')
  {
   brackets = 1;
   index = 1;
   while ((pars_p->line_p[index] != '\0') && (brackets > 0))
    {
     if (pars_p->line_p[index] == ')')
         brackets--;
     if (pars_p->line_p[index] == '(')
         brackets++;
     index++;
    }
   if (brackets != 0)
    error = TRUE;
   else
    {
     /* copy substring without enclosing brackets and evaluate it. if
        this is ok, then update parsing start position */
     for (i = 1; i < index; i++)
      sub_expr[i - 1] = pars_p->line_p[i];
     sub_expr[index - 2] = '\0';
     error = evaluate_float (sub_expr, &value1);
     if (!error)
      pars_p->par_pos = index - 1;
    }
  }
 if (!error)
  {
   /* look for a token and check for significance */
   get_tok (pars_p, delim);
   if (pars_p->tok_del == '\0')
    {                       /* no operator seen */
     if (pars_p->par_sta > 0)    /* bracket removal code used */
      *value_p = value1;
     else
      error = TRUE;   /* not a valid expression! */
    }
   else
    {
     /* have found an operator. If we stripped brackets in the first
        half of this code, then the interesting part of the line is
        now after the operator. If we did not, then evaluate both
        sides of operator */
     operation = pars_p->tok_del;
     if (pars_p->par_sta == 0)
      error = evaluate_float (pars_p->token, &value1);
     if (!error)
      {
       get_tok (pars_p, "");   /* all the way to the end of this line */
       error = evaluate_float (pars_p->token, &value2);
      }
     if (!error)
      {
       switch (operation)
        {
         case '+':
          *value_p = value1 + value2;
         break;
         case '-':
          *value_p = value1 - value2;
         break;
         case '*':
          *value_p = value1 * value2;
         break;
         case '/':
          *value_p = value1 / value2;
         break;
         default:
          error = TRUE;
        }
      }
    }
  }

 return (error);
}

/* ========================================================================
   specific evaluation of comparison between two numeric expressions.
   No leading spaces are allowed in the passed string.
   A single expression will yield a result of FALSE if equal to zero.
   Although real expressions are allowed, comparison is done in integers.
   A default result of TRUE is returned in the case of an error
   ======================================================================== */

int evaluate_comparison(char *token_p,int *result_p,short default_base)
{
 int    error = FALSE;
 int    RightResult, AndFound, OrFound, StayInBrackets;
 parse_t *parse_p;
 int     value1, value2;
 short     brackets = 0;
 short     index;
 unsigned char      FirstExprType;
 double  real_val;
 char    op1,op2 = 0;
 char    *str1 = NULL;
 char    *str2, *sub_expr;
 int     cmp;

 parse_p = alloc_parse_table_slot(strlen(token_p)+1);
 *result_p = FALSE;

 /* ignore spaces before brackets */
 init_pars(parse_p, token_p);
 index = 0;
 StayInBrackets = FALSE;
 while ((parse_p->line_p[index] == ' ') && (parse_p->line_p[index] != '\0'))
 index++;

 if ( (parse_p->line_p[index] == '('))
  {
   index ++;
   brackets = 1;
   StayInBrackets = TRUE;
   while((parse_p->line_p[index] != '\0') && (brackets > 0))
   {
    if (parse_p->line_p[index] == ')') brackets--;
    if (parse_p->line_p[index] == '(') brackets++;
    index ++;
   }
   if (brackets != 0)
   {
    free_parse_table_slot(); /* parse_p */
    return(TRUE);
   }
   /* look if thers is any Logical Operator inside.
      If so, evaluate it, else, this is a expression to be calculated : leave brackets */
   get_tok(parse_p, LogicalOps);
   if (parse_p->par_pos >= index) /* outside bracket */
   {
    StayInBrackets = FALSE;
   }
   init_pars(parse_p, token_p);
  } /* if ( (parse_p->line_p[index] == '(')) */

 if (StayInBrackets)
  {
   /* copy substring without enclosing brackets and evaluate it.
      if this is ok, then update parsing start position */
   sub_expr = alloc_token_table_slot(index);
   strncpy(sub_expr,&(parse_p->line_p[1]), index-2);
   sub_expr[index-2] = '\0';
   error = evaluate_comparison(sub_expr, result_p, default_base);
   free_token_table_slot(); /* sub_expr */
   if (!error)
    {
     parse_p->par_pos = index-1;
     get_tok(parse_p, ComparisonOps);
     if (parse_p->tok_del != '\0')
      {
       sub_expr = &(parse_p->line_p[parse_p->par_pos]);
       op1 = *sub_expr;
       sub_expr ++;
       if (is_delim(*sub_expr, ComparisonOps))
        {
         if (*sub_expr == op1)
          {
           sub_expr ++;
          }
         else
          {
           error = TRUE;
          }
        }
       if (!error)
        {
         error = evaluate_comparison(sub_expr, &RightResult, default_base);
        }
       if (!error)
        {
         if (op1 == '|')
          {
           *result_p |= RightResult;
          }
         else if (op1 == '&')
          {
           *result_p &= RightResult;
          }
        }
      }
    }
   free_parse_table_slot(); /* parse_p */
   return(error);
  }

 get_tok(parse_p, LogicalOps);
 //LogicalNOTFound = FALSE;
 if ( (parse_p->line_p[parse_p->par_pos] == '!') && (parse_p->line_p[parse_p->par_pos+1] != '='))
  {
  // LogicalNOTFound = TRUE;
   sub_expr = alloc_token_table_slot(strlen(token_p));
   strncpy(sub_expr,&(parse_p->line_p[parse_p->par_pos+1]), strlen(token_p));
   sub_expr[strlen(token_p)-1] = '\0';
   error = evaluate_comparison(sub_expr , result_p, default_base);
   free_token_table_slot(); /* sub_expr */
   if (!error )
    {
     *result_p = !(*result_p);
    }
   free_parse_table_slot(); /* parse_p */
   return(error);
  }

 /* look for AND (&&) or OR (||)*/
 AndFound = OrFound = FALSE;
 init_pars(parse_p, token_p);
 get_tok(parse_p, ComparisonOps);
 if ( (parse_p->tok_del == '&') && (parse_p->line_p[parse_p->par_pos+1] == '&'))
  {
   AndFound = TRUE;
  }
 else if ( (parse_p->tok_del == '|') && (parse_p->line_p[parse_p->par_pos+1] == '|'))
  {
   OrFound = TRUE;
  }
 if (AndFound || OrFound)
  {
   sub_expr = alloc_token_table_slot(parse_p->par_pos+1);
   strncpy(sub_expr,parse_p->line_p, parse_p->par_pos);
   sub_expr[parse_p->par_pos] = '\0';
   error = evaluate_comparison(sub_expr, result_p, default_base);
   free_token_table_slot(); /* sub_expr */
   if (!error)
    {
       sub_expr = &(parse_p->line_p[parse_p->par_pos+2]);
       error = evaluate_comparison(sub_expr, &RightResult, default_base);
    }
   if (!error)
    {
     if (OrFound)
      {
       *result_p |= RightResult;
      }
     else /* AndFound */
      {
       *result_p &= RightResult;
      }
    }
   free_parse_table_slot(); /* parse_p */
   return(error);
  }

 /* evaluate first arithmetic expression in line */
 init_pars(parse_p, token_p);
 get_tok(parse_p, LogicalOps);
 FirstExprType = INT_SYMBOL;
 error = evaluate_integer(parse_p->token, &value1, default_base);
 if (error) /* not an integer */
  {
   FirstExprType = FLT_SYMBOL;
   error = evaluate_float(parse_p->token, &real_val);
   if (!error)
    {
     value1 = (int) real_val;
    }
   if (error)
    {
     FirstExprType = STR_SYMBOL;
     str1 = alloc_token_table_slot(MAX_LINE_LEN);
     error = evaluate_string(parse_p->token, str1, MAX_LINE_LEN);
    }
  }
 if (!error)
  {
   /* deal with the case of a single expression value */
   if (parse_p->tok_del == '\0')
    {
     if (FirstExprType == INT_SYMBOL)
      {
       *result_p = !(value1 == 0);
      }
     else
      {
       *result_p = FALSE;
      }
    }
   else
    {
     /* look for valid single or pair of operators and move posn accordingly */
     op1 = parse_p->line_p[parse_p->par_pos];
     op2 = parse_p->line_p[parse_p->par_pos+1];
     if ((op2 != '>') && (op2 != '<') && (op2 != '='))
      {
       op2 = 0;
      }
     else
      {
       parse_p->par_pos++;
      }
     /* get token for rest of line and extract a value in a similar way */
     get_tok(parse_p, "");
     error = evaluate_integer(parse_p->token, &value2, default_base);
     if (error)
      {
       error = evaluate_float(parse_p->token, &real_val);
       if (!error)
        {
         value2 = (int) real_val;
        }
       if (error)
        {
         if (FirstExprType != STR_SYMBOL) /* str1 is unset */
          {
           *result_p = FALSE;
           free_parse_table_slot(); /* parse_p */
           return(FALSE);
          }
         str2 = alloc_token_table_slot(MAX_LINE_LEN);
         error = evaluate_string(parse_p->token, str2, MAX_LINE_LEN);
         cmp=strcmp(str1,str2);
         free_token_table_slot(); /* str2 */
         free_token_table_slot(); /* str1 */
         *result_p = FALSE;
         switch(op1)
          {
           case '=': switch(op2) {
            case '=':  if (cmp==0) *result_p = TRUE; break;
            case '<':  if (cmp<=0) *result_p = TRUE; break;
            case '>':  if (cmp>=0) *result_p = TRUE; break;
            default :  error = TRUE; break;
            } break;
           case '!': switch(op2) {
            case '=':  if (cmp!=0) *result_p = TRUE; break;
            default :  error = TRUE; break;
            } break;
           case '>': switch(op2) {
            case '=':  if (cmp>=0) *result_p = TRUE; break;
            case  0 :  if (cmp>0) *result_p = TRUE;  break;
            default :  error = TRUE; break;
            } break;
           case '<': switch(op2) {
            case '=':  if (cmp<=0) *result_p = TRUE; break;
            case '>':  if (cmp!=0) *result_p = TRUE; break;
            case  0 :  if (cmp<0) *result_p = TRUE;  break;
            default :  error = TRUE; break;
            } break;
          }
         free_parse_table_slot(); /* parse_p */
         return(error);
        }
      }
     if (!error) /* value2 has been set */
      {
       if (FirstExprType == STR_SYMBOL) /* value1 unset */
        {
         *result_p = FALSE;
         free_parse_table_slot(); /* parse_p */
         return(FALSE);
        }
       /* deal with the combination of two expression values */
       switch(op1)
        {
         case '=': switch(op2) {
           case '=':  *result_p = (value1 == value2); break;
           case '<':  *result_p = (value1 <= value2); break;
           case '>':  *result_p = (value1 >= value2); break;
           default :  error = TRUE; break;
         } break;
         case '!': switch(op2) {
           case '=':  *result_p = (value1 != value2); break;
           default :  error = TRUE; break;
         } break;
         case '>': switch(op2) {
           case '=':  *result_p = (value1 >= value2); break;
           case  0 :  *result_p = (value1 > value2);  break;
           default :  error = TRUE; break;
         } break;
         case '<': switch(op2) {
           case '=':  *result_p = (value1 <= value2); break;
           case '>':  *result_p = (value1 != value2); break;
           case  0 :  *result_p = (value1 < value2);  break;
           default :  error = TRUE; break;
         } break;
        }
      }
    }
  }

 free_parse_table_slot(); /* parse_p */
 return(error);
}

/* ========================================================================
   attempts a series of operations to extract a string value from a token
   ======================================================================== */

int evaluate_string(char *token_p,char *string_p,short max_len)
{
 symtab_t *symbol_p;
 int     error = FALSE;
 short      index,i,len;
 int      int_val;
 double   real_val;
 char tmp_val[TMP_CHAR_MAX_LINE_LENGTH];
 parse_t  *parse_p;

 string_p[0] = '\0';
 /* print("string evaluation of %s \n",token_p); */
 if (token_p[0] == '\0')
  {
   error = TRUE;
  }
 else
  {
   parse_p = alloc_parse_table_slot(strlen(token_p)+1);
   /* look for concatenation function */
   init_pars(parse_p, token_p);
   get_tok(parse_p,"+");
   if (parse_p->tok_del == '+')
    {
     /* call routine on halves of passed token */
     error = evaluate_string(parse_p->token, string_p, max_len);
     if (!error)
      {
       do
        {
         get_tok(parse_p, "+"); /* next '+' or rest of line */
         error  = evaluate_integer(parse_p->token, &int_val, number_base);
         if (!error)
          {
           snprintf(tmp_val, TMP_CHAR_MAX_LINE_LENGTH, "%d", int_val);
           strncat(string_p, tmp_val, TMP_CHAR_MAX_LINE_LENGTH);
          }
         else
          {
           error  = evaluate_float(parse_p->token, &real_val);
           if (!error)
            {
             snprintf(tmp_val, TMP_CHAR_MAX_LINE_LENGTH, "%G", real_val);
             strncat(string_p, tmp_val, TMP_CHAR_MAX_LINE_LENGTH);
            }
           else
            {
             len = strlen(string_p);
             error = evaluate_string(parse_p->token, &string_p[len], max_len-len);
            }
          }
        } while (!error && (parse_p->tok_del == '+'));
      }
    }
   else
    {
     symbol_p = look_for(token_p, STR_SYMBOL);
     if (symbol_p != (symtab_t*)NULL)
      {
       if ( ((short)strlen(symbol_p->value.str_val))<=max_len )
        {
         strncpy(string_p, symbol_p->value.str_val, strlen(symbol_p->value.str_val)+1);
        }
       else
        {
         print("symbol string value too long\n");
         error = TRUE;
        }
      }
     else
      {
       /* we assume since this is already been passed through the tokeniser that this
       is a valid string - starting with a quote mark and ending with a quote mark
       only containing escaped quote marks within its body. All that is needed
       for evaluation is to strip out the escapes */
       len = strlen(token_p);
       if ( len< max_len && ((token_p[0] == '"') && (token_p[len-1] =='"')) )
        {
         /* copy substring without enclosing quotes, preserving case */
         index = 1;
         i=0;
         while((token_p[index] != '\0') && (index < (len-1)))
          {
           if ((token_p[index+1] == '"') && (token_p[index] == ESCAPE_CHAR))
            {
             string_p[i++] = token_p[index+1];
             index++;
            }
           else
            {
             string_p[i++] = token_p[index];
            }
           index++;
          }
         string_p[i] = '\0';
         len = i;
        }
      else
       {
        if ( len >= max_len )
         {
          print("string value too long\n");
         }
        error = TRUE;
       }
     }
   }
  free_parse_table_slot();
 }

return(error);
}

/* ========================================================================
   attempts multiple evaluations of a token. If successful, assigns a
   symbol of appropriate type to the resultant value . Two retuns are
   provided. 'error' signififies that the token was in itself
   correct, but the assignment to the target symbol failed. 'not_done'
   is used to flag an unsuccessful evaluation. The force parameter can
   be used to force creation of the variable to hold the result without
   looking for possible match in symbol table
   ======================================================================== */

int evaluate_assign(char *target_p,char *token_p,int *error_p,int force)
{
 int            not_done=FALSE;
 int             int_val;
 double          real_val;
 char            string_val[MAX_TOK_LEN];

 *error_p = FALSE;
 if (strlen (target_p) != 0)
  {
   not_done = evaluate_integer (token_p, &int_val, number_base);
   if (!not_done)
    {
     if (force)
      *error_p = create_integer (target_p, int_val, FALSE);
     else
      *error_p = assign_integer (target_p, int_val, FALSE);
    }
   else
    {
     not_done = evaluate_float (token_p, &real_val);
     if (!not_done)
      {
       if (force)
        *error_p = create_float (target_p, real_val, FALSE);
       else
        *error_p = assign_float (target_p, real_val, FALSE);
      }
     else
      {
       not_done = evaluate_string (token_p, string_val, MAX_TOK_LEN);
       if (!not_done)
        {
         if (force)
          *error_p = create_string (target_p, string_val, FALSE);
         else
          *error_p = assign_string (target_p, string_val, FALSE);
        }
      }
    }
  }

 return (not_done);
}

/* ========================================================================
   a high level parsing routine which forms part of a set. The general
   specification is the incremental reading of an input string via the
   parsing structure, detecting a parameter refernce of the right type and
   passing back a result. If the input line at this point is blank, a default
   value is substituted for a result. If the next token on the input line
   does not have the right characteristics for a parameter of this type, then
   the routine returns TRUE to signify an error. The default parameter is
   passed back in this case as well as an error.
   ======================================================================== */

int cget_string(parse_t *pars_p,char *default_p,char *result_p,short max_len)
{
 int            error = FALSE;

 get_tok (pars_p, delim_set);
 if (pars_p->tok_len == 0)
  strncpy (result_p, default_p, max_len);
 else
  {
   error = evaluate_string (pars_p->token, result_p, max_len);
   if (error)
   strncpy (result_p, default_p, max_len);
  }
 return (error);
}

/* ========================================================================
   a high level parsing routine which forms part of a set. The general
   specification is the incremental reading of an input string via the
   parsing structure, detecting a parameter refernce of the right type and
   passing back a result. If the input line at this point is blank, a default
   value is substituted for a result. If the next token on the input line
   does not have the right characteristics for a parameter of this type, then
   the routine returns TRUE to signify an error. The default parameter is
   passed back in this case as well.
   ======================================================================== */

int cget_integer(parse_t *pars_p,int def_int,int * result_p)

{
 int            error = FALSE;

 get_tok (pars_p, delim_set);
 if (pars_p->tok_len == 0)
  *result_p = def_int;
 else
  {
   error = evaluate_integer (pars_p->token, result_p, number_base);
   if (error)
    {
     pars_p->par_pos = pars_p->par_sta;
     *result_p = def_int;
    }
  }

 return (error);
}

/* ========================================================================
   a high level parsing routine which forms part of a set. The general
   specification is the incremental reading of an input string via the
   parsing structure, detecting a parameter refernce of the right type and
   passing back a result. If the input line at this point is blank, a default
   value is substituted for a result. If the next token on the input line
   does not have the right characteristics for a parameter of this type, then
   the routine returns TRUE to signify an error. The default parameter is
   passed back in this case as well.
   ======================================================================== */

int cget_float(parse_t *pars_p,double def_flt,double *result_p)
{
 int            error = FALSE;

 get_tok (pars_p, delim_set);
 if (pars_p->tok_len == 0)
 *result_p = def_flt;
 else
  {
   error = evaluate_float (pars_p->token, result_p);
   if (error)
    *result_p = def_flt;
  }
 return (error);
}

/* ========================================================================
   a high level parsing routine which forms part of a set. The general
   specification is the incremental reading of an input string via the
   parsing structure, detecting a parameter refernce of the right type and
   passing back a result. If the input line at this point is blank, a default
   value is substituted for a result. If the next token on the input line
   does not have the right characteristics for a parameter of this type, then
   the routine returns TRUE to signify an error. The default parameter is
   passed back in this case as well.
   ======================================================================== */

int cget_item(parse_t *pars_p,char *default_p,char *result_p,short max_len)
{
 int            error = FALSE;

 /* simply returns next token on line or the default string */
 get_tok (pars_p, delim_set);
 if (pars_p->tok_len == 0)
  strncpy (result_p, default_p, max_len);
 else
  strncpy (result_p, pars_p->token, max_len);

 return (error);
}

/* ========================================================================
   Return the number of parameters provided on the testtool command line
   ======================================================================== */

int cget_token_count(parse_t * pars_p,short *result_p)
{
 char *pt;
 short car_cnt,tok_cnt;
 car_cnt = 0;
 tok_cnt = 0;
 pt      = pars_p->line_p;
 do
  {
   if ( *pt!=' ' && *pt!='\0' && *pt!=NL_CHAR && *pt!=CR_CHAR )
    {
     pt++;
     car_cnt++;
     if ( *pt==' ' || *pt=='\0' || *pt==NL_CHAR || *pt==CR_CHAR )
      {
       /* end of a token (alphanum carac. followed by a  separator) */
       tok_cnt++;
      }
    }
   else
    {
     pt++;
     car_cnt++;
    }
  }
 while(!((*pt=='\0')||(car_cnt>=MAX_LINE_LEN)));
 *result_p=tok_cnt;
 return(FALSE);
}

/* ========================================================================
   Record the input console entry until hit "enter" and return
   it as string.
   ======================================================================== */

int do_getstring(parse_t *pars_p,char *result_p)
{
 char  line_p[MAX_LINE_LEN];
// int   nbbytes = 0;

 memset( line_p, 0, sizeof(line_p));
 /* if we are at 'console level' use a user supplied input routine */
 io_read ("", line_p, MAX_LINE_LEN);
 assign_string(result_p, line_p, FALSE);
 return(FALSE);
}

/* ========================================================================
   obtains input line from current active source, returns FALSE if OK
   SHould this routine do echo of input lines?
   should this routine issue a prompt for interactive input?
   ======================================================================== */

int read_input(char *line_p,char *ip_prompt_p)
{
// short             cnt = 0;
 int             temp = EOF;

 if (cur_macro_p == (macro_t *) NULL)
  {
   if (cur_stream_p == (void *) NULL)
    {
     print ("Internal error - asked to read from NULL file \n\n\n");
     exit (1);
    }
   else
    {
     if (cur_stream_p == stdin)
      {
       /* if we are at 'console level' use a user supplied input
          routine */
       temp = 0;
       io_read (ip_prompt_p, line_p, MAX_LINE_LEN);
      }
     if (logfiled!=NULL)
      {
      //SYS_FWrite(line_p,strlen(line_p),1,logfiled);
      }
    }
  }
 else
  {
   temp = (cur_macro_p->line_p == (macro_t *) NULL);
   strcpy (line_p, cur_macro_p->line);
   cur_macro_p = cur_macro_p->line_p;
  }
 /* echo all non-interactive input if required */
 if (cur_echo && ((macro_depth > 1) || (cur_stream_p != stdin)))
  print ("-> %s\n", line_p);

 return (temp == EOF);
}

/* ========================================================================
   accepts a sequence of input lines that form the body of a macro. The
   definition line passed in contains the formal parameter part of the
   command line that triggered the definition and may be null. Macro
   definition is ended when the first token on a line is found to be 'END'
   ======================================================================== */

int define_macro(char *defline_p,char *name_p)
{
 symtab_t       *symbol_p;
 macro_t        *cur_line_p, *last_line_p;
 parse_t         pars;
 int            end = FALSE;
 int            error = FALSE;
 int            eof;
 short             temp;
 short             nests = 1;

 symbol_p = look_for (name_p, ANY_SYMBOL);
 if ((symbol_p == (symtab_t *) NULL) && (name_p[0] != '\0'))
  {
   symbol_p = insert_symbol (name_p, MAC_SYMBOL);
   if (symbol_p == (symtab_t *) NULL)
    {
     print ("Cannot create macro \"%s\" \n", name_p);
     error = TRUE;
    }
   else
    {
     symbol_p->fixed = FALSE;
     symbol_p->info_p = "command macro";
     symbol_p->value.mac_val = (macro_t *) malloc (sizeof (macro_t));
     if (symbol_p->value.mac_val == NULL)
      {
       print ("Cant allocate memory for macro value\n");
       error = TRUE;
      }
     else
      strcpy (symbol_p->value.mac_val->line, defline_p);
     last_line_p = symbol_p->value.mac_val;
     while (!end && !error)
      {
       cur_line_p = (macro_t *) malloc (sizeof (macro_t));
       if (cur_line_p == NULL)
        {
         print ("Cant allocate memory for macro line\n");
         error = TRUE;
        }
       else
        {
         last_line_p->line_p = cur_line_p;
         last_line_p = cur_line_p;
         eof = read_input (cur_line_p->line, "> ");
         if (eof)
          {
           print ("Unexpected EOF during macro definition\n", name_p);
           error = TRUE;
          }
         else
          {
           init_pars (&pars, cur_line_p->line);
           get_tok (&pars, delim_set);
           if ((is_matched (pars.token, "END", 2)) ||
               (is_matched (pars.token, "EXIT", 2)))
            {
             nests--;
             if (nests == 0)
              end = TRUE;
            }
           else if (is_control (pars.token, &temp))
            nests++;
          }
        }
      }
     last_line_p->line_p = (macro_t *) NULL;
     if (error)
      {
       print ("Cannot complete definition of \"%s\"\n", name_p);
       delete_symbol (name_p);
      }
    }
  }
 else
  {
   print ("Cannot define macro \"%s\" - name clash\n", name_p);
   error = TRUE;
  }

 return (error);
}

/* ========================================================================
   this routine assumes a string is passed which contains only a
   potential command name followed by a set of parameters. If the first
   token on the line is not a valid command, the routine returns TRUE. If
   it is, then the defined action routine for that command is invoked and the
   result of the execution passed back to the caller
   ======================================================================== */

int execute_command_line(char *line_p,char *target_p,int * result_p)
{
 symtab_t *command_p;
 parse_t   pars;
 int      error;

 error=FALSE;
 init_pars(&pars,line_p);
 get_tok(&pars,delim_set);
 command_p=look_for(pars.token,COM_SYMBOL);
 if (command_p!=(symtab_t *)NULL)
  {
   *result_p=command_p->value.com_val(&pars,target_p);
  }
 else
  {
   error=TRUE;
  }

 if(return_result==TRUE)
  {
   if (strlen(target_p)!=0)
    {
     int int_val;
     if (evaluate_integer(target_p,&int_val,number_base)==FALSE)
      {
       assign_integer(target_p,(error==TRUE?TRUE:*result_p),FALSE);
      }
    }
  }

 return(error);
}

/* ========================================================================
   this routine assumes a string is passed which contains only a
   potential macro name followed by a set of actual parameters.
   If the first token on the line is not a current macro, the routine returns
   TRUE. If it is, then the body of the macro is executed and the
   result of the execution passed back to the caller. If the macro attempts
   to pass back a value, the expression is evaluated into the target symbol
   name
   ======================================================================== */

int execute_macro(char *line_p,char *target_p,int *result_p)
{
 symtab_t       *macro_p;
 parse_t         formal_pars, actual_pars;
 int            error = FALSE;
 int            not_done = FALSE;
 char            rtn_expression[MAX_TOK_LEN];
 int             int_val;
 double          real_val;
 char            string_val[MAX_TOK_LEN];
 short             type=0;

 rtn_expression[0] = '\0';
 init_pars (&actual_pars, line_p);
 get_tok (&actual_pars, delim_set);
 macro_p = look_for (actual_pars.token, MAC_SYMBOL);
 if (macro_p != (symtab_t *) NULL)
  {
   /* define the formal parameters with values of actuals. look over the

      length of the invocation line and pick up the actual parameters
      determine their type and create an equivalent symbolic variable
      under the name of the formal parameter at the same position.
      formal parameters with no actual values are given default types
      and values. */

   macro_depth++;
   if (cur_echo)
    print ("Executing macro \"%s\"\n", macro_p->name_p);
   init_pars (&formal_pars, macro_p->value.mac_val->line);
   get_tok (&formal_pars, delim_set);
   while ((formal_pars.tok_len > 0) && !error)
    {
     get_tok (&actual_pars, delim_set);
     if (actual_pars.tok_len > 0)
      {
       not_done = evaluate_assign (formal_pars.token, actual_pars.token, &error, TRUE);
      }
     else
      not_done = evaluate_assign (formal_pars.token, "0", &error, TRUE);
     get_tok (&formal_pars, delim_set);
    }
   if (error || not_done)
    tag_current_line (&actual_pars,
                              "Failed to assign actual value to formal parameter");
   else
    {
     /* if all this parameterisation worked out OK, then actually
        execute the macro code and return the result. If an an
        expression was found at the end of the macro we must evaluate
        it into a temporary variable in the scope of the macro and
        then reassign it to the target variable in the enclosing scope
      */

     *result_p = command_loop (macro_p->value.mac_val->line_p,
                  (void *) NULL, rtn_expression, cur_echo);
     if (strlen (rtn_expression) != 0)
      {
       *result_p = evaluate_integer (rtn_expression, &int_val, number_base);
       if (!*result_p)
        type = INT_SYMBOL;
       else
        {
         *result_p = evaluate_float (rtn_expression, &real_val);
         if (!*result_p)
          type = FLT_SYMBOL;
         else
          {
           *result_p = evaluate_string (rtn_expression, string_val, MAX_TOK_LEN);
           if (!*result_p)
            type = STR_SYMBOL;
           else
            type = 0;
           }
        }
      }
    }
   macro_depth--;
   purge_symbols (macro_depth);
   /* now we are back within original scope, try to evaluate and assign
      return expression, using temporary variable to give value */
   if ((strlen (rtn_expression) != 0) && (type != 0))
    {
     switch (type)
      {
       case INT_SYMBOL:
        *result_p = assign_integer (target_p, int_val, FALSE);
       break;
       case FLT_SYMBOL:
        *result_p = assign_float (target_p, real_val, FALSE);
       break;
       case STR_SYMBOL:
        *result_p = assign_string (target_p, string_val, FALSE);
       break;
      }
    }
  }
 else
  error = TRUE;

 return (error);
}

/* ========================================================================
   performs the actions of displaying constants and symbols. If a
   line of parameters is present, each is interpreted and displayed. Other
   the whole current list of symbols is displayed.
   ======================================================================== */

int do_show(parse_t * pars_p,char *result_p)
{
 int            error = FALSE;
 symtab_t       *symbol_p;
 short             cnt = 0;
 macro_t        *line_p;

 if (get_tok (pars_p, delim_set) == 0)
  {                           /* no items on a line */
   print ("Currently defined symbols:\n\n");
   /* print symbol table info */
   while (cnt < symbol_cnt)
    {
     symbol_p = symtab_p[cnt++];
     switch (symbol_p->type)
      {
       case INT_SYMBOL:
        print ("%2d: %s: ", symbol_p->depth, symbol_p->name_p);
        print ("%ld - %s\n", symbol_p->value.int_val,
                     symbol_p->info_p);
       break;
       case FLT_SYMBOL:
        print ("%2d: %s: ", symbol_p->depth, symbol_p->name_p);
        print ("%G - %s\n", symbol_p->value.flt_val,
                   symbol_p->info_p);
       break;
       case STR_SYMBOL:
        print ("%2d: %s: ", symbol_p->depth, symbol_p->name_p);
        print ("\"%s\" - %s\n", symbol_p->value.str_val,
                   symbol_p->info_p);
       break;
       case MAC_SYMBOL:
        print ("%2d: %s: ", symbol_p->depth, symbol_p->name_p);
        print ("(%s) - %s\n", symbol_p->value.mac_val->line,
                 symbol_p->info_p);
       break;
      }
    }
  }
 else
  {
   symbol_p = look_for (pars_p->token, ANY_SYMBOL);
   if (symbol_p == NULL)
    tag_current_line (pars_p, "unrecognised symbol name");
   else
    {
     switch (symbol_p->type)
      {
       case INT_SYMBOL:
        print ("%d: %s: ", symbol_p->depth, symbol_p->name_p);
        print ("%ld (#%x) - %s\n", symbol_p->value.int_val,
               symbol_p->value.int_val,
               symbol_p->info_p);
       break;
       case FLT_SYMBOL:
        print ("%d: %s: ", symbol_p->depth, symbol_p->name_p);
        print ("%G - %s\n", symbol_p->value.flt_val,
               symbol_p->info_p);
       break;
       case STR_SYMBOL:
        print ("%d: %s: ", symbol_p->depth, symbol_p->name_p);
        print ("\"%s\" - %s\n", symbol_p->value.str_val,
               symbol_p->info_p);
       break;
       case COM_SYMBOL:
        print ("%d: %s: ", symbol_p->depth, symbol_p->name_p);
        print ("is a command\n");
       break;
       case MAC_SYMBOL:
        print ("%d: %s ", symbol_p->depth, symbol_p->name_p);
        print ("(%s) - %s\n", symbol_p->value.mac_val->line,
               symbol_p->info_p);
        /* print macro contents */
        line_p = symbol_p->value.mac_val->line_p;
        while (line_p != NULL)
        {
         print (">   %s\n", line_p->line);
         line_p = line_p->line_p;
        }
       break;
       default:
        print ("%d: %s has been deleted\n", symbol_p->depth,
                           symbol_p->name_p);
       break;
      }
    }
  }

 return (error);
}

/* ========================================================================
   Update global variable to set how many characters match is needed
   to call a variable match.
   ======================================================================== */

int do_config_testtool(parse_t *pars_p,char *result_p)
{
 int error;
 error=cget_integer(pars_p,2,&match_len);
 if ((error)||((match_len>=MAX_TOK_LEN)))
  {
   tag_current_line(pars_p, "Match length cannot be greater than MAX_TOK_LEN(512)");
   error = TRUE;
  }
 error=cget_integer(pars_p,TRUE,&return_result);
 if (error)
  {
   tag_current_line(pars_p,"return result can be TRUE/FALSE");
   error = TRUE;
  }
 error=cget_integer(pars_p,TRUE,&ignore_error_if_macro);
 if (error)
  {
   tag_current_line(pars_p,"ignore_error_if_macro can be TRUE/FALSE");
   error = TRUE;
  }

 return(error);
}

/* ========================================================================
   perform a fomatting action on a list of variables or values by
   invoking the sprintf routine
   ======================================================================== */

int do_print(parse_t *pars_p,char *result_p)
{
 int            error = FALSE;
 short             cnt;
 int             int_val;
 double          real_val;
 char            str_val[MAX_TOK_LEN];
 char            str_buf[MAX_LINE_LEN];

 str_buf[0] = '\0';
 cnt = 0;
 while ((pars_p->tok_del != '\0') && (strlen (str_buf) < 200))
  {
   get_tok (pars_p, delim_set);
   error = evaluate_integer (pars_p->token, &int_val, number_base);
   if (!error)
    {
     switch (number_base)
      {
       case 2:
        {
         int             i;

         str_val[0] = '$';
         str_val[33] = '\0';
         for (i = 31; i >= 0; i--)
          if (((int_val >> i) & 0x1) > 0)
           str_val[32 - i] = '1';
          else
           str_val[32 - i] = '0';
        }
       break;
       case 8:
        snprintf (str_val, MAX_TOK_LEN, "O%o", int_val);
       break;
       case 10:
        snprintf (str_val, MAX_TOK_LEN, "%d", int_val);
       break;
       case 16:
        snprintf (str_val, MAX_TOK_LEN, "#%x", int_val);
       break;
       default:
        error = TRUE;
        snprintf (str_val, MAX_TOK_LEN, "Invalid number base");
       break;
      }
     strcat (str_buf, str_val);
    }
   else
    {
     error = evaluate_float (pars_p->token, &real_val);
     if (!error)
      {
       sprintf (str_val, "%G", real_val);
       strcat (str_buf, str_val);
      }
     else
      {
       error = evaluate_string (pars_p->token, str_val, MAX_TOK_LEN);
       if (!error)
        strcat (str_buf, str_val);
       else
        {
         if (pars_p->tok_len > 0)
          strcat (str_buf, "******");
         else
          error = FALSE;
        }
      }
    }
   cnt++;
  }

 print ("%s\n", str_buf);
 return (error || assign_string (result_p, str_buf, FALSE));
}

/* ========================================================================
   Allows display of help information attached to each command or macro
   currently defined. If a parameter is present, the display is limited to
   help for that command. Otherwise a list is printed.
   ======================================================================== */

int do_help(parse_t *pars_p,char *result_p)
{
 int            error = FALSE;
 symtab_t       *symbol_p;
 short             cnt = 0;

 if (get_tok (pars_p, delim_set) == 0)
  {                           /* no items on a line */
   print ("The following %d commands are defined:\n\n", command_cnt);
   while (cnt < symbol_cnt)
    {
     symbol_p = symtab_p[cnt++];
     switch (symbol_p->type)
      {
       case COM_SYMBOL:
        print ("%-20s - %s\n", symbol_p->name_p, symbol_p->info_p);
       break;
       default:
        break;
      }
    }

   cnt=0;  /* Reset the counter */
   print ("\n\nThe following %d macros are defined:\n", macro_cnt);
   while (cnt < symbol_cnt)
    {
     symbol_p = symtab_p[cnt++];
     switch (symbol_p->type)
      {
       case MAC_SYMBOL:
        print ("%s ", symbol_p->name_p);
        print ("(%s) - %s\n", symbol_p->value.mac_val->line, symbol_p->info_p);
       break;
       default:
        break;
      }
    }
   print ("\n");
   print ("Command and macros can be used as statements or part of expressions,\n");
   print ("if appropriate, where the assignment is made to a symbol whose value\n");
   print ("is either defined or modified by this assignment. Generalized assignments\n");
   print ("can be made to symbols of integer, floating point or string types,\n");
   print ("including evaluation of arbitrary arithmetic expressions.\n\n");
   print ("Commands and assignments can be combined into parameterized macros\n");
   print ("that use FOR, IF, ELSE and WHILE constructs. Using the DEFINE\n");
   print ("command these macros can be set up for later invocation. All blocks\n");
   print ("within these macro constructs are terminated using the END statement.\n\n");
  }
 else
  {                           /* help XXX */
   symbol_p = look_for (pars_p->token, (MAC_SYMBOL | COM_SYMBOL));
   if (symbol_p != (symtab_t *) NULL)
    {
     switch (symbol_p->type)
      {
       case COM_SYMBOL:
        print ("%-12s - %s\n", symbol_p->name_p, symbol_p->info_p);
       break;
       case MAC_SYMBOL:
        print ("%s ", symbol_p->name_p);
        print ("(%s) - %s\n", symbol_p->value.mac_val->line, symbol_p->info_p);
       break;
       default:
       break;
      }                   /* end of switch */
    }                       /* end of if found one */
   else
    {
     tag_current_line (pars_p, "unrecognised command or macro");
     error = TRUE;
    }
  }                           /* end of help XXX */

 return (error || assign_string (result_p, "", FALSE));
}

/* ========================================================================
   alternate version of help : lists all the function and macros that are matching
   ======================================================================== */

int do_list(parse_t *pars_p,char *result_p)
{
 int            error = FALSE;
 symtab_t       *symbol_p;
 short             cnt = 0;

 if (get_tok (pars_p, delim_set) != 0)
  {
   print ("The following commands and macros are defined:\n");
   while (cnt < symbol_cnt)
    {
     symbol_p = symtab_p[cnt++];
     /* protect against deleted symbols */
     if (symbol_p->name_p != NULL)
      {
       if (is_matched (pars_p->token, symbol_p->name_p, 1) &&
          ((symbol_p->type | (MAC_SYMBOL | COM_SYMBOL)) != 0))
        switch (symbol_p->type)
         {
          case COM_SYMBOL:
           print ("%-20s - %s\n", symbol_p->name_p, symbol_p->info_p);
          break;
          case MAC_SYMBOL:
           print ("%s ", symbol_p->name_p);
           print ("(%s) - %s\n", symbol_p->value.mac_val->line, symbol_p->info_p);
          break;
          default:
          break;
         }           /* end of switch case */
      }                   /* end of symbol was not deleted */
    }                       /* end of while all list */
  }                           /* end of there was one */

 return (error || assign_string (result_p, "", FALSE));
}

/* ========================================================================
   performs the action of deleting symbols and macros. If
   line of parameters is present, each is interpreted
   and, if possible, deleted.
   ======================================================================== */

int do_delete(parse_t *pars_p,char *result_p)
{
 int   error = FALSE;
 symtab_t  *symbol_p;
 char *pt;
 int cnt, cnt_matches;

 cnt = 0;
 if (get_tok(pars_p, delim_set) == 0)
  {  /* no items on a line */
   tag_current_line(pars_p, "Expected symbol or macro name");
   error = TRUE;
  }
 else
  {
   pt = strchr( pars_p->token, '*');
   if ( pt != NULL && pt > pars_p->token )
    {
     cnt = 1;
     cnt_matches = 0;
     while ( cnt <= symbol_cnt )
      {
       symbol_p = symtab_p[symbol_cnt - cnt];
       if (symbol_p->name_p != NULL) /* not yet deleted symbol */
        {
         if ( strncmp( symbol_p->name_p, pars_p->token, (pt-pars_p->token) )==0 )
          {
           print("[%s] ", symbol_p->name_p);
           error = delete_symbol(symbol_p->name_p);
           if (error)
            {
             print("Cannot delete symbol\n");
            }
           else
            {
             print("deleted\n");
             /* symtab_p has been defragmented by delete_symbol, so re-start scanning */
             cnt = 0;
            }
           cnt_matches++;
          }
        }
       cnt++;
      } /* end while */
     if (cnt_matches == 0)
      {
       print("nothing to delete\n");
      }
    } /* end if pt not null */
   else
    {
     while ((pars_p->tok_len > 0) && !error)
      {
       symbol_p = look_for(pars_p->token, ANY_SYMBOL);
       if (symbol_p == (symtab_t*)NULL)
        {
         tag_current_line(pars_p, "Unrecognised symbol");
         error = TRUE;
        }
       else if (symbol_p->fixed || (symbol_p->type == COM_SYMBOL))
        {
         tag_current_line(pars_p, "Cannot delete fixed symbol or command");
         error = TRUE;
        }
       else
        {
         error = delete_symbol(pars_p->token);
         if (error)
          {
           tag_current_line(pars_p, "Cannot delete symbol out of current scope");
          }
         get_tok(pars_p, delim_set);
        }
      }
    }
  } /* end else */

 return(error || assign_string(result_p, "", FALSE));
}

/* ========================================================================
   toggles the echo of input lines
   ======================================================================== */

int do_verify(parse_t *pars_p,char *result_p)
{
 int            error;
 int             echo;

 error = cget_integer (pars_p, cur_echo, &echo);
 if (error)
  tag_current_line (pars_p, "expected command echo flag");
 else
  {
   cur_echo = (int) echo;
   if (cur_echo)
       print ("Command echo is enabled\n");
   else
       print ("Command echo is disabled\n");
  }
 return (error || assign_integer (result_p, echo, FALSE));
}

/* ========================================================================
   performs the action of defining the default number base
   ======================================================================== */

int do_base(parse_t *pars_p,char *result_p)
{
 int            error;
 int             base;
 int             old_base;

 old_base = number_base;
 error = cget_integer (pars_p, number_base, &base);
 if (error ||
     ((base != 16) && (base != 10) && (base != 8) && (base != 2)))
  {
   tag_current_line (pars_p, "Illegal number base");
   error = TRUE;
  }
 else
  {
   number_base = (short) base;
   print ("Number base = %d\n", number_base);
  }

 return (error || assign_integer (result_p, old_base, FALSE));
}

/* ========================================================================
   controls the logging of input and output to a journal file
   ======================================================================== */
#if 0
int do_log(parse_t *pars_p,char *result_p)
{
 int            error = FALSE;
 char            filename[MAX_LINE_LEN];
 char            def_filename[MAX_LINE_LEN];
 int             flag;

 evaluate_string ("LOG_FILE", def_filename, sizeof(def_filename));
 if (cget_string (pars_p, def_filename, filename, sizeof(filename)) ||
     (strlen (filename) == 0))
  {
   tag_current_line (pars_p, "Illegal log filename");
   error = TRUE;
  }
 else
  {
   /* close existing file, if any */
   if (logfiled != NULL)
    {
     log_output = 0;
     SYS_FClose (logfiled);
     logfiled=NULL;
     print ("Closed log file\n");
     return(FALSE);
    }
   /* attempt to open new file for writing */
   logfiled = SYS_FOpen (filename, "ab");
   if (logfiled != NULL)
    {
     /* look for log output option */
     cget_integer (pars_p, 0, &flag);
     log_output = (int) flag;
     if (log_output)
      print ("Logging input and output to file \"%s\"\n", filename);
     else
      print ("Logging input to file \"%s\"\n", filename);
    }
   else
    print ("Cannot open file \"%s\" for output\n", filename);
  }

 return (error || assign_string (result_p, filename, FALSE));
}
#endif
/* ========================================================================
   performs the execution of a command input file
   ======================================================================== */
#if 0
int do_source(parse_t *pars_p,char *result_p)
{
 int            error = FALSE;
 void           *inputfd;
 char            filename[MAX_LINE_LEN];
 char            def_filename[MAX_LINE_LEN];
 int             flag;

 evaluate_string ("COMMAND_FILE", def_filename, sizeof(def_filename));
 if (cget_string (pars_p, def_filename, filename, sizeof(filename)) ||
 (strlen (filename) == 0))
  {
   tag_current_line (pars_p, "Illegal input filename");
   error = TRUE;
  }
 else
  {
   inputfd = SYS_FOpen (filename, "rb");
   if (inputfd != NULL)
    {
     print ("input from file \"%s\"\n", filename);
     /* look for echo option */
     cget_integer (pars_p, 0, &flag);
     command_loop ((macro_t *) NULL, inputfd, (char *) NULL, (int) flag);
     SYS_FClose (inputfd);
    }
   else
    print ("Cannot open file \"%s\" for input\n", filename);
  }
 return (error || assign_string (result_p, filename, FALSE));
}
#endif
/* ========================================================================
   Removes named commands from named file
   =========================================================================== */
#if 0
int do_unsource(parse_t *pars_p,char *result_p)
{
 int   error = FALSE;
 FILE      *inputfd;
 char      filename[FILENAME_MAX];
 char      def_filename[FILENAME_MAX];
 int      flag;

 evaluate_string("COMMAND_FILE", def_filename, FILENAME_MAX);
 if (cget_string(pars_p, def_filename, filename, FILENAME_MAX) ||
     (strlen(filename) == 0))
  {
   tag_current_line(pars_p, "Illegal input filename");
   error = TRUE;
  }
 else
  {
   inputfd = SYS_FOpen (filename, "rb");
   if (inputfd != NULL)
    {
     print("Deleting symbols from file \"%s\"\n",filename);
     /* look for echo option */
     error = cget_integer(pars_p, 0, &flag);
     if (error)
      {
       tag_current_line(pars_p, "expected echo option flag");
      }
     else
      {
       uncommand_loop((macro_t*)NULL, inputfd, (char*)NULL,(int) flag);
      }
     SYS_FClose(inputfd);
    }
   else
    {
     print("Cannot open file \"%s\" for input\n", filename);
    }
  }

 return(error || assign_string(result_p, filename, FALSE));
}
#endif
/* ========================================================================
   Performs the execution of the content of a string
   =========================================================================== */

int do_eval(parse_t *pars_p,char *result_p)
{
 int   error   = FALSE;
 macro_t EvalLine;
 macro_t EndLine;

 if (!cget_string(pars_p, "", EvalLine.line, MAX_LINE_LEN))
  {
   /* Create a line with END as a string */
   EndLine.line_p = NULL;
   strncpy(EndLine.line, "END", 4);
   /* Link it to the line to evaluate */
   EvalLine.line_p = &EndLine;
   /* Execute the new macro */
   error = command_loop(&EvalLine,(FILE*) NULL, NULL, FALSE);
  }

 if(error)
  {
   assign_integer(result_p, TRUE, FALSE);
  }
 else
  {
   assign_integer(result_p, FALSE, FALSE);
  }

 return(FALSE);
}

/* ========================================================================
   Allows user to save state of play, in terms of macros and variables
   by placing definitions in a file such that they can be 'sourced'
   during a future run of the tool
   ======================================================================== */
#if 0
int do_save(parse_t *pars_p,char *result_p)
{
 int            error = FALSE;
 void           *outputfd;
 char            filename[MAX_LINE_LEN];
 char            def_filename[MAX_LINE_LEN];
 char            buffer[MAX_LINE_LEN];
 int             flag;
 symtab_t       *symbol_p;
 short             cnt = 0;
 macro_t        *line_p;

 evaluate_string ("COMMAND_FILE", def_filename, sizeof(def_filename));
 if (cget_string (pars_p, def_filename, filename, sizeof(filename)) ||
     (strlen (filename) == 0))
  {
   tag_current_line (pars_p, "Illegal output filename");
   error = TRUE;
  }
 else
  {
   outputfd = SYS_FOpen (filename, "wb");
   if (outputfd == NULL)
    {
     print ("Cannot open file \"%s\" for output\n", filename);
     error = TRUE;
    }
   else
    {
     print ("saving to file \"%s\"\n", filename);
     /* look for constants option */
     cget_integer (pars_p, 0, &flag);
     /* look for appropriate symbols to save. flag indicates we should
        save constants as well as variables */
     while (cnt < symbol_cnt)
      {
       symbol_p = symtab_p[cnt++];
       switch (symbol_p->type)
        {
         case INT_SYMBOL:
         if (!(symbol_p->fixed) || flag)
          {
           if (symbol_p->value.int_val < 0)
            sprintf (buffer, "%s = %d\n",
                    symbol_p->name_p, symbol_p->value.int_val);
           else
            sprintf (buffer, "%s = +%d\n",
                     symbol_p->name_p, symbol_p->value.int_val);
           SYS_FWrite(buffer,1,strlen(buffer),outputfd);
          }
         break;
         case FLT_SYMBOL:
          if (!(symbol_p->fixed) || flag)
           {
            sprintf (buffer, "%s = %E\n",
                     symbol_p->name_p, symbol_p->value.flt_val);
            SYS_FWrite(buffer,1,strlen(buffer),outputfd);
           }
         break;
         case STR_SYMBOL:
          if (!(symbol_p->fixed) || flag)
           {
            sprintf (buffer, "%s = \"%s\"\n",
                               symbol_p->name_p, symbol_p->value.str_val);
            SYS_FWrite(buffer,1,strlen(buffer),outputfd);
           }
         break;
        case MAC_SYMBOL:
         sprintf (buffer, "DEFINE %s %s\n",
                  symbol_p->name_p, symbol_p->value.mac_val->line);
         SYS_FWrite(buffer,1,strlen(buffer),outputfd);
         /* print macro contents */
         line_p = symbol_p->value.mac_val->line_p;
         while (line_p != NULL)
          {
           sprintf (buffer, "  %s\n", line_p->line);
           SYS_FWrite(buffer,1,strlen(buffer),outputfd);
           line_p = line_p->line_p;
          }
         break;
              default:
                  break;
        }
      }
     SYS_FClose(outputfd);
    }
  }

    return (error);
}
#endif
/* ========================================================================
   allows definition of command macros as a control command in itself
   ======================================================================== */

int do_define(parse_t *pars_p)
{
 int            error = FALSE;
 char            name[MAX_TOK_LEN];

 if (get_tok (pars_p, delim_set) == 0)
  {                           /* attempt to find macro name */
    tag_current_line (pars_p, "macro name expected");
    error = TRUE;
  }
 else
  {
   strcpy (name, pars_p->token);
   get_tok (pars_p, "");   /* get the rest of the line */
   error = define_macro (pars_p->token, name);
  }
 return (error);
}

/* ========================================================================
   allows definition of flexible command repetition loop
   ======================================================================== */

int do_for(parse_t *pars_p)
{
 int            error = FALSE;
 int             first = 0;
 int             second = 0;
 int             step;
 int             i;
 char            variable[MAX_TOK_LEN];
 char            name[MAX_TOK_LEN];

 if (get_tok (pars_p, delim_set) == 0)
  {                           /* attempt to find parameters */
   tag_current_line (pars_p, "for loop variable name expected");
   error = TRUE;
  }
 else
  {
   strcpy (variable, pars_p->token);
   error = assign_integer (variable, 0, FALSE);
   if (error)
    tag_current_line (pars_p, "expected integer loop variable name");
   else
    {
     error = cget_integer (pars_p, 1, &first);
     if (error)
      tag_current_line (pars_p, "invalid loop start value");
     else
      {
       error = cget_integer (pars_p, 1, &second);
       if (error)
        tag_current_line (pars_p, "invalid loop end value");
       else
        {
         error = (cget_integer (pars_p, 1, &step) ||
                 ((first > second) && (step > 0)) ||
                 ((first < second) && (step < 0)) ||
                 (step == 0));
         if (error)
          tag_current_line (pars_p, "invalid or inconsistent loop step value");
         else
          {
           sprintf (name, "FOR%d", macro_depth);
           error = define_macro ("", name);
           if (!error)
            {
             if (first <= second)
              {
               for (i = first; (i <= second) && !error; i = i + step)
                {
                 assign_integer (variable, i, FALSE);
                 execute_macro (name, "", &error);
                }
              }
             else
              {
               for (i = first; (i >= second) && !error; i = i + step)
                {
                 assign_integer (variable, i, FALSE);
                  execute_macro (name, "", &error);
                }
              }
            }
           delete_symbol (name);
          }
        }
      }
    }
  }

 return (error);
}

/* ========================================================================
   allows definition of conditional command repetition loop
   ======================================================================== */

int do_while(parse_t *pars_p)
{
 int            error = FALSE;
 int            result = FALSE;
 char            name[MAX_TOK_LEN];
 char            condition[MAX_TOK_LEN];

 if (get_tok (pars_p, delim_set) == 0)
  {                           /* attempt to find parameters */
   tag_current_line (pars_p, "comparison expression expected");
   error = TRUE;
  }
 else
  {
   error = evaluate_comparison (pars_p->token, &result, number_base);
   if (error)
    tag_current_line (pars_p, "illegal comparison");
   else
    {
     sprintf (name, "WHILE%d", macro_depth);
     strcpy (condition, pars_p->token);
     error = define_macro ("", name);
     if (!error)
      {
       while (result && !error)
        {
         execute_macro (name, "", &error);
         evaluate_comparison (condition, &result, number_base);
        }
       delete_symbol (name);
      }
    }
  }
 return (error);
}

/* ========================================================================
   allows definition of conditional command execution, passing the result
   of the condition evaluation back to a caller for use by the 'else' clause
   ======================================================================== */

int do_if(parse_t *pars_p,int *if_taken_p)
{
 int            error = FALSE;
 char            name[MAX_TOK_LEN];
 char            condition[MAX_TOK_LEN];

 if (get_tok (pars_p, delim_set) == 0)
  {                           /* attempt to find parameters */
   tag_current_line (pars_p, "comparison expression expected");
   error = TRUE;
  }
 else
  {
   error = evaluate_comparison (pars_p->token, if_taken_p, number_base);
   if (error)
    tag_current_line (pars_p, "illegal comparison");
   else
    {
     sprintf (name, "IF%d", macro_depth);
     strcpy (condition, pars_p->token);
     error = define_macro ("", name);
     if (!error)
     {
      if (*if_taken_p)
       execute_macro (name, "", &error);
      delete_symbol (name);
     }
    }
  }

 return (error);
}

/* ========================================================================
   allows definition of conditional command execution, passing the result
   of the condition evaluation back to a caller for use by the 'else' clause
   =========================================================================== */

int do_elif(parse_t *parse_p,int *elif_taken_p,int if_taken)
{
 int   error = FALSE;
 char   name[CONTROL_NAME_LENGTH];

 if (get_tok(parse_p, delim_set) == 0)
  {  /* attempt to find parameters */
   tag_current_line(parse_p, "comparison expression expected");
   error = TRUE;
  }
 else
  {
   error = evaluate_comparison(parse_p->token, elif_taken_p, number_base);
   if (error)
    {
     tag_current_line(parse_p, "illegal comparison");
    }
   else
    {
     sprintf(name, "ELIF%d", ElifNameCount ++);
     error = define_macro("", name);
     if (!error)
      {
       if ( (*elif_taken_p) && (!if_taken))
        {
         execute_macro(name,"", &error);
        }
       delete_symbol(name);
      }
     ElifNameCount --;
    }
  }

 return(error);
}

/* ========================================================================
   allows definition of conditional command execution after
   evaluation of the 'if condition
   ======================================================================== */

int do_else(int if_taken)
{
 int            error = FALSE;
 char            name[MAX_TOK_LEN];

 sprintf (name, "ELSE%d", macro_depth);
 error = define_macro ("", name);
 if (!error)
  {
   if (!if_taken)
    execute_macro (name, "", &error);
   delete_symbol (name);
  }
 return (error);
}

/* ========================================================================
   fundamental command execution loop. Commands come either from
   a supplied string or from an input stream (file or TTY). The loop
   identifies the type of input statement and determines the action to take
   as a result of this. A range of simple execution control primitives have
   been supplied. Block structure is implemented using temporary macro-like
   structures.
   ======================================================================== */

int command_loop(macro_t * macro_p,void *file_p,char *rtn_exp_p,int echo)
{
 int     error = FALSE;
 int     exit = FALSE;
 int     not_done = TRUE;
 int     sav_echo;
 macro_t *sav_macro_p;
 FILE    *sav_stream_p;
 parse_t  *parse_p;
 short      control_const;
 int     if_flag; /* set if seen in the last line examined */
 int     if_taken, elif_taken;
 int      i;
 char     *target, *StringBuffer;

 parse_p = alloc_parse_table_slot(MAX_LINE_LEN);
 StringBuffer = alloc_string_table_slot(MAX_LINE_LEN);

 sav_echo = cur_echo;
 sav_macro_p = cur_macro_p;
 sav_stream_p = cur_stream_p;
 cur_echo = echo;
 cur_macro_p = macro_p;
 cur_stream_p = file_p;
 if_flag = FALSE;

 /* exit from loop on all command error except when interactive */
 while( (cli_running==TRUE) && !exit && (!error || ((macro_depth <= 1) && (cur_stream_p == stdin))))
  {
   /* read an input line and start parsing process. This nesting level
   will terminate if the end of the stream is reached or an error
   is detected. The outermost loop, generally interactive, will not
   terminate on error. */
   exit = read_input(StringBuffer, prompt_p);
   init_pars(parse_p, StringBuffer);
   get_tok(parse_p, CommandLineDelimiter);

   if (exit || is_matched(parse_p->token, "EXIT", 2)
        || is_matched(parse_p->token, "END", 2))
    {
     exit = TRUE;
    }
   else if (is_matched(parse_p->token, "QUIT", 2))
   {
    cli_running = FALSE;
    exit        = TRUE;
   }
   else if (parse_p->tok_len > 0)
    {
     /* proceed with examination of an input line. Broadly speaking
        these come in three flavours - assignment, statement and control. The
        key distiguishing features are a leading '=' separator or
        the first token being a member of a small set of reserved
        control words. In each executable
        case, an attempt is made to  execute the line in various
        forms before declaring an error. */
     if (parse_p->tok_del == ' ')
      {
       /* allow any amount of white space before '=' in assignment */
       i = parse_p->par_pos;
       while (StringBuffer[i] == ' ')
        {
         i++;
        }
       if (StringBuffer[i] == '=')
        {
         parse_p->tok_del = '=';
         parse_p->par_pos = i;
        }
      }
     if (parse_p->tok_del == '=')
      {
       if_flag = FALSE;
       target = alloc_token_table_slot(strlen(parse_p->token)+1);

       strncpy(target, parse_p->token, strlen(parse_p->token)+1);
       get_tok(parse_p, "");        /* tokenise rest of line */
       not_done = evaluate_assign(target, parse_p->token, &error, FALSE);
       if (not_done)
           /* attempt to execute a command or macro assignment*/
           not_done = execute_command_line(parse_p->token, target, &error);
       if (not_done)
           not_done = execute_macro(parse_p->token, target, &error);
       if (not_done)
           /* report a warning */
           tag_current_line(parse_p, "Unrecognised assignment statement");

       if(ignore_error_if_macro)
        error=0;

       if (error)
           /* report an error in a separate fashion */
           print("Execution of \"%s\" or assignment of \"%s\" failed\n", parse_p->token, target);
       free_token_table_slot(); /* target */
      }
     else if (is_control(parse_p->token, &control_const))
      {
       switch(control_const)
        {
         case DEFINE_CONST:
          error = do_define(parse_p);
          if_flag = FALSE;
         break;
         case IF_CONST:
          error = do_if(parse_p, &if_taken);
          if (!error)
          if_flag = TRUE;
         break;
         case ELIF_CONST:
          if (!if_flag)
           {
            error = TRUE;
            tag_current_line(parse_p, "ELIF not allowed without IF");
           }
          else
           {
            error = do_elif(parse_p, &elif_taken, if_taken);
            if_taken |= elif_taken;
            if (error)
            {
             if_flag = FALSE;
            }
          }
         break;
         case ELSE_CONST:
          if (!if_flag)
           {
            error = TRUE;
            tag_current_line(parse_p, "ELSE not allowed without IF");
           }
          else
           {
            error = do_else(if_taken);
            if_flag = FALSE;
           }
         break;
         case WHILE_CONST:
          error = do_while(parse_p);
          if_flag = FALSE;
         break;
         case FOR_CONST:
          error = do_for(parse_p);
          if_flag = FALSE;
         break;
         default:
          tag_current_line(parse_p, "Unable to understand control construct");
          error = TRUE;
         break;
        }
      }
     else
      {
       if_flag = FALSE;
       not_done = execute_command_line(StringBuffer, "", &error);
       if (not_done)
        not_done = execute_macro(StringBuffer, "", &error);
       if (not_done)
        {
         tag_current_line(parse_p, "Unrecognised command statement");
         error = TRUE;
        }
       if(ignore_error_if_macro)
         error=0;
      }
    }
  }

 /* if this is a normal termination of the loop via an END statement,
  look for an expression to use as a result. As usual, the type depends
  on the context of the expression, and this is evaluated in the
  scope of the enclosing macro invocation */
 if (!error)
  {
   if ((get_tok(parse_p, delim_set) != 0) && (rtn_exp_p != (char*)NULL))
    {
     strcpy(rtn_exp_p, parse_p->token);
    }
   }

 /* restore original input information */
 cur_echo = sav_echo;
 cur_macro_p = sav_macro_p;
 cur_stream_p = sav_stream_p;

 free_string_table_slot();
 free_parse_table_slot();
 return(error);
}

/* ========================================================================

=========================================================================== */

int uncommand_loop(macro_t *macro_p,void *file_p, char *rtn_exp_p, int echo)
{
 int     error = FALSE;
 int     exit = FALSE;
 int     sav_echo;
 macro_t *sav_macro_p;
 FILE    *sav_stream_p;
 parse_t  *parse_p;
 short      control_const;
 char     *StringBuffer;

 parse_p = alloc_parse_table_slot(MAX_LINE_LEN);
 StringBuffer = alloc_string_table_slot(MAX_LINE_LEN);

 sav_echo = cur_echo;
 sav_macro_p = cur_macro_p;
 sav_stream_p = cur_stream_p;
 cur_echo = echo;
 cur_macro_p = macro_p;
 cur_stream_p = file_p;

 /* exit from loop on all command error except when interactive */
 while( !exit && (!error || ((macro_depth <= 1) && (cur_stream_p == stdin))))
  {
   /* read an input line and start parsing process. This nesting level
   will terminate if the end of the stream is reached or an error
   is detected. The outermost loop, generally interactive, will not
   terminate on error. */
   exit = read_input(StringBuffer, prompt_p);
   init_pars(parse_p, StringBuffer);
   get_tok(parse_p, CommandLineDelimiter);

   if (exit)
    {
     exit = TRUE;
    }
   else if (parse_p->tok_len > 0)
    {
     if (is_control(parse_p->token, &control_const))
      {
       switch(control_const)
        {
         case DEFINE_CONST:
          get_tok(parse_p, CommandLineDelimiter);
          error = delete_symbol(parse_p->token);
          if (error)
          {
           tag_current_line(parse_p, "Cannot delete symbol out of current scope");
          }
         break;
         case IF_CONST:
         case ELIF_CONST:
         case ELSE_CONST:
         case WHILE_CONST:
         case FOR_CONST:
         break;
         default:
          tag_current_line(parse_p, "Unable to understand control construct");
          error = TRUE;
         break;
        }
      }
    }
  }

 /* if this is a normal termination of the loop via an END statement,
    look for an expression to use as a result. As usual, the type depends
    on the context of the expression, and this is evaluated in the
    scope of the enclosing macro invocation */
 if (!error)
  {
   if ((get_tok(parse_p, delim_set) != 0) && (rtn_exp_p != (char*)NULL))
    {
     strcpy(rtn_exp_p, parse_p->token);
    }
  }
 /* restore original input information */
 cur_echo = sav_echo;
 cur_macro_p = sav_macro_p;
 cur_stream_p = sav_stream_p;

 free_string_table_slot();
 free_parse_table_slot();
 return(error);
}

/* ========================================================================
   Signal hander called under linux in case of sigint/sigterm/sigabort signals
   ======================================================================== */

void sighandler(int sig)
{
 //cli_running=FALSE;
}

/* ========================================================================
   Top level of the cli code, invokes intialisation and top level shell
   ======================================================================== */

void cli_init(int (*setup_r)(),int max_symbols,short default_base)
{
 macro_depth = 0;
 ElifNameCount = 0;
 number_base = default_base;
 init_sym_table (100 + max_symbols);
 init_string_table();
 init_token_table();
 init_parse_table();
 /* set up of all internal commands and constant values */
 register_command ("HELP", do_help, "<commandname> Displays help string for named commands and macros");
 register_command ("LIST", do_list, "<commandname or partial> Lists all strings of command and macro");
 register_command ("SHOW", do_show, "<symbolname> Displays symbol values and macro contents");
 register_command ("DELETE", do_delete, "<symbolnames> Removes named symbols or macros");
 register_command ("IOBASE", do_base, "<hex/decimal> Sets default I/O radix for input and output");
 register_command ("VERIFY", do_verify, "<flag> Sets echo of commands for macro execution");
 register_command ("PRINT", do_print, "Formats and prints variables");
 //register_command ("SOURCE", do_source, "<filestring><echoflag> Executes commands from named file");
 //register_command ("SAVE", do_save, "<filestring><constflag> Saves macros, var, consts to file");
 //register_command ("LOG", do_log, "<filestring><outputflag> Logs all command I/O to named file");
 //register_command("UNSOURCE", do_unsource,  "<filestring><echoflag> Removes named commands from named file");
 register_command("STATS", cli_statistics,  "<none> Display Symbol Statistics of Testtool");
 register_command("EVAL", do_eval,  "<commands> Executes the commands given as argument");
 register_command("GETSTRING", do_getstring, "Waits for a string and returns it");
 register_command("CONFIG_TESTTOOL", do_config_testtool,  "<num_char_to_match> <return_result> <ignore_error_in_macro> Configure testtool for required parameters");
 assign_integer ("HEXADECIMAL", 16, TRUE);
 assign_integer ("DECIMAL", 10, TRUE);
 assign_integer ("OCTAL", 8, TRUE);
 assign_integer ("BINARY", 2, TRUE);
 assign_integer ("TRUE", 1, TRUE);
 assign_integer ("FALSE", 0, TRUE);
 assign_float ("PI", 3.14156, TRUE);
 assign_string ("COMMAND_FILE", "default.com", FALSE);
 assign_string ("LOG_FILE", "default.log", FALSE);
 assign_string ("RED", "\033[31m", FALSE);
 assign_string ("BLUE", "\033[34m", FALSE);
 assign_string ("BLACK", "\033[0m", FALSE);
 assign_string ("CYAN", "\033[36m", FALSE);
 assign_string ("GREEN", "\033[32m", FALSE);
 assign_string ("PURPLE", "\033[35m", FALSE);
 assign_string ("BROWN", "\033[33m", FALSE);
 assign_string ("GREY", "\033[37m", FALSE);
 assign_string ("REDONBLUE", "\033[44;1;31m", FALSE);
 /* call user defined setup for commands and symbols */
 (*setup_r) ();
 macro_depth++;              /* increase nesting depth for initial input */
 logfiled    = NULL;         /* initialise log file                      */
 cli_running = TRUE;         /* cli is running                           */
 signal(SIGINT,&sighandler);
 signal(SIGTSTP,&sighandler);
}

/* ========================================================================
   Delete all the symbols
   ======================================================================== */

int cli_delete(void)
{
 symtab_t *symbol_p;
 short       cnt=0;
 int      Error;
 while(cnt<symbol_cnt)
  {
   symbol_p = symtab_p[cnt++];
   Error=delete_symbol(symbol_p->name_p);
   if (Error)
    {
     printf("Failed to delete %s !\n",symbol_p->name_p);
    }
  }
 return(FALSE);
}

/* ========================================================================
   top level of the cli code, invokes top level shell
   ======================================================================== */

void cli_run(char *ip_prompt_p, char *file_p)
{
	prompt_p = ip_prompt_p;
	while (cli_running==TRUE)
	{
		print ("\nPlease press a key... \n");
		readkey ();
		file_p = NULL;          /* Disable for later loops */
		command_loop ((macro_t *) NULL, stdin, (char *) NULL, FALSE);
	}

	//exit(0);
}
