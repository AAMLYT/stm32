#include "stm32f10x.h"                  // Device header

/*
对射式红外传感器
无遮挡时输出低电平，有遮挡时输出高电平


*/
uint16_t count = 0;//记录中断触发次数
void DsSensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//AFIO时钟
	//GPIO端口初始化
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;                                                                                                             ;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	//AFIO中断设置选择中断引脚线
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	//EXTI初始化配置
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line14; //外部中断线选择
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//中断触发或事件触发中断 ，这里选择中断触发
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;//上升沿触发
	EXTI_InitStruct.EXTI_LineCmd =ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	//优先级配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//抢占优先级与响应优先级分组，2个抢占与2个响应
	
	//设置NVIC初始化
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;//中断线选择
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级的优先级数
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;//响应优先级的优先级数
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
}


/*对射红外模块没有挡住的话输出低电平，挡住输出高电平*/
uint16_t DS_count(void)
{
	return count;
}

//中断函数，如果触发中断直接进入此函数
void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line14))//判断是否触发中断
	{
		//上升沿触发计数，防抖
		if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14))
		{
			count ++;
		}
			//下降沿沿触发计数，防抖
//		if (!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14))
//		{
//			count ++;
//		}
		//上升沿下降沿都触发
//		count ++;
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}


