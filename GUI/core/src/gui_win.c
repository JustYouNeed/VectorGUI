/**
  *******************************************************************************************************
  * File Name: gui_win.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2019-1-29
  * Brief: 窗口管理器
  *******************************************************************************************************
  * History
	*		1.Author: Vector
	*			Date: 2019-1-29
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

/*
  *******************************************************************************************************
  *                              LOCAL VARIABLE
  *******************************************************************************************************
*/
static uint16_t GUI_NumWindows = 0;
WIN_OBJ *WIN_Desktop = NULL, *WIN_HEAD = NULL;

/*
*********************************************************************************************************
*                         win_onPaint                 
*
* Description: 绘制窗口
*             
* Arguments  : *pWin: 要绘制的窗口结构体指针
*
* Reutrn     : None.
*
* Note(s)    : 该函数不应该由用户调用
*********************************************************************************************************
*/
void win_onPaint(const WIN_OBJ *pWin)
{
	int16_t XOffset = 0, YOffset = 0;
	
	GUI_Context.drawRect.x0 = pWin->rect.x0;
	GUI_Context.drawRect.y0 = pWin->rect.y0;
	GUI_Context.drawRect.width = pWin->rect.width;
	GUI_Context.drawRect.height = pWin->rect.height;
	
	if(pWin->flag & GUI_CF_FULL) return ;	/* 如果是全屏模式则不需要绘制标题 */
	
	gui_fillRectangle(pWin->rect.x0, pWin->rect.y0, pWin->rect.x0 + pWin->rect.width, pWin->rect.y0 + 14, 1);
	gui_drawRectangle(pWin->rect.x0, pWin->rect.y0, pWin->rect.x0 + pWin->rect.width, pWin->rect.y0 + pWin->rect.height, 1);
	
	XOffset = (pWin->rect.width - 6*gui_getStringLen(pWin->title))/2;
	YOffset = (14 - font_getYSize(GUI_Context.curFont))/2;
	
	gui_gotoXY(pWin->rect.x0 + XOffset, pWin->rect.y0 + YOffset);
	gui_dispString(pWin->title, 0);
}

/*
*********************************************************************************************************
*                      win_getObject                    
*
* Description: 获取窗口管理器结构体
*             
* Arguments  : hWin: 窗口管理器句柄
*							 *pObj: 窗口管理器结构体指针
*
* Reutrn     : 函数执行结果,相关定义看gui_types.h中的定义
*
* Note(s)    : None.
*********************************************************************************************************
*/
WIN_OBJ *win_getObject(WIN_Handle hWin, int16_t *err)
{	
	if(!hWin) 
	{
		*err = ERR_PARA;
		return NULL;
	}
	
	WIN_OBJ *pWin = WIN_HEAD;
	
	/* 查找链表 */
	while(pWin)
	{
		if(pWin->hWin ==  hWin) break;
		pWin = pWin->pNext;
	}
	
	*err = ERR_NONE;
	return pWin;
}

/*
*********************************************************************************************************
*                          win_addWindowToList                
*
* Description: 将一个窗口挂接到窗口链表中
*             
* Arguments  : *pWin: 要挂接的窗口指针
*
* Reutrn     : 0：挂接成功
*						   -1：挂接失败
*
* Note(s)    : 该函数不应该由用户调用
*********************************************************************************************************
*/
static int16_t win_addWindowToList(WIN_OBJ *pWin)
{
	if(!pWin) return -1;	/* 参数不合法 */
	
	/* 如果需要立即显示则设置为桌面 */
	if(pWin->flag & GUI_CF_SHOW) WIN_Desktop = pWin;
	
	/* 直接挂接到表头 */
	pWin->pNext = WIN_HEAD;
	WIN_HEAD = pWin;
	
	return 0;
}

/*
*********************************************************************************************************
*                        win_Invalidation                  
*
* Description: 使窗口无效化，窗口将在下次重绘
*             
* Arguments  : hWin: 要无效化的窗口句柄
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void win_Invalidation(WIN_Handle hWin)
{
	WIN_OBJ *pWin = NULL;
	int16_t err = ERR_NONE;
	
	if(!hWin) return ;
	
	pWin = win_getObject(hWin, &err);
	
	pWin->isInvalid = true;
}

/*
*********************************************************************************************************
*                           win_Create               
*
* Description: 创建一个窗口
*             
* Arguments  : id: 窗口id,同时也是该窗口的句柄
*							 *title: 窗口标题
*							 flag: 创建窗口标志位		
*	
* Reutrn     : 窗口句柄
*
* Note(s)    : None.
*********************************************************************************************************
*/
WIN_Handle win_Create(uint8_t id, uint8_t *title, GUI_CF_FALG flag)
{
	WIN_OBJ *pWin = (WIN_OBJ*)bsp_mem_Alloc(SRAMIN, sizeof(WIN_OBJ));	/* 先申请内存 */
	
	if(!pWin) return -1;	/* 内存申请失败直接返回 */
	
	/* 设置窗口大小,在初始版本中，窗口大小则为屏幕大小 */
	pWin->rect.x0 = 0;
	pWin->rect.y0 = 0;
	pWin->rect.width = LCD_X - 1;
	pWin->rect.height = LCD_Y - 1;
	pWin->title = title;
	pWin->noOfWidget = 0;		/* 初始控件数量为0 */
	pWin->pNext = NULL;	/* 下一个窗口为空 */
	pWin->pWidget = NULL;		/* 没有任何控件 */
	pWin->hWin = id;	/* 设置窗口句柄 */
	pWin->flag = flag;
	pWin->isInvalid = true;
	
	/* 如果该窗口有标题栏 */
	if(pWin->flag & GUI_CF_HALF)
	{
		pWin->clientRect.x0 = pWin->rect.x0 + 1;
		pWin->clientRect.y0 = pWin->rect.y0 + 14;
		pWin->clientRect.width = pWin->rect.width - 2;
		pWin->clientRect.height = pWin->rect.height - 14 - 1;
	}
	else
	{
		pWin->clientRect.x0 = pWin->rect.x0;
		pWin->clientRect.y0 = pWin->rect.y0;
		pWin->clientRect.width = pWin->rect.width;
		pWin->clientRect.height = pWin->rect.height;
	}
		
	win_addWindowToList(pWin);	/* 挂到窗口链表中 */
	
	GUI_NumWindows++;		/* 窗口数量增加 */
	
	return pWin->hWin;	/* 返回窗口编号 */
}

/*
*********************************************************************************************************
*                       win_Delete                    
*
* Description: 删除一个窗口
*             
* Arguments  : *hWin: 要删除的窗口句柄指针
*
* Reutrn     : 执行结果
*
* Note(s)    : None.
*********************************************************************************************************
*/
int16_t win_Delete(WIN_Handle *hWin)
{
	if(!hWin) return ERR_PARA;
	int16_t err = ERR_NONE;
	
	WIN_OBJ *preObj = NULL, *lstObj = NULL;
		
	if(WIN_HEAD->hWin == *hWin)	/* 要删除的窗口就是表头 */
	{
		lstObj = WIN_HEAD;
		while(lstObj->pWidget)	/* 需要先删除该窗口下的控件 */
		{
			widget_Delete(lstObj->pWidget->id);	/*删除该窗口下的控件 */
		}
		WIN_HEAD = WIN_HEAD->pNext;	/* 将桌面窗口指向下一个窗口就行了 */
	}
	else
	{
		preObj = WIN_HEAD;
		lstObj = WIN_HEAD->pNext;
		while(lstObj)
		{
			if(lstObj->hWin == *hWin)	/* 已经找到了目标窗口 */
			{
				preObj->pNext = lstObj->pNext;	/* 将该窗口节点从链表中断开 */
				break;
			}
			preObj = lstObj;	/* 保存上一个窗口 */
			lstObj = lstObj->pNext;	/* 指向下一个窗口 */
		}
		
		while(lstObj->pWidget)	/* 需要先删除该窗口下的控件 */
		{
			widget_Delete(lstObj->pWidget->id);	/*删除该窗口下的控件 */
		}
	}
	
	/* 如果找到了节点 */
	if(lstObj)
	{
		bsp_mem_Free(SRAMIN, lstObj);	/* 释放内存 */
		*hWin = 0;
		return 0;
	}
	
	return -1;
}

/*
*********************************************************************************************************
*                           win_boringToTop               
*
* Description: 将一个窗口放到最顶上，也就是桌面显示的窗口
*             
* Arguments  : hWin: 要设置为桌面窗口的窗口句柄
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
int16_t win_boringToTop(WIN_Handle hWin)
{
	if(!hWin)
		return -1;
	int16_t err = ERR_NONE;
	
	WIN_Desktop = win_getObject(hWin, &err);	
	WIN_Desktop->isInvalid = true;	/* 窗口无效化，需要重绘 */
	
	return err;
}

/********************************************  END OF FILE  *******************************************/

