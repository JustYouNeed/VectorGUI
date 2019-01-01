/**
  *******************************************************************************************************
  * File Name: pid.h
  * Author: Vector
  * Version: V2.3.0
  * Date: 2018-2-1
  * Brief: ���ļ��ṩ�˹���PID����������,�Լ���ر����Ķ���
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-2-1
	*			Mod: �������ļ�
	*
	*		2.Author: Vector
	*			Date: 2018-3-26
	*			Mod: �޸�PID�����������,ɾ������Ҫ�ı���,Error, Sum��
	*
	*		3.Author: Vector
	*			Date: 2018-4-22
	*			Mod: ����ģ��PID
	*
	*		4.Author: Vector
	*			Date: 2018-5-4
	*			Mod: �޸�PID�ṹ��,��ͨ��
	*					
  *
  *******************************************************************************************************
  */	
	
# ifndef __PID_H
# define __PID_H


/*  PID���ƽṹ��  */
typedef struct
{
	float Kp, Ki, Kd;		/*  ����ϵ��  */
	
	/*  ��ǰƫ��,�ϴ�ƫ��,���ϴ�ƫ��,����������Ҫ��������ʽPID  */
	float ErrorK, ErrorK_1, ErrorK_2;
	float Integral;		/*  ����  */
	float IntMax, IntRange;		/*  �������ֵ, ��������  */
}PID_TypeDef;

typedef struct
{
	float kp;
	float ki;
	float kd;
}PidParaObj_t;

typedef struct
{
	float desired;
	float error;
	float prevError;
	float integral;
	float derivative;
	
	PidParaObj_t pidPara;
	
	float intLimit;
	float dt;
}PidObj_t;


/*  ģ��PID���ƽṹ��  */
typedef struct 
{
	float ErrMax;		/*  �������  */
	float DErrMax;	/*  ���仯������  */
	volatile float Kerr;			/*  ƫ�����������,Kerr = N/ErrMax  */
	volatile float Kderr;		/*  ƫ��仯����������,Kderr = N/DErrMax  */
	
	float DeltaKpMax;		/*  �����KP��������  */
	float DeltaKiMax;		/*  KI��������  */
	float DeltaKdMax;		/*  KD��������  */
	volatile float Ku_P;					
	volatile float Ku_I;
	volatile float Ku_D;					/*  ��������,Ku_D = KdMax/N,��������������������Ŵ�  */
	volatile float KP;
	volatile float KI;
	volatile float KD;
		
	float KPMax, KIMax, KDMax;	/*  ��ģ��PID�����������������������ֵ  */
}fuzzyPidObj_t;

void pid_Init(PidObj_t *pid, const float desired, const PidParaObj_t pidPara, const float dt);
void pid_Reset(PidObj_t *pid);
void pid_SetIntegralLimit(PidObj_t *pid, const float limit);
void pid_SetDesired(PidObj_t *pid, const float desires);
float pid_Updata(PidObj_t *pid, const float error);
float pid_GetDesired(PidObj_t *pid);
float pid_SetKp(PidObj_t *pid, const float kp);
float pid_SetKi(PidObj_t *pid, const float ki);
float pid_SetKd(PidObj_t *pid, const float kd);
float pid_SetDt(PidObj_t *pid, const float dt);



void pid_ParaInit(void);
void pid_ReadPara(void);
void pid_StorePara(void);

void pid_PIDInit(PID_TypeDef *PID, float Kp, float Ki, float Kd, float IntMax, float IntRange);
float pid_IncrementalCalc(PID_TypeDef *PID, float Error);
float pid_PositionalCalc(PID_TypeDef *PID, float Error);

void fuzzy_PIDClac(fuzzyPidObj_t *Fuzzy, float Error, float DError);
void fuzzy_PIDInit(fuzzyPidObj_t *Fuzzy);

# endif
	
	
/********************************************  END OF FILE  *******************************************/
	

