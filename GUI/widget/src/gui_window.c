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

/*
*********************************************************************************************************
*                       _getObj                   
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
extern GUI_CONTEXT GUI_Context;
/*
*********************************************************************************************************
*                            _onPaint              
*
* Description: �����ػ溯��
*             
* Arguments  : *pWindow: ���ڿ��ƽṹ��ָ��
*
* Reutrn     : None.
*
* Note(s)    : ���ļ�˽�к���
*********************************************************************************************************
*/
static void _onPaint(const WINDOW_OBJ *pWindow)
{
	int16_t XOffset = 0, YOffset = 0;
	
	GUI_Context.drawRect.x0 = pWindow->rect.x0;
	GUI_Context.drawRect.y0 = pWindow->rect.y0;
	GUI_Context.drawRect.width = pWindow->rect.width;
	GUI_Context.drawRect.height = pWindow->rect.height;
	
	gui_fillRectangle(pWindow->rect.x0, pWindow->rect.y0, pWindow->rect.x0 + pWindow->rect.width, pWindow->rect.y0 + 14, 1);
	gui_drawRectangle(pWindow->rect.x0, pWindow->rect.y0, pWindow->rect.x0 + pWindow->rect.width, pWindow->rect.y0 + pWindow->rect.height, 1);
	
	XOffset = (pWindow->rect.width - 6*gui_getStringLen(pWindow->title))/2;
	YOffset = (14 - font_getYSize(GUI_Context.curFont))/2;
	
	gui_gotoXY(pWindow->rect.x0 + XOffset, pWindow->rect.y0 + YOffset);
	gui_dispString(pWindow->title, 0);
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
	msg_sendMsgNoData(hWindow, MSG_PAINT);
	
	return hWindow;
}

/********************************************  END OF FILE  *******************************************/
