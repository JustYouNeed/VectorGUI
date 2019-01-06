/**
  *******************************************************************************************************
  * File Name: gui_scrollbar.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: GUI滚动条控件头文件，部分函数参考正点原子遥控GUI中的程序，原作者为黄绍斌
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-30
	*			Mod: 建立文件
	*
  *******************************************************************************************************
  */
# ifndef __GUI_SCROLLBAR_H
# define __GUI_SCROLLBAR_H

# define SCROLLBAR_Handle	int16_t
typedef struct SCROLLBAR_OBJ SCROLLBAR_OBJ;

struct SCROLLBAR_OBJ
{
	GUI_RECT rect;
};

void scrollbar_onPaint(const SCROLLBAR_OBJ *pScrollbar);

# endif

/********************************************  END OF FILE  *******************************************/
