/**
  *******************************************************************************************************
  * File Name: gui_menu.c
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
/*
  *******************************************************************************************************
  *                              INCLUDE FILES
  *******************************************************************************************************
*/
# include "gui.h"

/*
*********************************************************************************************************
*                          menu_getObject                
*
* Description: 获取菜单结构体
*             
* Arguments  : hMenu: 菜单句柄
*							 *err: 用于保存执行结果
*
* Reutrn     : 获取到的菜单结构体
*
* Note(s)    : None.
*********************************************************************************************************
*/
MENU_OBJ *menu_getObject(MENU_Handle hMenu, int16_t *err)
{
	if(!hMenu) return NULL;
	
	WIDGET_OBJ *pObj = NULL;
	
		/* 获取按钮控件 */
	pObj = widget_getWidget(hMenu, err);	
	if(!pObj) 
	{
		*err = ERR_PARA;
		return NULL;
	}
	
	return (MENU_OBJ *)(pObj->widgetData);
}

/*
*********************************************************************************************************
*                                          
*
* Description: 
*             
* Arguments  : 
*
* Reutrn     : 
*
* Note(s)    : 
*********************************************************************************************************
*/
static void menu_defaultProc(WM_MESSAGE *pMsg)
{
	MENU_OBJ *pMenu = NULL;
	MENUNODE_OBJ *pNode = NULL;
	WIDGET_OBJ *pWidget = NULL;
	MENU_Handle hMenu = 0;
	int16_t err = ERR_NONE;
	
	hMenu = pMsg->hWin;
	
	/* 得到按钮控件结构体 */
	pWidget = widget_getWidget(hMenu, &err); 
	if(err != ERR_NONE) return;
	pMenu = (MENU_OBJ *)(pWidget->widgetData);
	
	switch(pMsg->msgId)
	{
		case MSG_KEY:
		{
			/* 得到按键消息 */
			MSG_KEY_INFO *key = (MSG_KEY_INFO *)&(pMsg->key);
			
			/* 按键被按下 */
			if(KEY_PRESS == key->keyStatus)
			{
				/* 菜单上移 */
				if(key->keyValue == MSG_KEY_UP)
				{
					if(pMenu->selMenu->preNode) /* 在前面还有结点的时候往前移 */
					{
						pMenu->selMenu = pMenu->selMenu->preNode;
						pMenu->selNo --;
//						if(pMenu->selNo <= -5)
//						{
//							pMenu->selNo = 0;
//							pMenu->headMenu = pMenu->headMenu->preNode;
//						}
						win_Invalidation(pWidget->id >> 10); /* 立刻重绘一遍 */
						gui_onPaint();
					}
				}
				else if(key->keyValue == MSG_KEY_DOWN)
				{
					if(pMenu->selMenu->lstNode) /* 菜单下移 */
					{
						pMenu->selMenu = pMenu->selMenu->lstNode;
						pMenu->selNo ++;
//						if(pMenu->selNo >= 5)
//						{
//							pMenu->selNo = 0;
//							pMenu->headMenu = pMenu->headMenu->lstNode;
//						}
						win_Invalidation(pWidget->id >> 10);
						gui_onPaint();
					}
				}else if(key->keyValue == MSG_KEY_PWR) /* 执行回调 */
				{
					if(pMenu->selMenu->_cb)
						pMenu->selMenu->_cb(pMsg);
				}
			}
		}break;
		default: break;
	}
}

/*
*********************************************************************************************************
*                        menu_onPaint                  
*
* Description: 菜单绘制函数
*             
* Arguments  : *pMenu: 菜单结构体指针
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void menu_onPaint(MENU_OBJ* pMenu)
{
	MENUNODE_OBJ *pNode = pMenu->headMenu;
	if(!pNode) return ;
	uint16_t x = 30, y = 16;
	uint8_t i = 0;
	
	/* 如果超出一个屏幕所能显示的范围，则开始显示的结点下移 */
	if(pMenu->selNo >= 5) i = pMenu->selNo - 4;
	while(i)
	{
		pNode = pNode->lstNode;
		i --;
	}
	
	/* 顺序绘制整个项目链表 */
	while(pNode)
	{
		gui_gotoXY(x, y);
		
		/* 选中的项目显示方式不一样 */
		if(pMenu->selMenu == pNode)
			gui_dispString(pNode->text, 0);
		else
			gui_dispString(pNode->text, 1);
		
		pNode = pNode->lstNode;
		
		y += 10;
		if(y > LCD_Y) break; /* 超过屏幕的部分不需要显示 */
	}
}

/*
*********************************************************************************************************
*                          menu_Create                
*
* Description: 创建一个菜单控件
*             
* Arguments  : x0, y0, width: 菜单的位置以及宽度
*							 id: 菜单id
*							 hParent: 父窗口句柄
*
* Reutrn     : 成功返回菜单句柄，失败则返回原因
*
* Note(s)    : None.
*********************************************************************************************************
*/
MENU_Handle menu_Create(uint16_t x0, uint16_t y0, uint16_t width, uint8_t id, WIN_Handle hParent)
{
	int16_t err = ERR_NONE;
	MENU_OBJ *pMenu = NULL;
	
	if(!hParent) return ERR_PARA;
	
	pMenu = (MENU_OBJ *)bsp_mem_Alloc(SRAMIN, sizeof(MENU_OBJ));
	if(!pMenu) return ERR_MEM;
	
	/* 设置菜单的位置 */
	pMenu->rect.x0 = x0;
	pMenu->rect.y0 = y0;
	pMenu->rect.width = width;
	pMenu->rect.height = 0;
	
	pMenu->selMenu = NULL;
	pMenu->headMenu = NULL;
	
	/* 初始没有任何条目 */
	pMenu->numOfMenus = 0;
	pMenu->selNo = 0;
	
	/* 将菜单插入到控件列表中 */
	err = widget_Create(WIDGET_MENU, pMenu, id, MSG_KEY_UP | MSG_KEY_DOWN | MSG_KEY_PWR, menu_defaultProc, hParent);
	return err;
}

/*
*********************************************************************************************************
*                          menu_Delete                
*
* Description: 删除一个菜单控件
*             
* Arguments  : *hMenu: 要删除的菜单控件句柄指针
*
* Reutrn     : 执行结果
*
* Note(s)    : None.
*********************************************************************************************************
*/
GUI_ERROR menu_Delete(MENU_Handle *hMenu)
{
	if(!*hMenu) return ERR_PARA;
	
	int16_t err = ERR_NONE;
	MENUNODE_OBJ *preNode = NULL, *lstNode = NULL;
	MENU_OBJ *pMenu = NULL;
	
	pMenu = menu_getObject(*hMenu, &err);
	if(err != ERR_NONE) return err;
	
	preNode = pMenu->headMenu;
	lstNode = preNode->lstNode;
	
	while(preNode)
	{
		bsp_mem_Free(SRAMIN, preNode);
		preNode = preNode->lstNode;
	}
	
	return widget_Delete(*hMenu);
}

/*
*********************************************************************************************************
*                         menu_insertItem                 
*
* Description: 向菜单控件中插入一个结点
*             
* Arguments  : hMenu: 菜单句柄
*							 *text: 菜单结点文本
*							 *_cb: 该结点的回调函数
*
* Reutrn     : 执行结果
*
* Note(s)    : None.
*********************************************************************************************************
*/
GUI_ERROR menu_insertItem(MENU_Handle hMenu, uint8_t *text, MENU_CALLBACK *_cb)
{
	if(!hMenu) return ERR_PARA;
	
	int16_t err = ERR_NONE;
	MENUNODE_OBJ *pMenuNode = NULL;
	MENU_OBJ *pMenu = NULL;
	
	/* 为菜单结点申请内存 */
	pMenuNode = (MENUNODE_OBJ *)bsp_mem_Alloc(SRAMIN, sizeof(MENUNODE_OBJ));
	if(!pMenuNode) return ERR_MEM;
	
	pMenuNode->isSelect = false;
	pMenuNode->preNode = NULL;
	pMenuNode->lstNode = NULL;
	pMenuNode->text = text;
	pMenuNode->_cb = _cb;
	
	pMenu = menu_getObject(hMenu, &err);
	if(err != ERR_NONE) return err;
	
	pMenu->numOfMenus ++; /* 菜单项目增加 */
	
	/* 如果是第一个插入的菜单结点，则直接设置为结点头 */
	if(pMenu->headMenu == NULL)
	{
		pMenu->headMenu = pMenuNode;
		pMenu->selMenu = pMenuNode;
	}
	else /* 挂接到表尾 */
	{
		MENUNODE_OBJ *pNodeTemp = pMenu->headMenu;
		while(pNodeTemp->lstNode) /* 查找表尾 */
			pNodeTemp = pNodeTemp->lstNode;
		
		pNodeTemp->lstNode = pMenuNode;
		pMenuNode->preNode = pNodeTemp;
	}
	
	return ERR_NONE;
}

/*
*********************************************************************************************************
*                      menu_deleteItem                    
*
* Description: 从菜单中删除一项
*             
* Arguments  : hMenu: 菜单句柄
*							 itemId: 要删除第几项
*
* Reutrn     : 执行结果
*
* Note(s)    : None.
*********************************************************************************************************
*/
GUI_ERROR menu_deleteItem(MENU_Handle hMenu, uint8_t itemId)
{
	/* 参数错误 */
	if(!hMenu || itemId < 0) return ERR_PARA;
	
	int16_t err = ERR_NONE;
	MENU_OBJ *pMenu = NULL;
	MENUNODE_OBJ *preNode = NULL, *lstNode = NULL;
	
	pMenu = menu_getObject(hMenu, &err);
	if(err != ERR_NONE) return err;
	
	/* 没有该项 */
	if(itemId > pMenu->numOfMenus) return ERR_PARA;
	
	/* 如果要删除的结点就是第一个结点,直接将头结点指向下一个 */
	if(itemId == 0) 
	{
		lstNode = pMenu->headMenu;
		pMenu->headMenu = pMenu->headMenu->lstNode;
		pMenu->headMenu->preNode = NULL;
	}
	else /* 如果不是第一个，则需要查找 */
	{
		uint16_t i = 1;		
		preNode = pMenu->headMenu;
		lstNode = pMenu->headMenu->lstNode;
		
		/* 顺序查找链表 */
		while(lstNode && i < itemId)
		{
			i++;
			preNode = lstNode;
			lstNode = lstNode->lstNode;
		}
	}
	
	/* 找到了则从结点链表中断开，并释放内存 */
	if(lstNode) 
	{
		preNode->lstNode = lstNode->lstNode;
		lstNode->lstNode->preNode = preNode;
		bsp_mem_Free(SRAMIN, lstNode);
	}
	else
		return ERR_PARA;
	
	return err;
}


/********************************************  END OF FILE  *******************************************/
