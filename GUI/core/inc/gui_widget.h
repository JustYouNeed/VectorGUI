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

# define WIDGET_HANDLE uint16_t
# define GUI_GET_HPARENT(hwin) ((hwin >> 10) & 0x3f)
# define GUI_GET_WIDGET_TYPE(hwin) ((hwin >> 6) & 0x0f)
# define GUI_GET_HWIDGET(hwin) ((hwin >> 0) & 0x3f)

typedef struct WIDGET_OBJ WIDGET_OBJ;
typedef void WIDGET_CALLBACK(WM_MESSAGE *pMsg);

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
WIDGET_OBJ *widget_getWidget(WIDGET_HANDLE hWidget, int16_t *err);
void widget_sortList(WIN_Handle hParent);
GUI_ERROR widget_Delete(WIDGET_HANDLE hWidget);
WIDGET_HANDLE widget_Create(WIDGET_TYPE widgetType, void *pObj, uint8_t id, uint16_t actKey, WIDGET_CALLBACK *_cb, WIN_Handle hParent);

# endif

/********************************************  END OF FILE  *******************************************/

