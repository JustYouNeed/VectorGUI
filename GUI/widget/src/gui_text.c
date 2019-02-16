/**
  *******************************************************************************************************
  * File Name: gui_text.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: ������ʾ����
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
# include "gui_text.h"
# include "gui_core.h"


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
uint16_t gui_getStringLen(const uint8_t *str)
{
	uint16_t len = 0;
	
	while(*str++) len ++;
	
	return len;
}

/*
*********************************************************************************************************
*                      gui_setFont                    
*
* Description: ��������
*             
* Arguments  : font: ����
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/extern GUI_CONTEXT GUI_Context;
void gui_setFont(GUI_FONT font)
{
	GUI_Context.curFont = font;
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
*/extern GUI_CONTEXT GUI_Context;
void gui_dispChar(uint8_t c, uint8_t mode)
{
	uint8_t YSize = 0;	/* �����С */
	uint8_t bytesPerChar = 0;	/* ÿһ����ĸ�ж����ֽ� */
	uint8_t charData = 0;		/* �ַ����� */
	uint8_t x = 0, y = 0, y0 = 0;	/* �������,��ʼY���� */
	uint32_t i = 0, j = 0;		
	uint16_t offset = 0;		/* ����ֿ�ƫ���� */
	
	/* ��ȡ�����������Ϣ */
//	XSize = font_getXSize(GUI_Context.curFont);
	YSize = font_getYSize(GUI_Context.curFont);
	bytesPerChar = font_getBytesPerChar(GUI_Context.curFont);
	
	x = GUI_Context.dispPosX;
	y = y0 = GUI_Context.dispPosY;
	
	/* �������ĸ����ֿ�ƫ���� */
	offset = (c - ' ') * bytesPerChar;	
	
	for(i = 0; i < bytesPerChar; i++)	/* �����ֿ����ݴ�� */
	{
		charData = font_getFontData(GUI_Context.curFont, offset + i);;
		
		for(j = 0; j < 8; j++)	/* һ��һ���ֽڴ��� */
		{
			if(charData & 0x80) gui_drawPoint(x, y, mode);	
			else gui_drawPoint(x, y, !mode);
			charData <<= 1;
			y++;
			if((y - y0) == YSize)
			{
				y = y0;
				x++;
				break;
			}
		}
	}
}



/*
*********************************************************************************************************
*                           gui_dispCharAt               
*
* Description: ��ָ��������ʾһ���ַ�
*             
* Arguments  : c: Ҫ��ʾ���ַ�
*							 x0,y0: ����
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_dispCharAt(uint8_t c, uint16_t x0, uint16_t y0, uint8_t mode)
{
	gui_gotoXY(x0, y0);
	gui_dispChar(c, mode);
}

/*
*********************************************************************************************************
*                        gui_dispString                  
*
* Description: ��ʾ�ַ���
*             
* Arguments  : *str: �ַ���
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/extern GUI_CONTEXT GUI_Context;
void gui_dispString(uint8_t *str, uint8_t mode)
{
	uint8_t XSize = 0, YSize = 0;
	XSize = font_getXSize(GUI_Context.curFont);
	YSize = font_getYSize(GUI_Context.curFont);
	
	while(*str)
	{
		if((GUI_Context.dispPosX + XSize) < LCD_MAX_X - 1) 
		{
			gui_dispChar(*str++, mode);
			gui_gotoXY(GUI_Context.dispPosX + XSize, GUI_Context.dispPosY);
		}
		else
			gui_gotoXY(0, GUI_Context.dispPosY + YSize);
	}
}
void gui_dispStringAt(uint8_t *str, uint16_t x0, uint16_t y0, uint8_t mode)
{
}

/********************************************  END OF FILE  *******************************************/
