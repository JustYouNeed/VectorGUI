/**
  *******************************************************************************************************
  * File Name: gui_progbar.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: GUI进度条控件头文件，部分函数参考正点原子遥控GUI中的程序，原作者为黄绍斌
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-30
	*			Mod: 建立文件
	*
  *******************************************************************************************************
  */	
# ifndef __GUI_PROGBAR_H
# define __GUI_PROGBAR_H

# define PROGBAR_Handle	int16_t

typedef struct 
{
	GUI_RECT rect;
}PROGBAR_OBJ;

void progbar_onPaint(const PROGBAR_OBJ* pProgbar);

# endif

/********************************************  END OF FILE  *******************************************/
