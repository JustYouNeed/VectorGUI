/**
  *******************************************************************************************************
  * File Name: bsp_led.c
  * Author: Vector
  * Version: V2.2.0
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
	*			Mod: 1.��LED��װ��һ���ṹ��,���ڹ���,ʹ�ø���ͳһ
	*					 2.����LED������˸����
	*
	*		3.Author: Vector
	*			Date: 2018-9-18
	*			Mod: ��������bsp_led_AddLED,��LED����Ӹ�Ϊע�᷽ʽ,��ֲ��Ϊ����
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

/*  ���尴������ʱ���ŵ�״̬  */
# define LED_OFF				1
# define LED_ON					0

/*  LED������ض���  */
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
* Description: ��ϵͳ���һ��LED��
*             
* Arguments  : 1.ledId: LED���,�����bsp_led.h�е�LED���ö�ٱ������,����ʹ��
*							 2.*ledPort: LED��ʹ�õĶ˿�
*							 3.ledPin: LED�Ƶ�����
*
* Reutrn     : None.
*
* Note(s)    : ���ͬһ�����ע���ٴ�,��Ḳ����һ������
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
* Description: ��ʼ��LED����
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : ע��,����ϵͳע��LED�Ƶ�ʱ����Ҫ��bsp_led.h�ж����LED��ID��˳��ע��,��������
*********************************************************************************************************
*/
void bsp_led_Init(void)
{
	bsp_led_AddLED(LED_RED, LED_RED_PORT, LED_RED_PIN, LED_MODE_A);			/*  ע��һ��LED��,���ӷ�ʽΪ�ߵ�ƽ����  */
	bsp_led_AddLED(LED_BLUE, LED_BLUE_PORT, LED_BLUE_PIN, LED_MODE_A);	
}


/*
*********************************************************************************************************
*                                          bsp_led_ON
*
* Description: ��һ��LED��
*             
* Arguments  : 1> ledId:	Ҫ�򿪵�LED�Ʊ��
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void bsp_led_ON(uint8_t ledId)
{
	/*  LED�Ƴ������ƻ���û����ϵͳע����ֱ�ӷ���  */
	if(ledId > ledCount || !LED[ledId].isInit) return ;
	
	if(ledId == LED_ALL)		/*  ����ȫ���ĵ�  */
	{
		for(uint8_t i = 0; i <= ledCount; i++)
		{
			if(LED[i].mode == LED_MODE_A)		/*  ����Ǹߵ�ƽ����������ߵ�ƽ  */
				PIN_OUT_HIGH(LED[i].port, LED[i].pin);
			else
				PIN_OUT_LOW(LED[i].port, LED[i].pin);
		}
	}
	else
	{
		if(LED[ledId].mode == LED_MODE_A)  /*  ����Ǹߵ�ƽ����������ߵ�ƽ  */
			PIN_OUT_HIGH(LED[ledId].port, LED[ledId].pin);
		else
			PIN_OUT_LOW(LED[ledId].port, LED[ledId].pin);
	}
}

/*
*********************************************************************************************************
*                                          bsp_led_OFF
*
* Description: �ر�һ��LED��
*             
* Arguments  : 1> ledId:	Ҫ�رյ�LED�Ʊ��
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void bsp_led_OFF(uint8_t ledId)
{
	/*  LED�Ƴ������ƻ���û����ϵͳע����ֱ�ӷ���  */
	if(ledId > ledCount || !LED[ledId].isInit) return ;
	
	if(ledId == LED_ALL)
	{
		for(uint8_t i = 0; i <= ledCount; i++)
		{
			if(LED[i].mode == LED_MODE_A) /*  ����Ǹߵ�ƽ����������͵�ƽ  */
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
* Description: �л�LED�Ƶ�״̬
*             
* Arguments  : 1> ledId:	Ҫ�л���LED�Ʊ��
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void bsp_led_Toggle(uint8_t ledId)
{
	/*  LED�Ƴ������ƻ���û����ϵͳע����ֱ�ӷ���  */
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
* Description: ��һ��LED�ƿ�ʼ��˸
*             
* Arguments  : 1> ledId: LED���, ��bsp_led.h�ж���
*							 2> BrightTime: ����ʱ��
*							 3> DarkTime: Ϩ��ʱ��
*							 4> Cycle: ��˸����
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void bsp_led_StartFlash(uint8_t ledId, uint16_t brightTime, uint16_t darkTime, uint16_t cycle)
{
	uint8_t i = 0;
		
	/*  ���û�е���ʱ�����LED���Ѿ����ڹر�״̬�Ͳ���Ҫ����  */
	if(brightTime == 0) return;		
	
	if(ledId == LED_ALL)
	{
		for(i = 0; i < ledCount; i++)
		{
			/*  LEDû����ϵͳע��������  */
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
		/*  LED�Ƴ������ƻ���û����ϵͳע����ֱ�ӷ���  */
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
* Description: ��һ��LED��ֹͣ��˸
*             
* Arguments  : 1> ledId: LED���,��bsp_led.h�ж���
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void bsp_led_StopFlash(uint8_t ledId)
{
	/*  LED�Ƴ������ƻ���û����ϵͳע����ֱ�ӷ���  */
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
* Description: ��ȡ�Ƶ���˸״̬,�Ƿ���˸���
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
* Description: LED�Ƶ���˸�����߳�,�����Ҫ����˸����,��Ӧ�ö����ڵ��øú���
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : �Ƽ���������Ϊ10ms
*********************************************************************************************************
*/
void bsp_led_Thread(void)
{
	uint8_t ledId = 0;
	
	/*  ѭ������ÿһ��LED  */
	for(ledId = 0; ledId <= ledCount; ledId++)
	{
		/*  ����ر�ʱ��Ϊ0���ߵ��Ѿ��ر���,�Ͳ���Ҫ�����������  */
		if(LED[ledId].enable == false || LED[ledId].darkTime == 0) continue;
		
		/*  �����ǰ�Ƶ�״̬�ǵ�����,����㻹�ж೤ʱ��ر�  */
		if(LED[ledId].state == LED_ON)
		{
			if(LED[ledId].darkTime > 0)	/*  ֻ���ڵ��йر�ʱ��Ž��д���  */
			{
				if(++ LED[ledId].count >= LED[ledId].brightTime)		/*  ����ʱ���Ѿ�����,�رյ�  */
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
		else if(LED[ledId].state == LED_OFF)		/*  �����ǰ״̬Ϊ�رյ�,���㻹�ж೤ʱ�����  */
		{
			if(++ LED[ledId].count >= LED[ledId].darkTime)		/*  ����ر�ʱ���Ѿ����ˣ��͵�����  */
			{
				/*  ���������һֱ��˸�Ļ��Ͳ���Ҫ����  */
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
