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
# ifndef __SENSOR_TYPES_H
# define __SENSOR_TYPES_H

#if defined(__CC_ARM) 
	#pragma anon_unions
#endif

typedef union 
{
	struct
	{
		int16_t x;
		int16_t y;
		int16_t z;
	};
	int16_t axis[3];
}Axis3i16_t;

typedef union
{
	struct
	{
		int32_t x;
		int32_t y;
		int32_t z;
	};
	int32_t axis[3];
}Axis3i32_t;

typedef union
{
	struct 
	{
		int64_t x;
		int64_t y;
		int64_t z;
	};
	int64_t axis[3];
}Axis3i64_t;

typedef union
{
	struct
	{
		float x;
		float y;
		float z;
	};
	float axis[3];
}Axis3f_t;

typedef struct
{
	float pressure;
	float temperature;
	float asl;
} baro_t;


typedef struct 
{
	Axis3f_t acc;
	Axis3f_t gyro;
	Axis3f_t mag;
	baro_t baro;
}sensorData_t;

# endif


/********************************************  END OF FILE  *******************************************/


