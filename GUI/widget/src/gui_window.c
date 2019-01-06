/**
  *******************************************************************************************************
  * File Name: gui_window.c
  * Author: Vector
  * Version: V1.0.1
  * Date: 2018-12-29
  * Brief: GUI���ڿؼ������ֺ����ο�����ԭ��ң��GUI�еĳ���ԭ����Ϊ���ܱ�
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-30
	*			Mod: �����ļ�
	*
	*		2.Authot: Vector
	*			Date: 2019-1-1
	*			Mod: ɾ������window_addToParent,ͳһ��widget_addToParent
	*
  *******************************************************************************************************
  */	
/*
  *******************************************************************************************************
  *                              INCLUDE FILES
  *******************************************************************************************************
*/
# include "gui.h"

static WINDOW_OBJ *_getObj(WINDOW_Handle hWindow, int16_t *err)
{
	WIDGET_OBJ *pObj = NULL;
	
		/* ��ȡ��ť�ؼ� */
	pObj = widget_getWidget(hWindow, err);	
	if(!pObj) 
	{
		*err = ERR_PARA;
		return NULL;
	}
	
	return (WINDOW_OBJ *)(pObj->widgetData);
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
static void _onPaint(const WINDOW_OBJ *pWindow)
{
//	gui_fillRectangle(pWindow->rect.x0, pWindow->rect.y0, pWindow->rect.x0 + pWindow->rect.width, pWindow->rect.y0 + 14, 1);
	gui_drawRectangle(pWindow->rect.x0, pWindow->rect.y0, pWindow->rect.x0 + pWindow->rect.width, pWindow->rect.y0 + pWindow->rect.height, 1);
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
static void window_defaultProc(WM_MESSAGE *pMsg)
{
	WINDOW_OBJ *pWindow = NULL;
	int16_t err = ERR_NONE;
	
	pWindow = _getObj(pMsg->hWin, &err);
	
	switch(pMsg->msgId)
	{
		case MSG_PAINT:
		{
			_onPaint(pWindow);
		}break;
	}
}

/*
*********************************************************************************************************
*                        window_createWindow                  
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
WINDOW_Handle window_Create(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint8_t *title, uint16_t id, WM_HWIN hParent)
{
	WINDOW_OBJ *pWindow = NULL;
	WINDOW_Handle hWindow = 0;
	
	if(!hParent) return -2;	/* �ؼ�һ��Ҫ�и����� */
	
	/* Ϊ�ؼ������ڴ� */
	pWindow = (WINDOW_OBJ*)bsp_mem_Alloc(SRAMIN, sizeof(WINDOW_OBJ));
	if(!pWindow) return -1;	/* �ڴ�����ʧ�� */
	
	pWindow->rect.x0 = x0;
	pWindow->rect.y0 = y0;
	pWindow->rect.width = width;
	pWindow->rect.height = height;
	pWindow->title = title;
	
	hWindow = widget_Create(WIDGET_WINDOW, pWindow, id, MSG_KEY_NULL, window_defaultProc, hParent);	
//	msg_sendMsgNoData(hWindow, MSG_PAINT);
	
	return hWindow;
}

/********************************************  END OF FILE  *******************************************/
