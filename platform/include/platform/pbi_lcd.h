/*
 * pbi_lcd.h
 *
 *  Created on: Mar 21, 2016
 *      Author: qqtian
 */

#ifndef PBI_LCD_H_
#define PBI_LCD_H_

/* Includes --------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************/
/* Public constants                                                           */
/******************************************************************************/


#define FIRST_LINE			0x01
#define SECOND_LINE			0x02

/******************************************************************************/
/* Public types                                                               */
/******************************************************************************/

/******************************************************************************/
/* Public macros                                                              */
/******************************************************************************/

/******************************************************************************/
/* Public functions prototypes                                                */
/******************************************************************************/
void Lcd_Init(void);
void Lcd_Display(int uiLine, char *ucS);
void Lcd_Clear(void) ;
void Lcd_Flash_Cursor(int iLine, int iLocation);
int LcdDisplayErrorAlarm(unsigned char *s, int t);
void Lcd_Display_All(char* first_line, char * second_line);
void Lcd_Backlight(int onoff);


#ifdef __cplusplus
}
#endif

#endif /* PBI_LCD_H_ */
