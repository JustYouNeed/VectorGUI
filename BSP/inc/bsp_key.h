/**
  *******************************************************************************************************
  * File Name: bsp_key.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-5-15
  * Brief: ���ļ��ṩ���йذ��������ĺ��������Լ���ر����Ķ���
  *******************************************************************************************************
  * History
	*		1.Author: Vector
	*			Date: 2018-5-15
	*			Mod: �������ļ�
  *
  *******************************************************************************************************
  */	
# ifndef __BSP_KEY_H
# define __BSP_KEY_H

/*  ����ID����  */
typedef enum{
	KEY_UP = 0x00,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_COUNT,
}keyId_e;


/*  ����ģʽ����  */
typedef enum
{	
	KEY_MODE_A,		/*  �������º�Ϊ�ߵ�ƽ  */
	KEY_MODE_K,		/*  �������º�Ϊ�͵�ƽ  */
}keyMode_e;

/*  ������ֵ����  */
typedef enum
{
	KEY_NONE = 0x00,
	
	KEY_UP_PRESS,
	KEY_UP_UP,
	KEY_UP_LONG,
	
	KEY_DOWN_PRESS,
	KEY_DOWN_UP,
	KEY_DOWN_LONG,
	
	KEY_LEFT_PRESS,
	KEY_LEFT_UP,
	KEY_LEFT_LONG,
	
	KEY_RIGHT_PRESS,
	KEY_RIGHT_UP,
	KEY_RIGHT_LONG,
}keyValue_e;


void bsp_key_Init(void);
void bsp_key_Scan(void);
void bsp_key_Clear(void);
void bsp_key_AddKey(uint8_t keyId, GPIO_TypeDef *keyPort, uint16_t keyPin, keyMode_e keyMode);
void bsp_key_SetPara(uint8_t keyId, uint16_t longTime, uint16_t repeatSpeed);

void bsp_key_PutKey(uint8_t keyValue);
uint8_t bsp_key_GetKey(void);
uint8_t bsp_key_GetState(keyId_e keyId);
	
# endif

/********************************************  END OF FILE  *******************************************/


