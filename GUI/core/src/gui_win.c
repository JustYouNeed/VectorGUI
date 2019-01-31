/**
  *******************************************************************************************************
  * File Name: gui_win.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2019-1-29
  * Brief: ���ڹ�����
  *******************************************************************************************************
  * History
	*		1.Author: Vector
	*			Date: 2019-1-29
	*			Mod: �����ļ�
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
* Description: ���ƴ���
*             
* Arguments  : *pWin: Ҫ���ƵĴ��ڽṹ��ָ��
*
* Reutrn     : None.
*
* Note(s)    : �ú�����Ӧ�����û�����
*********************************************************************************************************
*/
void win_onPaint(const WIN_OBJ *pWin)
{
	int16_t XOffset = 0, YOffset = 0;
	
	GUI_Context.drawRect.x0 = pWin->rect.x0;
	GUI_Context.drawRect.y0 = pWin->rect.y0;
	GUI_Context.drawRect.width = pWin->rect.width;
	GUI_Context.drawRect.height = pWin->rect.height;
	
	if(pWin->flag & GUI_CF_FULL) return ;	/* �����ȫ��ģʽ����Ҫ���Ʊ��� */
	
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
WIN_OBJ *win_getObject(WIN_Handle hWin, int16_t *err)
{	
	if(!hWin) 
	{
		*err = ERR_PARA;
		return NULL;
	}
	
	WIN_OBJ *pWin = WIN_HEAD;
	
	/* �������� */
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
* Description: ��һ�����ڹҽӵ�����������
*             
* Arguments  : *pWin: Ҫ�ҽӵĴ���ָ��
*
* Reutrn     : 0���ҽӳɹ�
*						   -1���ҽ�ʧ��
*
* Note(s)    : �ú�����Ӧ�����û�����
*********************************************************************************************************
*/
static int16_t win_addWindowToList(WIN_OBJ *pWin)
{
	if(!pWin) return -1;	/* �������Ϸ� */
	
	/* �����Ҫ������ʾ������Ϊ���� */
	if(pWin->flag & GUI_CF_SHOW) WIN_Desktop = pWin;
	
	/* ֱ�ӹҽӵ���ͷ */
	pWin->pNext = WIN_HEAD;
	WIN_HEAD = pWin;
	
	return 0;
}

/*
*********************************************************************************************************
*                        win_Invalidation                  
*
* Description: ʹ������Ч�������ڽ����´��ػ�
*             
* Arguments  : hWin: Ҫ��Ч���Ĵ��ھ��
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
* Description: ����һ������
*             
* Arguments  : id: ����id,ͬʱҲ�Ǹô��ڵľ��
*							 *title: ���ڱ���
*							 flag: �������ڱ�־λ		
*	
* Reutrn     : ���ھ��
*
* Note(s)    : None.
*********************************************************************************************************
*/
WIN_Handle win_Create(uint8_t id, uint8_t *title, GUI_CF_FALG flag)
{
	WIN_OBJ *pWin = (WIN_OBJ*)bsp_mem_Alloc(SRAMIN, sizeof(WIN_OBJ));	/* �������ڴ� */
	
	if(!pWin) return -1;	/* �ڴ�����ʧ��ֱ�ӷ��� */
	
	/* ���ô��ڴ�С,�ڳ�ʼ�汾�У����ڴ�С��Ϊ��Ļ��С */
	pWin->rect.x0 = 0;
	pWin->rect.y0 = 0;
	pWin->rect.width = LCD_X - 1;
	pWin->rect.height = LCD_Y - 1;
	pWin->title = title;
	pWin->noOfWidget = 0;		/* ��ʼ�ؼ�����Ϊ0 */
	pWin->pNext = NULL;	/* ��һ������Ϊ�� */
	pWin->pWidget = NULL;		/* û���κοؼ� */
	pWin->hWin = id;	/* ���ô��ھ�� */
	pWin->flag = flag;
	pWin->isInvalid = true;
	
	/* ����ô����б����� */
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
		
	win_addWindowToList(pWin);	/* �ҵ����������� */
	
	GUI_NumWindows++;		/* ������������ */
	
	return pWin->hWin;	/* ���ش��ڱ�� */
}

/*
*********************************************************************************************************
*                       win_Delete                    
*
* Description: ɾ��һ������
*             
* Arguments  : *hWin: Ҫɾ���Ĵ��ھ��ָ��
*
* Reutrn     : ִ�н��
*
* Note(s)    : None.
*********************************************************************************************************
*/
int16_t win_Delete(WIN_Handle *hWin)
{
	if(!hWin) return ERR_PARA;
	int16_t err = ERR_NONE;
	
	WIN_OBJ *preObj = NULL, *lstObj = NULL;
		
	if(WIN_HEAD->hWin == *hWin)	/* Ҫɾ���Ĵ��ھ��Ǳ�ͷ */
	{
		lstObj = WIN_HEAD;
		while(lstObj->pWidget)	/* ��Ҫ��ɾ���ô����µĿؼ� */
		{
			widget_Delete(lstObj->pWidget->id);	/*ɾ���ô����µĿؼ� */
		}
		WIN_HEAD = WIN_HEAD->pNext;	/* �����洰��ָ����һ�����ھ����� */
	}
	else
	{
		preObj = WIN_HEAD;
		lstObj = WIN_HEAD->pNext;
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
		
		while(lstObj->pWidget)	/* ��Ҫ��ɾ���ô����µĿؼ� */
		{
			widget_Delete(lstObj->pWidget->id);	/*ɾ���ô����µĿؼ� */
		}
	}
	
	/* ����ҵ��˽ڵ� */
	if(lstObj)
	{
		bsp_mem_Free(SRAMIN, lstObj);	/* �ͷ��ڴ� */
		*hWin = 0;
		return 0;
	}
	
	return -1;
}

/*
*********************************************************************************************************
*                           win_boringToTop               
*
* Description: ��һ�����ڷŵ���ϣ�Ҳ����������ʾ�Ĵ���
*             
* Arguments  : hWin: Ҫ����Ϊ���洰�ڵĴ��ھ��
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
	WIN_Desktop->isInvalid = true;	/* ������Ч������Ҫ�ػ� */
	
	return err;
}

/********************************************  END OF FILE  *******************************************/

