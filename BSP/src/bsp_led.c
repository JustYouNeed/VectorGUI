/**
  *******************************************************************************************************
  * File Name: bsp_led.c
  * Author: Vector
  * Version: V2.2.0
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
	*			Mod: 1.将LED封装成一个结构体,便于管理,使用更加统一
	*					 2.新增LED持续闪烁功能
	*
	*		3.Author: Vector
	*			Date: 2018-9-18
	*			Mod: 新增函数bsp_led_AddLED,改LED灯添加改为注册方式,移植更为方便
  *
  *******************************************************************************************************
  */	
	
/*
  *******************************************************************************************************
  *                              INCLUDE FILES
  *******************************************************************************************************
*/
# include "bsp.h"


/*
  *******************************************************************************************************
  *                              LOCAL DEFINITION
  *******************************************************************************************************
*/
# define LED_MAX_COUNT		16

/*  定义按键开关时引脚的状态  */
# define LED_OFF				1
# define LED_ON					0

/*  LED引脚相关定义  */
# define LED_RED_PORT		GPIOA
# define LED_RED_PIN		GPIO_Pin_9

# define LED_BLUE_PORT	GPIOA
# define LED_BLUE_PIN		GPIO_Pin_10


/*
  *******************************************************************************************************
  *                              LOCAL VARIABLE
  *******************************************************************************************************
*/
static ledCtrl_t LED[LED_MAX_COUNT];
static uint8_t ledCount = 0;


/*
*********************************************************************************************************
*                     bsp_led_AddLED                     
*
* Description: 向系统添加一个LED灯
*             
* Arguments  : 1.ledId: LED编号,最好在bsp_led.h中的LED编号枚举变量添加,便于使用
*							 2.*ledPort: LED灯使用的端口
*							 3.ledPin: LED灯的引脚
*
* Reutrn     : None.
*
* Note(s)    : 如果同一个编号注册再次,则会覆盖上一次设置
*********************************************************************************************************
*/
static void bsp_led_AddLED(uint8_t ledId, GPIO_TypeDef *ledPort, uint16_t ledPin, ledMode_e ledMode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	bsp_rcc_GPIOClockCmd(ledPort);
	
# ifdef VECTOR_F1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
# else	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
# endif

	GPIO_InitStructure.GPIO_Pin = ledPin;
	GPIO_Init(ledPort, &GPIO_InitStructure);
	
	LED[ledId].port = ledPort;
	LED[ledId].pin = ledPin;	
	LED[ledId].state = LED_OFF;
	LED[ledId].count = 0;
	LED[ledId].cycle = 0;
	LED[ledId].cycleCount = 0;
	LED[ledId].brightTime = 0;
	LED[ledId].darkTime = 0;
	LED[ledId].mode = ledMode;	
	LED[ledId].enable = true;
	LED[ledId].isInit = true;
	
	bsp_led_OFF(ledId);
	
	ledCount++;
}

/*
*********************************************************************************************************
*                                   bsp_led_Init       
*
* Description: 初始化LED引脚
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : 注意,在向系统注册LED灯的时候需要按bsp_led.h中定义的LED灯ID的顺序注册,否则会出错
*********************************************************************************************************
*/
void bsp_led_Init(void)
{
	bsp_led_AddLED(LED_RED, LED_RED_PORT, LED_RED_PIN, LED_MODE_A);			/*  注册一个LED灯,连接方式为高电平点亮  */
	bsp_led_AddLED(LED_BLUE, LED_BLUE_PORT, LED_BLUE_PIN, LED_MODE_A);	
}


/*
*********************************************************************************************************
*                                          bsp_led_ON
*
* Description: 打开一个LED灯
*             
* Arguments  : 1> ledId:	要打开的LED灯编号
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void bsp_led_ON(uint8_t ledId)
{
	/*  LED灯超过限制或者没有向系统注册则直接返回  */
	if(ledId > ledCount || !LED[ledId].isInit) return ;
	
	if(ledId == LED_ALL)		/*  点亮全部的灯  */
	{
		for(uint8_t i = 0; i <= ledCount; i++)
		{
			if(LED[i].mode == LED_MODE_A)		/*  如果是高电平点亮则输出高电平  */
				PIN_OUT_HIGH(LED[i].port, LED[i].pin);
			else
				PIN_OUT_LOW(LED[i].port, LED[i].pin);
		}
	}
	else
	{
		if(LED[ledId].mode == LED_MODE_A)  /*  如果是高电平点亮则输出高电平  */
			PIN_OUT_HIGH(LED[ledId].port, LED[ledId].pin);
		else
			PIN_OUT_LOW(LED[ledId].port, LED[ledId].pin);
	}
}

/*
*********************************************************************************************************
*                                          bsp_led_OFF
*
* Description: 关闭一个LED灯
*             
* Arguments  : 1> ledId:	要关闭的LED灯编号
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void bsp_led_OFF(uint8_t ledId)
{
	/*  LED灯超过限制或者没有向系统注册则直接返回  */
	if(ledId > ledCount || !LED[ledId].isInit) return ;
	
	if(ledId == LED_ALL)
	{
		for(uint8_t i = 0; i <= ledCount; i++)
		{
			if(LED[i].mode == LED_MODE_A) /*  如果是高电平点亮则输出低电平  */
				PIN_OUT_LOW(LED[i].port, LED[i].pin);
			else
				PIN_OUT_HIGH(LED[i].port, LED[i].pin);
		}
	}
	else
	{
		if(LED[ledId].mode == LED_MODE_A)
			PIN_OUT_LOW(LED[ledId].port, LED[ledId].pin);
		else
			PIN_OUT_HIGH(LED[ledId].port, LED[ledId].pin);
	}
}


/*
*********************************************************************************************************
*                                       bsp_led_Toggle   
*
* Description: 切换LED灯的状态
*             
* Arguments  : 1> ledId:	要切换的LED灯编号
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void bsp_led_Toggle(uint8_t ledId)
{
	/*  LED灯超过限制或者没有向系统注册则直接返回  */
	if(ledId > ledCount || !LED[ledId].isInit) return ;
	
	if(ledId == LED_ALL)
	{
		for(uint8_t i = 0; i < ledCount; i++)
		{
			PIN_TOGGLE(LED[i].port, LED[i].pin);
		}
	}
	else
	{
		PIN_TOGGLE(LED[ledId].port, LED[ledId].pin);
	}
}

/*
*********************************************************************************************************
*                         bsp_led_StartFlash                 
*
* Description: 让一个LED灯开始闪烁
*             
* Arguments  : 1> ledId: LED编号, 在bsp_led.h中定义
*							 2> BrightTime: 点亮时长
*							 3> DarkTime: 熄灭时长
*							 4> Cycle: 闪烁次数
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void bsp_led_StartFlash(uint8_t ledId, uint16_t brightTime, uint16_t darkTime, uint16_t cycle)
{
	uint8_t i = 0;
		
	/*  如果没有点亮时间或者LED灯已经处于关闭状态就不需要处理  */
	if(brightTime == 0) return;		
	
	if(ledId == LED_ALL)
	{
		for(i = 0; i < ledCount; i++)
		{
			/*  LED没有向系统注册则跳过  */
			if(!LED[i].isInit) continue ;
			
			LED[i].brightTime = brightTime;
			LED[i].darkTime = darkTime;
			LED[i].cycle = cycle;
			LED[i].count = 0;
			LED[i].state = LED_ON;
			LED[i].cycleCount = 0;
			LED[i].enable = true;
		}
	}
	else
	{
		/*  LED灯超过限制或者没有向系统注册则直接返回  */
		if(ledId > ledCount || !LED[ledId].isInit) return ;
		
		LED[ledId].brightTime = brightTime;
		LED[ledId].darkTime = darkTime;
		LED[ledId].cycle = cycle;
		LED[ledId].count = 0;
		LED[ledId].state = LED_ON;
		LED[ledId].cycleCount = 0;
		LED[ledId].enable = true;
	}
	bsp_led_ON(ledId);
}

/*
*********************************************************************************************************
*                      bsp_led_StopFlash                    
*
* Description: 让一个LED灯停止闪烁
*             
* Arguments  : 1> ledId: LED编号,在bsp_led.h中定义
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void bsp_led_StopFlash(uint8_t ledId)
{
	/*  LED灯超过限制或者没有向系统注册则直接返回  */
	if(ledId > ledCount || !LED[ledId].isInit) return ;
	
	if(ledId == LED_ALL)
	{
		for(uint8_t i = 0; i < ledCount; i++)
		{
			LED[i].brightTime = 0;
			LED[i].count = 0;
			LED[i].darkTime = 0;
			LED[i].enable = false;
			LED[i].cycle = 0;
			LED[i].cycleCount = 0;
		}
	}
	else
	{
		LED[ledId].brightTime = 0;
		LED[ledId].count = 0;
		LED[ledId].darkTime = 0;
		LED[ledId].enable = false;
		LED[ledId].cycle = 0;
		LED[ledId].cycleCount = 0;
	}
	bsp_led_OFF(ledId);
}


/*
*********************************************************************************************************
*                        bsp_led_GetFlashState                  
*
* Description: 获取灯的闪烁状态,是否闪烁完成
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
bool bsp_led_GetFlashState(uint8_t ledId)
{
	return LED[ledId].cycleCount >= LED[ledId].cycle;
}


/*
*********************************************************************************************************
*                        bsp_led_Thread                  
*
* Description: LED灯的闪烁处理线程,如果需要灯闪烁功能,则应该定周期调用该函数
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : 推荐调用周期为10ms
*********************************************************************************************************
*/
void bsp_led_Thread(void)
{
	uint8_t ledId = 0;
	
	/*  循环处理每一个LED  */
	for(ledId = 0; ledId <= ledCount; ledId++)
	{
		/*  如果关闭时间为0或者灯已经关闭了,就不需要处理这个灯了  */
		if(LED[ledId].enable == false || LED[ledId].darkTime == 0) continue;
		
		/*  如果当前灯的状态是点亮的,则计算还有多长时间关闭  */
		if(LED[ledId].state == LED_ON)
		{
			if(LED[ledId].darkTime > 0)	/*  只有在灯有关闭时间才进行处理  */
			{
				if(++ LED[ledId].count >= LED[ledId].brightTime)		/*  开启时间已经到了,关闭灯  */
				{
					bsp_led_OFF(ledId);
					LED[ledId].count = 0;
					LED[ledId].state = LED_OFF;
				}
			}
			else	
			{
				;
			}
		}
		else if(LED[ledId].state == LED_OFF)		/*  如果当前状态为关闭的,计算还有多长时间点亮  */
		{
			if(++ LED[ledId].count >= LED[ledId].darkTime)		/*  如果关闭时长已经到了，就点亮灯  */
			{
				/*  如果设置了一直闪烁的话就不需要处理  */
				if(LED[ledId].cycle > 0 && LED[ledId].cycle != LED_KEEP_FLASH)
				{
					if(++ LED[ledId].cycleCount >= LED[ledId].cycle)
					{
						LED[ledId].enable = false;
					}
					
					if(LED[ledId].enable == false)
					{
						LED[ledId].darkTime = 0;
						continue;
					}
				}
				
				LED[ledId].count = 0;
				LED[ledId].state = LED_ON;
				
				bsp_led_ON(ledId);
			}
		}
	}
}

/********************************************  END OF FILE  *******************************************/
