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

# include "math.h"
# include "stdio.h"
# include <stdint.h>
# include "stdbool.h"


# ifndef uint8_t 
	# define uint8_t unsigned char
# endif

# ifndef int8_t 
	# define int8_t signed char
# endif

# ifndef uint16_t 
	# define uint16_t unsigned short int
# endif

# ifndef int16_t 
	# define int16_t signed short int
# endif

# ifndef uint32_t 
	# define uint32_t unsigned int
# endif
	
# ifndef int32_t 
	# define int32_t signed int
# endif

# define GUI_COLOR	uint8_t
# define FONT		const uint8_t
# define GUI_EXTERN	extern
# define GUI_Handle	uint16_t
# define WIN_Handle	GUI_Handle


# define GUI_GET_HPARENT(hwin) ((hwin >> 10) & 0x3f)
# define GUI_GET_WIDGET_TYPE(hwin) ((hwin >> 6) & 0x0f)
# define GUI_GET_HWIDGET(hwin) ((hwin >> 0) & 0x3f)

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

# endif 
/********************************************  END OF FILE  *******************************************/
