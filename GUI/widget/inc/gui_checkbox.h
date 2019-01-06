/**
  *******************************************************************************************************
  * File Name: gui_checkbox.c
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
# ifndef __GUI_CHECKBOX_H
# define __GUI_CHECKBOX_H

# define CHECKBOX_Handle	int16_t
typedef struct CHECKBOX_OBJ CHECKBOX_OBJ;

struct CHECKBOX_OBJ
{
	GUI_RECT rect;
};


void checkbox_onPaint(const CHECKBOX_OBJ *pCheckbox);


# endif

/********************************************  END OF FILE  *******************************************/

