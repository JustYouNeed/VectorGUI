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
WM_HWIN hDesk = 0;
WM_OBJ *WM_Desktop = NULL;


/*
*********************************************************************************************************
*                              wm_addWindowToList            
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
		hDesk = WM_Desktop->hWin;
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
WM_OBJ *wm_getWindowObject(WM_HWIN hWin, int16_t *err)
{	
	if(!hWin) 
	{
		*err = ERR_PARA;
		return NULL;
	}
	
	WM_OBJ *pObj = WM_Desktop;
	
	/* 查找链表 */
	while(pObj)
	{
		if(pObj->hWin ==  hWin) break;
		pObj = pObj->pNext;
	}
	
	return pObj;
}
/*
*********************************************************************************************************
*                     wm_Init                     
*
* Description: 窗口管理器初始化,至少得有一个窗口管理器
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
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
* Description: 创建一个窗口管理器
*             
* Arguments  : x0, y0: 窗口管理器的起始坐标
*							 width, height: 窗口管理器的长宽
*					     status: 刚创建时窗口管理器的状态
*
* Reutrn     : 窗口管理器的句柄
*
* Note(s)    : None.
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
	
	/* 向窗口发送创建消息 */
	msg_sendMsgNoData(pWin->hWin, MSG_CREATE);
	
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
			widget_Delete(lstObj->pWidget->id);	/*删除该窗口下的控件 */
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
void wm_onPaint(void)
{
	if(WM_Desktop == NULL) return ;
	
	struct WIDGET_OBJ *pWidget = NULL;
	
//	gui_clear();
	
	/* 只绘制桌面 */
	if(WM_Desktop->pWidget)	/* 如果该窗口下面存在控件，则需要绘制控件 */
	{
		pWidget = WM_Desktop->pWidget;	/* 获取控件链表表头 */
		while(pWidget)		/* 遍历该窗口的控件,并重绘 */
		{
			msg_sendMsgNoData(pWidget->id, MSG_PAINT);
//			widget_onPaint(pWidget);
			pWidget = (struct WIDGET_OBJ *)(pWidget->pNext);	/* 获取下一个控件 */
		}
	}
//	gui_Refresh();	/* 更新屏幕 */
}

/*
*********************************************************************************************************
*                            wm_defaultProc              
*
* Description: WM的默认消息处理函数
*             
* Arguments  : 
*
* Reutrn     : 
*
* Note(s)    : 
*********************************************************************************************************
*/
void wm_defaultProc(WM_MESSAGE *pMsg)
{
	WM_HWIN hWin = pMsg->hWin;
	const void *p = pMsg->data.p;
	int16_t err = ERR_NONE;
	
	WM_OBJ *pWin = wm_getWindowObject(hWin, &err);
	
	switch(pMsg->msgId)
	{
		case MSG_KEY: 	/* 如下是按键按下的消息，则通知响应该按键的控件 */
		{
			WIDGET_OBJ *pObj = pWin->pWidget;	/* 先获取控件链表头 */
			while(pObj)	/* 循环通知每一个控件 */
			{
				if(pObj->actKey & pMsg->data.v)	/* 如果该是该控件会响应的按键类型则响应 */
				{
					if(pObj->_cb)
						pObj->_cb(pMsg);
				}
				pObj = pObj->pNext;
			}
		}break;
		case MSG_GET_CLIENT_WINDOW:break;
		case MSG_PAINT:
		{
			
		}break;
	}
	
	pMsg->data.v = 0;
	pMsg->data.p = 0;
}


/********************************************  END OF FILE  *******************************************/

