#include "stm32f10x.h"                  // Device header

/*


*/
uint16_t count1 = 0;
void Encode_Init(void)
{
	//时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//由AFIO选择中断的I/O口
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
	
	//设置EXTI初始化
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line0 | EXTI_Line1;         //外部中断线选择
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;             //中断触发或事件触发中断 ，这里选择中断触发
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;         //下降沿触发
	EXTI_InitStruct.EXTI_LineCmd =ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);              //抢占优先级与响应优先级分组，2个抢占与2个响应
	
	//设置NVIC初始化
	NVIC_InitTypeDef NVIC_InitStructure;						             //定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			       //选择配置NVIC的EXTI0线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				       //指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	   //指定NVIC线路的抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			     //指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);								               //将结构体变量交给NVIC_Init，配置NVIC外设
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			       //选择配置NVIC的EXTI1线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				       //指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	   //指定NVIC线路的抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			     //指定NVIC线路的响应优先级为2
	NVIC_Init(&NVIC_InitStructure);							
	
}
/*
编码器计数
返回计数值
*/
uint16_t RE_Num(void)
{
	uint16_t swap;
	swap = count1;
	count1 = 0;
	return swap;
	
}

/*
正转计数
*/
void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0))		                        //判断是否是外部中断0号线触发的中断
	{

		if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
		{
			if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1))		        //PA0的下降沿触发中断，此时检测另一相PA1的电平，目的是判断旋转方向
			{
				count1 ++;					                                  //此方向定义为正转，计数变量自增
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line0);			
													
	}
}

/*
反转计数
*/
void EXTI1_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line1) == SET)		               //判断是否是外部中断0号线触发的中断
	{
		
		if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1))
		{
			if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))		       //PA1的下降沿触发中断，此时检测另一相PA0的电平，目的是判断旋转方向
			{
				count1 --;					                                 //此方向定义为反转，计数变量自减
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line1);			
													
	}
}
