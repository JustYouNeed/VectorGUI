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
void button_onPaint(const BUTTON_OBJ* pButton)
{
	int16_t XOffset = 0, YOffset = 0;
	
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
	else	/* 没有按钮 */
	{
		gui_fillRectangle(pButton->rect.x0, pButton->rect.y0, pButton->rect.x0 + pButton->rect.width, pButton->rect.y0 + pButton->rect.height, 1);
		gui_gotoXY(pButton->rect.x0 + XOffset, pButton->rect.y0 + YOffset);
		gui_dispString(pButton->title, 0);
	}
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
	
	widget_getWidget(hButton, pWidget);	/* 读取该控件的结构体 */
	
	if(!pWidget) return 0;
	
	pButton = (BUTTON_OBJ*)pWidget->widgetData;
	
	return pButton->font;
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
GUI_ERROR button_setFont(BUTTON_Handle hButton, GUI_FONT font)
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
GUI_ERROR button_getText(BUTTON_Handle hButton, char *pText, uint16_t textLen)
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
GUI_ERROR button_setText(BUTTON_Handle hButton, const char *pText)
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
*							 hParent: 父窗口句柄
*
* Reutrn     : 按钮句柄
*
* Note(s)    : None.
*********************************************************************************************************
*/
BUTTON_Handle button_Create(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t id, uint8_t *title, WM_HWIN hParent)
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
	pButton->textAlign = GUI_TA_HCENTER | GUI_TA_VCENTER;
	
	r = widget_Create(WIDGET_BUTTON, pButton, id, hParent);
	
	return r;
}

/*
*********************************************************************************************************
*                           button_CreateEx               
*
* Description: 创建一个按钮控件
*             
* Arguments  : *pButton: 按钮结构体指针
*							 hParent: 父窗口句柄
*
* Reutrn     : 成功时返回按钮句柄
*
* Note(s)    : None.
*********************************************************************************************************
*/
BUTTON_Handle button_CreateEx(BUTTON_OBJ *pButton, uint8_t id, WM_HWIN hParent)
{
	if(!hParent) return 0;
	
	return widget_Create(WIDGET_BUTTON, pButton, id, hParent);
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
GUI_ERROR button_Delete(BUTTON_Handle *hButton)
{
	if(*hButton == 0) return ERR_PARA;
	
	return widget_Delete(hButton);
}
/********************************************  END OF FILE  *******************************************/
