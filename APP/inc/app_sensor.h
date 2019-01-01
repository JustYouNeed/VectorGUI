/**
  *******************************************************************************************************
  * File Name: app_sensor.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-8-2
  * Brief: VectorFly传感器部分
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-8-2
	*			Mod: 建立文件
  *
  *******************************************************************************************************
  */	
# ifndef __APP_SENSOR_H
# define __APP_SENSOR_H

void sensors_Task(void *p_arg);
bool sensors_Test(void);
bool sensors_IsCalibrated(void);

void sensors_AcquireData(sensorData_t *sensors);

bool sensors_GetGyroData(Axis3f_t *gyro);
bool sensors_GetAccData(Axis3f_t *acc);
bool sensors_GetMagData(Axis3f_t *mag);
bool sensors_GetBaroData(baro_t *baro);


# endif

/********************************************  END OF FILE  *******************************************/


