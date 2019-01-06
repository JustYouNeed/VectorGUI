/**
  *******************************************************************************************************
  * File Name: gui_window.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: GUI窗口控件，部分函数参考正点原子遥控GUI中的程序，原作者为黄绍斌
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-30
	*			Mod: 建立文件
	*
  *******************************************************************************************************
  */	
# ifndef __GUI_WINDOW_H
# define __GUI_WINDOW_H

# define WINDOW_Handle	int16_t
typedef struct WINDOW_OBJ WINDOW_OBJ;

/* window控件结构体 */
struct WINDOW_OBJ
{
	GUI_RECT rect;
	uint8_t *title;
	uint8_t status;
};

void window_onPaint(const WINDOW_OBJ *pWindow);

WINDOW_Handle window_Create(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint8_t *title, uint16_t id, WM_HWIN hParent);


# endif

/********************************************  END OF FILE  *******************************************/

