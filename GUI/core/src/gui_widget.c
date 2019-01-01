/**
  *******************************************************************************************************
  * File Name: gui_widget.c
  * Author: Vector
  * Version: V1.0.1
  * Date: 2018-12-29
  * Brief: GUI�ؼ�������
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-30
	*			Mod: �����ļ�
	*
	*		2.Authot: Vector
	*			Date: 2019-1-1
	*			Mod: ����widget_addToParent,���ڹ��ؿؼ���������
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
*                    widget_getWidget                      
*
* Description: ���ݿؼ�������Ҹÿؼ��ṹ��
*             
* Arguments  : hWidget: �ؼ����
*
* Reutrn     : 0����ÿؼ�������
*
* Note(s)    : None.
*********************************************************************************************************
*/
GUI_ERROR widget_getWidget(WIDGET_HANDLE hWidget, WIDGET_OBJ *pWidget)
{
	if(!hWidget) return ERR_PARA;	/* �������� */
	
	WM_OBJ *pWin = NULL;

	wm_getWindowObject(hWidget>>8, pWin);	/* �õ��ÿؼ��ĸ����� */
	if(!pWin || !pWin->pWidget) return NULL;	/* ��ȡ������ʧ�ܻ��߸ô�����û�пؼ�������ʧ�� */
	
	/* ���ҿؼ� */
	pWidget = pWin->pWidget;
	while(pWidget)
	{
		if(pWidget->id == hWidget)
			break;
		
		pWidget = pWidget->pNext;
	}
	
	return (pWidget)?(ERR_NONE):(ERR_PARA);
		
}


/*
*********************************************************************************************************
*                         widget_addToParent                 
*
* Description: ���ؼ����ص���������
*             
* Arguments  : widgetType: �ؼ�����
*							 *pObj: ����ؼ�ָ��
*							 hParent: ���ھ��
*
* Reutrn     : ����0��ʾ���سɹ�,����ʧ��
*
* Note(s)    : None.
*********************************************************************************************************
*/
WIDGET_HANDLE widget_Create(WIDGET_TYPE widgetType, void *pObj, uint8_t id, WM_HWIN hParent)
{
	WIDGET_OBJ *pWidget = NULL, *widgetTemp = NULL;
	WM_OBJ *pWin = NULL;
	
	if(!hParent) return 0;
	if(!pObj) return 0;
	
	/* ����Ϊ�ؼ������ڴ� */
	pWidget = (struct WIDGET_OBJ*)bsp_mem_Alloc(SRAMIN, sizeof(struct WIDGET_OBJ));
	if(!pWidget) return 0;
	
	pWidget->type = widgetType;
	pWidget->id = (hParent << 8 | id);
	pWidget->pNext = NULL;
	
	/* ���ݿؼ��������ÿؼ������� */
	switch(widgetType)
	{
		case WIDGET_BUTTON:pWidget->widgetData = (BUTTON_OBJ*)pObj;	break;
		case WIDGET_CHECKBOX: pWidget->widgetData = (CHECKBOX_OBJ*)pObj;	break;
		case WIDGET_MENU:pWidget->widgetData = (MENU_OBJ*)pObj;	break;
		case WIDGET_PROGBAR:pWidget->widgetData = (PROGBAR_OBJ*)pObj;	break;
		case WIDGET_SCROLLBAR:pWidget->widgetData = (SCROLLBAR_OBJ*)pObj;	break;
		case WIDGET_WINDOW:pWidget->widgetData = (WINDOW_OBJ*)pObj;	break;
	}
		
	/* ���ؼ����ص������� */
	wm_getWindowObject(hParent, pWin);	/* �õ������ڵĽṹ��ָ�� */
	if(pWin->pWidget == NULL)	/* ����ô��ڵ�ǰû�пؼ�,�͹ҽӸÿؼ� */
	{
		pWin->pWidget = pWidget;
	}
	else
	{
		widgetTemp = (pWin->pWidget);	/* �õ��ô��ڵĿؼ����� */
		while(widgetTemp->pNext)	/* ���ұ�β */
		{
			widgetTemp = (struct WIDGET_OBJ *)(widgetTemp->pNext);
		}
		widgetTemp->pNext = (struct WIDGET_OBJ*)pWidget;	/* ���ÿؼ��ҽӵ������ڿؼ������� */
	}
	
	return pWidget->id;
}

/*
*********************************************************************************************************
*                            widget_Delete              
*
* Description: ɾ��һ���ؼ�
*             
* Arguments  : *hWidget: Ҫɾ���Ŀؼ����ָ��
*
* Reutrn     : ����ֵ��gui_types.h�ж���
*
* Note(s)    : ɾ���ɹ�Ϊ�ؼ����������
*********************************************************************************************************
*/
GUI_ERROR widget_Delete(WIDGET_HANDLE *hWidget)
{
	WIDGET_OBJ *preObj	= NULL, *lstObj = NULL;
	WM_OBJ *pObj = NULL;
	
	/* ��ȡ�ÿؼ��ĸ����� */
	wm_getWindowObject(*hWidget >> 8, pObj);
	if(!pObj) return ERR_PARA;
	lstObj = preObj = pObj->pWidget;
	
	/* ����ǵ�һ���ؼ� */
	if(preObj->id == *hWidget)	
	{
		pObj->pWidget = preObj->pNext;	/* ��������ɾ�� */
	}
	else	/* ���ǵ�һ�� */
	{
		lstObj = preObj->pNext;
		while(lstObj)
		{
			if(lstObj->id == *hWidget)
			{
				preObj->pNext = lstObj->pNext;
				break;
			}
			preObj = lstObj;
			lstObj = lstObj->pNext;
		}
	}
	
	/* ����ҵ��� */
	if(lstObj)
	{
		bsp_mem_Free(SRAMIN, lstObj->widgetData);	/* �ͷſؼ����� */
		bsp_mem_Free(SRAMIN, lstObj);
		*hWidget = 0;
	}
	
	return ERR_NONE;
}

/*
*********************************************************************************************************
*                        widget_onPaint                  
*
* Description: �ؼ��ػ溯��
*             
* Arguments  : *pWidget: �ؼ�����
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void widget_onPaint(const struct WIDGET_OBJ *pWidget)
{
	if(!pWidget) return ;
	
	/* ���ݿؼ�����ѡ��ͬ�Ļ��ƺ��� */
	switch(pWidget->type)
	{
		case WIDGET_BUTTON: 	/* ��ť */
		{
			BUTTON_OBJ *pButton = (BUTTON_OBJ*)(pWidget->widgetData);	/* �õ������ṹ�� */
			button_onPaint(pButton);		/* ���ư��� */
		}break;
		case WIDGET_CHECKBOX:
		{
			CHECKBOX_OBJ *pCheckbox = (CHECKBOX_OBJ *)(pWidget->widgetData);
			checkbox_onPaint(pCheckbox);
		}break;
		case WIDGET_MENU:
		{
			MENU_OBJ *pMenu = (MENU_OBJ *)(pWidget->widgetData);
			menu_onPaint(pMenu);
		}break;
		case WIDGET_PROGBAR:
		{
			PROGBAR_OBJ *pProgbar = (PROGBAR_OBJ *)(pWidget->widgetData);
			progbar_onPaint(pProgbar);
		}break;
		case WIDGET_SCROLLBAR:
		{
			SCROLLBAR_OBJ *pScrollbar = (SCROLLBAR_OBJ *)(pWidget->widgetData);
			scrollbar_onPaint(pScrollbar);
		}break;
		case WIDGET_WINDOW:		/* window�ؼ� */
		{
			WINDOW_OBJ *pWindow = (WINDOW_OBJ *)(pWidget->widgetData);
			window_onPaint(pWindow);
		}break;
	}
}

/********************************************  END OF FILE  *******************************************/


