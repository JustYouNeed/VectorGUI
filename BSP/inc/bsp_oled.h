/**
  *******************************************************************************************************
  * File Name: bsp_oled.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-9-10
  * Brief: OLED��������
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-9-10
	*			Mod: �����ļ�
  *
  *******************************************************************************************************
  */
# ifndef __BSP_OLED_H
# define __BSP_OLED_H

# define OLED_ADDR	0x74

//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ
# define OLED_MODE 0

# define XLevelL		0x00
# define XLevelH		0x10
# define Max_Column	128
# define Max_Row		64
# define Brightness	0xFF 
# define X_WIDTH 	128
# define Y_WIDTH 	64	    			

# define OLED_CMD  0x00	//д����
# define OLED_DATA 0x40	//д����


typedef enum
{
	OLED_FONT_ASCII6X8 = 0x0,
	OLED_FONT_ASCII6X12,
	OLED_FONT_ASCII12X24,
}oledFont_e;

typedef enum 
{
	HorizontalFlip = 0x01,
	VerticalFlip = 0x02,
}oledDirFilp_e;


void bsp_oled_Init(void);
void bsp_oled_DisplayEnabled(bool enabled);
void bsp_oled_SetDisplayFlip(oledDirFilp_e flip);
void bsp_oled_SetBrightness(uint8_t level);
void bsp_oled_Clear(void);
void bsp_oled_Refresh(void);
void bsp_oled_Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);
void bsp_oled_SetPosition(uint8_t x, uint8_t y);
void bsp_oled_DrawPoint(uint8_t x, uint8_t y, uint8_t t);
uint8_t bsp_oled_ReadPoint(uint8_t x, uint8_t y);


void bsp_oled_ShowChar(uint8_t x, uint8_t y, uint8_t c, uint8_t f_w, uint8_t f_h, uint8_t mode);
void bsp_oled_ShowNum(uint8_t x, uint8_t y, int32_t num, uint8_t len, uint8_t f_w, uint8_t f_h);
void bsp_oled_ShowString(uint8_t x, uint8_t y, uint8_t *str, uint8_t f_w, uint8_t f_h);
void bsp_oled_ShowPicture(uint8_t x,uint8_t y,const uint8_t *p,uint8_t p_w,uint8_t p_h);

# endif


/********************************************  END OF FILE  *******************************************/

