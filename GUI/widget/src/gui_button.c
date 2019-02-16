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
# include "gui_widget.h"
# include "gui_button.h"
# include "gui_font.h"
# include "gui_text.h"
# include "gui_win.h"
# include "gui_mem.h"
# include "gui_core.h"

/*
*********************************************************************************************************
*                        buttonbutton_getObject                  
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
static BUTTON_OBJ *button_getObject(BUTTON_Handle hButton, int16_t *err)
{
	WIDGET_OBJ *pWidget = NULL;
	
		/* 获取按钮控件 */
	pWidget = widget_getWidget(hButton, err);	
	if(*err) return NULL;
	
	return (BUTTON_OBJ *)(pWidget->widgetData);
}

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
void button_onPaint(BUTTON_OBJ *pButton)
{
	int16_t XOffset = 0, YOffset = 0;
	
	gui_rectCrop(&GUI_Context.drawRect, &pButton->rect);
	
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
static void button_defaultProc(BUTTON_Handle hButton, GUI_KEY_INFO *pKey)
{
	BUTTON_OBJ *pButton = NULL;
	int16_t err = ERR_NONE;
	
	/* 得到按钮控件结构体 */
	pButton = button_getObject(hButton, &err);
	if(err) return ;
	
	/* 设置按钮状态 */
	if(pKey->keyStatus == GUI_KEY_PRESS)
	{
		pButton->isPress = true; 
	}
	else if(pKey->keyStatus == GUI_KEY_RELEASED)
	{
		pButton->isPress = false;
	}
	
	/* 只有按钮状态改变了才需要重绘 */
	pButton->lstState = pButton->isPress;
	if(pButton->lstState != pButton->preState)
	{
		/* 绘制按钮按下的状态 */
		win_Invalidation(WIN_Desktop->hWin);
//		gui_onPaint();
	}
	pButton->preState = pButton->lstState;
	
	/* 存在回调函数的话执行回调函数 */
	if(pButton->_cb) pButton->_cb(pKey);
}

/*
*********************************************************************************************************
*                        button_setCallback                  
*
* Description: 设置按钮的回调函数,在按钮按下或者释放的时候回调函数将会被调用
*             
* Arguments  : hButton: 按钮的句柄
*							 *_cb: 回调函数
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void button_setCallback(BUTTON_Handle hButton, BUTTON_CALLBACK *_cb)
{
	BUTTON_OBJ * pButton = NULL;
	int16_t err = ERR_NONE;
	if(!hButton) return ;
	
	pButton = button_getObject(hButton, &err);
	if(err) return ;
	
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
	
	if(err) return (GUI_FONT)0;
	
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
	
	pButton = button_getObject(hButton, &err);
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
	
	pButton = button_getObject(hButton, &err);
	if(err) return (GUI_ERROR)err;
	
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
	
	pButton = button_getObject(hButton, &err);
	pButton->title = (uint8_t *)pText;
	
	win_Invalidation(hButton >> 10);
	
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
BUTTON_Handle button_Create(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t id, uint8_t *title, uint16_t actKey, WIN_Handle hParent)
{
	int16_t r = 0;
	uint8_t *pMem = NULL;
	BUTTON_OBJ *pButton = NULL;
	WIDGET_OBJ *pWidget = NULL;
	
	if(!hParent) return 0;	/* 按键必须有父窗口 */
	
	pMem = (uint8_t *)gui_memAlloc(sizeof(BUTTON_OBJ) + sizeof(WIDGET_OBJ));	/* 为按键申请内存 */
	if(!pMem) return 0;	/* 内存申请失败 */
	
	pWidget = (WIDGET_OBJ *)pMem;
	pButton = (BUTTON_OBJ *)(pMem + sizeof(WIDGET_OBJ));
	
	/* 设置按钮参数 */
	pButton->rect.x0 = x0;
	pButton->rect.y0 = y0;
	pButton->rect.width = width;
	pButton->rect.height = height;
	pButton->isPress = 0;
	pButton->title = title;
	pButton->_cb = NULL;
	pButton->textAlign = GUI_TA_HCENTER | GUI_TA_VCENTER;
	pButton->lstState = false;
	pButton->preState = false;
	
	pWidget->widgetData = (void *)pButton;
	r = widget_Create(WIDGET_BUTTON, pWidget, id, actKey, button_defaultProc, hParent);
	
//	msg_sendMsgNoData(r, MSG_PAINT);	/* 创建完成后发送一个重绘消息 */
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
BUTTON_Handle button_CreateEx(BUTTON_OBJ *pButton, uint8_t id, uint16_t actKey, WIN_Handle hParent)
{
	if(!hParent) return 0;
	
//	return widget_Create(WIDGET_BUTTON, pButton, id, actKey, button_defaultProc, hParent);
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
	
	widget_Delete(*hButton);
	*hButton = 0;
	
	return ERR_NONE;
}
/********************************************  END OF FILE  *******************************************/
