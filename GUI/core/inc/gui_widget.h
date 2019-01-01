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

void widget_onPaint(const struct WIDGET_OBJ *pWidget);
GUI_ERROR widget_getWidget(WIDGET_HANDLE hWidget, WIDGET_OBJ *pWidget);

GUI_ERROR widget_Delete(WIDGET_HANDLE *hWidget);
WIDGET_HANDLE widget_Create(WIDGET_TYPE widgetType, void *pObj, uint8_t id, WM_HWIN hParent);

# endif

/********************************************  END OF FILE  *******************************************/

