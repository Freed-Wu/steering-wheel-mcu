#include "stm32f10x.h"

/* ����ԭ���б� */
void GPIO_Configuration(void); //GPIO��ʼ��
void USART_Configuration(void);//���ڳ�ʼ��
void NVIC_Configuration(void); //�����ж�������
void CAN_Configuration(void);  //CAN��ʼ��
void CanWriteData(uint16_t ID);//CAN����һ֡����
