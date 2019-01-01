/**
  *******************************************************************************************************
  * File Name: gui_basic.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: �ṩGUI����2D��ͼ���������ֺ����ο�����ԭ��ң��GUI�еĳ���ԭ����Ϊ���ܱ�
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-29
	*			Mod: �����ļ�
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
* Description: ������Ƕȵ���
*             
* Arguments  : x0,y0: ��ʼ�������
*							 x1,y1: �յ�����
*							 color: �ߵ���ɫ
*
* Reutrn     : None.
*
* Note(s)    : ���ڵ�ɫ����ɫ��Ч
*********************************************************************************************************
*/
void gui_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, GUI_COLOR color)
{
	int32_t dx = 0, dy = 0;
	int8_t dx_dir = 0, dy_dir = 0;
	int32_t dx_x2 = 0, dy_x2 = 0;
	int32_t di = 0;
	
	dx = x1 - x0;	/* ������֮��Ĳ�ֵ */
	dy = y1 - y0;
	
	/* �ж��������� */
	if(dx > 0) dx_dir = 1;
	else if(dx < 0) dx_dir = -1;
	else 	/* �Ǵ�ֱ�ߣ�ֱ�ӻ���ֱ�� */
		gui_drawVLine(x0, y0, y1, color);
	
	if(dy > 0) dy_dir = 1;
	else if(dy < 0) dy_dir = -1;
	else /* ��ˮƽ�ߣ�ֱ�ӻ�ˮƽ�� */
		gui_drawHLine(x0, y0, x1, color);
	
	/* ��dx��dyȡ����ֵ */
	dx = dx_dir * dx;
	dy = dy_dir * dy;

	/* ����2����dx��dyֵ */
	dx_x2 = dx*2;
	dy_x2 = dy*2;
	
	/* ʹ��Bresenham�����л�ֱ�� */
	if(dx>=dy)						// ����dx>=dy����ʹ��x��Ϊ��׼
	{  
		di = dy_x2 - dx;
		while(x0!=x1)
		{  
			gui_drawPoint(x0, y0, color);
			x0 += dx_dir;
			if(di<0)
				di += dy_x2;		// �������һ���ľ���ֵ
			else
			{  
				di += dy_x2 - dx_x2;
				y0 += dy_dir;
			}
		}
		gui_drawPoint(x0, y0, color);	// ��ʾ���һ��
	}
	else							// ����dx<dy����ʹ��y��Ϊ��׼
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
		gui_drawPoint(x0, y0, color);		// ��ʾ���һ��
	} 
}

/*
*********************************************************************************************************
*                    gui_drawLineWidth                      
*
* Description: ���ߺ���,���������߿�
*             
* Arguments  : x0,y0: �������
*							 x1,y1: �յ�����
*							 width: �߿�(0 - 50)
*							 color: ��ɫ
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_drawLineWidth(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t width, GUI_COLOR color)
{
		int32_t   dx;						// ֱ��x���ֵ����
	int32_t   dy;          				// ֱ��y���ֵ����
	int8_t    dx_dir;					// x����������Ϊ-1ʱ��ֵ����Ϊ1ʱ��ֵ����
	int8_t    dy_dir;					// y����������Ϊ-1ʱ��ֵ����Ϊ1ʱ��ֵ����
	int32_t   dx_x2;					// dx*2ֵ���������ڼӿ������ٶ�
	int32_t   dy_x2;					// dy*2ֵ���������ڼӿ������ٶ�
	int32_t   di;						// ���߱���
	int32_t   wx, wy;					// �߿����
	int32_t   draw_a, draw_b;

	/* �������� */
	if(width==0) return;
	if(width>50) width = 50;

	dx = x1-x0;							// ��ȡ����֮��Ĳ�ֵ
	dy = y1-y0;
	wx = width/2;
	wy = width-wx-1;

	/* �ж��������򣬻��Ƿ�Ϊˮƽ�ߡ���ֱ�ߡ��� */
	if(dx>0)							// �ж�x�᷽��
	{  
		dx_dir = 1;						// dx>0������dx_dir=1
	}
	else
	{  
		if(dx<0) 
			dx_dir = -1;				// dx<0������dx_dir=-1
		else/* dx==0������ֱ�ߣ���һ�� */
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

	if(dy>0)							// �ж�y�᷽��
		dy_dir = 1;						// dy>0������dy_dir=1
	else
	{  
		if(dy<0)
			dy_dir = -1;				// dy<0������dy_dir=-1	
		else /* dy==0����ˮƽ�ߣ���һ�� */
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

	/* ��dx��dyȡ����ֵ */
	dx = dx_dir * dx;
	dy = dy_dir * dy;

	/* ����2����dx��dyֵ */
	dx_x2 = dx*2;
	dy_x2 = dy*2;

	/* ʹ��Bresenham�����л�ֱ�� */
	if(dx>=dy)						// ����dx>=dy����ʹ��x��Ϊ��׼
	{  
		di = dy_x2 - dx;
		while(x0!=x1)/* x����������������y���򣬼�����ֱ�� */
		{  
			draw_a = y0-wx;
			if(draw_a<0) draw_a = 0;
			draw_b = y0+wy;
			gui_drawVLine(x0, draw_a, draw_b, color);

			x0 += dx_dir;				
			if(di<0)
				di += dy_x2;			// �������һ���ľ���ֵ
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
	else								// ����dx<dy����ʹ��y��Ϊ��׼
	{  
		di = dx_x2 - dy;
		while(y0!=y1) /* y����������������x���򣬼���ˮƽ�� */
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
* Description: GUI��ˮƽ�ߺ���
*             
* Arguments  : x0,y0����ʼ������
*							 x1: �յ�x���꣬��Ϊ��ˮƽ�㣬y����һ��������ֻ��Ҫ�ṩ��һ�����x����
*							 color: �ߵ���ɫ�����ڵ�ɫ����Ϊ0ʱ��Ϊ1ʱ��ʾ
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_drawHLine(uint16_t x0, uint16_t y0, uint16_t x1, GUI_COLOR color)
{
	uint8_t temp = 0;
	
	if(x0 > x1) /* ��������������յ����꣬�򽻻�һ�£����ڻ�ͼ */
	{
		temp = x1; x1 = x0; x0 = temp;
	}
	
	/* ���� */
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
* Description: ����ֱ��
*             
* Arguments  : x0,y0����ʼ������
*							 y1: �յ�y����
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_drawVLine(uint16_t x0, uint16_t y0, uint16_t y1, GUI_COLOR color)
{
	uint16_t temp = 0;
	
	if(y0 > y1) /* ��������������յ����꣬�򽻻�һ�£����ڻ�ͼ */
	{
		temp = y1; y1 = y0; y0 = temp;
	}
	
	/* ���� */
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
* Description: GUI��Բ������
*             
* Arguments  : x,y: Բ����������
*							 radius: Բ���뾶
*							 startAngle,endAngle: ��ʼ����ֹ�Ƕ�
*							 color: ��ɫ
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_drawArc(uint16_t x, uint16_t y, uint16_t radius, uint16_t startAngle, uint16_t endAngle, GUI_COLOR color)
{
	int32_t  draw_x, draw_y;					// ��ͼ�������
	int32_t  op_x, op_y;						// ��������
	int32_t  op_2rr;							// 2*radius*radiusֵ����

	int32_t  pno_angle;						// �Ƚǵ�ĸ���
	uint8_t  draw_on;							// ���㿪�أ�Ϊ1ʱ���㣬Ϊ0ʱ����


	/* �������� */
	if(radius == 0) return;							// �뾶Ϊ0��ֱ���˳�
	if(startAngle == endAngle) return;			// ��ʼ�Ƕ�����ֹ�Ƕ���ͬ���˳�
	if( (startAngle >= 360) || (endAngle >= 360) ) return;

	op_2rr = 2*radius*radius;							// ����radiusƽ������2
	pno_angle = 0;
	/* �ȼ�����ڴ˰뾶�µ�45�ȵ�Բ���ĵ��� */       
	op_x = radius;
	op_y = 0;
	while(1)
	{  pno_angle++; 							// �������         
		/* ������һ�� */
		op_y++;
		if( (2*op_x*op_x + 2*op_y*op_y - op_2rr - 2*op_x +1)>0 ) 	// ʹ�����ȽϷ�ʵ�ֻ�Բ��
		{  op_x--;
		}
		if(op_y>=op_x) break;
	}

	draw_on = 0;								// �ʼ�ػ��㿪��
	/* ������ʼ�㼰�յ� */
	if(endAngle > startAngle) draw_on = 1;		// ���յ������㣬���һ��ʼ������(359)
	startAngle = (360 - startAngle)*pno_angle/45;
	endAngle = (360 - endAngle)*pno_angle/45;
	if(startAngle == 0) startAngle=1;
	if(endAngle == 0) endAngle=1;

	/* ��ʼ˳ʱ�뻭������359�ȿ�ʼ(��4����) */
	pno_angle = 0;

	draw_x = x+radius;
	draw_y = y;         
	op_x = radius;
	op_y = 0;
	while(1)
	{  /* ������һ�� */
		op_y++;
		draw_y--;
		if( (2*op_x*op_x + 2*op_y*op_y - op_2rr - 2*op_x +1)>0 ) 	// ʹ�����ȽϷ�ʵ�ֻ�Բ��
		{  op_x--;
			 draw_x--;
		}
		if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);			// ��ʼ��ͼ
		pno_angle++;
		if( (pno_angle == startAngle)||(pno_angle == endAngle) )			// �����������յ㣬���㿪��ȡ��
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
	{  /* ������һ�� */
		op_x--;
		draw_x--;
		if( (2*op_x*op_x + 2*op_y*op_y - op_2rr + 2*op_y +1)<=0 ) // ʹ�����ȽϷ�ʵ�ֻ�Բ��
		{  op_y++;
			 draw_y--;
		}
		if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);			// ��ʼ��ͼ
		pno_angle++;
		if( (pno_angle == startAngle)||(pno_angle == endAngle) )			// �����������յ㣬���㿪��ȡ��
		{  draw_on = 1-draw_on;
			 if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);
			 gui_drawLine(x, y, draw_x, draw_y, color);
		} 
		
		if(op_x <= 0)
		{  if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);		// ��ʼ��ͼ
			 break;
		}
	}


	/* ��ʼ˳ʱ�뻭������269�ȿ�ʼ(��3����) */
	draw_y = y-radius;
	draw_x = x;         
	op_y = radius;
	op_x = 0;
	while(1)
	{  /* ������һ�� */
		op_x++;
		draw_x--;
		if( (2*op_x*op_x + 2*op_y*op_y - op_2rr - 2*op_y +1)>0 ) // ʹ�����ȽϷ�ʵ�ֻ�Բ��
		{  op_y--;
			 draw_y++;
		}
		if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);			// ��ʼ��ͼ
		pno_angle++;
		if( (pno_angle == startAngle)||(pno_angle == endAngle) )			// �����������յ㣬���㿪��ȡ��
		{  draw_on = 1-draw_on;
			 if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);
			 gui_drawLine(x, y, draw_x, draw_y, color);
		} 
		
		if(op_x >= op_y)
		{  if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);		// ��ʼ��ͼ
			 break;
		}
	}

	while(1)
	{  /* ������һ�� */
		op_y--;
		draw_y++;
		if( (2*op_x*op_x + 2*op_y*op_y - op_2rr + 2*op_x +1)<=0 ) // ʹ�����ȽϷ�ʵ�ֻ�Բ��
		{  op_x++;
			 draw_x--;
		}
		if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);			// ��ʼ��ͼ
		pno_angle++;
		if( (pno_angle == startAngle)||(pno_angle == endAngle) )			// �����������յ㣬���㿪��ȡ��
		{  draw_on = 1-draw_on;
			 if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);
			 gui_drawLine(x, y, draw_x, draw_y, color);
		} 
		if(op_y <= 0)
		{  if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);
			 break;
		}
	}


	/* ��ʼ˳ʱ�뻭������179�ȿ�ʼ(��2����) */
	draw_x = x-radius;
	draw_y = y;         
	op_x = radius;
	op_y = 0;
	while(1)
	{  /* ������һ�� */
		op_y++;
		draw_y++;
		if( (2*op_x*op_x + 2*op_y*op_y - op_2rr - 2*op_x +1)>0 ) 	// ʹ�����ȽϷ�ʵ�ֻ�Բ��
		{  op_x--;
			 draw_x++;
		}
		if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);			// ��ʼ��ͼ
		pno_angle++;
		if( (pno_angle == startAngle)||(pno_angle == endAngle) )			// �����������յ㣬���㿪��ȡ��
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
	{  /* ������һ�� */
		op_x--;
		draw_x++;
		if( (2*op_x*op_x + 2*op_y*op_y - op_2rr + 2*op_y +1)<=0 ) // ʹ�����ȽϷ�ʵ�ֻ�Բ��
		{  op_y++;
			 draw_y++;
		}
		if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);			// ��ʼ��ͼ
		pno_angle++;
		if( (pno_angle == startAngle)||(pno_angle == endAngle) )			// �����������յ㣬���㿪��ȡ��
		{  draw_on = 1-draw_on;
			 if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);
			 gui_drawLine(x, y, draw_x, draw_y, color);
		} 
		
		if(op_x <= 0)
		{  if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);		// ��ʼ��ͼ
			 break;
		}
	}


	/* ��ʼ˳ʱ�뻭������89�ȿ�ʼ(��1����) */
	draw_y = y+radius;
	draw_x = x;         
	op_y = radius;
	op_x = 0;
	while(1)
	{  /* ������һ�� */
		op_x++;
		draw_x++;
		if( (2*op_x*op_x + 2*op_y*op_y - op_2rr - 2*op_y +1)>0 ) // ʹ�����ȽϷ�ʵ�ֻ�Բ��
		{  op_y--;
			 draw_y--;
		}
		if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);			// ��ʼ��ͼ
		pno_angle++;
		if( (pno_angle == startAngle)||(pno_angle == endAngle) )			// �����������յ㣬���㿪��ȡ��
		{  draw_on = 1-draw_on;
			 if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);
			 gui_drawLine(x, y, draw_x, draw_y, color);
		} 
		
		if(op_x>=op_y)
		{  if(draw_on==1) gui_drawPoint(draw_x, draw_y, color);		// ��ʼ��ͼ
			 break;
		}
	}

	while(1)
	{  /* ������һ�� */
		op_y--;
		draw_y--;
		if( (2*op_x*op_x + 2*op_y*op_y - op_2rr + 2*op_x +1)<=0 ) // ʹ�����ȽϷ�ʵ�ֻ�Բ��
		{  op_x++;
			 draw_x++;
		}
		if(draw_on == 1) gui_drawPoint(draw_x, draw_y, color);			// ��ʼ��ͼ
		pno_angle++;
		if( (pno_angle == startAngle)||(pno_angle == endAngle) )			// �����������յ㣬���㿪��ȡ��
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
* Description: ������
*             
* Arguments  : x0,y0: �������Ͻǵ�����
*							 x1,y1: �������½ǵ�����
*							 color: ��ɫ
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_drawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, GUI_COLOR color)
{
	gui_drawHLine(x0, y0, x1, color);	/* ��ˮƽ�� */
	gui_drawHLine(x0, y1, x1, color);
	gui_drawVLine(x0, y0, y1, color); /* ����ֱ�� */
	gui_drawVLine(x1, y0, y1, color);
}

/*
*********************************************************************************************************
*                      gui_fillRectangle                    
*
* Description: �������
*             
* Arguments  : x0,y0: �������Ͻǵ�����
*							 x1,y1: �������½ǵ�����
*							 color: ��ɫ
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_fillRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, GUI_COLOR color)
{
	uint16_t temp = 0;
	
	if(x0 == x1) /* x��������ȣ�˵���Ǵ�ֱ�� */
	{
		gui_drawVLine(x0, y0, y1, color);
		return;
	}
	
	if(y0 == y1)	/* y������ȣ�˵����ˮƽ�� */
	{
		gui_drawHLine(x0, y0, x1, color);
		return ;
	}
	
	/* �ȴ���������Ĵ�С�����ں��洦�� */
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
* Description: ��������
*             
* Arguments  : x0,y0: ���������Ͻ�����
*						   sideLen: �����α߳�
*							 color: ��ɫ
*
* Reutrn     : None.
*
* Note(s)    : ���������������ϱ߳����ܴ�����Ļ����
*********************************************************************************************************
*/
void gui_drawSquare(uint16_t x0, uint16_t y0, uint16_t sideLen, GUI_COLOR color)
{
	/* �ж������Ƿ���Ч */
	if(x0 > LCD_X || y0 > LCD_Y) return;
	if((x0 + sideLen) > LCD_X) return;
	if((y0 + sideLen) > LCD_Y) return;
	
	/* �߳�Ϊ0�򻭵� */
	if(sideLen == 0) gui_drawPoint(x0, y0, color);
	
	gui_drawRectangle(x0, y0, x0 + sideLen, y0 + sideLen, color);
}

/*
*********************************************************************************************************
*                  gui_fillSquare                        
*
* Description: ���������
*             
* Arguments  : x0,y0: ���������Ͻ�����
*						   sideLen: �����α߳�
*							 color: ��ɫ
*
* Reutrn     : None.
*
* Note(s)    : ���������������ϱ߳����ܴ�����Ļ����
*********************************************************************************************************
*/
void gui_fillSquare(uint16_t x0, uint16_t y0, uint16_t sideLen, GUI_COLOR color)
{
	/* �ж������Ƿ���Ч */
	if(x0 > LCD_X || y0 > LCD_Y) return;
	if((x0 + sideLen) > LCD_X) return;
	if((y0 + sideLen) > LCD_Y) return;
	
	/* �߳�Ϊ0�򻭵� */
	if(sideLen == 0) gui_drawPoint(x0, y0, color);
	
	gui_fillRectangle(x0, y0, x0 + sideLen, y0 + sideLen, color);
}

/*
*********************************************************************************************************
*                  gui_drawPolygon                        
*
* Description: ������Σ�ͼ�αպ�
*             
* Arguments  : polygon: ����Σ�Ϊ�㼯��
*							 points: ����
*							 color: ��ɫ
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_drawPolygon(const GUI_POINT *pPolygon, uint8_t points, GUI_COLOR color)
{
	uint16_t x0 = 0, y0 = 0;	/* ���ڻ��� */
	uint16_t orgin_x0 = 0, orgin_y0 = 0;	/* ���ڱ����һ��������꣬ */
	
	if(points == 0) return ;	/* ��ĸ�����Чֱ�ӷ��� */
	else if(points == 1) /* ֻ��һ���㣬����Ҳֱ�ӷ��� */
	{
		gui_drawPoint(pPolygon->x, pPolygon->y, color);
		return ;
	}
	else if(points == 2)	/* ֻ�������㣬��ֻ��һ���ߣ����귵�� */
	{
		gui_drawLine(pPolygon->x, pPolygon->y, (pPolygon+1)->x, pPolygon->y, color);
		return ;
	}
	
	/* ����������꣬��Ϊ����ͼ����Ҫ�պ� */
	orgin_x0 = pPolygon->x;
	orgin_y0 = pPolygon->y;
	
	while(points > 1)	/* ��ΪҪ������ܹ���һ���� */
	{
		x0 = pPolygon->x;
		y0 = pPolygon->y;
		pPolygon++;	/* ָ����һ���� */
		gui_drawLine(x0, y0, pPolygon->x, pPolygon->y, color);
		
		points--;	/* ��������һ�� */
	}	
	/* �����һ���ߣ����ɱպ�ͼ�� */
	gui_drawLine(pPolygon->x, pPolygon->y, orgin_x0, orgin_y0, color);
}

/*
*********************************************************************************************************
*                  gui_drawPolygon                        
*
* Description: �������ߣ�ͼ�β��պ�
*             
* Arguments  : polygon: ����Σ�Ϊ�㼯��
*							 points: ����
*							 color: ��ɫ
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_drawPolyLine(const GUI_POINT *pPolyLine, uint8_t points, GUI_COLOR color)
{
	uint16_t x0 = 0, y0 = 0;	/* ���ڻ��� */

GUI_Context.drawPosX = 1;
	
	if(points == 0) return ;	/* ��ĸ�����Чֱ�ӷ��� */
	else if(points == 1) /* ֻ��һ���㣬����Ҳֱ�ӷ��� */
	{
		gui_drawPoint(pPolyLine->x, pPolyLine->y, color);
		return ;
	}
	else if(points == 2)	/* ֻ�������㣬��ֻ��һ���ߣ����귵�� */
	{
		gui_drawLine(pPolyLine->x, pPolyLine->y, (pPolyLine+1)->x, pPolyLine->y, color);
		return ;
	}
	
	while(points > 1)	/* ��ΪҪ������ܹ���һ���� */
	{
		x0 = pPolyLine->x;
		y0 = pPolyLine->y;
		pPolyLine++;	/* ָ����һ���� */
		gui_drawLine(x0, y0, pPolyLine->x, pPolyLine->y, color);
		
		points--;	/* ��������һ�� */
	}	
}

/*
*********************************************************************************************************
*                      gui_drawCircle                    
*
* Description: ��Բ����
*             
* Arguments  : x,y: Բ������
*							 radius: Բ�뾶
*							 color: ��ɫ
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_drawCircle(uint16_t x, uint16_t y, uint16_t radius, GUI_COLOR color)
{
	int32_t  draw_x0, draw_y0;			// ��ͼ���������
	int32_t  draw_x1, draw_y1;	
	int32_t  draw_x2, draw_y2;	
	int32_t  draw_x3, draw_y3;	
	int32_t  draw_x4, draw_y4;	
	int32_t  draw_x5, draw_y5;	
	int32_t  draw_x6, draw_y6;	
	int32_t  draw_x7, draw_y7;	
	int32_t  xx, yy;					// ��Բ���Ʊ���
	int32_t  di;						// ���߱���

	/* �������� */
	if(0 == radius) gui_drawPoint(x, y, color);;

	/* �����8�������(0��45��90��135��180��225��270��)��������ʾ */
	draw_x0 = draw_x1 = x;
	draw_y0 = draw_y1 = y + radius;
	if(draw_y0 < LCD_Y) gui_drawPoint(draw_x0, draw_y0, color);	// 90��

	draw_x2 = draw_x3 = x;
	draw_y2 = draw_y3 = y - radius;
	if(draw_y2 >= 0) gui_drawPoint(draw_x2, draw_y2, color);			// 270��


	draw_x4 = draw_x6 = x + radius;
	draw_y4 = draw_y6 = y;
	if(draw_x4 < LCD_X) gui_drawPoint(draw_x4, draw_y4, color);	// 0��

	draw_x5 = draw_x7 = x - radius;
	draw_y5 = draw_y7 = y;
	if(draw_x5 >= 0) gui_drawPoint(draw_x5, draw_y5, color);			// 180��   
	if(1 == radius) return;					// ���뾶Ϊ1������Բ����
   
   
	/* ʹ��Bresenham�����л�Բ */
	di = 3 - 2*radius;					// ��ʼ�����߱���

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

		/* Ҫ�жϵ�ǰ���Ƿ�����Ч��Χ�� */
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
* Description: Բ�����
*             
* Arguments  : x,y: Բ������
*							 radius: Բ�뾶
*							 color: ��ɫ
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_fillCircle(uint16_t x, uint16_t y, uint16_t radius, GUI_COLOR color)
{
	int32_t  draw_x0, draw_y0;			// ��ͼ���������
	int32_t  draw_x1, draw_y1;	
	int32_t  draw_x2, draw_y2;	
	int32_t  draw_x3, draw_y3;	
	int32_t  draw_x4, draw_y4;	
	int32_t  draw_x5, draw_y5;	
	int32_t  draw_x6, draw_y6;	
	int32_t  draw_x7, draw_y7;	
	int32_t  fill_x0, fill_y0;			// �������ı�����ʹ�ô�ֱ�����
	int32_t  fill_x1;
	int32_t  xx, yy;					// ��Բ���Ʊ���
	int32_t  di;						// ���߱���

	/* �������� */
	if(0 == radius) return;

	/* �����4�������(0��90��180��270��)��������ʾ */
	draw_x0 = draw_x1 = x;
	draw_y0 = draw_y1 = y + radius;
	if(draw_y0<LCD_Y)
		gui_drawPoint(draw_x0, draw_y0, color);	// 90��

	draw_x2 = draw_x3 = x;
	draw_y2 = draw_y3 = y - radius;
	if(draw_y2>=0)
		gui_drawPoint(draw_x2, draw_y2, color);	// 270��

	draw_x4 = draw_x6 = x + radius;
	draw_y4 = draw_y6 = y;
	if(draw_x4<LCD_X) 
	{  
		gui_drawPoint(draw_x4, draw_y4, color);	// 0��
		fill_x1 = draw_x4;
	}
	else
		fill_x1 = LCD_X;
	
	fill_y0 = y;							// �������������ʼ��fill_x0
	fill_x0 = x - radius;						// �����������������fill_y1
	if(fill_x0<0) fill_x0 = 0;
	gui_drawHLine(fill_x0, fill_y0, fill_x1, color);

	draw_x5 = draw_x7 = x - radius;
	draw_y5 = draw_y7 = y;
	if(draw_x5>=0) 
		gui_drawPoint(draw_x5, draw_y5, color);	// 180��
	if(1==radius) return;


	/* ʹ��Bresenham�����л�Բ */
	di = 3 - 2*radius;							// ��ʼ�����߱���

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
		
		/* Ҫ�жϵ�ǰ���Ƿ�����Ч��Χ�� */
		if( (draw_x0<=LCD_X)&&(draw_y0>=0) )	
			gui_drawPoint(draw_x0, draw_y0, color);

		if( (draw_x1>=0)&&(draw_y1>=0) )	
			gui_drawPoint(draw_x1, draw_y1, color);
	  
		/* �ڶ���ˮֱ�����(�°�Բ�ĵ�) */
		if(draw_x1>=0)
		{  /* �������������ʼ��fill_x0 */
			fill_x0 = draw_x1;
			/* �������������ʼ��fill_y0 */
			fill_y0 = draw_y1;
			if(fill_y0>LCD_Y) fill_y0 = LCD_Y;
			if(fill_y0<0) fill_y0 = 0; 
			/* �����������������fill_x1 */									
			fill_x1 = x*2 - draw_x1;				
			if(fill_x1>LCD_X) fill_x1 = LCD_X;
			gui_drawHLine(fill_x0, fill_y0, fill_x1, color);
		}
	  
		if( (draw_x2<=LCD_X)&&(draw_y2<=LCD_Y) )	
			gui_drawPoint(draw_x2, draw_y2, color);   
		  
		if( (draw_x3>=0)&&(draw_y3<=LCD_Y) )	
			gui_drawPoint(draw_x3, draw_y3, color);
	  
		/* ���ĵ㴹ֱ�����(�ϰ�Բ�ĵ�) */
		if(draw_x3>=0)
		{  /* �������������ʼ��fill_x0 */
			fill_x0 = draw_x3;
			/* �������������ʼ��fill_y0 */
			fill_y0 = draw_y3;
			if(fill_y0>LCD_Y) fill_y0 = LCD_Y;
			if(fill_y0<0) fill_y0 = 0;
			/* �����������������fill_x1 */									
			fill_x1 = x*2 - draw_x3;				
			if(fill_x1>LCD_X) fill_x1 = LCD_X;
			gui_drawHLine(fill_x0, fill_y0, fill_x1, color);
		}	 
		if( (draw_x4<=LCD_X)&&(draw_y4>=0) )	
			gui_drawPoint(draw_x4, draw_y4, color);

		if( (draw_x5>=0)&&(draw_y5>=0) )	
			gui_drawPoint(draw_x5, draw_y5, color);
	  
		/* �����㴹ֱ�����(�ϰ�Բ�ĵ�) */
		if(draw_x5>=0)
		{  /* �������������ʼ��fill_x0 */
			fill_x0 = draw_x5;
			/* �������������ʼ��fill_y0 */
			fill_y0 = draw_y5;
			if(fill_y0>LCD_Y) fill_y0 = LCD_Y;
			if(fill_y0<0) fill_y0 = 0;
			/* �����������������fill_x1 */									
			fill_x1 = x*2 - draw_x5;				
			if(fill_x1>LCD_X) fill_x1 = LCD_X;
			gui_drawHLine(fill_x0, fill_y0, fill_x1, color);
		}

		if( (draw_x6<=LCD_X)&&(draw_y6<=LCD_Y) )	
			gui_drawPoint(draw_x6, draw_y6, color);
		if( (draw_x7>=0)&&(draw_y7<=LCD_Y) )	
			gui_drawPoint(draw_x7, draw_y7, color);

		/* �ڰ˵㴹ֱ�����(�ϰ�Բ�ĵ�) */
		if(draw_x7>=0)
		{  /* �������������ʼ��fill_x0 */
			fill_x0 = draw_x7;
			/* �������������ʼ��fill_y0 */
			fill_y0 = draw_y7;
			if(fill_y0>LCD_Y) fill_y0 = LCD_Y;
			if(fill_y0<0) fill_y0 = 0;
			/* �����������������fill_x1 */									
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
* Description: ������Բ
*             
* Arguments  : x,y: ��ԲԲ��
*							 a,b: ��Բ�������᳤
*							 color: ��ɫ
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_drawEllipse(uint16_t x, uint16_t y, uint16_t a, uint16_t b, GUI_COLOR color)
{
	uint16_t x0 = 0, y0 = 0, x1 = 0, y1 = 0;
	int32_t  draw_x0, draw_y0;			// ��ͼ���������
	int32_t  draw_x1, draw_y1;
	int32_t  draw_x2, draw_y2;
	int32_t  draw_x3, draw_y3;
	int32_t  xx, yy;					// ��ͼ���Ʊ���

	int32_t  center_x, center_y;		// ��Բ���ĵ��������
	int32_t  radius_x, radius_y;		// ��Բ�İ뾶��x��뾶��y��뾶
	int32_t  radius_xx, radius_yy;		// �뾶��ƽ��ֵ
	int32_t  radius_xx2, radius_yy2;	// �뾶��ƽ��ֵ������
	int32_t  di;						// ������߱���
	
	x0 = x - a/2;	/* ��������ߵĵ��x���� */
	x1 = x + a/2;	/* ���ұߵĵ� */
	y0 = y - b/2;	/* �ϱ� */
	y1 = y + a/2;	/* �±� */
	
	/* �������� */
	if( (x0==x1) || (y0==y1) ) return;

	/* �������Բ���ĵ����� */
	center_x = (x0 + x1) >> 1;			
	center_y = (y0 + y1) >> 1;

	/* �������Բ�İ뾶��x��뾶��y��뾶 */
	if(x0 > x1)
		radius_x = (x0 - x1) >> 1;
	else
		radius_x = (x1 - x0) >> 1;
	if(y0 > y1)
		radius_y = (y0 - y1) >> 1;
	else
		radius_y = (y1 - y0) >> 1;
   
	/* ����뾶ƽ��ֵ */
	radius_xx = radius_x * radius_x;
	radius_yy = radius_y * radius_y;

	/* ����뾶ƽ��ֵ��2ֵ */
	radius_xx2 = radius_xx<<1;
	radius_yy2 = radius_yy<<1;

	/* ��ʼ����ͼ���� */
	xx = 0;
	yy = radius_y;
	di = radius_yy2 + radius_xx - radius_xx2*radius_y ;	// ��ʼ�����߱��� 

	/* �������Բy���ϵ������˵����꣬��Ϊ��ͼ��� */
	draw_x0 = draw_x1 = draw_x2 = draw_x3 = center_x;
	draw_y0 = draw_y1 = center_y + radius_y;
	draw_y2 = draw_y3 = center_y - radius_y;

	gui_drawPoint(draw_x0, draw_y0, color);					// ��y���ϵ������˵� 
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
		xx ++;						// x���1
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

			xx ++;						// x���1	 		
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
* Description: �������Բ
*             
* Arguments  : x,y: ��ԲԲ��
*							 a,b: ��Բ�������᳤
*							 color: ��ɫ
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_fillEllipse(uint16_t x, uint16_t y, uint16_t a, uint16_t b, GUI_COLOR color)
{
	uint16_t x0 = 0, y0 = 0, x1 = 0, y1 = 0;
	int32_t  draw_x0, draw_y0;			// ��ͼ���������
	int32_t  draw_x1, draw_y1;
	int32_t  draw_x2, draw_y2;
	int32_t  draw_x3, draw_y3;
	int32_t  xx, yy;					// ��ͼ���Ʊ���

	int32_t  center_x, center_y;		// ��Բ���ĵ��������
	int32_t  radius_x, radius_y;		// ��Բ�İ뾶��x��뾶��y��뾶
	int32_t  radius_xx, radius_yy;		// �뾶��ƽ��ֵ
	int32_t  radius_xx2, radius_yy2;	// �뾶��ƽ��ֵ������
	int32_t  di;						// ������߱���

	x0 = x - a/2;	/* ��������ߵĵ��x���� */
	x1 = x + a/2;	/* ���ұߵĵ� */
	y0 = y - b/2;	/* �ϱ� */
	y1 = y + a/2;	/* �±� */
	
	/* �������� */
	if( (x0==x1) || (y0==y1) ) return;

	/* �������Բ���ĵ����� */
	center_x = (x0 + x1) >> 1;			
	center_y = (y0 + y1) >> 1;

	/* �������Բ�İ뾶��x��뾶��y��뾶 */
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

	/* ����뾶��ƽ��ֵ */
	radius_xx = radius_x * radius_x;
	radius_yy = radius_y * radius_y;

	/* ����뾶��4ֵ */
	radius_xx2 = radius_xx<<1;
	radius_yy2 = radius_yy<<1;

	/* ��ʼ����ͼ���� */
	xx = 0;
	yy = radius_y;

	di = radius_yy2 + radius_xx - radius_xx2*radius_y ;	// ��ʼ�����߱��� 

	/* �������Բy���ϵ������˵����꣬��Ϊ��ͼ��� */
	draw_x0 = draw_x1 = draw_x2 = draw_x3 = center_x;
	draw_y0 = draw_y1 = center_y + radius_y;
	draw_y2 = draw_y3 = center_y - radius_y;


	gui_drawPoint(draw_x0, draw_y0, color);					// ��y���ϵ������˵�
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

	xx ++;						// x���1
		
	draw_x0++;
	draw_x1--;
	draw_x2++;
	draw_x3--;

	gui_drawPoint(draw_x0, draw_y0, color);
	gui_drawPoint(draw_x1, draw_y1, color);
	gui_drawPoint(draw_x2, draw_y2, color);
	gui_drawPoint(draw_x3, draw_y3, color);

	/* ��y���ѱ仯��������� */
	if(di>=0)
	{  gui_drawHLine(draw_x0, draw_y0, draw_x1, color);
		 gui_drawHLine(draw_x2, draw_y2, draw_x3, color);
	}
	}

	di = radius_xx2*(yy-1)*(yy-1) + radius_yy2*xx*xx + radius_yy + radius_yy2*xx - radius_xx2*radius_yy;
	while(yy>=0) 
	{  if(di<0)
	{  di+= radius_xx2*3 + 4*radius_yy*xx + 4*radius_yy - 2*radius_xx2*yy;
		
		 xx ++;						// x���1	 		
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

	/* y���ѱ仯��������� */
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


