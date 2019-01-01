/**
  *******************************************************************************************************
  * File Name: config.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-8-23
  * Brief: Vector配置文件
  *******************************************************************************************************
  * History
  *		Author: Vector
	*		Date: 2018-8-23
	*		Mod: 建立文件
  *
  *******************************************************************************************************
  */	
	
# ifndef __CONFIG_H
# define __CONFIG_H

/*  串口部分配置  */
# define UART_LINK_COM				USART1
# define UART_LINK_PORT				GPIOB
# define UART_LINK_RX_PIN			GPIO_Pin_6
# define UART_LINK_TX_PIN			GPIO_Pin_7
# define UART_LINK_BAUDRATE		961200

/*  MPU9250引脚配置部分  */
# define MPU_SDA_PORT			GPIOB
# define MPU_SDA_PIN			GPIO_Pin_8

# define MPU_SCL_PORT			GPIOB
# define MPU_SCL_PIN			GPIO_Pin_9

# define MPU_INT_PORT			GPIOB
# define MPU_INT_PIN			GPIO_Pin_5




# define SENSORS_USE_MAGNETOMTER			1u
# define SENSORS_USE_BAROMTER					0u


#define RATE_5_HZ		5
#define RATE_10_HZ		10
#define RATE_25_HZ		25
#define RATE_50_HZ		50
#define RATE_100_HZ		100
#define RATE_200_HZ 	200
#define RATE_250_HZ 	250
#define RATE_500_HZ 	500
#define RATE_1000_HZ 	1000

#define MAIN_LOOP_RATE 	RATE_1000_HZ
#define MAIN_LOOP_DT	(uint32_t)(1000/MAIN_LOOP_RATE)	/*单位ms*/

#define RATE_DO_EXECUTE(RATE_HZ, TICK) ((TICK % (MAIN_LOOP_RATE / RATE_HZ)) == 0)

# endif


/********************************************  END OF FILE  *******************************************/


