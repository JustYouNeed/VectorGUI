/**
  *******************************************************************************************************
  * File Name: gui_button.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: GUI�����ؼ�ͷ�ļ������ֺ����ο�����ԭ��ң��GUI�еĳ���ԭ����Ϊ���ܱ�
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-30
	*			Mod: �����ļ�
	*
  *******************************************************************************************************
  */	
# ifndef __GUI_BUTTON_H
# define __GUI_BUTTON_H

# define BUTTON_Handle	uint16_t
typedef struct BUTTON_OBJ BUTTON_OBJ;
typedef void BUTTON_CALLBACK(WM_MESSAGE *pMsg);

#pragma pack (8)
/* �����ṹ�� */
struct BUTTON_OBJ
{
	GUI_RECT rect;	/* �������� */
	uint8_t *title;	/* �������� */
	uint8_t isPress;	/* ��ť�Ƿ񱻰��� */
	BUTTON_CALLBACK *_cb; /* ��ť�ص����� */
	GUI_FONT font;		/* ��ť���� */
	GUI_TEXT_ALIGN textAlign;	/* ������뷽ʽ */
};
#pragma pack ()

void button_setCallback(BUTTON_Handle hButton, BUTTON_CALLBACK *_cb);

GUI_FONT button_getFont(BUTTON_Handle hButton);
GUI_ERROR button_setFont(BUTTON_Handle hButton, GUI_FONT font);

GUI_ERROR button_getText(BUTTON_Handle hButton, uint8_t *pText, uint16_t textLen);
GUI_ERROR button_setText(BUTTON_Handle hButton, const uint8_t *pText);

GUI_ERROR button_setTextAlign(BUTTON_Handle hButton);
GUI_ERROR button_getTextAlign(BUTTON_Handle hButton);

BUTTON_Handle button_Create(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t id, uint8_t *title, uint16_t actKey, WM_HWIN hParent);
BUTTON_Handle button_CreateEx(BUTTON_OBJ *pButton, uint8_t id, uint16_t actKey, WM_HWIN hParent);
GUI_ERROR button_Delete(BUTTON_Handle *hButton);

# endif

/********************************************  END OF FILE  *******************************************/




