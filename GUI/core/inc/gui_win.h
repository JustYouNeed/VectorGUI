/**
  *******************************************************************************************************
  * File Name: gui_win.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2019-1-29
  * Brief: 窗口管理器
  *******************************************************************************************************
  * History
	*		1.Author: Vector
	*			Date: 2019-1-29
	*			Mod: 建立文件
  *
  *******************************************************************************************************
  */
# ifndef __GUI_WIN_H_
# define __GUI_WIN_H_

# include "gui_com.h"
# include "gui_widget.h"

typedef struct WIN_OBJ WIN_OBJ;
typedef enum
{
	GUI_CF_SHOW = 0x01 << 0,
	GUI_CF_FULL = 0x01 << 1,
	GUI_CF_HALF = 0x01 << 2,
}GUI_CF_FALG;

/* 窗口结构体 */
struct WIN_OBJ
{
	WIN_Handle hWin;	/* 窗口句柄 */
	uint16_t flag;
	uint16_t noOfWidget;	/* 该窗口下所挂载的控件数量 */
	uint8_t *title;
	uint8_t isInvalid;
	
	GUI_RECT rect;
	GUI_RECT clientRect;			

	WIDGET_OBJ *pWidget;	/* 子控件 */
	WIN_OBJ *pNext;	/* 下一个窗口 */
};


WIN_Handle win_Create(uint8_t id, uint8_t *title, GUI_CF_FALG flag);
int16_t win_Delete(WIN_Handle *hWin);
WIN_OBJ *win_getObject(WIN_Handle hWin, int16_t *err);

int16_t win_boringToTop(WIN_Handle hWin);
void win_Invalidation(WIN_Handle hWin);
void win_onPaint(const WIN_OBJ *pWin);

extern WIN_OBJ *WIN_Desktop;
# endif
/********************************************  END OF FILE  *******************************************/

