/**
  *******************************************************************************************************
  * File Name: gui_basic.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: 提供GUI基本绘图函数，参考正点原子遥控GUI中的程序，原作者为黄绍斌
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-29
	*			Mod: 建立文件
	*
  *******************************************************************************************************
  */
# ifndef  __GUI_BASIC_H
# define  __GUI_BASIC_H

/*
  *******************************************************************************************************
  *                              INCLUDE FILES
  *******************************************************************************************************
*/
# include "gui_com.h"

/* 画水平线 */
void gui_drawHLine(uint16_t x0, uint16_t y0, uint16_t x1, GUI_COLOR color);

/* 画垂直线 */
void gui_drawVLine(uint16_t x0, uint16_t y0, uint16_t y1, GUI_COLOR color);

/* 画任意角度的线 */
void gui_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, GUI_COLOR color);

/* 画有宽度的线 */
void gui_drawLineWidth(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t width, GUI_COLOR color);

/* 画圆弧 */
void gui_drawArc(uint16_t x, uint16_t y, uint16_t radius, uint16_t startAngle, uint16_t endAngle, GUI_COLOR color);

/* 画矩形 */
void gui_drawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, GUI_COLOR color);

/* 清除矩形区域 */
void gui_clearRect(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height);

/* 填充矩形区域 */
void gui_fillRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, GUI_COLOR color);

/* 画正方形 */
void gui_drawSquare(uint16_t x0, uint16_t y0, uint16_t sideLen, GUI_COLOR color);

/* 填充正方形 */
void gui_fillSquare(uint16_t x0, uint16_t y0, uint16_t sideLen, GUI_COLOR color);

/* 画多边形 */
void gui_drawPolygon(const GUI_POINT *pPolygon, uint8_t points, GUI_COLOR color);

/* 画多边形线 */
void gui_drawPolyLine(const GUI_POINT *pPolyLine, uint8_t points, GUI_COLOR color);

/* 画圆形 */
void gui_drawCircle(uint16_t x0, uint16_t y0, double r, GUI_COLOR color);

/* 填充圆形 */
void gui_fillCircle(uint16_t x, uint16_t y, uint16_t radius, GUI_COLOR color);

/* 画正椭圆 */
void gui_drawEllipse(uint16_t x, uint16_t y, uint16_t a, uint16_t b, GUI_COLOR color);

/* 填充正椭圆 */
void gui_fillEllipse(uint16_t x, uint16_t y, uint16_t a, uint16_t b, GUI_COLOR color);

/* 画扇形 */
void gui_drawSector(uint16_t x, uint16_t y, uint16_t r, uint16_t startAngle, uint16_t endAngle, GUI_COLOR color);

/* 填充扇形 */
void gui_fillSector(uint16_t x, uint16_t y, uint16_t r, uint16_t startAngle, uint16_t endAngle, GUI_COLOR color);

void gui_rectCrop(GUI_RECT *rect1, GUI_RECT *rect2);
#endif

/********************************************  END OF FILE  *******************************************/

