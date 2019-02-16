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
# include "gui_widget.h"
# include "gui_button.h"
# include "gui_font.h"
# include "gui_text.h"
# include "gui_win.h"
# include "gui_mem.h"
# include "gui_core.h"

/*
*********************************************************************************************************
*                        buttonbutton_getObject                  
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
static BUTTON_OBJ *button_getObject(BUTTON_Handle hButton, int16_t *err)
{
	WIDGET_OBJ *pWidget = NULL;
	
		/* ��ȡ��ť�ؼ� */
	pWidget = widget_getWidget(hButton, err);	
	if(*err) return NULL;
	
	return (BUTTON_OBJ *)(pWidget->widgetData);
}

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
void button_onPaint(BUTTON_OBJ *pButton)
{
	int16_t XOffset = 0, YOffset = 0;
	
	gui_rectCrop(&GUI_Context.drawRect, &pButton->rect);
	
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
static void button_defaultProc(BUTTON_Handle hButton, GUI_KEY_INFO *pKey)
{
	BUTTON_OBJ *pButton = NULL;
	int16_t err = ERR_NONE;
	
	/* �õ���ť�ؼ��ṹ�� */
	pButton = button_getObject(hButton, &err);
	if(err) return ;
	
	/* ���ð�ť״̬ */
	if(pKey->keyStatus == GUI_KEY_PRESS)
	{
		pButton->isPress = true; 
	}
	else if(pKey->keyStatus == GUI_KEY_RELEASED)
	{
		pButton->isPress = false;
	}
	
	/* ֻ�а�ť״̬�ı��˲���Ҫ�ػ� */
	pButton->lstState = pButton->isPress;
	if(pButton->lstState != pButton->preState)
	{
		/* ���ư�ť���µ�״̬ */
		win_Invalidation(WIN_Desktop->hWin);
//		gui_onPaint();
	}
	pButton->preState = pButton->lstState;
	
	/* ���ڻص������Ļ�ִ�лص����� */
	if(pButton->_cb) pButton->_cb(pKey);
}

/*
*********************************************************************************************************
*                        button_setCallback                  
*
* Description: ���ð�ť�Ļص�����,�ڰ�ť���»����ͷŵ�ʱ��ص��������ᱻ����
*             
* Arguments  : hButton: ��ť�ľ��
*							 *_cb: �ص�����
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void button_setCallback(BUTTON_Handle hButton, BUTTON_CALLBACK *_cb)
{
	BUTTON_OBJ * pButton = NULL;
	int16_t err = ERR_NONE;
	if(!hButton) return ;
	
	pButton = button_getObject(hButton, &err);
	if(err) return ;
	
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
	
	if(err) return (GUI_FONT)0;
	
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
	
	pButton = button_getObject(hButton, &err);
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
	
	pButton = button_getObject(hButton, &err);
	if(err) return (GUI_ERROR)err;
	
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
	
	pButton = button_getObject(hButton, &err);
	pButton->title = (uint8_t *)pText;
	
	win_Invalidation(hButton >> 10);
	
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
BUTTON_Handle button_Create(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t id, uint8_t *title, uint16_t actKey, WIN_Handle hParent)
{
	int16_t r = 0;
	uint8_t *pMem = NULL;
	BUTTON_OBJ *pButton = NULL;
	WIDGET_OBJ *pWidget = NULL;
	
	if(!hParent) return 0;	/* ���������и����� */
	
	pMem = (uint8_t *)gui_memAlloc(sizeof(BUTTON_OBJ) + sizeof(WIDGET_OBJ));	/* Ϊ���������ڴ� */
	if(!pMem) return 0;	/* �ڴ�����ʧ�� */
	
	pWidget = (WIDGET_OBJ *)pMem;
	pButton = (BUTTON_OBJ *)(pMem + sizeof(WIDGET_OBJ));
	
	/* ���ð�ť���� */
	pButton->rect.x0 = x0;
	pButton->rect.y0 = y0;
	pButton->rect.width = width;
	pButton->rect.height = height;
	pButton->isPress = 0;
	pButton->title = title;
	pButton->_cb = NULL;
	pButton->textAlign = GUI_TA_HCENTER | GUI_TA_VCENTER;
	pButton->lstState = false;
	pButton->preState = false;
	
	pWidget->widgetData = (void *)pButton;
	r = widget_Create(WIDGET_BUTTON, pWidget, id, actKey, button_defaultProc, hParent);
	
//	msg_sendMsgNoData(r, MSG_PAINT);	/* ������ɺ���һ���ػ���Ϣ */
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
BUTTON_Handle button_CreateEx(BUTTON_OBJ *pButton, uint8_t id, uint16_t actKey, WIN_Handle hParent)
{
	if(!hParent) return 0;
	
//	return widget_Create(WIDGET_BUTTON, pButton, id, actKey, button_defaultProc, hParent);
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
	
	widget_Delete(*hButton);
	*hButton = 0;
	
	return ERR_NONE;
}
/********************************************  END OF FILE  *******************************************/
