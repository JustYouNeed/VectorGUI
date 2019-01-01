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
# ifndef __ATTITUDE_TYPES_H
# define __ATTITUDE_TYPES_H

#if defined(__CC_ARM) 
	#pragma anon_unions
#endif

typedef struct
{
	uint32_t timeStamp;
	
	float roll;
	float pitch;
	float yaw;
}attitude_t;

struct vec3_s
{
	uint32_t timeStamp;
	
	float x; 
	float y;
	float z;
};

typedef struct vec3_s point_t;
typedef struct vec3_s velocity_t;
typedef struct vec3_s acc_t;


/* Orientation as a quaternion */
typedef struct quaternion_s 
{
	uint32_t timeStamp;

	union 
	{
		struct 
		{
			float q0;
			float q1;
			float q2;
			float q3;
		};
		struct 
		{
			float x;
			float y;
			float z;
			float w;
		};
	};
} quaternion_t;


typedef struct
{
	attitude_t attitude;
	quaternion_t attitudeQuaternion;
	point_t position;
	velocity_t velocity;
	acc_t acc;
}state_t;

# endif


/********************************************  END OF FILE  *******************************************/


