/**
  *******************************************************************************************************
  * File Name: 
  * Author: 
  * Version: 
  * Date: 
  * Brief: 
  *******************************************************************************************************
  * History
  *
  *
  *******************************************************************************************************
  */	
/*
  *******************************************************************************************************
  *                              INCLUDE FILES
  *******************************************************************************************************
*/
# include "app.h"


#define ATTITUDE_UPDATE_RATE 	RATE_500_HZ
#define ATTITUDE_UPDATE_DT 		(1.0f / ATTITUDE_UPDATE_RATE)

#define DEG2RAD		0.017453293f	/* 度转弧度 π/180 */
#define RAD2DEG		57.29578f		/* 弧度转度 180/π */

/*
  *******************************************************************************************************
  *                              LOCAL VARIABLE
  *******************************************************************************************************
*/
static const float imuKp = 0.4f;
static const float imuKi = 0.001f;
static float exInt = 0.0f;
static float eyInt = 0.0f;
static float ezInt = 0.0f;
	
static float q0 = 1.0f;
static float q1 = 0.0f;
static float q2 = 0.0f;
static float q3 = 0.0f;

static float baseAccZ = 1.0f;
static bool isSensorCalibrated = false;
static bool isIMUCalibrated = false;


static state_t state;
static sensorData_t sensor;


/*
*********************************************************************************************************
*                       invSqrt                   
*
* Description: 快速开平方根
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
static float invSqrt(float x)
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long *)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float *)&i;
	y = y * (1.5f - (halfx * y * y));
	
	return y;
}

/*
*********************************************************************************************************
*                        attitude_imuUpdata                  
*
* Description: 姿态四元数更新
*             
* Arguments  : 1.acc: 加速度计数据
*							 2.gyro: 陀螺仪数据
*							 3.state: 四轴状态结构体指针
*							 4.dt: 积分时间
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
static void attitude_imuUpdata(Axis3f_t acc, Axis3f_t gyro, state_t *state, float dt)
{
	float normalise = 0.0f;
	float ex = 0.0f, ey = 0.0f, ez = 0.0f;
	float q0s, q1s, q2s, q3s;
	static float R11, R21;
	static float vecxZ, vecyZ, veczZ;
	float halfT = 0.5f * dt;
	Axis3f_t tempAcc = acc;
	
	gyro.x *= DEG2RAD;
	gyro.y *= DEG2RAD;
	gyro.z *= DEG2RAD;
	
	if((acc.x != 0) && (acc.y != 0) && (acc.z != 0))
	{
		normalise = invSqrt(acc.x * acc.x + acc.y * acc.y + acc.z * acc.z);
		acc.x *= normalise;
		acc.y *= normalise;
		acc.z *= normalise;
		
		/*加速计读取的方向与重力加速计方向的差值，用向量叉乘计算*/
		ex = (acc.y * veczZ - acc.z * vecyZ);
		ey = (acc.z * vecxZ - acc.x * veczZ);
		ez = (acc.x * vecyZ - acc.y * vecxZ);
		
		/*误差累计，与积分常数相乘*/
		exInt += imuKi * ex * dt;
		eyInt += imuKi * ey * dt;
		ezInt += imuKi * ez * dt;
		
		/*用叉积误差来做PI修正陀螺零偏，即抵消陀螺读数中的偏移量*/
		gyro.x += imuKp * ex + exInt;
		gyro.y += imuKp * ey + eyInt;
		gyro.z += imuKp * ez + ezInt;
	}

	/* 一阶近似算法，四元数运动学方程的离散化形式和积分 */
	float q0Last = q0;
	float q1Last = q1;
	float q2Last = q2;
	float q3Last = q3;
	q0 += (-q1Last * gyro.x - q2Last * gyro.y - q3Last * gyro.z) * halfT;
	q1 += ( q0Last * gyro.x + q2Last * gyro.z - q3Last * gyro.y) * halfT;
	q2 += ( q0Last * gyro.y - q1Last * gyro.z + q3Last * gyro.x) * halfT;
	q3 += ( q0Last * gyro.z + q1Last * gyro.y - q2Last * gyro.x) * halfT;
	
	/*单位化四元数*/
	normalise = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= normalise;
	q1 *= normalise;
	q2 *= normalise;
	q3 *= normalise;
	/*四元数的平方*/
	q0s = q0 * q0;
	q1s = q1 * q1;
	q2s = q2 * q2;
	q3s = q3 * q3;
	
	R11 = q0s + q1s - q2s - q3s;	/*矩阵(1,1)项*/
	R21 = 2 * (q1 * q2 + q0 * q3);	/*矩阵(2,1)项*/

	/*机体坐标系下的Z方向向量*/
	vecxZ = 2 * (q1 * q3 - q0 * q2);/*矩阵(3,1)项*/
	vecyZ = 2 * (q0 * q1 + q2 * q3);/*矩阵(3,2)项*/
	veczZ = q0s - q1s - q2s + q3s;	/*矩阵(3,3)项*/
	
	if (vecxZ>1) vecxZ=1;
	if (vecxZ<-1) vecxZ=-1;
	
	state->attitude.pitch = -asinf(vecxZ) * RAD2DEG;
	state->attitude.roll = atan2f(vecyZ, veczZ) * RAD2DEG;
	state->attitude.yaw = atan2f(R21, R11) * RAD2DEG;
	
	if(!isIMUCalibrated)
	{
		baseAccZ = tempAcc.x * vecxZ + tempAcc.y * vecyZ + tempAcc.z * veczZ;
		isIMUCalibrated = true;
	}
	
	state->acc.z = tempAcc.x* vecxZ + tempAcc.y * vecyZ + tempAcc.z * veczZ - baseAccZ;	/*Z轴加速度(去除重力加速度)*/
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
static void attitude_StateEstimator(state_t *state, const sensorData_t *sensor, const uint32_t tick)
{
	/*  姿态解算500Hz的频率  */
	if (RATE_DO_EXECUTE(ATTITUDE_UPDATE_RATE, tick)) /* 500Hz 2ms update  */
	{	  
		attitude_imuUpdata(sensor->acc, sensor->gyro, state, ATTITUDE_UPDATE_DT);
	}
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
void attitude_Task(void *p_arg)
{
	uint32_t tick = 0;
	uint32_t lastWakeTime = xTaskGetTickCount();
	
	/*  等待传感器数据校准  */
	while(sensors_IsCalibrated())
	{
		vTaskDelayUntil(&lastWakeTime, MAIN_LOOP_DT);
	}
	/*  等待传感器稳定  */
	vTaskDelay(1000);
	while(1)
	{
		vTaskDelayUntil(&lastWakeTime, MAIN_LOOP_DT);
		
		sensors_AcquireData(&sensor);		/*  获取传感器的数据  */
		attitude_StateEstimator(&state, &sensor, tick);		/*  解算四轴姿态  */
		
		tick++;	
	}
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
void attitude_GetData(attitude_t *attitude)
{
	attitude->pitch = state.attitude.pitch;
	attitude->roll = state.attitude.roll;
	attitude->yaw = state.attitude.yaw;
}

/********************************************  END OF FILE  *******************************************/


