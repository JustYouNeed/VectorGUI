/**
  *******************************************************************************************************
  * File Name: gui_key.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2019-2-1
  * Brief: GUI的按键输入部分
  *******************************************************************************************************
  * History
	*		1.Author: Vector
	*			Date: 2019-2-1
	*			Mod: 建立文件
  *
  *******************************************************************************************************
  */
# ifndef __GUI_KEY_H_
# define __GUI_KEY_H_

# include "gui_com.h"

typedef enum GUI_KEY_ENUM	GUI_KEY_ENUM;
typedef struct GUI_KEY_INFO GUI_KEY_INFO;


struct GUI_KEY_INFO
{
	uint16_t keyValue;
	uint8_t keyStatus;
};

typedef enum
{
	GUI_KEY_PRESS = 0x01,
	GUI_KEY_RELEASED = 0x02,
}GUI_KEYSTATUS_ENUM;

/* 输入到GUI的按键消息 */
enum GUI_KEY_ENUM
{
	GUI_KEY_NULL = 0x00,
	GUI_KEY_UP = 0x01 << 0,
	GUI_KEY_DOWN = 0x01 << 1,
	GUI_KEY_LEFT = 0x01 << 2,
	GUI_KEY_RIGHT = 0x01 << 3,
	GUI_KEY_OK = 0x01 << 4,
	GUI_KEY_CANCEL = 0x01 << 5,
	GUI_KEY_DELETE = 0x01 << 6,
	GUI_KEY_PWR = 0x01 << 6,
	
	GUI_KEY_0 = 0x01 << 8,
	GUI_KEY_1 = 0x01 << 9,
	GUI_KEY_2 = 0x01 << 10,
	GUI_KEY_3 = 0x01 << 11,
	GUI_KEY_4 = 0x01 << 12,
	GUI_KEY_5 = 0x01 << 13,
	GUI_KEY_6 = 0x01 << 14,
	GUI_KEY_7 = 0x01 << 15,
	GUI_KEY_8 = 0x01 << 16,
	GUI_KEY_9 = 0x01 << 17,
};

void gui_inputKey(uint32_t keyValue);
void gui_onKey(void);

# endif
/********************************************  END OF FILE  *******************************************/

