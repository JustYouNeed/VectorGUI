/**
  *******************************************************************************************************
  * File Name: gui_scrollbar.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: GUI�������ؼ�ͷ�ļ������ֺ����ο�����ԭ��ң��GUI�еĳ���ԭ����Ϊ���ܱ�
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-30
	*			Mod: �����ļ�
	*
  *******************************************************************************************************
  */
# ifndef __GUI_SCROLLBAR_H
# define __GUI_SCROLLBAR_H

# define SCROLLBAR_Handle	int16_t
typedef struct SCROLLBAR_OBJ SCROLLBAR_OBJ;

struct SCROLLBAR_OBJ
{
	GUI_RECT rect;
};

void scrollbar_onPaint(const SCROLLBAR_OBJ *pScrollbar);

# endif

/********************************************  END OF FILE  *******************************************/
