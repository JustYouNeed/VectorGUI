/**
  *******************************************************************************************************
  * File Name: gui_button.c
  * Author: Vector
  * Version: V1.0.1
  * Date: 2018-12-29
  * Brief: GUI按键控件，部分函数参考正点原子遥控GUI中的程序，原作者为黄绍斌
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-30
	*			Mod: 建立文件
	*
	*		2.Authot: Vector
	*			Date: 2019-1-1
	*			Mod: 删除函数button_addToParent,统一到widget_addToParent
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
*                        button_onPaint                  
*
* Description: 按钮控件绘制函数
*             
* Arguments  : *pButton: 按钮结构体
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
static void _onPaint(const BUTTON_OBJ* pButton)
{
	int16_t XOffset = 0, YOffset = 0;
	
	gui_fillRectangle(pButton->rect.x0, pButton->rect.y0, pButton->rect.x0 + pButton->rect.width, pButton->rect.y0 + pButton->rect.height, 0);
	
	/* 计算按钮标题相对按钮起点的偏移量 */
	if(pButton->textAlign & GUI_TA_LEFT)
	{
		if(pButton->textAlign & GUI_TA_TOP)
		{
			XOffset = 1;
			YOffset = 1;
		}else if(pButton->textAlign & GUI_TA_VCENTER)
		{
			XOffset = 1;
			YOffset = (pButton->rect.height - font_getYSize(GUI_Context.curFont))/2;
		}else if(pButton->textAlign & GUI_TA_BOTTOM)
		{
			XOffset = 1;
			YOffset = pButton->rect.height - font_getYSize(GUI_Context.curFont);
		}
	}else if(pButton->textAlign & GUI_TA_RIGHT)
	{
		if(pButton->textAlign & GUI_TA_TOP)
		{
			XOffset = pButton->rect.width - font_getXSize(pButton->font)*gui_getStringLen(pButton->title);
			YOffset = 1;
		}else if(pButton->textAlign & GUI_TA_VCENTER)
		{
			XOffset = pButton->rect.width - font_getXSize(pButton->font)*gui_getStringLen(pButton->title);;
			YOffset = (pButton->rect.height - font_getYSize(GUI_Context.curFont))/2;
		}else if(pButton->textAlign & GUI_TA_BOTTOM)
		{
			XOffset = 1;
			YOffset = pButton->rect.height - font_getYSize(GUI_Context.curFont);
		}
	}else
	{
		XOffset = (pButton->rect.width - font_getXSize(pButton->font)*gui_getStringLen(pButton->title))/2;
		YOffset = (pButton->rect.height - font_getYSize(GUI_Context.curFont))/2;
	}
	
	/* 如果按钮按下 */
	if(pButton->isPress)
	{
		gui_drawRectangle(pButton->rect.x0, pButton->rect.y0, pButton->rect.x0 + pButton->rect.width, pButton->rect.y0 + pButton->rect.height, 1);
		gui_gotoXY(pButton->rect.x0 + XOffset, pButton->rect.y0 + YOffset);
		gui_dispString(pButton->title, 1);
	}
	else	/* 没有按下 */
	{
		gui_fillRectangle(pButton->rect.x0, pButton->rect.y0, pButton->rect.x0 + pButton->rect.width, pButton->rect.y0 + pButton->rect.height, 1);
		gui_gotoXY(pButton->rect.x0 + XOffset, pButton->rect.y0 + YOffset);
		gui_dispString(pButton->title, 0);
	}
}

/*
*********************************************************************************************************
*                            _delete              
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
static void _delete(BUTTON_Handle hButton)
{	
	widget_Delete(hButton);
}

/*
*********************************************************************************************************
*                        button_getButtonObj                  
*
* Description: 获取按钮控制结构体
*             
* Arguments  : hButton: 按钮句柄
*						   *pButton: 按钮结构体指针
*
* Reutrn     : 执行结果
*
* Note(s)    : 本文件私有函数
*********************************************************************************************************
*/
static BUTTON_OBJ *_getButtonObj(BUTTON_Handle hButton, int16_t *err)
{
	WIDGET_OBJ *pObj = NULL;
	
		/* 获取按钮控件 */
	pObj = widget_getWidget(hButton, err);	
	if(!pObj) 
	{
		*err = ERR_PARA;
		return NULL;
	}
	
	return (BUTTON_OBJ *)(pObj->widgetData);
}

/*
*********************************************************************************************************
*                             button_defaultProc             
*
* Description: 默认按键回调函数
*             
* Arguments  : *pMsg: 消息指针
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
static void button_defaultProc(WM_MESSAGE *pMsg)
{
	BUTTON_Handle hObj;
	BUTTON_OBJ *pButton = NULL;
	WIDGET_OBJ *pWidget = NULL;
	int16_t err = ERR_NONE;
	
	hObj = pMsg->hWin;
	pWidget = widget_getWidget(hObj, &err);
	pButton = (BUTTON_OBJ *)(pWidget->widgetData);
	
	/* 根据不同的消息进行不同的处理 */
	switch(pMsg->msgId)
	{
		case MSG_CREATE: break;
		case MSG_KEY: 	/* 按键消息 */
		{
			/* 得到按键消息 */
			MSG_KEY_INFO *key = (MSG_KEY_INFO *)&(pMsg->key);
			
			/* 如果该按键在控件的响应按键里面则响应 */
			if(key->keyValue & pWidget->actKey)
			{
				if(key->keyStatus == KEY_PRESS) /* 如果按键按下了 */
				{
					pButton->isPress = true; 
					_onPaint(pButton);
					if(pButton->_cb) pButton->_cb(pMsg);
				}
				else if(key->keyStatus == KEY_RELEASED)	/* 按键释放了 */
				{
					pButton->isPress = false;
					_onPaint(pButton);
				}
			}
		}break;
		case MSG_PAINT: 	/* 按键重绘消息 */
		{
			_onPaint(pButton);
		}break;
		case MSG_DELETE:	/* 删除按键消息 */
		{
			_delete(hObj);
		}break;
		default: wm_defaultProc(pMsg);
	}
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
void button_setCallback(BUTTON_Handle hButton, BUTTON_CALLBACK *_cb)
{
	BUTTON_OBJ * pButton = NULL;
	int16_t err = ERR_NONE;
	if(!hButton) return ;
	
	pButton = _getButtonObj(hButton, &err);
	if(!pButton) return ;
	
	pButton->_cb = _cb;
}

/*
*********************************************************************************************************
*                      button_getFont                    
*
* Description: 获取按钮字体
*             
* Arguments  : 按钮句柄
*
* Reutrn     : 该按钮字体
*
* Note(s)    : None.
*********************************************************************************************************
*/
GUI_FONT button_getFont(BUTTON_Handle hButton)
{
	WIDGET_OBJ *pWidget = NULL;
	BUTTON_OBJ *pButton = NULL;
	int16_t err = ERR_NONE;
	
	pWidget = widget_getWidget(hButton, &err);	/* 读取该控件的结构体 */
	
	if(!pWidget) return 0;
	
	pButton = (BUTTON_OBJ*)pWidget->widgetData;
	
	return pButton->font;
}

/*
*********************************************************************************************************
*                         button_setFont                 
*
* Description: 设置按钮标题字体
*             
* Arguments  : hButton: 按钮句柄
*							 font: 字体
*
* Reutrn     : 执行结果
*
* Note(s)    : None.
*********************************************************************************************************
*/
GUI_ERROR button_setFont(BUTTON_Handle hButton, GUI_FONT font)
{
	if(!hButton) return ERR_PARA;
	BUTTON_OBJ *pButton = NULL;
	int16_t err = ERR_NONE;
	
	pButton = _getButtonObj(hButton, &err);
	pButton->font = font;
	
	return ERR_NONE;
}

/*
*********************************************************************************************************
*                       button_getText                   
*
* Description: 获取按钮文本
*             
* Arguments  : hButton: 按钮句柄
*							 *pText: 文本缓存区
*							 textLen: 文本长度
*
* Reutrn     : 执行结果
*
* Note(s)    : None.
*********************************************************************************************************
*/
GUI_ERROR button_getText(BUTTON_Handle hButton, uint8_t *pText, uint16_t textLen)
{
	if(!hButton) return ERR_PARA;
	
	BUTTON_OBJ *pButton = NULL;
	int16_t err = ERR_NONE;
	
	pButton = _getButtonObj(hButton, &err);
	pText = pButton->title;
	
	return ERR_NONE;
}

/*
*********************************************************************************************************
*                      button_setText                    
*
* Description: 设置按钮文本
*             
* Arguments  : hButton: 按钮句柄
*							 *pText: 要设置的文本
*
* Reutrn     : 执行结果
*
* Note(s)    : None.
*********************************************************************************************************
*/
GUI_ERROR button_setText(BUTTON_Handle hButton, const uint8_t *pText)
{
	if(!hButton) return ERR_PARA;
	
	BUTTON_OBJ *pButton = NULL;
	int16_t err = ERR_NONE;
	
	pButton = _getButtonObj(hButton, &err);
	pButton->title = (uint8_t *)pText;
	
	return ERR_NONE;
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
GUI_ERROR button_setTextAlign(BUTTON_Handle hButton)
{
	return ERR_NONE;
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
GUI_ERROR button_getTextAlign(BUTTON_Handle hButton)
{
	return ERR_NONE;
}

/*
*********************************************************************************************************
*                       button_Create                   
*
* Description: 创建一个按钮控件
*             
* Arguments  : x0,y0: 按钮坐标
*							 width, height: 按钮的长宽
*							 id: 按钮id
*							 title: 按钮显示的标题
*							 该按钮会响应什么按键消息
*							 hParent: 父窗口句柄
*
* Reutrn     : 按钮句柄
*
* Note(s)    : None.
*********************************************************************************************************
*/
BUTTON_Handle button_Create(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t id, uint8_t *title, uint16_t actKey, WM_HWIN hParent)
{
	int16_t r = 0;
	BUTTON_OBJ *pButton;
	
	if(!hParent) return 0;	/* 按键必须有父窗口 */
	
	pButton = (BUTTON_OBJ*)bsp_mem_Alloc(SRAMIN, sizeof(BUTTON_OBJ));	/* 为按键申请内存 */
	if(!pButton) return 0;	/* 内存申请失败 */
	
	/* 设置按钮参数 */
	pButton->rect.x0 = x0;
	pButton->rect.y0 = y0;
	pButton->rect.width = width;
	pButton->rect.height = height;
	pButton->isPress = 0;
	pButton->title = title;
	pButton->_cb = NULL;
	pButton->textAlign = GUI_TA_HCENTER | GUI_TA_VCENTER;
	
	r = widget_Create(WIDGET_BUTTON, pButton, id, actKey, button_defaultProc, hParent);
	
	msg_sendMsgNoData(r, MSG_PAINT);	/* 创建完成后发送一个重绘消息 */
	return r;
}

/*
*********************************************************************************************************
*                           button_CreateEx               
*
* Description: 创建一个按钮控件
*             
* Arguments  : *pButton: 按钮结构体指针
*							 id: 按钮ID
*							 actKey: 按钮响应的按键
*							 hParent: 父窗口句柄
*
* Reutrn     : 成功时返回按钮句柄
*
* Note(s)    : None.
*********************************************************************************************************
*/
BUTTON_Handle button_CreateEx(BUTTON_OBJ *pButton, uint8_t id, uint16_t actKey, WM_HWIN hParent)
{
	if(!hParent) return 0;
	
	return widget_Create(WIDGET_BUTTON, pButton, id, actKey, button_defaultProc, hParent);
}

/*
*********************************************************************************************************
*                           button_Delete               
*
* Description: 删除一个按钮控件
*             
* Arguments  : *hButton: 要删除的按钮控件句柄指针
*
* Reutrn     : 执行结果
*
* Note(s)    : 如果按钮控件删除成功，则按钮句柄会置为0
*********************************************************************************************************
*/
GUI_ERROR button_Delete(BUTTON_Handle *hButton)
{
	if(!*hButton) return ERR_PARA; /* 参数错误 */
	
	msg_sendMsgNoData(*hButton, MSG_DELETE);
	*hButton = 0;
}
/********************************************  END OF FILE  *******************************************/
