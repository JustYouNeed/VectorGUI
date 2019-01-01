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
	if(!pWin) return -1;	/* �������Ϸ� */
	
	if(pWin->status)		/* �����������ʾ����ҵ���ͷ */
	{
		pWin->pNext = WM_Desktop;
		WM_Desktop = pWin;
		
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
	
	/* �������� */
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
* Description: ����һ������
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
			widget_Delete(&(lstObj->pWidget->id));	/*ɾ���ô����µĿؼ� */
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
void wm_pocess(void)
{
	WM_OBJ *pWin = NULL;
	struct WIDGET_OBJ *pWidget = NULL;
	
	pWin = WM_Desktop;	/* ��ȡ���������ͷ */
		
	if(pWin == NULL) return ;	/* ��ͷΪ�յĻ�ֱ�ӷ��� */
	
	gui_clear();
	
//	while(pWin)	/* �ػ������������� */
//	{
		if(pWin->pWidget)	/* ����ô���������ڿؼ�������Ҫ���ƿؼ� */
		{
			pWidget = pWin->pWidget;	/* ��ȡ�ؼ������ͷ */
			while(pWidget)		/* �����ô��ڵĿؼ�,���ػ� */
			{
				widget_onPaint(pWidget);
				pWidget = (struct WIDGET_OBJ *)(pWidget->pNext);	/* ��ȡ��һ���ؼ� */
			}
		}
//		pWin = pWin->pNext;		/* �õ���һ�����ڽṹ�� */
//	}
	
	gui_Refresh();	/* ������Ļ */
}


/********************************************  END OF FILE  *******************************************/

