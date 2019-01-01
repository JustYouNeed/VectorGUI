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

uint16_t gui_readLeftPoint(uint16_t x0, uint16_t y0, uint8_t color);
uint16_t gui_readRightPoint(uint16_t x0, uint16_t y0, uint8_t color);
int8_t gui_comparePointColor(uint16_t x0, uint16_t y0, uint8_t color);

void gui_drawHLine(uint16_t x0, uint16_t y0, uint16_t x1, GUI_COLOR color);
void gui_drawVLine(uint16_t x0, uint16_t y0, uint16_t y1, GUI_COLOR color);
void gui_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, GUI_COLOR color);
void gui_drawLineWidth(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t width, GUI_COLOR color);

void gui_drawArc(uint16_t x, uint16_t y, uint16_t radius, uint16_t startAngle, uint16_t endAngle, GUI_COLOR color);

void gui_drawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, GUI_COLOR color);
void gui_fillRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, GUI_COLOR color);

void gui_drawSquare(uint16_t x0, uint16_t y0, uint16_t sideLen, GUI_COLOR color);
void gui_fillSquare(uint16_t x0, uint16_t y0, uint16_t sideLen, GUI_COLOR color);

void gui_drawPolygon(const GUI_POINT *pPolygon, uint8_t points, GUI_COLOR color);
void gui_drawPolyLine(const GUI_POINT *pPolyLine, uint8_t points, GUI_COLOR color);

void gui_drawCircle(uint16_t x, uint16_t y, uint16_t radius, GUI_COLOR color);
void gui_fillCircle(uint16_t x, uint16_t y, uint16_t radius, GUI_COLOR color);

void gui_drawEllipse(uint16_t x, uint16_t y, uint16_t a, uint16_t b, GUI_COLOR color);
void gui_fillEllipse(uint16_t x, uint16_t y, uint16_t a, uint16_t b, GUI_COLOR color);

void gui_drawSector(uint16_t x, uint16_t y, uint16_t r, uint16_t startAngle, uint16_t endAngle, GUI_COLOR color);
void gui_fillSector(uint16_t x, uint16_t y, uint16_t r, uint16_t startAngle, uint16_t endAngle, GUI_COLOR color);

#endif

/********************************************  END OF FILE  *******************************************/

