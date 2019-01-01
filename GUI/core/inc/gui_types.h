/**
  *******************************************************************************************************
  * File Name: gui_types.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: 本文件提供GUI基本数据类型定义
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-29
	*			Mod: 建立文件
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
	ERR_NONE = 0x0,	/* 没有错误 */
	ERR_MEM = -1, /* 申请内存失败 */
	ERR_PARA = -2, /* 参数错误 */
}GUI_ERROR;


/* GUI点的定义 */
typedef struct 
{
	int16_t x;
	int16_t y;
}GUI_POINT;

/* GUI矩形的定义 */
typedef struct
{
	uint16_t x0, y0;	/* 矩形左上角点的坐标 */
	uint16_t width, height;	/* 长宽 */
}GUI_RECT;

/* 控件类型 */
typedef enum
{
	WIDGET_BUTTON = 0x00,
	WIDGET_MENU,
	WIDGET_PROGBAR,
	WIDGET_SCROLLBAR,
	WIDGET_WINDOW,
	WIDGET_CHECKBOX,
}WIDGET_TYPE;


/* 文本对齐方式 */
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
/* 控件结构体 */
struct WIDGET_OBJ
{
	WIDGET_TYPE type;		/* 控件类型 */
	uint8_t layer;			/* 控件属于哪一层，代表控件显示顺序，该值越小，控件就处于越上面 */
	uint16_t id;
	struct WIDGET_OBJ *pNext;	/* 兄弟控件 */
	
	void *widgetData;			/* 控件数据 */
};
#pragma pack ()
typedef struct WIDGET_OBJ WIDGET_OBJ;

/* 窗口结构体 */
struct WM_OBJ
{
	GUI_RECT rect;	/* 主窗口 */
	GUI_RECT invalidRect;	/* 无效窗口 */
	WM_HWIN hWin;					/* 窗口句柄 */
	uint16_t status;
	uint16_t noOfWidget;	/* 该窗口下所挂载的控件数量 */
	struct WIDGET_OBJ *pWidget;	/* 子控件 */
	struct WM_OBJ *pNext;	/* 下一个窗口 */
};
typedef struct WM_OBJ WM_OBJ;





# endif 
/********************************************  END OF FILE  *******************************************/
