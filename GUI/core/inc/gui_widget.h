/**
  *******************************************************************************************************
  * File Name: gui_widget.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: GUI控件处理部分
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-30
	*			Mod: 建立文件
	*
  *******************************************************************************************************
  */
# ifndef __GUI_WIDGET_H
# define __GUI_WIDGET_H

# include "gui_com.h"
# include "gui_key.h"

# define WIDGET_Handle GUI_Handle

/* 控件类型 */
typedef enum
{
	WIDGET_BUTTON = 0x01,
	WIDGET_MENU,
	WIDGET_PROGBAR,
	WIDGET_SCROLLBAR,
	WIDGET_CHECKBOX,
}WIDGET_TYPE;

typedef struct WIDGET_OBJ WIDGET_OBJ;
typedef void WIDGET_CALLBACK(WIDGET_Handle hWidget, GUI_KEY_INFO *pKey);

#pragma pack(8) 
/* 控件结构体 */
struct WIDGET_OBJ
{
	WIDGET_TYPE type;		/* 控件类型 */
	uint8_t layer;			/* 控件属于哪一层，代表控件显示顺序，该值越小，控件就处于越上面 */
	uint16_t id;
	uint16_t actKey;		/* 该控件响应的按键消息类型 */
	
	WIDGET_OBJ *pNext;	/* 兄弟控件 */
	WIDGET_CALLBACK *_cb;
	void *widgetData;			/* 控件数据 */
};
#pragma pack ()


void widget_onPaint(struct WIDGET_OBJ *pWidget);
WIDGET_OBJ *widget_getWidget(WIDGET_Handle hWidget, int16_t *err);
void widget_sortList(WIN_Handle hParent);
GUI_ERROR widget_Delete(WIDGET_Handle hWidget);
WIDGET_Handle widget_Create(WIDGET_TYPE widgetType, WIDGET_OBJ *pWidget, uint8_t id, uint16_t actKey, WIDGET_CALLBACK *_cb, WIN_Handle hParent);

# endif

/********************************************  END OF FILE  *******************************************/

