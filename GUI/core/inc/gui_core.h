/**
  *******************************************************************************************************
  * File Name: gui_core.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-30
  * Brief: GUI内核文件
  *******************************************************************************************************
  * History
  *   1.Author: Vector
  *		Date: 2018-12-30
  *		Mod: 建立文件
  *
  *******************************************************************************************************
  */
# ifndef __GUI_CORE_H
# define __GUI_CORE_H 

# include "gui_com.h"
# include "gui_font.h"

/* GUI上下文结构体，保存GUI信息 */
typedef struct
{
	uint16_t drawPosX, drawPosY;		/* 画图点 */
	uint16_t dispPosX, dispPosY;		/* 显示点 */
	
	GUI_FONT	curFont;
	GUI_RECT drawRect;
}GUI_CONTEXT;


void gui_Init(void);
void gui_onPaint(void);
void gui_clear(void);
void gui_excute(void);
void gui_gotoXY(uint16_t x, uint16_t y);

extern GUI_CONTEXT GUI_Context;
# endif
/********************************************  END OF FILE  *******************************************/

