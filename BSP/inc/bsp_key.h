/**
  *******************************************************************************************************
  * File Name: bsp_key.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-5-15
  * Brief: 本文件提供了有关按键操作的函数声明以及相关变量的定义
  *******************************************************************************************************
  * History
	*		1.Author: Vector
	*			Date: 2018-5-15
	*			Mod: 建立本文件
  *
  *******************************************************************************************************
  */	
# ifndef __BSP_KEY_H
# define __BSP_KEY_H

/*  按键ID定义  */
typedef enum{
	KEY_UP = 0x00,
	KEY_DOWN = 0x01,
	KEY_LEFT = 0x02,
	KEY_RIGHT = 0x03,
	KEY_PWR = 0x04,
	KEY_COUNT = 0x05,
	KEY_NULL = 0x00,
}keyId_e;


/*  按键模式定义  */
typedef enum
{	
	KEY_MODE_A,		/*  按键按下后为高电平  */
	KEY_MODE_K,		/*  按键按下后为低电平  */
}keyMode_e;

/*  按键键值定义  */
typedef enum
{
	KEY_NONE = 0x00,
	
	KEY_UP_PRESS = ((KEY_UP << 8) | 0x01),
	KEY_UP_UP = ((KEY_UP << 8) | 0x02),
	KEY_UP_LONG = ((KEY_UP << 8) | 0x03),
	
	KEY_DOWN_PRESS = ((KEY_DOWN << 8) | 0x01),
	KEY_DOWN_UP = ((KEY_DOWN << 8) | 0x02),
	KEY_DOWN_LONG = ((KEY_DOWN << 8) | 0x03),
	
	KEY_LEFT_PRESS = ((KEY_LEFT << 8) | 0x01),
	KEY_LEFT_UP = ((KEY_LEFT << 8) | 0x02),
	KEY_LEFT_LONG = ((KEY_LEFT << 8) | 0x03),
	
	KEY_RIGHT_PRESS = ((KEY_RIGHT << 8) | 0x01),
	KEY_RIGHT_UP = ((KEY_RIGHT << 8) | 0x02),
	KEY_RIGHT_LONG = ((KEY_RIGHT << 8) | 0x03),
	
	KEY_PWR_PRESS = ((KEY_PWR << 8) | 0x01),
	KEY_PWR_UP = ((KEY_PWR << 8) | 0x02),
	KEY_PWR_LONG = ((KEY_PWR << 8) | 0x03),
}keyValue_e;


void bsp_key_Init(void);
void bsp_key_Scan(void);
void bsp_key_Clear(void);
void bsp_key_AddKey(uint8_t keyId, GPIO_TypeDef *keyPort, uint16_t keyPin, keyMode_e keyMode);
void bsp_key_SetPara(uint8_t keyId, uint16_t longTime, uint16_t repeatSpeed);

void bsp_key_PutKey(uint16_t keyValue);
uint16_t bsp_key_GetKey(void);
uint8_t bsp_key_GetState(keyId_e keyId);
	
# endif

/********************************************  END OF FILE  *******************************************/


