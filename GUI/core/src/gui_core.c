/**
  *******************************************************************************************************
  * File Name: gui_core.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-30
  * Brief: GUI内核文件
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


GUI_CONTEXT GUI_Context;

/*
*********************************************************************************************************
*                          gui_gotoXY                
*
* Description: 跳转到指定坐标
*             
* Arguments  : x,y: 指定坐标
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_gotoXY(uint16_t x, uint16_t y)
{
	GUI_Context.dispPosX = x;
	GUI_Context.dispPosY = y;
}

/*
*********************************************************************************************************
*                        gui_Init                  
*
* Description: GUI初始化函数
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_Init(void)
{
	gui_deviceInit();
//	wm_Init();
	
	GUI_Context.curFont = GUI_FONT_ASCII_6X8;
	GUI_Context.dispPosX = 0;
	GUI_Context.dispPosY = 0;
	GUI_Context.drawPosX = 0;
	GUI_Context.drawPosY = 0;
	GUI_Context.drawRect.x0 = 0;
	GUI_Context.drawRect.y0 = 0;
	GUI_Context.drawRect.width = 127;
	GUI_Context.drawRect.height = 63;
}

/*
*********************************************************************************************************
*                              gui_clear            
*
* Description: GUI清屏函数
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_clear(void)
{
	gui_fillSCR(0);
}

/*
*********************************************************************************************************
*                         gui_clearRect                 
*
* Description: 清除一块矩形区域
*             
* Arguments  : x0,y0: 矩形区域左上角点的坐标
*							 width, height: 矩形区域长宽
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_clearRect(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height)
{
	gui_fillRectangle(x0, y0, x0 + width, y0 + height, 0);
}

void gui_onPaint()
{
	if(WIN_Desktop == NULL) return ;
	WIDGET_OBJ *pWidget = NULL;
	/* 只绘制桌面 */
	if(WIN_Desktop->isInvalid)	/* 只有在窗口无效的时候才需要绘制 */
	{
		gui_clear();
		win_onPaint(WIN_Desktop);	/* 先绘制窗口 */
		pWidget = WIN_Desktop->pWidget;	/* 获取控件链表表头 */
		while(pWidget)		/* 遍历该窗口的控件,并重绘 */
		{
			widget_onPaint(pWidget);
			pWidget = (pWidget->pNext);	/* 获取下一个控件 */
		}
		WIN_Desktop->isInvalid = false;	/* 窗口设置为有效 */
		gui_Refresh();	/* 更新屏幕 */
	}
}

/*
*********************************************************************************************************
*                      gui_excute                    
*
* Description: GUI线程执行函数，负责GUI的重绘以及消息的处理
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_excute(void)
{
//	gui_clear();
	gui_onPaint();
	msg_process();
//	gui_Refresh();	/* 更新屏幕 */
}


/********************************************  END OF FILE  *******************************************/

