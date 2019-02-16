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
# include "gui_widget.h"
# include "gui_mem.h"
# include "gui_win.h"
# include "gui_core.h"

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
WIDGET_OBJ *widget_getWidget(WIDGET_Handle hWidget, int16_t *err)
{
	if(!hWidget) 
	{
		*err = ERR_PARA;
		return NULL;	/* 参数错误 */
	}
	
	WIN_OBJ *pWin = NULL;
	WIDGET_OBJ *pWidget = NULL;
	
	pWin = win_getObject(GUI_GET_HPARENT(hWidget), err);	/* 得到该控件的父窗口 */
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
*                         widget_Create                 
*
* Description: 创建一个控件
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
WIDGET_Handle widget_Create(WIDGET_TYPE widgetType, WIDGET_OBJ *pWidget, uint8_t id, uint16_t actKey, WIDGET_CALLBACK *_cb, WIN_Handle hParent)
{
	WIDGET_OBJ *widgetTemp = NULL;
	WIN_OBJ *pWin = NULL;
	uint8_t widgetLayer = 0;
	int16_t err = ERR_NONE;

	if(!hParent || !pWidget) return 0;
		
	/* 填写控件参数 */
	{
		pWidget->type = widgetType;
		pWidget->id = ((hParent << 10) | (widgetType << 6) | (id << 0));
		pWidget->pNext = NULL;	
		pWidget->actKey = actKey;
		pWidget->_cb = _cb;
		pWidget->layer = widgetLayer; /* 设置控件的层 */
	}
	
	/* 将控件挂载到窗口上 */
	{
		pWin = win_getObject(hParent, &err);	/* 得到父窗口的结构体指针 */
		
		pWin->noOfWidget++;		/* 该窗口下的控件数量增加 */
		pWin->isInvalid = true;
		
		if(pWin->pWidget == NULL)	/* 如果该窗口当前没有控件,就挂接该控件 */
		{
			pWin->pWidget = pWidget;
			widgetLayer++;		
		}
		else
		{
			widgetTemp = (pWin->pWidget);	/* 得到该窗口的控件链表 */
			while(widgetTemp->pNext)	/* 查找表尾 */
			{
				widgetTemp = (struct WIDGET_OBJ *)(widgetTemp->pNext);
				widgetLayer++;
			}
			widgetTemp->pNext = (struct WIDGET_OBJ*)pWidget;	/* 将该控件挂接到父窗口控件链表上 */
		}
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
GUI_ERROR widget_Delete(WIDGET_Handle hWidget)
{
	WIDGET_OBJ *preWidget	= NULL, *lstWidget = NULL;
	WIN_OBJ *pWin = NULL;
	int16_t err = ERR_NONE;
	WIN_Handle hWin = GUI_GET_HPARENT(hWidget);
	
	/* 获取该控件的父窗口 */
	pWin = win_getObject(hWin, &err);
	pWin->isInvalid = true;
	
	if(!pWin) return ERR_PARA;
	lstWidget = preWidget = pWin->pWidget;
	
	/* 如果是第一个控件 */
	if(preWidget->id == hWidget)	
	{
		pWin->pWidget = preWidget->pNext;	/* 从链表中删除 */
	}
	else	/* 不是第一个 */
	{
		lstWidget = preWidget->pNext;
		while(lstWidget)
		{
			if(lstWidget->id == hWidget)
			{
				preWidget->pNext = lstWidget->pNext;
				break;
			}
			preWidget = lstWidget;
			lstWidget = lstWidget->pNext;
		}
	}
	
	/* 如果找到了 */
	if(lstWidget)
	{
		gui_memFree(lstWidget);
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
void widget_onPaint(struct WIDGET_OBJ *pWidget)
{
	if(!pWidget) return ;
	int16_t err = ERR_NONE;
	WIN_OBJ *pWin = win_getObject(GUI_GET_HPARENT(pWidget->id), &err);
	
	GUI_Context.drawRect.x0 = pWin->clientRect.x0;
	GUI_Context.drawRect.y0 = pWin->clientRect.y0;
	GUI_Context.drawRect.height = pWin->clientRect.height;
	GUI_Context.drawRect.width = pWin->clientRect.width;

	/* 根据控件类型选择不同的绘制函数 */
	switch(pWidget->type)
	{
		case WIDGET_BUTTON: 	/* 按钮 */
		{
			button_onPaint(pWidget->widgetData);		/* 绘制按键 */
		}break;
		case WIDGET_CHECKBOX:
		{
			checkbox_onPaint(pWidget->widgetData);
		}break;
		case WIDGET_MENU:
		{
			menu_onPaint(pWidget->widgetData);
		}break;
		case WIDGET_PROGBAR:
		{
			progbar_onPaint(pWidget->widgetData);
		}break;
		case WIDGET_SCROLLBAR:
		{
			scrollbar_onPaint(pWidget->widgetData);
		}break;
	}
}

/********************************************  END OF FILE  *******************************************/


