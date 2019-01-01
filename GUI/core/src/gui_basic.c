/**
  *******************************************************************************************************
  * File Name: gui_basic.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: 提供GUI基本2D绘图函数，部分函数参考正点原子遥控GUI中的程序，原作者为黄绍斌
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-29
	*			Mod: 建立文件
	*
  *******************************************************************************************************
  */	
/*
  *******************************************************************************************************
  *                              INCLUDE FILES
  *******************************************************************************************************
*/
# include "gui.h"

uint16_t gui_readLeftPoint(uint16_t x0, uint16_t y0, uint8_t color)
{
	return 0;
}
uint16_t gui_readRightPoint(uint16_t x0, uint16_t y0, uint8_t color);
int8_t gui_comparePointColor(uint16_t x0, uint16_t y0, uint8_t color);

/*
*********************************************************************************************************
*                           gui_drawLine               
*
* Description: 画任意角度的线
*             
* Arguments  : x0,y0: 起始点的坐标
*							 x1,y1: 终点坐标
*							 color: 线的颜色
*
* Reutrn     : None.
*
* Note(s)    : 对于单色屏颜色无效
*********************************************************************************************************
*/
void gui_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, GUI_COLOR color)
{
	int32_t dx = 0, dy = 0;
	int8_t dx_dir = 0, dy_dir = 0;
	int32_t dx_x2 = 0, dy_x2 = 0;
	int32_t di = 0;
	
	dx = x1 - x0;	/* 求两点之间的差值 */
	dy = y1 - y0;
	
	/* 判断增长方向 */
	if(dx > 0) dx_dir = 1;
	else if(dx < 0) dx_dir = -1;
	else 	/* 是垂直线，直接画垂直线 */
		gui_drawVLine(x0, y0, y1, color);
	
	if(dy > 0) dy_dir = 1;
	else if(dy < 0) dy_dir = -1;
	else /* 是水平线，直接画水平线 */
		gui_drawHLine(x0, y0, x1, color);
	
	/* 将dx、dy取绝对值 */
	dx = dx_dir * dx;
	dy = dy_dir * dy;

	/* 计算2倍的dx及dy值 */
	dx_x2 = dx*2;
	dy_x2 = dy*2;
	
	/* 使用Bresenham法进行画直线 */
	if(dx>=dy)						// 对于dx>=dy，则使用x轴为基准
	{  
		di = dy_x2 - dx;
		while(x0!=x1)
		{  
			gui_drawPoint(x0, y0, color);
			x0 += dx_dir;
			if(di<0)
				di += dy_x2;		// 计算出下一步的决策值
			else
			{  
				di += dy_x2 - dx_x2;
				y0 += dy_dir;
			}
		}
		gui_drawPoint(x0, y0, color);	// 显示最后一点
	}
	else							// 对于dx<dy，则使用y轴为基准
	{  
		di = dx_x2 - dy;
		while(y0!=y1)
		{  
			gui_drawPoint(x0, y0, color);
			y0 += dy_dir;
			if(di<0)
				di += dx_x2;
			else
			{  
				di += dx_x2 - dy_x2;
				x0 += dx_dir;
			}
		}
		gui_drawPoint(x0, y0, color);		// 显示最后一点
	} 
}

/*
*********************************************************************************************************
*                    gui_drawLineWidth                      
*
* Description: 画线函数,可以设置线宽
*             
* Arguments  : x0,y0: 起点坐标
*							 x1,y1: 终点坐标
*							 width: 线宽(0 - 50)
*							 color: 颜色
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_drawLineWidth(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t width, GUI_COLOR color)
{
		int32_t   dx;						// 直线x轴差值变量
	int32_t   dy;          				// 直线y轴差值变量
	int8_t    dx_dir;					// x轴增长方向，为-1时减值方向，为1时增值方向
	int8_t    dy_dir;					// y轴增长方向，为-1时减值方向，为1时增值方向
	int32_t   dx_x2;					// dx*2值变量，用于加快运算速度
	int32_t   dy_x2;					// dy*2值变量，用于加快运算速度
	int32_t   di;						// 决策变量
	int32_t   wx, wy;					// 线宽变量
	int32_t   draw_a, draw_b;

	/* 参数过滤 */
	if(width==0) return;
	if(width>50) width = 50;

	dx = x1-x0;							// 求取两点之间的差值
	dy = y1-y0;
	wx = width/2;
	wy = width-wx-1;

	/* 判断增长方向，或是否为水平线、垂直线、点 */
	if(dx>0)							// 判断x轴方向
	{  
		dx_dir = 1;						// dx>0，设置dx_dir=1
	}
	else
	{  
		if(dx<0) 
			dx_dir = -1;				// dx<0，设置dx_dir=-1
		else/* dx==0，画垂直线，或一点 */
		{  						
			wx = x0-wx;
			if(wx<0) wx = 0;
			wy = x0+wy;
			while(1)
			{  
				x0 = wx;
				gui_drawVLine(x0, y0, y1, color);
				if(wx>=wy) break;
				wx++;
			}
			return;
		}
	}

	if(dy>0)							// 判断y轴方向
		dy_dir = 1;						// dy>0，设置dy_dir=1
	else
	{  
		if(dy<0)
			dy_dir = -1;				// dy<0，设置dy_dir=-1	
		else /* dy==0，画水平线，或一点 */
		{ 
			wx = y0-wx;
			if(wx<0) wx = 0;
			wy = y0+wy;
			while(1)
			{  
				y0 = wx;
				gui_drawHLine(x0, y0, x1, color);
				if(wx>=wy) break;
				wx++;
			}
			return;
		}
	}

	/* 将dx、dy取绝对值 */
	dx = dx_dir * dx;
	dy = dy_dir * dy;

	/* 计算2倍的dx及dy值 */
	dx_x2 = dx*2;
	dy_x2 = dy*2;

	/* 使用Bresenham法进行画直线 */
	if(dx>=dy)						// 对于dx>=dy，则使用x轴为基准
	{  
		di = dy_x2 - dx;
		while(x0!=x1)/* x轴向增长，则宽度在y方向，即画垂直线 */
		{  
			draw_a = y0-wx;
			if(draw_a<0) draw_a = 0;
			draw_b = y0+wy;
			gui_drawVLine(x0, draw_a, draw_b, color);

			x0 += dx_dir;				
			if(di<0)
				di += dy_x2;			// 计算出下一步的决策值
			else
			{  
				di += dy_x2 - dx_x2;
				y0 += dy_dir;
			}
		}
		draw_a = y0-wx;
		if(draw_a<0) draw_a = 0;
		draw_b = y0+wy;
		gui_drawVLine(x0, draw_a, draw_b, color);
	}
	else								// 对于dx<dy，则使用y轴为基准
	{  
		di = dx_x2 - dy;
		while(y0!=y1) /* y轴向增长，则宽度在x方向，即画水平线 */
		{ 
			draw_a = x0-wx;
			if(draw_a<0) draw_a = 0;
			draw_b = x0+wy;
			gui_drawHLine(draw_a, y0, draw_b, color);
			y0 += dy_dir;
			if(di<0)
				di += dx_x2;
			else
			{  
				di += dx_x2 - dy_x2;
				x0 += dx_dir;
			}
		}
		draw_a = x0-wx;
		if(draw_a<0) draw_a = 0;
		draw_b = x0+wy;
		gui_drawHLine(draw_a, y0, draw_b, color);
	} 
}

/*
*********************************************************************************************************
*                   gui_drawHLine                       
*
* Description: GUI画水平线函数
*             
* Arguments  : x0,y0：起始点坐标
*							 x1: 终点x坐标，因为是水平点，y坐标一样，所以只需要提供另一个点的x坐标
*							 color: 线的颜色，对于单色屏，为0时灭，为1时显示
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_drawHLine(uint16_t x0, uint16_t y0, uint16_t x1, GUI_COLOR color)
{
	uint8_t temp = 0;
	
	if(x0 > x1) /* 如果起点坐标大于终点坐标，则交换一下，便于画图 */
	{
		temp = x1; x1 = x0; x0 = temp;
	}
	
	/* 画线 */
	do
	{
		gui_drawPoint(x0, y0, color);
		x0++;
	}while(x1 >= x0);
}

/*
*********************************************************************************************************
*                 gui_drawVLine                         
*
* Description: 画垂直线
*             
* Arguments  : x0,y0：起始点坐标
*							 y1: 终点y坐标
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_drawVLine(uint16_t x0, uint16_t y0, uint16_t y1, GUI_COLOR color)
{
	uint16_t temp = 0;
	
	if(y0 > y1) /* 如果起点坐标大于终点坐标，则交换一下，便于画图 */
	{
		temp = y1; y1 = y0; y0 = temp;
	}
	
	/* 画线 */
	do
	{
		gui_drawPoint(x0, y0, color);
		y0++;
	}while(y1 >= y0);
}

/*
*********************************************************************************************************
*                      gui_drawArc                    
*
* Description: GUI画圆弧函数
*             
* Arguments  : x,y: 圆弧中心坐标
*							 radius: 圆弧半径
*							 startAngle,endAngle: 起始和终止角度
*							 color: 颜色
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_drawArc(uint16_t x, uint16_t y, uint16_t radius, uint16_t startAngle, uint16_t endAngle, GUI_COLOR color)
{
	int32_t  draw_x, draw_y;					// 画图坐标变量
	int32_t  op_x, op_y;						// 操作坐标
	int32_t  op_2rr;							// 2*radius*radius值变量

	int32_t  pno_angle;						// 度角点的个数
	uint8_t  draw_on;							// 画点开关，为1时画点，为0时不画


	/* 参数过滤 */
	if(radius == 0) return;							// 半径为0则直接退出
	if(startAngle == endAngle) return;			// 起始角度与终止角度相同，退出
	if( (startAngle >= 360) || (endAngle >= 360) ) return;

	op_2rr = 2*radius*radius;							// 计算radius平方乖以2
	pno_angle = 0;
	/* 先计算出在此半径下的45度的圆弧的点数 */       
	op_x = radius;
	op_y = 0;
	while(1)
	{  pno_angle++; 							// 画点计数         
		/* 计算下一点 */
		op_y++;
		if( (2*op_x*op_x + 2*op_y*op_y - op_2rr - 2*op_x +1)>0 ) 	// 使用逐点比较法实现画圆弧
		{  op_x--;
		}
		if(op_y>=op_x) break;
	}

	draw_on = 0;								// 最开始关画点开关
	/* 设置起始点及终点 */
	if(endAngle > startAngle) draw_on = 1;		// 若终点大于起点，则从一开始即画点(359)
	startAngle = (360 - startAngle)*pno_angle/45;
	endAngle = (360 - endAngle)*pno_angle/45;
	if(startAngle == 0) startAngle=1;
	if(endAngle == 0) endAngle=1;

	/* 开始顺时针画弧，从359度开始(第4像限) */
	pno_angle = 0;

	draw_x = x+radius;
	draw_y = y;         
	op_x = radius;
	op_y = 0;
	while(1)
	{  /* 计算下一点 */
		op_y++;
		draw_y--;
		if( (2*op_x*op_x + 2*op_y*op_y - op_2rr - 2*op_x +1)>0 ) 	// 使用逐点比较法实现画圆弧
		{  op_x--;
			 draw_x--;
		}
		if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);			// 开始画图
		pno_angle++;
		if( (pno_angle == startAngle)||(pno_angle == endAngle) )			// 若遇到起点或终点，画点开关取反
		{  draw_on = 1-draw_on;
			 if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);
			 gui_drawLine(x, y, draw_x, draw_y, color);
		} 
		if(op_y >= op_x)
		{  if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);
			 break;
		}
	}

	while(1)
	{  /* 计算下一点 */
		op_x--;
		draw_x--;
		if( (2*op_x*op_x + 2*op_y*op_y - op_2rr + 2*op_y +1)<=0 ) // 使用逐点比较法实现画圆弧
		{  op_y++;
			 draw_y--;
		}
		if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);			// 开始画图
		pno_angle++;
		if( (pno_angle == startAngle)||(pno_angle == endAngle) )			// 若遇到起点或终点，画点开关取反
		{  draw_on = 1-draw_on;
			 if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);
			 gui_drawLine(x, y, draw_x, draw_y, color);
		} 
		
		if(op_x <= 0)
		{  if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);		// 开始画图
			 break;
		}
	}


	/* 开始顺时针画弧，从269度开始(第3像限) */
	draw_y = y-radius;
	draw_x = x;         
	op_y = radius;
	op_x = 0;
	while(1)
	{  /* 计算下一点 */
		op_x++;
		draw_x--;
		if( (2*op_x*op_x + 2*op_y*op_y - op_2rr - 2*op_y +1)>0 ) // 使用逐点比较法实现画圆弧
		{  op_y--;
			 draw_y++;
		}
		if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);			// 开始画图
		pno_angle++;
		if( (pno_angle == startAngle)||(pno_angle == endAngle) )			// 若遇到起点或终点，画点开关取反
		{  draw_on = 1-draw_on;
			 if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);
			 gui_drawLine(x, y, draw_x, draw_y, color);
		} 
		
		if(op_x >= op_y)
		{  if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);		// 开始画图
			 break;
		}
	}

	while(1)
	{  /* 计算下一点 */
		op_y--;
		draw_y++;
		if( (2*op_x*op_x + 2*op_y*op_y - op_2rr + 2*op_x +1)<=0 ) // 使用逐点比较法实现画圆弧
		{  op_x++;
			 draw_x--;
		}
		if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);			// 开始画图
		pno_angle++;
		if( (pno_angle == startAngle)||(pno_angle == endAngle) )			// 若遇到起点或终点，画点开关取反
		{  draw_on = 1-draw_on;
			 if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);
			 gui_drawLine(x, y, draw_x, draw_y, color);
		} 
		if(op_y <= 0)
		{  if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);
			 break;
		}
	}


	/* 开始顺时针画弧，从179度开始(第2像限) */
	draw_x = x-radius;
	draw_y = y;         
	op_x = radius;
	op_y = 0;
	while(1)
	{  /* 计算下一点 */
		op_y++;
		draw_y++;
		if( (2*op_x*op_x + 2*op_y*op_y - op_2rr - 2*op_x +1)>0 ) 	// 使用逐点比较法实现画圆弧
		{  op_x--;
			 draw_x++;
		}
		if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);			// 开始画图
		pno_angle++;
		if( (pno_angle == startAngle)||(pno_angle == endAngle) )			// 若遇到起点或终点，画点开关取反
		{  draw_on = 1-draw_on;
			 if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);
			 gui_drawLine(x, y, draw_x, draw_y, color);
		} 
		if(op_y >= op_x)
		{  if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);
			 break;
		}
	}

	while(1)
	{  /* 计算下一点 */
		op_x--;
		draw_x++;
		if( (2*op_x*op_x + 2*op_y*op_y - op_2rr + 2*op_y +1)<=0 ) // 使用逐点比较法实现画圆弧
		{  op_y++;
			 draw_y++;
		}
		if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);			// 开始画图
		pno_angle++;
		if( (pno_angle == startAngle)||(pno_angle == endAngle) )			// 若遇到起点或终点，画点开关取反
		{  draw_on = 1-draw_on;
			 if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);
			 gui_drawLine(x, y, draw_x, draw_y, color);
		} 
		
		if(op_x <= 0)
		{  if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);		// 开始画图
			 break;
		}
	}


	/* 开始顺时针画弧，从89度开始(第1像限) */
	draw_y = y+radius;
	draw_x = x;         
	op_y = radius;
	op_x = 0;
	while(1)
	{  /* 计算下一点 */
		op_x++;
		draw_x++;
		if( (2*op_x*op_x + 2*op_y*op_y - op_2rr - 2*op_y +1)>0 ) // 使用逐点比较法实现画圆弧
		{  op_y--;
			 draw_y--;
		}
		if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);			// 开始画图
		pno_angle++;
		if( (pno_angle == startAngle)||(pno_angle == endAngle) )			// 若遇到起点或终点，画点开关取反
		{  draw_on = 1-draw_on;
			 if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);
			 gui_drawLine(x, y, draw_x, draw_y, color);
		} 
		
		if(op_x>=op_y)
		{  if(draw_on==1) gui_drawPoint(draw_x, draw_y, color);		// 开始画图
			 break;
		}
	}

	while(1)
	{  /* 计算下一点 */
		op_y--;
		draw_y--;
		if( (2*op_x*op_x + 2*op_y*op_y - op_2rr + 2*op_x +1)<=0 ) // 使用逐点比较法实现画圆弧
		{  op_x++;
			 draw_x++;
		}
		if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);			// 开始画图
		pno_angle++;
		if( (pno_angle == startAngle)||(pno_angle == endAngle) )			// 若遇到起点或终点，画点开关取反
		{  draw_on = 1-draw_on;
			 if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);
			 gui_drawLine(x, y, draw_x, draw_y, color);
		} 
		if(op_y<=0)
		{  if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);
			 break;
		}
	}
}

/*
*********************************************************************************************************
*                      gui_drawRectangle                    
*
* Description: 画矩形
*             
* Arguments  : x0,y0: 矩形左上角的坐标
*							 x1,y1: 矩形右下角的坐标
*							 color: 颜色
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_drawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, GUI_COLOR color)
{
	gui_drawHLine(x0, y0, x1, color);	/* 画水平线 */
	gui_drawHLine(x0, y1, x1, color);
	gui_drawVLine(x0, y0, y1, color); /* 画垂直线 */
	gui_drawVLine(x1, y0, y1, color);
}

/*
*********************************************************************************************************
*                      gui_fillRectangle                    
*
* Description: 矩形填充
*             
* Arguments  : x0,y0: 矩形左上角的坐标
*							 x1,y1: 矩形右下角的坐标
*							 color: 颜色
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_fillRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, GUI_COLOR color)
{
	uint16_t temp = 0;
	
	if(x0 == x1) /* x轴坐标相等，说明是垂直线 */
	{
		gui_drawVLine(x0, y0, y1, color);
		return;
	}
	
	if(y0 == y1)	/* y坐标相等，说明是水平线 */
	{
		gui_drawHLine(x0, y0, x1, color);
		return ;
	}
	
	/* 先处理两个点的大小，便于后面处理 */
	if(x0 > x1)
	{
		temp = x0;
		x0 = x1;
		x1 = temp;
	}
	
	if(y0 > y1)
	{
		temp = y0;
		y0 = y1;
		y1 = temp;
	}
	
	do
	{
		gui_drawHLine(x0, y0, x1, color);
		y0++;
	}while(y0 <= y1);
}

/*
*********************************************************************************************************
*                     gui_drawSquare                     
*
* Description: 画正方形
*             
* Arguments  : x0,y0: 正方形左上角坐标
*						   sideLen: 正方形边长
*							 color: 颜色
*
* Reutrn     : None.
*
* Note(s)    : 正方形坐标起点加上边长不能大于屏幕长宽
*********************************************************************************************************
*/
void gui_drawSquare(uint16_t x0, uint16_t y0, uint16_t sideLen, GUI_COLOR color)
{
	/* 判断数据是否有效 */
	if(x0 > LCD_X || y0 > LCD_Y) return;
	if((x0 + sideLen) > LCD_X) return;
	if((y0 + sideLen) > LCD_Y) return;
	
	/* 边长为0则画点 */
	if(sideLen == 0) gui_drawPoint(x0, y0, color);
	
	gui_drawRectangle(x0, y0, x0 + sideLen, y0 + sideLen, color);
}

/*
*********************************************************************************************************
*                  gui_fillSquare                        
*
* Description: 正方形填充
*             
* Arguments  : x0,y0: 正方形左上角坐标
*						   sideLen: 正方形边长
*							 color: 颜色
*
* Reutrn     : None.
*
* Note(s)    : 正方形坐标起点加上边长不能大于屏幕长宽
*********************************************************************************************************
*/
void gui_fillSquare(uint16_t x0, uint16_t y0, uint16_t sideLen, GUI_COLOR color)
{
	/* 判断数据是否有效 */
	if(x0 > LCD_X || y0 > LCD_Y) return;
	if((x0 + sideLen) > LCD_X) return;
	if((y0 + sideLen) > LCD_Y) return;
	
	/* 边长为0则画点 */
	if(sideLen == 0) gui_drawPoint(x0, y0, color);
	
	gui_fillRectangle(x0, y0, x0 + sideLen, y0 + sideLen, color);
}

/*
*********************************************************************************************************
*                  gui_drawPolygon                        
*
* Description: 画多边形，图形闭合
*             
* Arguments  : polygon: 多边形，为点集合
*							 points: 点数
*							 color: 颜色
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_drawPolygon(const GUI_POINT *pPolygon, uint8_t points, GUI_COLOR color)
{
	uint16_t x0 = 0, y0 = 0;	/* 用于画线 */
	uint16_t orgin_x0 = 0, orgin_y0 = 0;	/* 用于保存第一个点的坐标， */
	
	if(points == 0) return ;	/* 点的个数无效直接返回 */
	else if(points == 1) /* 只有一个点，画完也直接返回 */
	{
		gui_drawPoint(pPolygon->x, pPolygon->y, color);
		return ;
	}
	else if(points == 2)	/* 只有两个点，则只画一条线，画完返回 */
	{
		gui_drawLine(pPolygon->x, pPolygon->y, (pPolygon+1)->x, pPolygon->y, color);
		return ;
	}
	
	/* 保存起点坐标，因为画的图形需要闭合 */
	orgin_x0 = pPolygon->x;
	orgin_y0 = pPolygon->y;
	
	while(points > 1)	/* 因为要两点才能构成一条线 */
	{
		x0 = pPolygon->x;
		y0 = pPolygon->y;
		pPolygon++;	/* 指向下一个点 */
		gui_drawLine(x0, y0, pPolygon->x, pPolygon->y, color);
		
		points--;	/* 点数减少一个 */
	}	
	/* 画最后一条线，构成闭合图形 */
	gui_drawLine(pPolygon->x, pPolygon->y, orgin_x0, orgin_y0, color);
}

/*
*********************************************************************************************************
*                  gui_drawPolygon                        
*
* Description: 画多条线，图形不闭合
*             
* Arguments  : polygon: 多边形，为点集合
*							 points: 点数
*							 color: 颜色
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_drawPolyLine(const GUI_POINT *pPolyLine, uint8_t points, GUI_COLOR color)
{
	uint16_t x0 = 0, y0 = 0;	/* 用于画线 */

GUI_Context.drawPosX = 1;
	
	if(points == 0) return ;	/* 点的个数无效直接返回 */
	else if(points == 1) /* 只有一个点，画完也直接返回 */
	{
		gui_drawPoint(pPolyLine->x, pPolyLine->y, color);
		return ;
	}
	else if(points == 2)	/* 只有两个点，则只画一条线，画完返回 */
	{
		gui_drawLine(pPolyLine->x, pPolyLine->y, (pPolyLine+1)->x, pPolyLine->y, color);
		return ;
	}
	
	while(points > 1)	/* 因为要两点才能构成一条线 */
	{
		x0 = pPolyLine->x;
		y0 = pPolyLine->y;
		pPolyLine++;	/* 指向下一个点 */
		gui_drawLine(x0, y0, pPolyLine->x, pPolyLine->y, color);
		
		points--;	/* 点数减少一个 */
	}	
}

/*
*********************************************************************************************************
*                      gui_drawCircle                    
*
* Description: 画圆函数
*             
* Arguments  : x,y: 圆心坐标
*							 radius: 圆半径
*							 color: 颜色
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_drawCircle(uint16_t x, uint16_t y, uint16_t radius, GUI_COLOR color)
{
	int32_t  draw_x0, draw_y0;			// 刽图点坐标变量
	int32_t  draw_x1, draw_y1;	
	int32_t  draw_x2, draw_y2;	
	int32_t  draw_x3, draw_y3;	
	int32_t  draw_x4, draw_y4;	
	int32_t  draw_x5, draw_y5;	
	int32_t  draw_x6, draw_y6;	
	int32_t  draw_x7, draw_y7;	
	int32_t  xx, yy;					// 画圆控制变量
	int32_t  di;						// 决策变量

	/* 参数过滤 */
	if(0 == radius) gui_drawPoint(x, y, color);;

	/* 计算出8个特殊点(0、45、90、135、180、225、270度)，进行显示 */
	draw_x0 = draw_x1 = x;
	draw_y0 = draw_y1 = y + radius;
	if(draw_y0 < LCD_Y) gui_drawPoint(draw_x0, draw_y0, color);	// 90度

	draw_x2 = draw_x3 = x;
	draw_y2 = draw_y3 = y - radius;
	if(draw_y2 >= 0) gui_drawPoint(draw_x2, draw_y2, color);			// 270度


	draw_x4 = draw_x6 = x + radius;
	draw_y4 = draw_y6 = y;
	if(draw_x4 < LCD_X) gui_drawPoint(draw_x4, draw_y4, color);	// 0度

	draw_x5 = draw_x7 = x - radius;
	draw_y5 = draw_y7 = y;
	if(draw_x5 >= 0) gui_drawPoint(draw_x5, draw_y5, color);			// 180度   
	if(1 == radius) return;					// 若半径为1，则已圆画完
   
   
	/* 使用Bresenham法进行画圆 */
	di = 3 - 2*radius;					// 初始化决策变量

	xx = 0;
	yy = radius;	
	while(xx < yy)
	{  
		if(di < 0)
			di += 4*xx + 6;	      
		else
		{  
			di += 4*(xx - yy) + 10;
			yy--;	  
			draw_y0--;
			draw_y1--;
			draw_y2++;
			draw_y3++;
			draw_x4--;
			draw_x5++;
			draw_x6--;
			draw_x7++;	 	
		}
		xx++;   
		draw_x0++;
		draw_x1--;
		draw_x2++;
		draw_x3--;
		draw_y4++;
		draw_y5++;
		draw_y6--;
		draw_y7--;

		/* 要判断当前点是否在有效范围内 */
		if( (draw_x0<=LCD_X)&&(draw_y0>=0) )	
			gui_drawPoint(draw_x0, draw_y0, color);

		if( (draw_x1>=0)&&(draw_y1>=0) )	
			gui_drawPoint(draw_x1, draw_y1, color);

		if( (draw_x2<=LCD_X)&&(draw_y2<=LCD_Y) )	
			gui_drawPoint(draw_x2, draw_y2, color);   

		if( (draw_x3>=0)&&(draw_y3<=LCD_Y) )	
			gui_drawPoint(draw_x3, draw_y3, color);

		if( (draw_x4<=LCD_X)&&(draw_y4>=0) )	
			gui_drawPoint(draw_x4, draw_y4, color);

		if( (draw_x5>=0)&&(draw_y5>=0) )	
			gui_drawPoint(draw_x5, draw_y5, color);

		if( (draw_x6<=LCD_X)&&(draw_y6<=LCD_Y) )	
			gui_drawPoint(draw_x6, draw_y6, color);

		if( (draw_x7>=0)&&(draw_y7<=LCD_Y) )	
			gui_drawPoint(draw_x7, draw_y7, color);
	}
}

/*
*********************************************************************************************************
*                   gui_fillCircle                       
*
* Description: 圆形填充
*             
* Arguments  : x,y: 圆心坐标
*							 radius: 圆半径
*							 color: 颜色
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_fillCircle(uint16_t x, uint16_t y, uint16_t radius, GUI_COLOR color)
{
	int32_t  draw_x0, draw_y0;			// 刽图点坐标变量
	int32_t  draw_x1, draw_y1;	
	int32_t  draw_x2, draw_y2;	
	int32_t  draw_x3, draw_y3;	
	int32_t  draw_x4, draw_y4;	
	int32_t  draw_x5, draw_y5;	
	int32_t  draw_x6, draw_y6;	
	int32_t  draw_x7, draw_y7;	
	int32_t  fill_x0, fill_y0;			// 填充所需的变量，使用垂直线填充
	int32_t  fill_x1;
	int32_t  xx, yy;					// 画圆控制变量
	int32_t  di;						// 决策变量

	/* 参数过滤 */
	if(0 == radius) return;

	/* 计算出4个特殊点(0、90、180、270度)，进行显示 */
	draw_x0 = draw_x1 = x;
	draw_y0 = draw_y1 = y + radius;
	if(draw_y0<LCD_Y)
		gui_drawPoint(draw_x0, draw_y0, color);	// 90度

	draw_x2 = draw_x3 = x;
	draw_y2 = draw_y3 = y - radius;
	if(draw_y2>=0)
		gui_drawPoint(draw_x2, draw_y2, color);	// 270度

	draw_x4 = draw_x6 = x + radius;
	draw_y4 = draw_y6 = y;
	if(draw_x4<LCD_X) 
	{  
		gui_drawPoint(draw_x4, draw_y4, color);	// 0度
		fill_x1 = draw_x4;
	}
	else
		fill_x1 = LCD_X;
	
	fill_y0 = y;							// 设置填充线条起始点fill_x0
	fill_x0 = x - radius;						// 设置填充线条结束点fill_y1
	if(fill_x0<0) fill_x0 = 0;
	gui_drawHLine(fill_x0, fill_y0, fill_x1, color);

	draw_x5 = draw_x7 = x - radius;
	draw_y5 = draw_y7 = y;
	if(draw_x5>=0) 
		gui_drawPoint(draw_x5, draw_y5, color);	// 180度
	if(1==radius) return;


	/* 使用Bresenham法进行画圆 */
	di = 3 - 2*radius;							// 初始化决策变量

	xx = 0;
	yy = radius;
	while(xx<yy)
	{  
		if(di<0)
			di += 4*xx + 6;
		else
		{  
			di += 4*(xx - yy) + 10;
			yy--;	  
			draw_y0--;
			draw_y1--;
			draw_y2++;
			draw_y3++;
			draw_x4--;
			draw_x5++;
			draw_x6--;
			draw_x7++;		 
		}
		xx++;   
		draw_x0++;
		draw_x1--;
		draw_x2++;
		draw_x3--;
		draw_y4++;
		draw_y5++;
		draw_y6--;
		draw_y7--;
		
		/* 要判断当前点是否在有效范围内 */
		if( (draw_x0<=LCD_X)&&(draw_y0>=0) )	
			gui_drawPoint(draw_x0, draw_y0, color);

		if( (draw_x1>=0)&&(draw_y1>=0) )	
			gui_drawPoint(draw_x1, draw_y1, color);
	  
		/* 第二点水直线填充(下半圆的点) */
		if(draw_x1>=0)
		{  /* 设置填充线条起始点fill_x0 */
			fill_x0 = draw_x1;
			/* 设置填充线条起始点fill_y0 */
			fill_y0 = draw_y1;
			if(fill_y0>LCD_Y) fill_y0 = LCD_Y;
			if(fill_y0<0) fill_y0 = 0; 
			/* 设置填充线条结束点fill_x1 */									
			fill_x1 = x*2 - draw_x1;				
			if(fill_x1>LCD_X) fill_x1 = LCD_X;
			gui_drawHLine(fill_x0, fill_y0, fill_x1, color);
		}
	  
		if( (draw_x2<=LCD_X)&&(draw_y2<=LCD_Y) )	
			gui_drawPoint(draw_x2, draw_y2, color);   
		  
		if( (draw_x3>=0)&&(draw_y3<=LCD_Y) )	
			gui_drawPoint(draw_x3, draw_y3, color);
	  
		/* 第四点垂直线填充(上半圆的点) */
		if(draw_x3>=0)
		{  /* 设置填充线条起始点fill_x0 */
			fill_x0 = draw_x3;
			/* 设置填充线条起始点fill_y0 */
			fill_y0 = draw_y3;
			if(fill_y0>LCD_Y) fill_y0 = LCD_Y;
			if(fill_y0<0) fill_y0 = 0;
			/* 设置填充线条结束点fill_x1 */									
			fill_x1 = x*2 - draw_x3;				
			if(fill_x1>LCD_X) fill_x1 = LCD_X;
			gui_drawHLine(fill_x0, fill_y0, fill_x1, color);
		}	 
		if( (draw_x4<=LCD_X)&&(draw_y4>=0) )	
			gui_drawPoint(draw_x4, draw_y4, color);

		if( (draw_x5>=0)&&(draw_y5>=0) )	
			gui_drawPoint(draw_x5, draw_y5, color);
	  
		/* 第六点垂直线填充(上半圆的点) */
		if(draw_x5>=0)
		{  /* 设置填充线条起始点fill_x0 */
			fill_x0 = draw_x5;
			/* 设置填充线条起始点fill_y0 */
			fill_y0 = draw_y5;
			if(fill_y0>LCD_Y) fill_y0 = LCD_Y;
			if(fill_y0<0) fill_y0 = 0;
			/* 设置填充线条结束点fill_x1 */									
			fill_x1 = x*2 - draw_x5;				
			if(fill_x1>LCD_X) fill_x1 = LCD_X;
			gui_drawHLine(fill_x0, fill_y0, fill_x1, color);
		}

		if( (draw_x6<=LCD_X)&&(draw_y6<=LCD_Y) )	
			gui_drawPoint(draw_x6, draw_y6, color);
		if( (draw_x7>=0)&&(draw_y7<=LCD_Y) )	
			gui_drawPoint(draw_x7, draw_y7, color);

		/* 第八点垂直线填充(上半圆的点) */
		if(draw_x7>=0)
		{  /* 设置填充线条起始点fill_x0 */
			fill_x0 = draw_x7;
			/* 设置填充线条起始点fill_y0 */
			fill_y0 = draw_y7;
			if(fill_y0>LCD_Y) fill_y0 = LCD_Y;
			if(fill_y0<0) fill_y0 = 0;
			/* 设置填充线条结束点fill_x1 */									
			fill_x1 = x*2 - draw_x7;				
			if(fill_x1>LCD_X) fill_x1 = LCD_X;
			gui_drawHLine(fill_x0, fill_y0, fill_x1, color);
		}
	}
}

/*
*********************************************************************************************************
*                         gui_drawEllipse                 
*
* Description: 画正椭圆
*             
* Arguments  : x,y: 椭圆圆心
*							 a,b: 椭圆长短轴轴长
*							 color: 颜色
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_drawEllipse(uint16_t x, uint16_t y, uint16_t a, uint16_t b, GUI_COLOR color)
{
	uint16_t x0 = 0, y0 = 0, x1 = 0, y1 = 0;
	int32_t  draw_x0, draw_y0;			// 刽图点坐标变量
	int32_t  draw_x1, draw_y1;
	int32_t  draw_x2, draw_y2;
	int32_t  draw_x3, draw_y3;
	int32_t  xx, yy;					// 画图控制变量

	int32_t  center_x, center_y;		// 椭圆中心点坐标变量
	int32_t  radius_x, radius_y;		// 椭圆的半径，x轴半径和y轴半径
	int32_t  radius_xx, radius_yy;		// 半径乘平方值
	int32_t  radius_xx2, radius_yy2;	// 半径乘平方值的两倍
	int32_t  di;						// 定义决策变量
	
	x0 = x - a/2;	/* 计算最左边的点的x坐标 */
	x1 = x + a/2;	/* 最右边的点 */
	y0 = y - b/2;	/* 上边 */
	y1 = y + a/2;	/* 下边 */
	
	/* 参数过滤 */
	if( (x0==x1) || (y0==y1) ) return;

	/* 计算出椭圆中心点坐标 */
	center_x = (x0 + x1) >> 1;			
	center_y = (y0 + y1) >> 1;

	/* 计算出椭圆的半径，x轴半径和y轴半径 */
	if(x0 > x1)
		radius_x = (x0 - x1) >> 1;
	else
		radius_x = (x1 - x0) >> 1;
	if(y0 > y1)
		radius_y = (y0 - y1) >> 1;
	else
		radius_y = (y1 - y0) >> 1;
   
	/* 计算半径平方值 */
	radius_xx = radius_x * radius_x;
	radius_yy = radius_y * radius_y;

	/* 计算半径平方值乘2值 */
	radius_xx2 = radius_xx<<1;
	radius_yy2 = radius_yy<<1;

	/* 初始化画图变量 */
	xx = 0;
	yy = radius_y;
	di = radius_yy2 + radius_xx - radius_xx2*radius_y ;	// 初始化决策变量 

	/* 计算出椭圆y轴上的两个端点坐标，作为作图起点 */
	draw_x0 = draw_x1 = draw_x2 = draw_x3 = center_x;
	draw_y0 = draw_y1 = center_y + radius_y;
	draw_y2 = draw_y3 = center_y - radius_y;

	gui_drawPoint(draw_x0, draw_y0, color);					// 画y轴上的两个端点 
	gui_drawPoint(draw_x2, draw_y2, color);

	while( (radius_yy*xx) < (radius_xx*yy) ) 
	{  	
		if(di<0)
	 		di+= radius_yy2*(2*xx+3);
		else
		{  
			di += radius_yy2*(2*xx+3) + 4*radius_xx - 4*radius_xx*yy;
			yy--;
			draw_y0--;
			draw_y1--;
			draw_y2++;
			draw_y3++;				 
		}
		xx ++;						// x轴加1
		draw_x0++;
		draw_x1--;
		draw_x2++;
		draw_x3--;

		gui_drawPoint(draw_x0, draw_y0, color);
		gui_drawPoint(draw_x1, draw_y1, color);
		gui_drawPoint(draw_x2, draw_y2, color);
		gui_drawPoint(draw_x3, draw_y3, color);
	}
  
	di = radius_xx2*(yy-1)*(yy-1) + radius_yy2*xx*xx + radius_yy + radius_yy2*xx - radius_xx2*radius_yy;
	while(yy>=0) 
	{  
		if(di<0)
		{  
			di+= radius_xx2*3 + 4*radius_yy*xx + 4*radius_yy - 2*radius_xx2*yy;

			xx ++;						// x轴加1	 		
			draw_x0++;
			draw_x1--;
			draw_x2++;
			draw_x3--;  
		}
		else
			di += radius_xx2*3 - 2*radius_xx2*yy;	 	 		     			 
		yy--;
		draw_y0--;
		draw_y1--;
		draw_y2++;
		draw_y3++;	

		gui_drawPoint(draw_x0, draw_y0, color);
		gui_drawPoint(draw_x1, draw_y1, color);
		gui_drawPoint(draw_x2, draw_y2, color);
		gui_drawPoint(draw_x3, draw_y3, color);
	}  
}

/*
*********************************************************************************************************
*                       gui_fillEllipse                   
*
* Description: 填充正椭圆
*             
* Arguments  : x,y: 椭圆圆心
*							 a,b: 椭圆长短轴轴长
*							 color: 颜色
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_fillEllipse(uint16_t x, uint16_t y, uint16_t a, uint16_t b, GUI_COLOR color)
{
	uint16_t x0 = 0, y0 = 0, x1 = 0, y1 = 0;
	int32_t  draw_x0, draw_y0;			// 刽图点坐标变量
	int32_t  draw_x1, draw_y1;
	int32_t  draw_x2, draw_y2;
	int32_t  draw_x3, draw_y3;
	int32_t  xx, yy;					// 画图控制变量

	int32_t  center_x, center_y;		// 椭圆中心点坐标变量
	int32_t  radius_x, radius_y;		// 椭圆的半径，x轴半径和y轴半径
	int32_t  radius_xx, radius_yy;		// 半径乘平方值
	int32_t  radius_xx2, radius_yy2;	// 半径乘平方值的两倍
	int32_t  di;						// 定义决策变量

	x0 = x - a/2;	/* 计算最左边的点的x坐标 */
	x1 = x + a/2;	/* 最右边的点 */
	y0 = y - b/2;	/* 上边 */
	y1 = y + a/2;	/* 下边 */
	
	/* 参数过滤 */
	if( (x0==x1) || (y0==y1) ) return;

	/* 计算出椭圆中心点坐标 */
	center_x = (x0 + x1) >> 1;			
	center_y = (y0 + y1) >> 1;

	/* 计算出椭圆的半径，x轴半径和y轴半径 */
	if(x0 > x1)
	{  radius_x = (x0 - x1) >> 1;
	}
	else
	{  radius_x = (x1 - x0) >> 1;
	}
	if(y0 > y1)
	{  radius_y = (y0 - y1) >> 1;
	}
	else
	{  radius_y = (y1 - y0) >> 1;
	}

	/* 计算半径乘平方值 */
	radius_xx = radius_x * radius_x;
	radius_yy = radius_y * radius_y;

	/* 计算半径乘4值 */
	radius_xx2 = radius_xx<<1;
	radius_yy2 = radius_yy<<1;

	/* 初始化画图变量 */
	xx = 0;
	yy = radius_y;

	di = radius_yy2 + radius_xx - radius_xx2*radius_y ;	// 初始化决策变量 

	/* 计算出椭圆y轴上的两个端点坐标，作为作图起点 */
	draw_x0 = draw_x1 = draw_x2 = draw_x3 = center_x;
	draw_y0 = draw_y1 = center_y + radius_y;
	draw_y2 = draw_y3 = center_y - radius_y;


	gui_drawPoint(draw_x0, draw_y0, color);					// 画y轴上的两个端点
	gui_drawPoint(draw_x2, draw_y2, color);

	while( (radius_yy*xx) < (radius_xx*yy) ) 
	{  if(di<0)
	{  di+= radius_yy2*(2*xx+3);
	}
	else
	{  di += radius_yy2*(2*xx+3) + 4*radius_xx - 4*radius_xx*yy;
		
		 yy--;
	 draw_y0--;
	 draw_y1--;
	 draw_y2++;
	 draw_y3++;				 
	}

	xx ++;						// x轴加1
		
	draw_x0++;
	draw_x1--;
	draw_x2++;
	draw_x3--;

	gui_drawPoint(draw_x0, draw_y0, color);
	gui_drawPoint(draw_x1, draw_y1, color);
	gui_drawPoint(draw_x2, draw_y2, color);
	gui_drawPoint(draw_x3, draw_y3, color);

	/* 若y轴已变化，进行填充 */
	if(di>=0)
	{  gui_drawHLine(draw_x0, draw_y0, draw_x1, color);
		 gui_drawHLine(draw_x2, draw_y2, draw_x3, color);
	}
	}

	di = radius_xx2*(yy-1)*(yy-1) + radius_yy2*xx*xx + radius_yy + radius_yy2*xx - radius_xx2*radius_yy;
	while(yy>=0) 
	{  if(di<0)
	{  di+= radius_xx2*3 + 4*radius_yy*xx + 4*radius_yy - 2*radius_xx2*yy;
		
		 xx ++;						// x轴加1	 		
		 draw_x0++;
		 draw_x1--;
		 draw_x2++;
		 draw_x3--;  
	}
	else
	{  di += radius_xx2*3 - 2*radius_xx2*yy;	 	 		     			 
	}

	yy--;
	draw_y0--;
	draw_y1--;
	draw_y2++;
	draw_y3++;	

	gui_drawPoint(draw_x0, draw_y0, color);
	gui_drawPoint(draw_x1, draw_y1, color);
	gui_drawPoint(draw_x2, draw_y2, color);
	gui_drawPoint(draw_x3, draw_y3, color);

	/* y轴已变化，进行填充 */
	gui_drawHLine(draw_x0, draw_y0, draw_x1, color);
	gui_drawHLine(draw_x2, draw_y2, draw_x3, color); 
	}  	
}

/*
*********************************************************************************************************
*                                          
*
* Description: 
*             
* Arguments  : 
*
* Reutrn     : 
*
* Note(s)    : 
*********************************************************************************************************
*/
void gui_drawSector(uint16_t x, uint16_t y, uint16_t radius, uint16_t startAngle, uint16_t endAngle, GUI_COLOR color)
{
}

/*
*********************************************************************************************************
*                                          
*
* Description: 
*             
* Arguments  : 
*
* Reutrn     : 
*
* Note(s)    : 
*********************************************************************************************************
*/
void gui_fillSector(uint16_t x, uint16_t y, uint16_t radius, uint16_t startAngle, uint16_t endAngle, GUI_COLOR color)
{
}

/********************************************  END OF FILE  *******************************************/


