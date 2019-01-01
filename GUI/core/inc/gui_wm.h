/**
  *******************************************************************************************************
  * File Name: gui_wm.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-30
  * Brief: GUI窗口管理部分
  *******************************************************************************************************
  * History
  *		1.Author: Vetor
	*			Date: 2018-12-24
	*			Mod: 建立文件
  *******************************************************************************************************
  */		
# ifndef __GUI_WM_H
# define __GUI_WM_H

void wm_Init(void);

GUI_ERROR wm_Delete(WM_HWIN *hWin);
WM_HWIN	wm_Create(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t status);

GUI_ERROR wm_getWindowObject(WM_HWIN hWin, WM_OBJ *pObj);

void wm_pocess(void);


# endif


/********************************************  END OF FILE  *******************************************/

