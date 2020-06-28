#include "delay.h"
#include "stm32f10x.h"
#include "sys.h"
#include "can.h"
//#include "stm32f10x_can.h"
uint32_t CAN_ID1,CAN_ID2;//��ʶ��
uint8_t CAN_DATA0,CAN_DATA1,CAN_DATA2,CAN_DATA3,CAN_DATA4,CAN_DATA5,CAN_DATA6,CAN_DATA7;//����
uint8_t CanFlag,Display,IDE;//�м����

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /*GPIOAʱ��ʹ��*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  /*CAN1ʱ��ʹ��*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

  /* ����CAN.RX->PA11���� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);   
  /* ����CAN.TX->PA12���� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);	     

  /* ����LED->PA8����(��NiRen-STM32mini��Ϊ��)*/		 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
* ������  : NVIC_Configuration
* ����    : �����ж�������
* ����    : None
* ���    : None
* ����    : None
* ע��		: None
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
* ������  : CAN_Configuration
* ����    : ����CAN�����������ڱ�׼֡ģʽ����ʹ�ܽ����ж�
* ����    : None
* ���    : None
* ����    : None
* ע��		: None
*******************************************************************************/
void CAN_Configuration(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;

  NVIC_Configuration();
  GPIO_Configuration();
  /* CAN �Ĵ�����ʼ�� */
  CAN_DeInit(CAN1);//��CAN1���мĴ�������Ϊȱʡֵ
  CAN_StructInit(&CAN_InitStructure);//���ṹ�������ȱʡֵ����

  /* CAN �Ĵ�����ʼ�� ����CANͨ�Ų����ʵļ�����Բ鿴STM32���Ĳο��ֲ�*/
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = ENABLE;//ʹ���Զ����߹��������������Զ�������
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = ENABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;//����ģʽ
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
  CAN_InitStructure.CAN_Prescaler = 18;

  if (CAN_Init(CAN1,&CAN_InitStructure) == CANINITFAILED)//��ʼ��CAN 		
  {
     //���������ӳ�ʼ��ʧ�ܴ���
  }	
  /* CAN ���������ʼ�� */
  CAN_FilterInitStructure.CAN_FilterNumber=0;
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//��������������0
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;//���������
  CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);//ʹ��CAN�����ж�
  CAN_FilterInit(&CAN_FilterInitStructure);
}

/*******************************************************************************
* ������  : CanWriteData
* ����    : ����һ֡���ݵ�CAN����
* ����    : ���ݱ�ʶ��
* ���    : None
* ����    : None
* ע��		: None
*******************************************************************************/
void CanWriteData(uint16_t ID)
{
  CanTxMsg TxMessage;

  CAN_DATA0=0X01;  CAN_DATA1=0X02;  
  CAN_DATA2=0X03;  CAN_DATA3=0X04;  
  CAN_DATA4=0X05;  CAN_DATA5=0X06;
  CAN_DATA6=0X07;  CAN_DATA7=0X08; 
 	
  //TxMessage.StdId = ID;        
  TxMessage.ExtId = ID;        //��չ֡��ʶ��
  TxMessage.RTR = CAN_RTR_DATA;//����֡
  TxMessage.IDE = CAN_ID_EXT ; //��չ֡ģʽ
  TxMessage.DLC = 8;           //���ݳ���
  TxMessage.Data[0] = CAN_DATA0;//������������    
  TxMessage.Data[1] = CAN_DATA1;    
  TxMessage.Data[2] = CAN_DATA2;    
  TxMessage.Data[3] = CAN_DATA3;    
  TxMessage.Data[4] = CAN_DATA4;    
  TxMessage.Data[5] = CAN_DATA5;     
  TxMessage.Data[6] = CAN_DATA6;    
  TxMessage.Data[7] = CAN_DATA7;      
  CAN_Transmit(CAN1,&TxMessage);//����һ֡���ݵ�CAN����
}

/*******************************************************************************
* ������  : USB_LP_CAN1_RX0_IRQHandler
* ����    : CAN�����ж�
* ����    : None
* ���    : None
* ����    : None
* ע��		: None
*******************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  CanRxMsg RxMessage;
  CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);//��ȡ����0����
  CAN_ID1=RxMessage.StdId;      //��ȡ��չ֡��ʶ��
	CAN_ID2=RxMessage.ExtId;   
	IDE=RxMessage.IDE;
  CAN_DATA0=RxMessage.Data[0]; //��ȡ����
  CAN_DATA1=RxMessage.Data[1];
  CAN_DATA2=RxMessage.Data[2];
  CAN_DATA3=RxMessage.Data[3];
  CAN_DATA4=RxMessage.Data[4];
  CAN_DATA5=RxMessage.Data[5];
  CAN_DATA6=RxMessage.Data[6];
  CAN_DATA7=RxMessage.Data[7];
  CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);//������жϱ�־
  CanFlag = ENABLE;  //�ý�����ɱ�־
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
