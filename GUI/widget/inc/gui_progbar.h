/**
  *******************************************************************************************************
  * File Name: gui_progbar.h
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
# ifndef __GUI_PROGBAR_H
# define __GUI_PROGBAR_H

# define PROGBAR_Handle	int16_t
typedef struct PROGBAR_OBJ PROGBAR_OBJ;

/* �������ṹ������ */
struct PROGBAR_OBJ
{
	uint16_t x, y; /* ���������� */
	uint16_t width, height; /* ���������� */
	
	uint8_t ratio;  /* ������ֵ */
};

void progbar_onPaint(const PROGBAR_OBJ* pProgbar);
PROGBAR_Handle progbar_Create(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t id, WIN_Handle hParent);
GUI_ERROR progbar_increaseRatio(PROGBAR_Handle hProgbar, int8_t value);
uint8_t progbar_getRatio(PROGBAR_Handle hProgbar);


# endif
/********************************************  END OF FILE  *******************************************/
