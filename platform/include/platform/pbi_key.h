/*
 * lcd_key.h
 *
 *  Created on: Mar 21, 2016
 *      Author: qqtian
 */

#ifndef LCD_KEY_H_
#define LCD_KEY_H_

/*keys definition*/
#define KEY_TIMEOUT         0xa0
#define DOWNKEY				0xa1
#define UPKEY				0xa2
#define LEFTKEY				0xa3
#define RIGHTKEY			0xa4
#define ENTERKEY			0xa5
#define EXITKEY				0xa6

#define KEY_DEBUG

void key_init(void);
int Get_Key_Value(void);
int	Get_Key_Value_Timeout(int iCount);

#endif /* LCD_KEY_H_ */
