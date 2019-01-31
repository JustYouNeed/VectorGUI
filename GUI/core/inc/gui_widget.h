/**
  *******************************************************************************************************
  * File Name: gui_widget.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: GUI�ؼ�������
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-30
	*			Mod: �����ļ�
	*
  *******************************************************************************************************
  */
# ifndef __GUI_WIDGET_H
# define __GUI_WIDGET_H

# define WIDGET_HANDLE uint16_t
# define GUI_GET_HPARENT(hwin) ((hwin >> 10) & 0x3f)
# define GUI_GET_WIDGET_TYPE(hwin) ((hwin >> 6) & 0x0f)
# define GUI_GET_HWIDGET(hwin) ((hwin >> 0) & 0x3f)

typedef struct WIDGET_OBJ WIDGET_OBJ;
typedef void WIDGET_CALLBACK(WM_MESSAGE *pMsg);

#pragma pack(8) 
/* �ؼ��ṹ�� */
struct WIDGET_OBJ
{
	WIDGET_TYPE type;		/* �ؼ����� */
	uint8_t layer;			/* �ؼ�������һ�㣬����ؼ���ʾ˳�򣬸�ֵԽС���ؼ��ʹ���Խ���� */
	uint16_t id;
	uint16_t actKey;		/* �ÿؼ���Ӧ�İ�����Ϣ���� */
	
	WIDGET_OBJ *pNext;	/* �ֵܿؼ� */
	WIDGET_CALLBACK *_cb;
	void *widgetData;			/* �ؼ����� */
};
#pragma pack ()


void widget_onPaint(struct WIDGET_OBJ *pWidget);
WIDGET_OBJ *widget_getWidget(WIDGET_HANDLE hWidget, int16_t *err);
void widget_sortList(WIN_Handle hParent);
GUI_ERROR widget_Delete(WIDGET_HANDLE hWidget);
WIDGET_HANDLE widget_Create(WIDGET_TYPE widgetType, void *pObj, uint8_t id, uint16_t actKey, WIDGET_CALLBACK *_cb, WIN_Handle hParent);

# endif

/********************************************  END OF FILE  *******************************************/

