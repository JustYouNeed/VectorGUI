/**
  *******************************************************************************************************
  * File Name: gui_types.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: ���ļ��ṩGUI�����������Ͷ���
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-29
	*			Mod: �����ļ�
  *******************************************************************************************************
  */	
# ifndef __GUI_TYPES_H
# define __GUI_TYPES_H

# define GUI_COLOR	uint8_t

# define FONT		const uint8_t 
# define GUI_EXTERN	extern
# define TEXT_Handle	uint8_t
# define WM_HWIN	int16_t 

typedef enum
{
	ERR_NONE = 0x0,	/* û�д��� */
	ERR_MEM = -1, /* �����ڴ�ʧ�� */
	ERR_PARA = -2, /* �������� */
}GUI_ERROR;


/* GUI��Ķ��� */
typedef struct 
{
	int16_t x;
	int16_t y;
}GUI_POINT;

/* GUI���εĶ��� */
typedef struct
{
	uint16_t x0, y0;	/* �������Ͻǵ������ */
	uint16_t width, height;	/* ���� */
}GUI_RECT;

/* �ؼ����� */
typedef enum
{
	WIDGET_BUTTON = 0x00,
	WIDGET_MENU,
	WIDGET_PROGBAR,
	WIDGET_SCROLLBAR,
	WIDGET_WINDOW,
	WIDGET_CHECKBOX,
}WIDGET_TYPE;


/* �ı����뷽ʽ */
typedef enum
{
	GUI_TA_LEFT = 0x01<<0,
	GUI_TA_HCENTER = 0x01<<1,
	GUI_TA_RIGHT = 0x01<<2,
	GUI_TA_TOP = 0x01<<3,
	GUI_TA_VCENTER = 0x01<<4,
	GUI_TA_BOTTOM = 0x01<<5,
}GUI_TEXT_ALIGN;


#pragma pack(8) 
/* �ؼ��ṹ�� */
struct WIDGET_OBJ
{
	WIDGET_TYPE type;		/* �ؼ����� */
	uint8_t layer;			/* �ؼ�������һ�㣬����ؼ���ʾ˳�򣬸�ֵԽС���ؼ��ʹ���Խ���� */
	uint16_t id;
	struct WIDGET_OBJ *pNext;	/* �ֵܿؼ� */
	
	void *widgetData;			/* �ؼ����� */
};
#pragma pack ()
typedef struct WIDGET_OBJ WIDGET_OBJ;

/* ���ڽṹ�� */
struct WM_OBJ
{
	GUI_RECT rect;	/* ������ */
	GUI_RECT invalidRect;	/* ��Ч���� */
	WM_HWIN hWin;					/* ���ھ�� */
	uint16_t status;
	uint16_t noOfWidget;	/* �ô����������صĿؼ����� */
	struct WIDGET_OBJ *pWidget;	/* �ӿؼ� */
	struct WM_OBJ *pNext;	/* ��һ������ */
};
typedef struct WM_OBJ WM_OBJ;





# endif 
/********************************************  END OF FILE  *******************************************/
