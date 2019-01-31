/**
  *******************************************************************************************************
  * File Name: gui_menu.c
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
* Description: ��ȡ�˵��ṹ��
*             
* Arguments  : hMenu: �˵����
*							 *err: ���ڱ���ִ�н��
*
* Reutrn     : ��ȡ���Ĳ˵��ṹ��
*
* Note(s)    : None.
*********************************************************************************************************
*/
MENU_OBJ *menu_getObject(MENU_Handle hMenu, int16_t *err)
{
	if(!hMenu) return NULL;
	
	WIDGET_OBJ *pObj = NULL;
	
		/* ��ȡ��ť�ؼ� */
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
	
	/* �õ���ť�ؼ��ṹ�� */
	pWidget = widget_getWidget(hMenu, &err); 
	if(err != ERR_NONE) return;
	pMenu = (MENU_OBJ *)(pWidget->widgetData);
	
	switch(pMsg->msgId)
	{
		case MSG_KEY:
		{
			/* �õ�������Ϣ */
			MSG_KEY_INFO *key = (MSG_KEY_INFO *)&(pMsg->key);
			
			/* ���������� */
			if(KEY_PRESS == key->keyStatus)
			{
				/* �˵����� */
				if(key->keyValue == MSG_KEY_UP)
				{
					if(pMenu->selMenu->preNode) /* ��ǰ�滹�н���ʱ����ǰ�� */
					{
						pMenu->selMenu = pMenu->selMenu->preNode;
						pMenu->selNo --;
//						if(pMenu->selNo <= -5)
//						{
//							pMenu->selNo = 0;
//							pMenu->headMenu = pMenu->headMenu->preNode;
//						}
						win_Invalidation(pWidget->id >> 10); /* �����ػ�һ�� */
						gui_onPaint();
					}
				}
				else if(key->keyValue == MSG_KEY_DOWN)
				{
					if(pMenu->selMenu->lstNode) /* �˵����� */
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
				}else if(key->keyValue == MSG_KEY_PWR) /* ִ�лص� */
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
* Description: �˵����ƺ���
*             
* Arguments  : *pMenu: �˵��ṹ��ָ��
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
	
	/* �������һ����Ļ������ʾ�ķ�Χ����ʼ��ʾ�Ľ������ */
	if(pMenu->selNo >= 5) i = pMenu->selNo - 4;
	while(i)
	{
		pNode = pNode->lstNode;
		i --;
	}
	
	/* ˳�����������Ŀ���� */
	while(pNode)
	{
		gui_gotoXY(x, y);
		
		/* ѡ�е���Ŀ��ʾ��ʽ��һ�� */
		if(pMenu->selMenu == pNode)
			gui_dispString(pNode->text, 0);
		else
			gui_dispString(pNode->text, 1);
		
		pNode = pNode->lstNode;
		
		y += 10;
		if(y > LCD_Y) break; /* ������Ļ�Ĳ��ֲ���Ҫ��ʾ */
	}
}

/*
*********************************************************************************************************
*                          menu_Create                
*
* Description: ����һ���˵��ؼ�
*             
* Arguments  : x0, y0, width: �˵���λ���Լ����
*							 id: �˵�id
*							 hParent: �����ھ��
*
* Reutrn     : �ɹ����ز˵������ʧ���򷵻�ԭ��
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
	
	/* ���ò˵���λ�� */
	pMenu->rect.x0 = x0;
	pMenu->rect.y0 = y0;
	pMenu->rect.width = width;
	pMenu->rect.height = 0;
	
	pMenu->selMenu = NULL;
	pMenu->headMenu = NULL;
	
	/* ��ʼû���κ���Ŀ */
	pMenu->numOfMenus = 0;
	pMenu->selNo = 0;
	
	/* ���˵����뵽�ؼ��б��� */
	err = widget_Create(WIDGET_MENU, pMenu, id, MSG_KEY_UP | MSG_KEY_DOWN | MSG_KEY_PWR, menu_defaultProc, hParent);
	return err;
}

/*
*********************************************************************************************************
*                          menu_Delete                
*
* Description: ɾ��һ���˵��ؼ�
*             
* Arguments  : *hMenu: Ҫɾ���Ĳ˵��ؼ����ָ��
*
* Reutrn     : ִ�н��
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
* Description: ��˵��ؼ��в���һ�����
*             
* Arguments  : hMenu: �˵����
*							 *text: �˵�����ı�
*							 *_cb: �ý��Ļص�����
*
* Reutrn     : ִ�н��
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
	
	/* Ϊ�˵���������ڴ� */
	pMenuNode = (MENUNODE_OBJ *)bsp_mem_Alloc(SRAMIN, sizeof(MENUNODE_OBJ));
	if(!pMenuNode) return ERR_MEM;
	
	pMenuNode->isSelect = false;
	pMenuNode->preNode = NULL;
	pMenuNode->lstNode = NULL;
	pMenuNode->text = text;
	pMenuNode->_cb = _cb;
	
	pMenu = menu_getObject(hMenu, &err);
	if(err != ERR_NONE) return err;
	
	pMenu->numOfMenus ++; /* �˵���Ŀ���� */
	
	/* ����ǵ�һ������Ĳ˵���㣬��ֱ������Ϊ���ͷ */
	if(pMenu->headMenu == NULL)
	{
		pMenu->headMenu = pMenuNode;
		pMenu->selMenu = pMenuNode;
	}
	else /* �ҽӵ���β */
	{
		MENUNODE_OBJ *pNodeTemp = pMenu->headMenu;
		while(pNodeTemp->lstNode) /* ���ұ�β */
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
* Description: �Ӳ˵���ɾ��һ��
*             
* Arguments  : hMenu: �˵����
*							 itemId: Ҫɾ���ڼ���
*
* Reutrn     : ִ�н��
*
* Note(s)    : None.
*********************************************************************************************************
*/
GUI_ERROR menu_deleteItem(MENU_Handle hMenu, uint8_t itemId)
{
	/* �������� */
	if(!hMenu || itemId < 0) return ERR_PARA;
	
	int16_t err = ERR_NONE;
	MENU_OBJ *pMenu = NULL;
	MENUNODE_OBJ *preNode = NULL, *lstNode = NULL;
	
	pMenu = menu_getObject(hMenu, &err);
	if(err != ERR_NONE) return err;
	
	/* û�и��� */
	if(itemId > pMenu->numOfMenus) return ERR_PARA;
	
	/* ���Ҫɾ���Ľ����ǵ�һ�����,ֱ�ӽ�ͷ���ָ����һ�� */
	if(itemId == 0) 
	{
		lstNode = pMenu->headMenu;
		pMenu->headMenu = pMenu->headMenu->lstNode;
		pMenu->headMenu->preNode = NULL;
	}
	else /* ������ǵ�һ��������Ҫ���� */
	{
		uint16_t i = 1;		
		preNode = pMenu->headMenu;
		lstNode = pMenu->headMenu->lstNode;
		
		/* ˳��������� */
		while(lstNode && i < itemId)
		{
			i++;
			preNode = lstNode;
			lstNode = lstNode->lstNode;
		}
	}
	
	/* �ҵ�����ӽ�������жϿ������ͷ��ڴ� */
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
