/**
  *******************************************************************************************************
  * File Name: gui_font.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: GUI字体头文件
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-30
	*			Mod: 建立文件
	*
  *******************************************************************************************************
  */	
# ifndef __GUI_FONT_H
# define __GUI_FONT_H

/* 字体类型 */
typedef enum
{
	GUI_FONT_ASCII_6X8 = 0x0,
	GUI_FONT_ASCII_6X12,
	GUI_FONT_ASCII_12X24,
}GUI_FONT;


typedef struct 
{
	uint8_t XSize, YSize;
	uint8_t bytesPerChar;
	const unsigned char *pData;
}GUI_FONT_INFO;

uint8_t font_getXSize(GUI_FONT font);
uint8_t font_getYSize(GUI_FONT font);
uint8_t font_getBytesPerChar(GUI_FONT font);
uint8_t font_getFontData(GUI_FONT font, uint16_t no);

# endif

/********************************************  END OF FILE  *******************************************/

