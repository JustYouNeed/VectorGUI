/**
  *******************************************************************************************************
  * File Name: rocker.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-24
  * Brief: 处理手柄数据
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-24
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
# include "gui.h"

/*  摇杆中值  */
# define THRUST_MID_VALUE	 130
# define ROLL_MID_VALUE		 129
# define PITCH_MID_VALUE	 130
# define YAW_MID_VALUE		125

# define ROCKER_DEAD_RANGE	7			/*  摇杆死区值  */

# define ROCKER_FILTER_TIMES	8		/*  摇杆数据滤波次数  */

/*
  *******************************************************************************************************
  *                              LOCAL VARIABLE
  *******************************************************************************************************
*/
static bool isInit = false;		/*  摇杆初始化标志位  */
static xSemaphoreHandle rockerDateTransDone = NULL;		/*  摇杆数据传输完成信号量  */
static uint16_t thrustDataRaw[ROCKER_FILTER_TIMES];		/*  存储原始数据  */
static uint16_t pitchDataRaw[ROCKER_FILTER_TIMES];		
static uint16_t rollDataRaw[ROCKER_FILTER_TIMES];
static uint16_t yawDataRaw[ROCKER_FILTER_TIMES];
static uint8_t rockerDataCnt = 0;			/*  摇杆接收数据计数变量  */
static rocker_t rocker;		/*  摇杆结构体  */

/*
*********************************************************************************************************
*                    rocker_MsgInit                      
*
* Description: 摇杆信号量初始化函数
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
static void rocker_MsgInit(void)
{
	rockerDateTransDone = xSemaphoreCreateBinary();
}

/*
*********************************************************************************************************
*                          rocker_DeviceInit                
*
* Description: 摇杆设备初始化函数
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
static void rocker_DeviceInit(void)
{	
	bsp_rocker_Init();
}

/*
*********************************************************************************************************
*                      rocker_Init                    
*
* Description: 摇杆初始化函数
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
static void rocker_Init(void)
{
	if(isInit) return;
	
	rocker_DeviceInit();
	rocker_MsgInit();
	
	isInit = true;
}


/*
*********************************************************************************************************
*                            rocker_DataHandler              
*
* Description: 摇杆数据处理函数
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
static void rocker_DataHandler(void)
{
	uint8_t i = 0;
	int16_t thrust = 0, roll = 0, pitch = 0, yaw = 0;
	
	for(i = 0; i < ROCKER_FILTER_TIMES; i++)
	{
		thrust += thrustDataRaw[i];
		roll += rollDataRaw[i];
		yaw += yawDataRaw[i];
		pitch += pitchDataRaw[i];
	}
	
	/*  将各通道数据转换成百分比,扩大10倍，便于处理  */
	rocker.thrust = (((thrust >> 3) - THRUST_MID_VALUE) * 1000) / ((255 - ROCKER_DEAD_RANGE) / 2);
	rocker.roll = (((roll >> 3) - ROLL_MID_VALUE) * 1000) / ((255 - ROCKER_DEAD_RANGE) / 2);
	rocker.pitch = (((pitch >> 3) - PITCH_MID_VALUE) * 1000) / ((255 - ROCKER_DEAD_RANGE) / 2);
	rocker.yaw = (((yaw >> 3) - YAW_MID_VALUE) * 1000) / ((255 - ROCKER_DEAD_RANGE) / 2);
}


/*
*********************************************************************************************************
*                        task_RockerTask                  
*
* Description: 摇杆任务
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void task_RockerTask(void *p_arg)
{
	rocker_Init();
	
	while(1)
	{
		xSemaphoreTake(rockerDateTransDone, portMAX_DELAY);	/*  等待数据传输完成  */
		
		rocker_DataHandler();		/*  处理摇杆数据  */
	}
}

/*
*********************************************************************************************************
*                           DMA2_Stream0_IRQHandler               
*
* Description: DMA2数据流0中断函数,用于摇杆ADC中断数据处理
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void __attribute__((used)) DMA2_Stream0_IRQHandler(void)
{
	portBASE_TYPE	xHigherPriorityTaskWoken = pdFALSE;
	
	/*  读取各通道数据  */
	thrustDataRaw[rockerDataCnt] = bsp_rocker_GetValue(0);
	rollDataRaw[rockerDataCnt] = bsp_rocker_GetValue(3);
	yawDataRaw[rockerDataCnt] = bsp_rocker_GetValue(1);
	pitchDataRaw[rockerDataCnt] = bsp_rocker_GetValue(2);
	
	DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
	
	/*  读取多次用于滤波,当读取完成一次后释放标志量,通知任务处理数据  */
	if(++rockerDataCnt >= ROCKER_FILTER_TIMES) 
	{
		rockerDataCnt = 0;
		xSemaphoreGiveFromISR(rockerDateTransDone, &xHigherPriorityTaskWoken);
	}
}

/********************************************  END OF FILE  *******************************************/
