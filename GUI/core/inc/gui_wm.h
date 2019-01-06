/**
  *******************************************************************************************************
  * File Name: gui_wm.h
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
# ifndef __GUI_WM_H
# define __GUI_WM_H

typedef struct WM_OBJ WM_OBJ;
typedef void WM_CALLBACK(WM_MESSAGE *pMsg);


/* ���ڽṹ�� */
struct WM_OBJ
{
	GUI_RECT rect;	/* ������ */
	GUI_RECT invalidRect;	/* ��Ч���� */
	WM_HWIN hWin;					/* ���ھ�� */
	uint16_t status;
	uint16_t noOfWidget;	/* �ô����������صĿؼ����� */
	WM_CALLBACK *_cb;
	WIDGET_OBJ *pWidget;	/* �ӿؼ� */
	WM_OBJ *pNext;	/* ��һ������ */
};

void wm_Init(void);
GUI_ERROR wm_Delete(WM_HWIN *hWin);
WM_HWIN	wm_Create(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t status);

WM_OBJ *wm_getWindowObject(WM_HWIN hWin, int16_t *err);

void wm_onPaint(void);
void wm_defaultProc(WM_MESSAGE *pMsg);

# endif


/********************************************  END OF FILE  *******************************************/

