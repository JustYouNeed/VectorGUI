/**
  *******************************************************************************************************
  * File Name: gui_wm.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-30
  * Brief: GUI���ڹ�����
  *******************************************************************************************************
  * History
  *		1.Author: Vetor
	*			Date: 2018-12-24
	*			Mod: �����ļ�
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
	if(!pWin) return -1;	/* �������Ϸ� */
	
	if(pWin->status)		/* �����������ʾ����ҵ���ͷ */
	{
		pWin->pNext = WM_Desktop;
		WM_Desktop = pWin;
		hDesk = WM_Desktop->hWin;
		//�����ػ���Ϣ
	}
	else	/* ����Ҫ������ʾ��ҵ��ڶ���λ�� */
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
* Description: ��ȡ���ڹ������ṹ��
*             
* Arguments  : hWin: ���ڹ��������
*							 *pObj: ���ڹ������ṹ��ָ��
*
* Reutrn     : ����ִ�н��,��ض��忴gui_types.h�еĶ���
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
	
	/* �������� */
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
* Description: ���ڹ�������ʼ��,���ٵ���һ�����ڹ�����
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
* Description: ����һ�����ڹ�����
*             
* Arguments  : x0, y0: ���ڹ���������ʼ����
*							 width, height: ���ڹ������ĳ���
*					     status: �մ���ʱ���ڹ�������״̬
*
* Reutrn     : ���ڹ������ľ��
*
* Note(s)    : None.
*********************************************************************************************************
*/
WM_HWIN	wm_Create(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t status)
{
	WM_OBJ *pWin = (WM_OBJ*)bsp_mem_Alloc(SRAMIN, sizeof(WM_OBJ));	/* �������ڴ� */
	
	if(!pWin) return -1;	/* �ڴ�����ʧ��ֱ�ӷ��� */
	
	/* ���ô��ڴ�С,�ڳ�ʼ�汾�У����ڴ�С��Ϊ��Ļ��С */
	pWin->rect.x0 = x0;
	pWin->rect.y0 = y0;
	pWin->rect.width = width;
	pWin->rect.height = height;
	pWin->noOfWidget = 0;		/* ��ʼ�ؼ�����Ϊ0 */
	
	pWin->pNext = NULL;	/* ��һ������Ϊ�� */
	pWin->status = status;	/* ����״̬ */
	pWin->pWidget = NULL;		/* û���κοؼ� */
		
	wm_addWindowToList(pWin);	/* �ҵ����������� */
	
	wm_NumWindows++;		/* ������������ */
	wm_HWIN++;
	pWin->hWin = wm_HWIN;	/* ���ô��ھ�� */
	
	/* �򴰿ڷ��ʹ�����Ϣ */
	msg_sendMsgNoData(pWin->hWin, MSG_CREATE);
	
	return pWin->hWin;	/* ���ش��ڱ�� */
}

/*
*********************************************************************************************************
*                        wm_Delete                 
*
* Description: ɾ��һ�����ڹ�����
*             
* Arguments  : hWin: ���ڴ��������
*
* Reutrn     : ɾ���Ĵ��ھ��
*
* Note(s)    : None.
*********************************************************************************************************
*/
GUI_ERROR wm_Delete(WM_HWIN *hWin)
{
	if(!hWin) return ERR_PARA;
	
	WM_OBJ *preObj = NULL, *lstObj = NULL;
	
	lstObj = preObj = WM_Desktop;
	
	if(WM_Desktop->hWin == *hWin)	/* Ҫɾ���Ĵ��ھ��ǵ�ǰ���� */
	{
		lstObj = WM_Desktop;
		WM_Desktop = WM_Desktop->pNext;	/* �����洰��ָ����һ�����ھ����� */
	}
	else
	{
		lstObj = WM_Desktop->pNext;
		while(lstObj)
		{
			if(lstObj->hWin == *hWin)	/* �Ѿ��ҵ���Ŀ�괰�� */
			{
				preObj->pNext = lstObj->pNext;	/* ���ô��ڽڵ�������жϿ� */
				break;
			}
			preObj = lstObj;	/* ������һ������ */
			lstObj = lstObj->pNext;	/* ָ����һ������ */
		}
	}
	
	/* ����ҵ��˽ڵ� */
	if(lstObj)
	{
		if(lstObj->pWidget)	/* ��Ҫ��ɾ���ô����µĿؼ� */
		{
			widget_Delete(lstObj->pWidget->id);	/*ɾ���ô����µĿؼ� */
		}
		bsp_mem_Free(SRAMIN, lstObj);	/* �ͷ��ڴ� */
		*hWin = 0;
		return 0;
	}
	
	return -1;
}

/*
*********************************************************************************************************
*                           wm_pocess               
*
* Description: ���ڹ���������������Ҫ�����ڼ����ڿؼ����ػ�
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
	
	/* ֻ�������� */
	if(WM_Desktop->pWidget)	/* ����ô���������ڿؼ�������Ҫ���ƿؼ� */
	{
		pWidget = WM_Desktop->pWidget;	/* ��ȡ�ؼ������ͷ */
		while(pWidget)		/* �����ô��ڵĿؼ�,���ػ� */
		{
			msg_sendMsgNoData(pWidget->id, MSG_PAINT);
//			widget_onPaint(pWidget);
			pWidget = (struct WIDGET_OBJ *)(pWidget->pNext);	/* ��ȡ��һ���ؼ� */
		}
	}
//	gui_Refresh();	/* ������Ļ */
}

/*
*********************************************************************************************************
*                            wm_defaultProc              
*
* Description: WM��Ĭ����Ϣ������
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
		case MSG_KEY: 	/* �����ǰ������µ���Ϣ����֪ͨ��Ӧ�ð����Ŀؼ� */
		{
			WIDGET_OBJ *pObj = pWin->pWidget;	/* �Ȼ�ȡ�ؼ�����ͷ */
			while(pObj)	/* ѭ��֪ͨÿһ���ؼ� */
			{
				if(pObj->actKey & pMsg->data.v)	/* ������Ǹÿؼ�����Ӧ�İ�����������Ӧ */
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

