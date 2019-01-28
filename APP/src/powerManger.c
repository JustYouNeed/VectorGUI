/**
  *******************************************************************************************************
  * File Name: powerManger.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-23
  * Brief: 遥控器的电源管理部分
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-23
	*			Mod: 建立文件
  *
  *******************************************************************************************************
  */	
/*
  *******************************************************************************************************
  *                              INCLUDE FILES
  *******************************************************************************************************
*/
# include "app.h"


# define OLED_PWR_PORT	GPIOA
# define OLED_PWR_PIN		GPIO_Pin_5
# define OLED_PWR_Clr()	GPIO_ResetBits(OLED_PWR_PORT, OLED_PWR_PIN)
# define OLED_PWR_Set()	GPIO_SetBits(OLED_PWR_PORT, OLED_PWR_PIN)

/*
  *******************************************************************************************************
  *                              LOCAL VARIABLE
  *******************************************************************************************************
*/
static bool isInit = false;

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
static void power_initPwrCtr(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //PA0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化	
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);//PA0 连接到中断线0
	
 
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
  EXTI_Init(&EXTI_InitStructure);//配置
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//外部中断0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置NVIC
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
static void power_enterStandby(void)
{
	RCC_AHB1PeriphResetCmd(0x04ff, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	RTC_ITConfig(RTC_IT_TS|RTC_IT_WUT|RTC_IT_ALRB|RTC_IT_ALRA,DISABLE);//关闭RTC相关中断，可能在RTC实验打开了。
	RTC_ClearITPendingBit(RTC_IT_TS|RTC_IT_WUT|RTC_IT_ALRB|RTC_IT_ALRA);//清楚RTC相关中断标志位。
	
	power_initPwrCtr();
	
	PWR_ClearFlag(PWR_FLAG_WU);//清除Wake-up 标志
	
	PWR_WakeUpPinCmd(ENABLE);//设置WKUP用于唤醒
	
	PWR_EnterSTANDBYMode();	//进入待机模式
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
void powerManger_Init(void)
{
	if(isInit) return ;
	uint32_t cnt = 0;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	bsp_rcc_GPIOClockCmd(OLED_PWR_PORT);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin = OLED_PWR_PIN;
	GPIO_Init(OLED_PWR_PORT, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //PA0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化	
	
//		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == RESET)
//	{
//		bsp_led_Toggle(LED_RED);
//		while(cnt++<2000);
////			power_enterStandby();
//	}
//	else
//	{
//		bsp_led_Toggle(LED_RED);
//		power_enterStandby();
//	}

	isInit = true;
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
void power_Enabled(bool enabled)
{
	if(!isInit) 
		powerManger_Init();
	
	if(!enabled)
	{
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == RESET);
		power_enterStandby();
	}
	else
		OLED_PWR_Set();
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
void EXTI0_IRQHandler(void)
{ 		    		    				     
	uint32_t cnt = 0;
  EXTI_ClearITPendingBit(EXTI_Line0); // 清除LINE10上的中断标志位
	
//	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == RESET)
//	{
//		bsp_led_Toggle(LED_RED);
//		while(cnt++<2000);
////			power_enterStandby();
//	}
//	else
//	{
//		bsp_led_Toggle(LED_RED);
//		power_enterStandby();
//	}
//	if(Check_WKUP())//关机?
//	{		  
//		Sys_Enter_Standby(); //进入待机模式 
//	}
} 

/********************************************  END OF FILE  *******************************************/
