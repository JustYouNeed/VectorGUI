/**
  *******************************************************************************************************
  * File Name: gui_menu.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: GUI�˵��ؼ������ֺ����ο�����ԭ��ң��GUI�еĳ���ԭ����Ϊ���ܱ�
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-30
	*			Mod: �����ļ�
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

/* �˵��ṹ�嶨�� */
struct MENU_OBJ
{
	GUI_RECT rect;		/*  �˵���λ���Լ���С */
	uint16_t numOfMenus; /* һ���ж��ٸ��˵� */
	int8_t selNo;
	
	MENUNODE_OBJ *selMenu; /* ��ǰѡ�еĲ˵� */
	MENUNODE_OBJ *headMenu; /* �˵�ͷ�ڵ� */
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
