#include "stm32f10x.h"

/* 函数原型列表 */
void GPIO_Configuration(void); //GPIO初始化
void USART_Configuration(void);//串口初始化
void NVIC_Configuration(void); //配置中断向量表
void CAN_Configuration(void);  //CAN初始化
void CanWriteData(uint16_t ID);//CAN发送一帧数据
