/**
  *******************************************************************************************************
  * File Name: gui_drv.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: GUI�ײ���ͷ�ļ�
  *******************************************************************************************************
  * History
  *   1.Author: Vetor
  *		Date: 2018-12-29
  *		Mod: �����ļ�
  *******************************************************************************************************
  */	
# ifndef __GUI_DRV_H
# define __GUI_DRV_H

void gui_deviceInit(void);
void gui_fillSCR(GUI_COLOR color);
void gui_clearSCR(void);

void gui_drawPoint(uint8_t x, uint8_t y, GUI_COLOR color);
uint8_t gui_readPoint(uint8_t x, uint8_t y);

void gui_Refresh(void);

# endif

/********************************************  END OF FILE  *******************************************/

