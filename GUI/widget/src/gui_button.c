/**
  *******************************************************************************************************
  * File Name: gui_button.c
  * Author: Vector
  * Version: V1.0.1
  * Date: 2018-12-29
  * Brief: GUI�����ؼ������ֺ����ο�����ԭ��ң��GUI�еĳ���ԭ����Ϊ���ܱ�
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-30
	*			Mod: �����ļ�
	*
	*		2.Authot: Vector
	*			Date: 2019-1-1
	*			Mod: ɾ������button_addToParent,ͳһ��widget_addToParent
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
*                        button_onPaint                  
*
* Description: ��ť�ؼ����ƺ���
*             
* Arguments  : *pButton: ��ť�ṹ��
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void button_onPaint(const BUTTON_OBJ* pButton)
{
	int16_t XOffset = 0, YOffset = 0;
	
	/* ���㰴ť������԰�ť����ƫ���� */
	if(pButton->textAlign & GUI_TA_LEFT)
	{
		if(pButton->textAlign & GUI_TA_TOP)
		{
			XOffset = 1;
			YOffset = 1;
		}else if(pButton->textAlign & GUI_TA_VCENTER)
		{
			XOffset = 1;
			YOffset = (pButton->rect.height - font_getYSize(GUI_Context.curFont))/2;
		}else if(pButton->textAlign & GUI_TA_BOTTOM)
		{
			XOffset = 1;
			YOffset = pButton->rect.height - font_getYSize(GUI_Context.curFont);
		}
	}else if(pButton->textAlign & GUI_TA_RIGHT)
	{
		if(pButton->textAlign & GUI_TA_TOP)
		{
			XOffset = pButton->rect.width - font_getXSize(pButton->font)*gui_getStringLen(pButton->title);
			YOffset = 1;
		}else if(pButton->textAlign & GUI_TA_VCENTER)
		{
			XOffset = pButton->rect.width - font_getXSize(pButton->font)*gui_getStringLen(pButton->title);;
			YOffset = (pButton->rect.height - font_getYSize(GUI_Context.curFont))/2;
		}else if(pButton->textAlign & GUI_TA_BOTTOM)
		{
			XOffset = 1;
			YOffset = pButton->rect.height - font_getYSize(GUI_Context.curFont);
		}
	}else
	{
		XOffset = (pButton->rect.width - font_getXSize(pButton->font)*gui_getStringLen(pButton->title))/2;
		YOffset = (pButton->rect.height - font_getYSize(GUI_Context.curFont))/2;
	}
	
	/* �����ť���� */
	if(pButton->isPress)
	{
		gui_drawRectangle(pButton->rect.x0, pButton->rect.y0, pButton->rect.x0 + pButton->rect.width, pButton->rect.y0 + pButton->rect.height, 1);
		gui_gotoXY(pButton->rect.x0 + XOffset, pButton->rect.y0 + YOffset);
		gui_dispString(pButton->title, 1);
	}
	else	/* û�а�ť */
	{
		gui_fillRectangle(pButton->rect.x0, pButton->rect.y0, pButton->rect.x0 + pButton->rect.width, pButton->rect.y0 + pButton->rect.height, 1);
		gui_gotoXY(pButton->rect.x0 + XOffset, pButton->rect.y0 + YOffset);
		gui_dispString(pButton->title, 0);
	}
}


/*
*********************************************************************************************************
*                      button_getFont                    
*
* Description: ��ȡ��ť����
*             
* Arguments  : ��ť���
*
* Reutrn     : �ð�ť����
*
* Note(s)    : None.
*********************************************************************************************************
*/
GUI_FONT button_getFont(BUTTON_Handle hButton)
{
	WIDGET_OBJ *pWidget = NULL;
	BUTTON_OBJ *pButton = NULL;
	
	widget_getWidget(hButton, pWidget);	/* ��ȡ�ÿؼ��Ľṹ�� */
	
	if(!pWidget) return 0;
	
	pButton = (BUTTON_OBJ*)pWidget->widgetData;
	
	return pButton->font;
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
GUI_ERROR button_setFont(BUTTON_Handle hButton, GUI_FONT font)
{
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
GUI_ERROR button_getText(BUTTON_Handle hButton, char *pText, uint16_t textLen)
{
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
GUI_ERROR button_setText(BUTTON_Handle hButton, const char *pText)
{
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
GUI_ERROR button_setTextAlign(BUTTON_Handle hButton)
{
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
GUI_ERROR button_getTextAlign(BUTTON_Handle hButton)
{
	return ERR_NONE;
}

/*
*********************************************************************************************************
*                       button_Create                   
*
* Description: ����һ����ť�ؼ�
*             
* Arguments  : x0,y0: ��ť����
*							 width, height: ��ť�ĳ���
*							 id: ��ťid
*							 hParent: �����ھ��
*
* Reutrn     : ��ť���
*
* Note(s)    : None.
*********************************************************************************************************
*/
BUTTON_Handle button_Create(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t id, uint8_t *title, WM_HWIN hParent)
{
	int16_t r = 0;
	BUTTON_OBJ *pButton;
	
	if(!hParent) return 0;	/* ���������и����� */
	
	pButton = (BUTTON_OBJ*)bsp_mem_Alloc(SRAMIN, sizeof(BUTTON_OBJ));	/* Ϊ���������ڴ� */
	if(!pButton) return 0;	/* �ڴ�����ʧ�� */
	
	/* ���ð�ť���� */
	pButton->rect.x0 = x0;
	pButton->rect.y0 = y0;
	pButton->rect.width = width;
	pButton->rect.height = height;
	pButton->isPress = 0;
	pButton->title = title;
	pButton->textAlign = GUI_TA_HCENTER | GUI_TA_VCENTER;
	
	r = widget_Create(WIDGET_BUTTON, pButton, id, hParent);
	
	return r;
}

/*
*********************************************************************************************************
*                           button_CreateEx               
*
* Description: ����һ����ť�ؼ�
*             
* Arguments  : *pButton: ��ť�ṹ��ָ��
*							 hParent: �����ھ��
*
* Reutrn     : �ɹ�ʱ���ذ�ť���
*
* Note(s)    : None.
*********************************************************************************************************
*/
BUTTON_Handle button_CreateEx(BUTTON_OBJ *pButton, uint8_t id, WM_HWIN hParent)
{
	if(!hParent) return 0;
	
	return widget_Create(WIDGET_BUTTON, pButton, id, hParent);
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
GUI_ERROR button_Delete(BUTTON_Handle *hButton)
{
	if(*hButton == 0) return ERR_PARA;
	
	return widget_Delete(hButton);
}
/********************************************  END OF FILE  *******************************************/
