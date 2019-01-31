/**
  *******************************************************************************************************
  * File Name: gui_progbar.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: GUI进度条控件
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-30
	*			Mod: 建立文件
	*
	*		2.Author: Vector
	*			Date: 2019-1-31
	*			Mod: 完成基本函数
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
*                      progbar_getObject                    
*
* Description: 获取进度条控件结构体
*             
* Arguments  : hProgbar: 进度条句柄
*						   *err: 用于保存执行结果
*
* Reutrn     : 进度条控件控件结构体
*
* Note(s)    : None.
*********************************************************************************************************
*/
static PROGBAR_OBJ *progbar_getObject(PROGBAR_Handle hProgbar, int16_t *err)
{
	if(!hProgbar)
	{
		*err = ERR_PARA;
		return NULL;
	}
	
	WIDGET_OBJ *pWidget = NULL;
	
		/* 获取按钮控件 */
	pWidget = widget_getWidget(hProgbar, err);	
	if(!pWidget) 
	{
		*err = ERR_PARA;
		return NULL;
	}
	
	return (PROGBAR_OBJ *)(pWidget->widgetData);
}

/*
*********************************************************************************************************
*                       progbar_onPaint                   
*
* Description: progbar绘制函数
*             
* Arguments  : *pProgbar: Progbar结构体指针
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void progbar_onPaint(const PROGBAR_OBJ* pProgbar)
{
	if(!pProgbar) return ;
	char ratio[4];
	
	float width = 0;
	
	width = (float)((pProgbar->width-1) * pProgbar->ratio / 100);
	
	gui_drawRectangle(pProgbar->x, pProgbar->y, pProgbar->x + pProgbar->width, pProgbar->y + pProgbar->height, 1);
	gui_fillRectangle(pProgbar->x + 1, pProgbar->y + 1, pProgbar->x + width, pProgbar->y + pProgbar->height-1, 1);
	gui_gotoXY(pProgbar->x + pProgbar->width + 2, pProgbar->y + 2);
	
	sprintf(ratio, "%d%s", pProgbar->ratio, "%");
	gui_dispString((uint8_t*)ratio, 1);
}

/*
*********************************************************************************************************
*                   progbar_Create                       
*
* Description: 创建一个进度条控件
*             
* Arguments  : x, y: 进度条控件的坐标
*						   width, height: 进度条控件的长宽
*						   id: 进度条控件的id，必须保证Id唯一
*					     hParent: 进度条控件父窗口
*
* Reutrn     : 进度条句柄
*
* Note(s)    : None.
*********************************************************************************************************
*/
PROGBAR_Handle progbar_Create(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t id, WIN_Handle hParent)
{
	if(!hParent) return 0;
	
	int16_t err = ERR_NONE;
	PROGBAR_OBJ *pProgbar = NULL;
	
	pProgbar = (PROGBAR_OBJ *)bsp_mem_Alloc(SRAMIN, sizeof(PROGBAR_OBJ));
	if(!pProgbar) return 0;
	
	pProgbar->x = x;
	pProgbar->y = y;
	pProgbar->width = width;
	pProgbar->height = height;
	pProgbar->ratio = 0;
	
		/* 将菜单插入到控件列表中 */
	err = widget_Create(WIDGET_PROGBAR, pProgbar, id, 0, NULL, hParent);
	return err;
}

/*
*********************************************************************************************************
*                        progbar_increaseRatio                  
*
* Description: 增加进度条的值
*             
* Arguments  : hProgbar: 进度条句柄
*							 value: 要增加的值，可正可负
*
* Reutrn     : 执行结果
*
* Note(s)    : None.
*********************************************************************************************************
*/
GUI_ERROR progbar_increaseRatio(PROGBAR_Handle hProgbar, int8_t value)
{
	if(!hProgbar) return ERR_PARA;
	
	int16_t err = ERR_NONE;
	
	PROGBAR_OBJ *pProgbar = progbar_getObject(hProgbar, &err);
	if(err != ERR_NONE) return err;
	
	/* 限幅	*/
	if(pProgbar->ratio + value < 0)
		pProgbar->ratio = 0;
	else if(pProgbar->ratio + value> 100)
		pProgbar->ratio = 100;
	else
		pProgbar->ratio += value;
	
	/* 无效化窗口，引起窗口重绘 */
	win_Invalidation(hProgbar >> 10);
	return ERR_NONE;
}

/*
*********************************************************************************************************
*                        progbar_getRatio                  
*
* Description: 获取进度条的值
*             
* Arguments  : hProgbar: 进度条句柄
*
* Reutrn     : 进度条值
*
* Note(s)    : None.
*********************************************************************************************************
*/
uint8_t progbar_getRatio(PROGBAR_Handle hProgbar)
{
	if(!hProgbar) return 0;
	
	int16_t err = ERR_NONE;
	
	PROGBAR_OBJ *pProgbar = progbar_getObject(hProgbar, &err);
	if(err != ERR_NONE) return err;
	
	return pProgbar->ratio;
}


/********************************************  END OF FILE  *******************************************/
