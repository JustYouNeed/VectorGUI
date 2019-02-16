/**
  *******************************************************************************************************
  * File Name: gui_core.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-30
  * Brief: GUI�ں��ļ�
  *******************************************************************************************************
  * History
  *   1.Author: Vector
  *		Date: 2018-12-30
  *		Mod: �����ļ�
  *
  *******************************************************************************************************
  */
# ifndef __GUI_CORE_H
# define __GUI_CORE_H 

# include "gui_com.h"
# include "gui_font.h"

/* GUI�����Ľṹ�壬����GUI��Ϣ */
typedef struct
{
	uint16_t drawPosX, drawPosY;		/* ��ͼ�� */
	uint16_t dispPosX, dispPosY;		/* ��ʾ�� */
	
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

