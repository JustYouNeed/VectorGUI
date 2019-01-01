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

typedef struct
{
	GUI_RECT rect;
}SCROLLBAR_OBJ;

void scrollbar_onPaint(const SCROLLBAR_OBJ *pScrollbar);

# endif

/********************************************  END OF FILE  *******************************************/
