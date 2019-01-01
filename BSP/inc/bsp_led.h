/**
  *******************************************************************************************************
  * File Name: bsp_led.h
  * Author: Vector
  * Version: V1.1.0
  * Date: 2018-2-17
  * Brief: ���ļ�Ϊ�弶LED������
  *******************************************************************************************************
  * History
	*		1.Author:	Vector
	*			Date:	2018-2-17
  *			Mod:	�����ļ�
	*
	*		2.Author: Vector
	*			Date: 2018-8-2
	*			Mod: ΪLED�ṹ��������������,���ڱ���LED GPIO_PORT, GPIO_Pin,����LED���Žӷ�,��LED�����ͳһ
  *
  *******************************************************************************************************
  */	
# ifndef __BSP_LED_H
# define __BSP_LED_H

# define LED_KEEP_FLASH		(uint16_t)0xffff


/*  LED IDö�ٱ���  */
typedef enum
{
	LED_RED = 0X0,
	LED_BLUE,
	LED_COUNT,
	LED_ALL,
}ledId_e;

/*  LED�ӷ�ö������,�����ֽӷ�,
		һ��GPIO����ߵ�ƽ����,һ������͵�ƽ����  */
typedef enum
{
	LED_MODE_A = 0x0,		/*  �ߵ�ƽ����  */
	LED_MODE_K,					/*  �͵�ƽ����  */
}ledMode_e;

/*  LED���ƽṹ�����  */
typedef struct
{
	GPIO_TypeDef* port;
	uint16_t			pin;
	ledMode_e 			mode;
	bool enable;
	bool isInit;
	
	uint8_t state;			/*  ״̬  */
	uint16_t brightTime;/*  ����ʱ��,�����߳�����  */
	uint16_t darkTime;	/*  �ر�ʱ��,�����߳�����  */
	uint16_t cycle;			/*  ѭ������  */
	uint16_t count;			/*  ��������  */
	uint16_t cycleCount;/*  �Ѿ�ѭ���˶��ٴ�  */
}ledCtrl_t;


void bsp_led_Init(void);
void bsp_led_Toggle(uint8_t ledId);
void bsp_led_ON(uint8_t ledId);
void bsp_led_OFF(uint8_t ledId);
uint8_t bsp_led_GetState(uint8_t ledId);
void bsp_led_StartFlash(uint8_t ledId, uint16_t brightTime, uint16_t darkTime, uint16_t cycle);
void bsp_led_StopFlash(uint8_t ledId);
bool bsp_led_GetFlashState(uint8_t ledId);
void bsp_led_Thread(void);
# endif


/********************************************  END OF FILE  *******************************************/
