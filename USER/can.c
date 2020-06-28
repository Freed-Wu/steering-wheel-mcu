#include "delay.h"
#include "stm32f10x.h"
#include "sys.h"
#include "can.h"
//#include "stm32f10x_can.h"
uint32_t CAN_ID1,CAN_ID2;//标识符
uint8_t CAN_DATA0,CAN_DATA1,CAN_DATA2,CAN_DATA3,CAN_DATA4,CAN_DATA5,CAN_DATA6,CAN_DATA7;//数据
uint8_t CanFlag,Display,IDE;//中间变量

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /*GPIOA时钟使能*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  /*CAN1时钟使能*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

  /* 配置CAN.RX->PA11引脚 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);   
  /* 配置CAN.TX->PA12引脚 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);	     

  /* 配置LED->PA8引脚(以NiRen-STM32mini板为例)*/		 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
* 函数名  : NVIC_Configuration
* 描述    : 配置中断向量表
* 输入    : None
* 输出    : None
* 返回    : None
* 注意		: None
*******************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable CAN1 RX0 interrupt IRQ channel */
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* 函数名  : CAN_Configuration
* 描述    : 配置CAN控制器工作在标准帧模式，并使能接收中断
* 输入    : None
* 输出    : None
* 返回    : None
* 注意		: None
*******************************************************************************/
void CAN_Configuration(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;

  NVIC_Configuration();
  GPIO_Configuration();
  /* CAN 寄存器初始化 */
  CAN_DeInit(CAN1);//将CAN1所有寄存器设置为缺省值
  CAN_StructInit(&CAN_InitStructure);//将结构体参数按缺省值填入

  /* CAN 寄存器初始化 关于CAN通信波特率的计算可以查看STM32中文参考手册*/
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = ENABLE;//使能自动离线管理，断线重连能自动连接上
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = ENABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;//正常模式
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
  CAN_InitStructure.CAN_Prescaler = 18;

  if (CAN_Init(CAN1,&CAN_InitStructure) == CANINITFAILED)//初始化CAN 		
  {
     //这里可以添加初始化失败处理
  }	
  /* CAN 过滤器组初始化 */
  CAN_FilterInitStructure.CAN_FilterNumber=0;
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器关联邮箱0
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;//激活过滤器
  CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);//使能CAN接收中断
  CAN_FilterInit(&CAN_FilterInitStructure);
}

/*******************************************************************************
* 函数名  : CanWriteData
* 描述    : 发送一帧数据到CAN总线
* 输入    : 数据标识符
* 输出    : None
* 返回    : None
* 注意		: None
*******************************************************************************/
void CanWriteData(uint16_t ID)
{
  CanTxMsg TxMessage;

  CAN_DATA0=0X01;  CAN_DATA1=0X02;  
  CAN_DATA2=0X03;  CAN_DATA3=0X04;  
  CAN_DATA4=0X05;  CAN_DATA5=0X06;
  CAN_DATA6=0X07;  CAN_DATA7=0X08; 
 	
  //TxMessage.StdId = ID;        
  TxMessage.ExtId = ID;        //拓展帧标识符
  TxMessage.RTR = CAN_RTR_DATA;//数据帧
  TxMessage.IDE = CAN_ID_EXT ; //拓展帧模式
  TxMessage.DLC = 8;           //数据长度
  TxMessage.Data[0] = CAN_DATA0;//发送数据内容    
  TxMessage.Data[1] = CAN_DATA1;    
  TxMessage.Data[2] = CAN_DATA2;    
  TxMessage.Data[3] = CAN_DATA3;    
  TxMessage.Data[4] = CAN_DATA4;    
  TxMessage.Data[5] = CAN_DATA5;     
  TxMessage.Data[6] = CAN_DATA6;    
  TxMessage.Data[7] = CAN_DATA7;      
  CAN_Transmit(CAN1,&TxMessage);//发送一帧数据到CAN总线
}

/*******************************************************************************
* 函数名  : USB_LP_CAN1_RX0_IRQHandler
* 描述    : CAN接收中断
* 输入    : None
* 输出    : None
* 返回    : None
* 注意		: None
*******************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  CanRxMsg RxMessage;
  CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);//获取邮箱0数据
  CAN_ID1=RxMessage.StdId;      //获取拓展帧标识符
	CAN_ID2=RxMessage.ExtId;   
	IDE=RxMessage.IDE;
  CAN_DATA0=RxMessage.Data[0]; //获取数据
  CAN_DATA1=RxMessage.Data[1];
  CAN_DATA2=RxMessage.Data[2];
  CAN_DATA3=RxMessage.Data[3];
  CAN_DATA4=RxMessage.Data[4];
  CAN_DATA5=RxMessage.Data[5];
  CAN_DATA6=RxMessage.Data[6];
  CAN_DATA7=RxMessage.Data[7];
  CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);//清接收中断标志
  CanFlag = ENABLE;  //置接收完成标志
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
