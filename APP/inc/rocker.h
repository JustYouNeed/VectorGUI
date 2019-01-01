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
# ifndef __ROCKER_H_
# define __ROCKER_H_


typedef struct
{
	int16_t thrust;
	int16_t roll;
	int16_t pitch;
	int16_t yaw;
}rocker_t;

void task_RockerTask(void *p_arg);

# endif

/********************************************  END OF FILE  *******************************************/
