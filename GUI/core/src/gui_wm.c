/**
  *******************************************************************************************************
  * File Name: gui_wm.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-30
  * Brief: GUI窗口管理部分
  *******************************************************************************************************
  * History
  *		1.Author: Vetor
	*			Date: 2018-12-24
	*			Mod: 建立文件
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
static uint16_t wm_NumWindows = 0;
static uint16_t wm_HWIN = 0;
static WM_HWIN hDesk = 0;
static WM_OBJ *WM_Desktop = NULL;


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
static WM_HWIN wm_addWindowToList(WM_OBJ *pWin)
{
	if(!pWin) return -1;	/* 参数不合法 */
	
	if(pWin->status)		/* 如果是立即显示，则挂到表头 */
	{
		pWin->pNext = WM_Desktop;
		WM_Desktop = pWin;
		
		//发送重绘消息
	}
	else	/* 不需要立即显示则挂到第二个位置 */
	{
		pWin->pNext = WM_Desktop->pNext;
		WM_Desktop->pNext = pWin;
	}
	
	return pWin->hWin;
}


/*
*********************************************************************************************************
*                      wm_getWindowObject                    
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
GUI_ERROR wm_getWindowObject(WM_HWIN hWin, WM_OBJ *pObj)
{
	pObj = WM_Desktop;
	
	if(!hWin) return ERR_PARA;
	
	/* 查找链表 */
	while(pObj)
	{
		if(pObj->hWin ==  hWin) break;
		pObj = pObj->pNext;
	}
	
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
void wm_Init(void)
{
	hDesk = wm_Create(0, 0, LCD_X -1 , LCD_Y - 1, 1);
}

/*
*********************************************************************************************************
*                         wm_createWindow                 
*
* Description: 创建一个窗口
*             
* Arguments  : 
*
* Reutrn     : 
*
* Note(s)    : 
*********************************************************************************************************
*/
WM_HWIN	wm_Create(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t status)
{
	WM_OBJ *pWin = (WM_OBJ*)bsp_mem_Alloc(SRAMIN, sizeof(WM_OBJ));	/* 先申请内存 */
	
	if(!pWin) return -1;	/* 内存申请失败直接返回 */
	
	/* 设置窗口大小,在初始版本中，窗口大小则为屏幕大小 */
	pWin->rect.x0 = x0;
	pWin->rect.y0 = y0;
	pWin->rect.width = width;
	pWin->rect.height = height;
	pWin->noOfWidget = 0;		/* 初始控件数量为0 */
	
	pWin->pNext = NULL;	/* 下一个窗口为空 */
	pWin->status = status;	/* 设置状态 */
	pWin->pWidget = NULL;		/* 没有任何控件 */
		
	wm_addWindowToList(pWin);	/* 挂到窗口链表中 */
	
	wm_NumWindows++;		/* 窗口数量增加 */
	wm_HWIN++;
	pWin->hWin = wm_HWIN;	/* 设置窗口句柄 */
	
	return pWin->hWin;	/* 返回窗口编号 */
}

/*
*********************************************************************************************************
*                        wm_Delete                 
*
* Description: 删除一个窗口管理器
*             
* Arguments  : hWin: 窗口处理器句柄
*
* Reutrn     : 删除的窗口句柄
*
* Note(s)    : None.
*********************************************************************************************************
*/
GUI_ERROR wm_Delete(WM_HWIN *hWin)
{
	if(!hWin) return ERR_PARA;
	
	WM_OBJ *preObj = NULL, *lstObj = NULL;
	
	lstObj = preObj = WM_Desktop;
	
	if(WM_Desktop->hWin == *hWin)	/* 要删除的窗口就是当前窗口 */
	{
		lstObj = WM_Desktop;
		WM_Desktop = WM_Desktop->pNext;	/* 将桌面窗口指向下一个窗口就行了 */
	}
	else
	{
		lstObj = WM_Desktop->pNext;
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
	}
	
	/* 如果找到了节点 */
	if(lstObj)
	{
		if(lstObj->pWidget)	/* 需要先删除该窗口下的控件 */
		{
			widget_Delete(&(lstObj->pWidget->id));	/*删除该窗口下的控件 */
		}
		bsp_mem_Free(SRAMIN, lstObj);	/* 释放内存 */
		*hWin = 0;
		return 0;
	}
	
	return -1;
}

/*
*********************************************************************************************************
*                           wm_pocess               
*
* Description: 窗口管理器处理函数，主要处理窗口及窗口控件的重绘
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void wm_pocess(void)
{
	WM_OBJ *pWin = NULL;
	struct WIDGET_OBJ *pWidget = NULL;
	
	pWin = WM_Desktop;	/* 获取窗口链表表头 */
		
	if(pWin == NULL) return ;	/* 表头为空的话直接返回 */
	
	gui_clear();
	
//	while(pWin)	/* 重绘整个窗口链表 */
//	{
		if(pWin->pWidget)	/* 如果该窗口下面存在控件，则需要绘制控件 */
		{
			pWidget = pWin->pWidget;	/* 获取控件链表表头 */
			while(pWidget)		/* 遍历该窗口的控件,并重绘 */
			{
				widget_onPaint(pWidget);
				pWidget = (struct WIDGET_OBJ *)(pWidget->pNext);	/* 获取下一个控件 */
			}
		}
//		pWin = pWin->pNext;		/* 得到下一个窗口结构体 */
//	}
	
	gui_Refresh();	/* 更新屏幕 */
}


/********************************************  END OF FILE  *******************************************/

