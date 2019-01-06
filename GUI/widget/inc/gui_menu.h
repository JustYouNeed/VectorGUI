/**
  *******************************************************************************************************
  * File Name: gui_menu.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: GUI菜单控件，部分函数参考正点原子遥控GUI中的程序，原作者为黄绍斌
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-30
	*			Mod: 建立文件
	*
  *******************************************************************************************************
  */
# ifndef __GUI_MENU_H
# define __GUI_MENU_H

# define MENU_Handle	uint16_t
typedef struct MENU_OBJ MENU_OBJ;


struct MENU_OBJ
{
	GUI_RECT rect;
};

void menu_onPaint(const MENU_OBJ* pMenu);

# endif

/********************************************  END OF FILE  *******************************************/
