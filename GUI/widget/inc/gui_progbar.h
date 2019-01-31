/**
  *******************************************************************************************************
  * File Name: gui_progbar.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: GUI进度条控件头文件，部分函数参考正点原子遥控GUI中的程序，原作者为黄绍斌
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-30
	*			Mod: 建立文件
	*
  *******************************************************************************************************
  */	
# ifndef __GUI_PROGBAR_H
# define __GUI_PROGBAR_H

# define PROGBAR_Handle	int16_t
typedef struct PROGBAR_OBJ PROGBAR_OBJ;

/* 进度条结构体声明 */
struct PROGBAR_OBJ
{
	uint16_t x, y; /* 进度条坐标 */
	uint16_t width, height; /* 进度条长宽 */
	
	uint8_t ratio;  /* 进度条值 */
};

void progbar_onPaint(const PROGBAR_OBJ* pProgbar);
PROGBAR_Handle progbar_Create(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t id, WIN_Handle hParent);
GUI_ERROR progbar_increaseRatio(PROGBAR_Handle hProgbar, int8_t value);
uint8_t progbar_getRatio(PROGBAR_Handle hProgbar);


# endif
/********************************************  END OF FILE  *******************************************/
