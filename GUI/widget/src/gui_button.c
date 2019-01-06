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
static void _onPaint(const BUTTON_OBJ* pButton)
{
	int16_t XOffset = 0, YOffset = 0;
	
	gui_fillRectangle(pButton->rect.x0, pButton->rect.y0, pButton->rect.x0 + pButton->rect.width, pButton->rect.y0 + pButton->rect.height, 0);
	
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
	else	/* û�а��� */
	{
		gui_fillRectangle(pButton->rect.x0, pButton->rect.y0, pButton->rect.x0 + pButton->rect.width, pButton->rect.y0 + pButton->rect.height, 1);
		gui_gotoXY(pButton->rect.x0 + XOffset, pButton->rect.y0 + YOffset);
		gui_dispString(pButton->title, 0);
	}
}

/*
*********************************************************************************************************
*                            _delete              
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
static void _delete(BUTTON_Handle hButton)
{	
	widget_Delete(hButton);
}

/*
*********************************************************************************************************
*                        button_getButtonObj                  
*
* Description: ��ȡ��ť���ƽṹ��
*             
* Arguments  : hButton: ��ť���
*						   *pButton: ��ť�ṹ��ָ��
*
* Reutrn     : ִ�н��
*
* Note(s)    : ���ļ�˽�к���
*********************************************************************************************************
*/
static BUTTON_OBJ *_getButtonObj(BUTTON_Handle hButton, int16_t *err)
{
	WIDGET_OBJ *pObj = NULL;
	
		/* ��ȡ��ť�ؼ� */
	pObj = widget_getWidget(hButton, err);	
	if(!pObj) 
	{
		*err = ERR_PARA;
		return NULL;
	}
	
	return (BUTTON_OBJ *)(pObj->widgetData);
}

/*
*********************************************************************************************************
*                             button_defaultProc             
*
* Description: Ĭ�ϰ����ص�����
*             
* Arguments  : *pMsg: ��Ϣָ��
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
static void button_defaultProc(WM_MESSAGE *pMsg)
{
	BUTTON_Handle hObj;
	BUTTON_OBJ *pButton = NULL;
	WIDGET_OBJ *pWidget = NULL;
	int16_t err = ERR_NONE;
	
	hObj = pMsg->hWin;
	pWidget = widget_getWidget(hObj, &err);
	pButton = (BUTTON_OBJ *)(pWidget->widgetData);
	
	/* ���ݲ�ͬ����Ϣ���в�ͬ�Ĵ��� */
	switch(pMsg->msgId)
	{
		case MSG_CREATE: break;
		case MSG_KEY: 	/* ������Ϣ */
		{
			/* �õ�������Ϣ */
			MSG_KEY_INFO *key = (MSG_KEY_INFO *)&(pMsg->key);
			
			/* ����ð����ڿؼ�����Ӧ������������Ӧ */
			if(key->keyValue & pWidget->actKey)
			{
				if(key->keyStatus == KEY_PRESS) /* ������������� */
				{
					pButton->isPress = true; 
					_onPaint(pButton);
					if(pButton->_cb) pButton->_cb(pMsg);
				}
				else if(key->keyStatus == KEY_RELEASED)	/* �����ͷ��� */
				{
					pButton->isPress = false;
					_onPaint(pButton);
				}
			}
		}break;
		case MSG_PAINT: 	/* �����ػ���Ϣ */
		{
			_onPaint(pButton);
		}break;
		case MSG_DELETE:	/* ɾ��������Ϣ */
		{
			_delete(hObj);
		}break;
		default: wm_defaultProc(pMsg);
	}
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
void button_setCallback(BUTTON_Handle hButton, BUTTON_CALLBACK *_cb)
{
	BUTTON_OBJ * pButton = NULL;
	int16_t err = ERR_NONE;
	if(!hButton) return ;
	
	pButton = _getButtonObj(hButton, &err);
	if(!pButton) return ;
	
	pButton->_cb = _cb;
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
	int16_t err = ERR_NONE;
	
	pWidget = widget_getWidget(hButton, &err);	/* ��ȡ�ÿؼ��Ľṹ�� */
	
	if(!pWidget) return 0;
	
	pButton = (BUTTON_OBJ*)pWidget->widgetData;
	
	return pButton->font;
}

/*
*********************************************************************************************************
*                         button_setFont                 
*
* Description: ���ð�ť��������
*             
* Arguments  : hButton: ��ť���
*							 font: ����
*
* Reutrn     : ִ�н��
*
* Note(s)    : None.
*********************************************************************************************************
*/
GUI_ERROR button_setFont(BUTTON_Handle hButton, GUI_FONT font)
{
	if(!hButton) return ERR_PARA;
	BUTTON_OBJ *pButton = NULL;
	int16_t err = ERR_NONE;
	
	pButton = _getButtonObj(hButton, &err);
	pButton->font = font;
	
	return ERR_NONE;
}

/*
*********************************************************************************************************
*                       button_getText                   
*
* Description: ��ȡ��ť�ı�
*             
* Arguments  : hButton: ��ť���
*							 *pText: �ı�������
*							 textLen: �ı�����
*
* Reutrn     : ִ�н��
*
* Note(s)    : None.
*********************************************************************************************************
*/
GUI_ERROR button_getText(BUTTON_Handle hButton, uint8_t *pText, uint16_t textLen)
{
	if(!hButton) return ERR_PARA;
	
	BUTTON_OBJ *pButton = NULL;
	int16_t err = ERR_NONE;
	
	pButton = _getButtonObj(hButton, &err);
	pText = pButton->title;
	
	return ERR_NONE;
}

/*
*********************************************************************************************************
*                      button_setText                    
*
* Description: ���ð�ť�ı�
*             
* Arguments  : hButton: ��ť���
*							 *pText: Ҫ���õ��ı�
*
* Reutrn     : ִ�н��
*
* Note(s)    : None.
*********************************************************************************************************
*/
GUI_ERROR button_setText(BUTTON_Handle hButton, const uint8_t *pText)
{
	if(!hButton) return ERR_PARA;
	
	BUTTON_OBJ *pButton = NULL;
	int16_t err = ERR_NONE;
	
	pButton = _getButtonObj(hButton, &err);
	pButton->title = (uint8_t *)pText;
	
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
*							 title: ��ť��ʾ�ı���
*							 �ð�ť����Ӧʲô������Ϣ
*							 hParent: �����ھ��
*
* Reutrn     : ��ť���
*
* Note(s)    : None.
*********************************************************************************************************
*/
BUTTON_Handle button_Create(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t id, uint8_t *title, uint16_t actKey, WM_HWIN hParent)
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
	pButton->_cb = NULL;
	pButton->textAlign = GUI_TA_HCENTER | GUI_TA_VCENTER;
	
	r = widget_Create(WIDGET_BUTTON, pButton, id, actKey, button_defaultProc, hParent);
	
	msg_sendMsgNoData(r, MSG_PAINT);	/* ������ɺ���һ���ػ���Ϣ */
	return r;
}

/*
*********************************************************************************************************
*                           button_CreateEx               
*
* Description: ����һ����ť�ؼ�
*             
* Arguments  : *pButton: ��ť�ṹ��ָ��
*							 id: ��ťID
*							 actKey: ��ť��Ӧ�İ���
*							 hParent: �����ھ��
*
* Reutrn     : �ɹ�ʱ���ذ�ť���
*
* Note(s)    : None.
*********************************************************************************************************
*/
BUTTON_Handle button_CreateEx(BUTTON_OBJ *pButton, uint8_t id, uint16_t actKey, WM_HWIN hParent)
{
	if(!hParent) return 0;
	
	return widget_Create(WIDGET_BUTTON, pButton, id, actKey, button_defaultProc, hParent);
}

/*
*********************************************************************************************************
*                           button_Delete               
*
* Description: ɾ��һ����ť�ؼ�
*             
* Arguments  : *hButton: Ҫɾ���İ�ť�ؼ����ָ��
*
* Reutrn     : ִ�н��
*
* Note(s)    : �����ť�ؼ�ɾ���ɹ�����ť�������Ϊ0
*********************************************************************************************************
*/
GUI_ERROR button_Delete(BUTTON_Handle *hButton)
{
	if(!*hButton) return ERR_PARA; /* �������� */
	
	msg_sendMsgNoData(*hButton, MSG_DELETE);
	*hButton = 0;
}
/********************************************  END OF FILE  *******************************************/
