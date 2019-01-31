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
typedef struct MENUNODE_OBJ MENUNODE_OBJ;

typedef void *(pMENU_CALLBACK)(void *p_arg);
typedef void MENU_CALLBACK(WM_MESSAGE *p_arg);

/* 菜单结构体定义 */
struct MENU_OBJ
{
	GUI_RECT rect;		/*  菜单的位置以及大小 */
	uint16_t numOfMenus; /* 一共有多少个菜单 */
	int8_t selNo;
	
	MENUNODE_OBJ *selMenu; /* 当前选中的菜单 */
	MENUNODE_OBJ *headMenu; /* 菜单头节点 */
};


struct MENUNODE_OBJ
{
	bool isSelect;
	uint8_t *text;
	
	MENUNODE_OBJ *preNode;
	MENUNODE_OBJ *lstNode;
	MENU_CALLBACK *_cb;
};

MENU_Handle menu_Create(uint16_t x0, uint16_t y0, uint16_t width, uint8_t id, WIN_Handle hParent);
GUI_ERROR menu_Delete(MENU_Handle *hMenu);
GUI_ERROR menu_insertItem(MENU_Handle hMenu, uint8_t *text, MENU_CALLBACK *_cb);
GUI_ERROR menu_deleteItem(MENU_Handle hMenu, uint8_t itemId);
void menu_onPaint(MENU_OBJ* pMenu);

# endif

/********************************************  END OF FILE  *******************************************/
