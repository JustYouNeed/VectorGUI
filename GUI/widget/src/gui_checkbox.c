/**
  *******************************************************************************************************
  * File Name: gui_checkbox.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: GUI窗口控件，部分函数参考正点原子遥控GUI中的程序，原作者为黄绍斌
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
# include "gui_checkbox.h"
# include "gui_widget.h"
# include "gui_mem.h"
# include "gui_core.h"
# include "gui_win.h"

/*
*********************************************************************************************************
*                          checkbox_onPaint                
*
* Description: Checkbox绘制函数
*             
* Arguments  : 
*
* Reutrn     : 
*
* Note(s)    : 
*********************************************************************************************************
*/
void checkbox_onPaint(const CHECKBOX_OBJ *pCheckbox)
{
	gui_drawCircle(15, 28, 8, 1);
	gui_fillCircle(15, 28, 5, 1);
	
	gui_drawCircle(15, 28+20, 8, 1);
	gui_fillCircle(15, 28+20, 5, 1);
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
CHECKBOX_Handle  checkbox_Create(uint16_t x0, uint16_t y0, uint8_t *text, uint16_t id, WIN_Handle hParent)
{
	CHECKBOX_OBJ *pCheckbox = NULL;
	WIDGET_OBJ *pWidget = NULL;
	uint8_t *pMem = NULL;
	int16_t r;
	
	pMem = (uint8_t *)gui_memAlloc(sizeof(CHECKBOX_OBJ) + sizeof(WIDGET_OBJ));
	if(!pMem) return 0;
	
	pWidget = (WIDGET_OBJ *)pMem;
	pCheckbox = (CHECKBOX_OBJ *)(pMem + sizeof(CHECKBOX_OBJ));
	
	pCheckbox->rect.x0 = x0;
	pCheckbox->rect.y0 = y0;
	
	pWidget->widgetData = (void *)pCheckbox;
	r = widget_Create(WIDGET_CHECKBOX, pWidget, id, GUI_KEY_NULL , NULL, hParent);	
	
	return r;
}

/********************************************  END OF FILE  *******************************************/

