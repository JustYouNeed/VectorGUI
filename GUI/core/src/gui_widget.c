/**
  *******************************************************************************************************
  * File Name: gui_widget.c
  * Author: Vector
  * Version: V1.0.1
  * Date: 2018-12-29
  * Brief: GUI控件处理部分
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-30
	*			Mod: 建立文件
	*
	*		2.Authot: Vector
	*			Date: 2019-1-1
	*			Mod: 增加widget_addToParent,用于挂载控件到窗口上
	*
  *******************************************************************************************************
  */	
/*
  *******************************************************************************************************
  *                              INCLUDE FILES
  *******************************************************************************************************
*/
# include "gui.h"


///*
//*********************************************************************************************************
//*                                          
//*
//* Description: 
//*             
//* Arguments  : 
//*
//* Reutrn     : 
//*
//* Note(s)    : 
//*********************************************************************************************************
//*/
//void widget_sortList(WM_HWIN hParent)
//{
//	WM_OBJ *pWin = NULL;
//	int16_t err = ERR_NONE;
//	
//	pWin = wm_getWindowObject(GUI_GET_HPARENT(hWidget), err);	/* 得到该控件的父窗口 */
//}

/*
*********************************************************************************************************
*                    widget_getWidget                      
*
* Description: 根据控件句柄查找该控件结构体
*             
* Arguments  : hWidget: 控件句柄
*
* Reutrn     : 0代表该控件不存在
*
* Note(s)    : None.
*********************************************************************************************************
*/
WIDGET_OBJ *widget_getWidget(WIDGET_HANDLE hWidget, int16_t *err)
{
	if(!hWidget) 
	{
		*err = ERR_PARA;
		return NULL;	/* 参数错误 */
	}
	
	WM_OBJ *pWin = NULL;
	WIDGET_OBJ *pWidget = NULL;
	
	pWin = wm_getWindowObject(GUI_GET_HPARENT(hWidget), err);	/* 得到该控件的父窗口 */
	if(!pWin || !pWin->pWidget) return NULL;	/* 获取父窗口失败或者该窗口下没有控件都返回失败 */
	
	/* 查找控件 */
	pWidget = pWin->pWidget;
	while(pWidget)
	{
		if(pWidget->id == hWidget)
			break;
		
		pWidget = pWidget->pNext;
	}
	*err = ERR_NONE;
	return pWidget;
		
}

/*
*********************************************************************************************************
*                         widget_addToParent                 
*
* Description: 将控件挂载到父窗口上
*             
* Arguments  : widgetType: 控件类型
*							 *pObj: 具体控件指针
*							 hParent: 窗口句柄
*
* Reutrn     : 返回0表示挂载成功,其他失败
*
* Note(s)    : None.
*********************************************************************************************************
*/
WIDGET_HANDLE widget_Create(WIDGET_TYPE widgetType, void *pObj, uint8_t id, uint16_t actKey, WIDGET_CALLBACK *_cb, WM_HWIN hParent)
{
	WIDGET_OBJ *pWidget = NULL, *widgetTemp = NULL;
	WM_OBJ *pWin;
	int16_t err = ERR_NONE;
	
	if(!hParent) return 0;
	if(!pObj) return 0;
	
	/* 首先为控件申请内存 */
	pWidget = (struct WIDGET_OBJ*)bsp_mem_Alloc(SRAMIN, sizeof(struct WIDGET_OBJ));
	if(!pWidget) return 0;
	
	pWidget->type = widgetType;
	pWidget->id = ((hParent << 10) | (widgetType << 6) | (id << 0));
	pWidget->pNext = NULL;
	pWidget->actKey = actKey;
	pWidget->_cb = _cb;
	
	pWidget->widgetData = (void *)pObj;
//	/* 根据控件类型设置控件数据区 */
//	switch(widgetType)
//	{
//		case WIDGET_BUTTON:pWidget->widgetData = (BUTTON_OBJ*)pObj;	break;
//		case WIDGET_CHECKBOX: pWidget->widgetData = (CHECKBOX_OBJ*)pObj;	break;
//		case WIDGET_MENU:pWidget->widgetData = (MENU_OBJ*)pObj;	break;
//		case WIDGET_PROGBAR:pWidget->widgetData = (PROGBAR_OBJ*)pObj;	break;
//		case WIDGET_SCROLLBAR:pWidget->widgetData = (SCROLLBAR_OBJ*)pObj;	break;
//		case WIDGET_WINDOW:pWidget->widgetData = (WINDOW_OBJ*)pObj;	break;
//	}
		
	/* 将控件挂载到窗口上 */
	pWin = wm_getWindowObject(hParent, &err);	/* 得到父窗口的结构体指针 */
	
	pWidget->layer = pWin->noOfWidget;		/* 设置控件的层 */
	pWin->noOfWidget++;		/* 该窗口下的控件数量增加 */

	if(pWin->pWidget == NULL)	/* 如果该窗口当前没有控件,就挂接该控件 */
	{
		pWin->pWidget = pWidget;
	}
	else
	{
		widgetTemp = (pWin->pWidget);	/* 得到该窗口的控件链表 */
		while(widgetTemp->pNext)	/* 查找表尾 */
		{
			widgetTemp = (struct WIDGET_OBJ *)(widgetTemp->pNext);
		}
		widgetTemp->pNext = (struct WIDGET_OBJ*)pWidget;	/* 将该控件挂接到父窗口控件链表上 */
	}
	
	return pWidget->id;
}

/*
*********************************************************************************************************
*                            widget_Delete              
*
* Description: 删除一个控件
*             
* Arguments  : hWidget: 要删除的控件句柄
*
* Reutrn     : 返回值见gui_types.h中定义
*
* Note(s)    : 删除成功为控件句柄会清零
*********************************************************************************************************
*/
GUI_ERROR widget_Delete(WIDGET_HANDLE hWidget)
{
	WIDGET_OBJ *preObj	= NULL, *lstObj = NULL;
	WM_OBJ *pObj = NULL;
	int16_t err = ERR_NONE;
	
	/* 获取该控件的父窗口 */
	pObj = wm_getWindowObject(hWidget >> 10, &err);
	if(!pObj) return ERR_PARA;
	lstObj = preObj = pObj->pWidget;
	
	/* 如果是第一个控件 */
	if(preObj->id == hWidget)	
	{
		pObj->pWidget = preObj->pNext;	/* 从链表中删除 */
	}
	else	/* 不是第一个 */
	{
		lstObj = preObj->pNext;
		while(lstObj)
		{
			if(lstObj->id == hWidget)
			{
				preObj->pNext = lstObj->pNext;
				break;
			}
			preObj = lstObj;
			lstObj = lstObj->pNext;
		}
	}
	
	/* 如果找到了 */
	if(lstObj)
	{
		bsp_mem_Free(SRAMIN, lstObj->widgetData);	/* 释放控件数据 */
		bsp_mem_Free(SRAMIN, lstObj);
	}
	
	return ERR_NONE;
}

/*
*********************************************************************************************************
*                        widget_onPaint                  
*
* Description: 控件重绘函数
*             
* Arguments  : *pWidget: 控件链表
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void widget_onPaint(const struct WIDGET_OBJ *pWidget)
{
//	if(!pWidget) return ;
//	
//	/* 根据控件类型选择不同的绘制函数 */
//	switch(pWidget->type)
//	{
//		case WIDGET_BUTTON: 	/* 按钮 */
//		{
//			BUTTON_OBJ *pButton = (BUTTON_OBJ*)(pWidget->widgetData);	/* 得到按键结构体 */
////			button_onPaint(pButton);		/* 绘制按键 */
//		}break;
//		case WIDGET_CHECKBOX:
//		{
//			CHECKBOX_OBJ *pCheckbox = (CHECKBOX_OBJ *)(pWidget->widgetData);
//			checkbox_onPaint(pCheckbox);
//		}break;
//		case WIDGET_MENU:
//		{
//			MENU_OBJ *pMenu = (MENU_OBJ *)(pWidget->widgetData);
//			menu_onPaint(pMenu);
//		}break;
//		case WIDGET_PROGBAR:
//		{
//			PROGBAR_OBJ *pProgbar = (PROGBAR_OBJ *)(pWidget->widgetData);
//			progbar_onPaint(pProgbar);
//		}break;
//		case WIDGET_SCROLLBAR:
//		{
//			SCROLLBAR_OBJ *pScrollbar = (SCROLLBAR_OBJ *)(pWidget->widgetData);
//			scrollbar_onPaint(pScrollbar);
//		}break;
//		case WIDGET_WINDOW:		/* window控件 */
//		{
//			WINDOW_OBJ *pWindow = (WINDOW_OBJ *)(pWidget->widgetData);
//			window_onPaint(pWindow);
//		}break;
//	}
}

/********************************************  END OF FILE  *******************************************/


