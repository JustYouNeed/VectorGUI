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


# define OLED_PWR_PORT		GPIOA
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
void powerManger_Init(void)
{
	if(isInit) return ;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	bsp_rcc_GPIOClockCmd(OLED_PWR_PORT);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin = OLED_PWR_PIN;
	GPIO_Init(OLED_PWR_PORT, &GPIO_InitStructure);
	
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
		OLED_PWR_Clr();
	else
		OLED_PWR_Set();
}

/********************************************  END OF FILE  *******************************************/
