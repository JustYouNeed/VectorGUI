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
# include "gui_widget.h"
# include "gui_mem.h"
# include "gui_win.h"
# include "gui_core.h"

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
WIDGET_OBJ *widget_getWidget(WIDGET_Handle hWidget, int16_t *err)
{
	if(!hWidget) 
	{
		*err = ERR_PARA;
		return NULL;	/* �������� */
	}
	
	WIN_OBJ *pWin = NULL;
	WIDGET_OBJ *pWidget = NULL;
	
	pWin = win_getObject(GUI_GET_HPARENT(hWidget), err);	/* �õ��ÿؼ��ĸ����� */
	if(!pWin || !pWin->pWidget) return NULL;	/* ��ȡ������ʧ�ܻ��߸ô�����û�пؼ�������ʧ�� */
	
	/* ���ҿؼ� */
	pWidget = pWin->pWidget;
	while(pWidget)
	{
		if(pWidget->id == hWidget)
			break;
		
		pWidget = pWidget->pNext;
	}
	*err = ERR_NONE;
	return pWidget;
		
}

/*
*********************************************************************************************************
*                         widget_Create                 
*
* Description: ����һ���ؼ�
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
WIDGET_Handle widget_Create(WIDGET_TYPE widgetType, WIDGET_OBJ *pWidget, uint8_t id, uint16_t actKey, WIDGET_CALLBACK *_cb, WIN_Handle hParent)
{
	WIDGET_OBJ *widgetTemp = NULL;
	WIN_OBJ *pWin = NULL;
	uint8_t widgetLayer = 0;
	int16_t err = ERR_NONE;

	if(!hParent || !pWidget) return 0;
		
	/* ��д�ؼ����� */
	{
		pWidget->type = widgetType;
		pWidget->id = ((hParent << 10) | (widgetType << 6) | (id << 0));
		pWidget->pNext = NULL;	
		pWidget->actKey = actKey;
		pWidget->_cb = _cb;
		pWidget->layer = widgetLayer; /* ���ÿؼ��Ĳ� */
	}
	
	/* ���ؼ����ص������� */
	{
		pWin = win_getObject(hParent, &err);	/* �õ������ڵĽṹ��ָ�� */
		
		pWin->noOfWidget++;		/* �ô����µĿؼ��������� */
		pWin->isInvalid = true;
		
		if(pWin->pWidget == NULL)	/* ����ô��ڵ�ǰû�пؼ�,�͹ҽӸÿؼ� */
		{
			pWin->pWidget = pWidget;
			widgetLayer++;		
		}
		else
		{
			widgetTemp = (pWin->pWidget);	/* �õ��ô��ڵĿؼ����� */
			while(widgetTemp->pNext)	/* ���ұ�β */
			{
				widgetTemp = (struct WIDGET_OBJ *)(widgetTemp->pNext);
				widgetLayer++;
			}
			widgetTemp->pNext = (struct WIDGET_OBJ*)pWidget;	/* ���ÿؼ��ҽӵ������ڿؼ������� */
		}
	}
	return pWidget->id;
}

/*
*********************************************************************************************************
*                            widget_Delete              
*
* Description: ɾ��һ���ؼ�
*             
* Arguments  : hWidget: Ҫɾ���Ŀؼ����
*
* Reutrn     : ����ֵ��gui_types.h�ж���
*
* Note(s)    : ɾ���ɹ�Ϊ�ؼ����������
*********************************************************************************************************
*/
GUI_ERROR widget_Delete(WIDGET_Handle hWidget)
{
	WIDGET_OBJ *preWidget	= NULL, *lstWidget = NULL;
	WIN_OBJ *pWin = NULL;
	int16_t err = ERR_NONE;
	WIN_Handle hWin = GUI_GET_HPARENT(hWidget);
	
	/* ��ȡ�ÿؼ��ĸ����� */
	pWin = win_getObject(hWin, &err);
	pWin->isInvalid = true;
	
	if(!pWin) return ERR_PARA;
	lstWidget = preWidget = pWin->pWidget;
	
	/* ����ǵ�һ���ؼ� */
	if(preWidget->id == hWidget)	
	{
		pWin->pWidget = preWidget->pNext;	/* ��������ɾ�� */
	}
	else	/* ���ǵ�һ�� */
	{
		lstWidget = preWidget->pNext;
		while(lstWidget)
		{
			if(lstWidget->id == hWidget)
			{
				preWidget->pNext = lstWidget->pNext;
				break;
			}
			preWidget = lstWidget;
			lstWidget = lstWidget->pNext;
		}
	}
	
	/* ����ҵ��� */
	if(lstWidget)
	{
		gui_memFree(lstWidget);
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
void widget_onPaint(struct WIDGET_OBJ *pWidget)
{
	if(!pWidget) return ;
	int16_t err = ERR_NONE;
	WIN_OBJ *pWin = win_getObject(GUI_GET_HPARENT(pWidget->id), &err);
	
	GUI_Context.drawRect.x0 = pWin->clientRect.x0;
	GUI_Context.drawRect.y0 = pWin->clientRect.y0;
	GUI_Context.drawRect.height = pWin->clientRect.height;
	GUI_Context.drawRect.width = pWin->clientRect.width;

	/* ���ݿؼ�����ѡ��ͬ�Ļ��ƺ��� */
	switch(pWidget->type)
	{
		case WIDGET_BUTTON: 	/* ��ť */
		{
			button_onPaint(pWidget->widgetData);		/* ���ư��� */
		}break;
		case WIDGET_CHECKBOX:
		{
			checkbox_onPaint(pWidget->widgetData);
		}break;
		case WIDGET_MENU:
		{
			menu_onPaint(pWidget->widgetData);
		}break;
		case WIDGET_PROGBAR:
		{
			progbar_onPaint(pWidget->widgetData);
		}break;
		case WIDGET_SCROLLBAR:
		{
			scrollbar_onPaint(pWidget->widgetData);
		}break;
	}
}

/********************************************  END OF FILE  *******************************************/


