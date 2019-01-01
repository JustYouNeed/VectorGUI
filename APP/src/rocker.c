/**
  *******************************************************************************************************
  * File Name: rocker.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-24
  * Brief: �����ֱ�����
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-24
	*			Mod: �����ļ�
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

/*  ҡ����ֵ  */
# define THRUST_MID_VALUE	 130
# define ROLL_MID_VALUE		 129
# define PITCH_MID_VALUE	 130
# define YAW_MID_VALUE		125

# define ROCKER_DEAD_RANGE	7			/*  ҡ������ֵ  */

# define ROCKER_FILTER_TIMES	8		/*  ҡ�������˲�����  */

/*
  *******************************************************************************************************
  *                              LOCAL VARIABLE
  *******************************************************************************************************
*/
static bool isInit = false;		/*  ҡ�˳�ʼ����־λ  */
static xSemaphoreHandle rockerDateTransDone = NULL;		/*  ҡ�����ݴ�������ź���  */
static uint16_t thrustDataRaw[ROCKER_FILTER_TIMES];		/*  �洢ԭʼ����  */
static uint16_t pitchDataRaw[ROCKER_FILTER_TIMES];		
static uint16_t rollDataRaw[ROCKER_FILTER_TIMES];
static uint16_t yawDataRaw[ROCKER_FILTER_TIMES];
static uint8_t rockerDataCnt = 0;			/*  ҡ�˽������ݼ�������  */
static rocker_t rocker;		/*  ҡ�˽ṹ��  */

/*
*********************************************************************************************************
*                    rocker_MsgInit                      
*
* Description: ҡ���ź�����ʼ������
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
* Description: ҡ���豸��ʼ������
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
* Description: ҡ�˳�ʼ������
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
* Description: ҡ�����ݴ�����
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
	
	/*  ����ͨ������ת���ɰٷֱ�,����10�������ڴ���  */
	rocker.thrust = (((thrust >> 3) - THRUST_MID_VALUE) * 1000) / ((255 - ROCKER_DEAD_RANGE) / 2);
	rocker.roll = (((roll >> 3) - ROLL_MID_VALUE) * 1000) / ((255 - ROCKER_DEAD_RANGE) / 2);
	rocker.pitch = (((pitch >> 3) - PITCH_MID_VALUE) * 1000) / ((255 - ROCKER_DEAD_RANGE) / 2);
	rocker.yaw = (((yaw >> 3) - YAW_MID_VALUE) * 1000) / ((255 - ROCKER_DEAD_RANGE) / 2);
}


/*
*********************************************************************************************************
*                        task_RockerTask                  
*
* Description: ҡ������
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
		xSemaphoreTake(rockerDateTransDone, portMAX_DELAY);	/*  �ȴ����ݴ������  */
		
		rocker_DataHandler();		/*  ����ҡ������  */
	}
}

/*
*********************************************************************************************************
*                           DMA2_Stream0_IRQHandler               
*
* Description: DMA2������0�жϺ���,����ҡ��ADC�ж����ݴ���
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
	
	/*  ��ȡ��ͨ������  */
	thrustDataRaw[rockerDataCnt] = bsp_rocker_GetValue(0);
	rollDataRaw[rockerDataCnt] = bsp_rocker_GetValue(3);
	yawDataRaw[rockerDataCnt] = bsp_rocker_GetValue(1);
	pitchDataRaw[rockerDataCnt] = bsp_rocker_GetValue(2);
	
	DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
	
	/*  ��ȡ��������˲�,����ȡ���һ�κ��ͷű�־��,֪ͨ����������  */
	if(++rockerDataCnt >= ROCKER_FILTER_TIMES) 
	{
		rockerDataCnt = 0;
		xSemaphoreGiveFromISR(rockerDateTransDone, &xHigherPriorityTaskWoken);
	}
}

/********************************************  END OF FILE  *******************************************/
