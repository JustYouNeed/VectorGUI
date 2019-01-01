/**
  *******************************************************************************************************
  * File Name: bsp_led.h
  * Author: Vector
  * Version: V1.1.0
  * Date: 2018-2-17
  * Brief: 本文件为板级LED灯驱动
  *******************************************************************************************************
  * History
	*		1.Author:	Vector
	*			Date:	2018-2-17
  *			Mod:	建立文件
	*
	*		2.Author: Vector
	*			Date: 2018-8-2
	*			Mod: 为LED结构体新增三个变量,用于保存LED GPIO_PORT, GPIO_Pin,还有LED引脚接法,让LED管理更统一
  *
  *******************************************************************************************************
  */	
# ifndef __BSP_LED_H
# define __BSP_LED_H

# define LED_KEEP_FLASH		(uint16_t)0xffff


/*  LED ID枚举变量  */
typedef enum
{
	LED_RED = 0X0,
	LED_BLUE,
	LED_COUNT,
	LED_ALL,
}ledId_e;

/*  LED接法枚举类型,有两种接法,
		一种GPIO输出高电平点亮,一种输出低电平点亮  */
typedef enum
{
	LED_MODE_A = 0x0,		/*  高电平点亮  */
	LED_MODE_K,					/*  低电平点亮  */
}ledMode_e;

/*  LED控制结构体变量  */
typedef struct
{
	GPIO_TypeDef* port;
	uint16_t			pin;
	ledMode_e 			mode;
	bool enable;
	bool isInit;
	
	uint8_t state;			/*  状态  */
	uint16_t brightTime;/*  点亮时间,乘以线程周期  */
	uint16_t darkTime;	/*  关闭时间,乘以线程周期  */
	uint16_t cycle;			/*  循环次数  */
	uint16_t count;			/*  计数变量  */
	uint16_t cycleCount;/*  已经循环了多少次  */
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
