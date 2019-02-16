/**
  *******************************************************************************************************
  * File Name: gui_text.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: 文字显示功能
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-29
	*			Mod: 建立文件
	*
  *******************************************************************************************************
  */
# ifndef __GUI_TEXT_H
# define __GUI_TEXT_H

# include "gui_com.h"
# include "gui_font.h"

void gui_setFont(GUI_FONT font);

uint16_t gui_getStringLen(const uint8_t *str);

void gui_dispChar(uint8_t c, uint8_t mode);
void gui_dispCharAt(uint8_t c, uint16_t x0, uint16_t y0, uint8_t mode);

void gui_dispString(uint8_t *str, uint8_t mode);
void gui_dispStringAt(uint8_t *str, uint16_t x0, uint16_t y0, uint8_t mode);


# endif

/********************************************  END OF FILE  *******************************************/

