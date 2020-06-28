/*****************************************************************************
M31S1322_STM32_V1 2015��5��	 www.lcdsoc.com
ʹ��˵����
1����DEMOʹ��F103VCT6��Ƭ�� 8m�ⲿ����
2��M47SP1322 ����˫1322��������ÿ������������256*64���أ�ͨ��CS1,CS2���� һ��4λ��ʾ1�е�ֵ��һ��Ҫд��2���ֽڣ�4��
3�������ѹ3.3v IO�ߵ�ƽΪ 0.8*3.3v   IO�͵�ƽΪ 0.2*3.3v
4����Ҫ������oled.c �ļ�
5��ASCII�ֿ� �Զ��庺�ֿ��oled.c�����Լ���Ҫȡ��
6�����塢����������oled.h
7������IO��ģ��SPIͨѶ��ʽ����oledģ�飬�������£�
OLED�˿ڶ��� 4��SPI
CS1	~ PE6
RES ~ PE5
DC   ~ PE4
CLK  ~ PE3
DIN ~ PE2
CS2 ~ PE1

8��x������ּ��d���ҪΪ4�ı�����
******************************************************************************/

#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "can.h"
#include "usart.h"
#include "LED.h"
void Int_To_Str(int x,char *Str);
extern uint32_t CAN_ID1,CAN_ID2;//��ʶ��
extern uint8_t CAN_DATA0,CAN_DATA1,CAN_DATA2,CAN_DATA3,CAN_DATA4,CAN_DATA5,CAN_DATA6,CAN_DATA7;//����
extern uint8_t CanFlag,Display,IDE;//�м����

int main(void)
{	int tem=99,voltage=0;
	unsigned char a[10];
	unsigned char b[10];
	int m=100;
	int velocity=0;
 	delay_init();			 //��ʱ������ʼ��
	//NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	OLED_Init();			//��ʼ��OLED
	CAN_Configuration();  //CAN��ʼ��
	USART_Configuration();
	LED_Configuration();

	Asc12_24(72,50 ,"Emotion") ;  // 12*24����ASCII��  Every english character takes up 8
	delay_ms(1864);					   // ��ʱ
	delay_ms(1864);
	Fill_Block(0x00,0x00,0x3F,0x00,0x7F); //��ȫ��

	Int_To_Str(m,a);
	while(1)
	{	//CanWriteData(0x03);
		if(CanFlag == ENABLE ) //�յ�һ֡��ȷ�����ݣ�����ӡ������
		{	if(IDE==CAN_ID_EXT)
			{	if(CAN_ID2 == 0x186540f3)
				{	tem=CAN_DATA0&0x04;
					if(tem==0)
					{	GPIO_SetBits(GPIOB ,GPIO_Pin_7);
						GPIO_ResetBits(GPIOB ,GPIO_Pin_5);
					}
					else
						GPIO_ResetBits(GPIOB ,GPIO_Pin_5|GPIO_Pin_7);
				}
				if(CAN_ID2 == 0x186040f3)
				{
					voltage=(CAN_DATA0<<8)+CAN_DATA1;
					Int_To_Str(voltage,a);
				}
				if(CAN_ID2 == 0x8888)
				{
					velocity=CAN_DATA0;
					Int_To_Str(velocity,b);
				}
			}
			else
			{	if(CAN_ID1 == 0x06)
				{	int value = CAN_DATA2;
					if (value <= 0x19)
					{	GPIO_SetBits(GPIOB ,GPIO_Pin_0);
						GPIO_ResetBits(GPIOB ,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_6);
					}
					else if (value <= 0x32&&value>0x19)
					{	GPIO_SetBits(GPIOB ,GPIO_Pin_0|GPIO_Pin_1);
						GPIO_ResetBits(GPIOB ,GPIO_Pin_2|GPIO_Pin_6);
					}
					else if (value <= 0x4B&&value > 0x32)
					{	GPIO_SetBits(GPIOB ,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
						GPIO_ResetBits(GPIOB ,GPIO_Pin_6);
					}
					else if (value >=0x4B)
					{	GPIO_SetBits(GPIOB ,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_6);
					}
				}
				else if(CAN_ID1 == 0x05)
				{	if(CAN_DATA4==0x83)
					{	GPIO_SetBits(GPIOB ,GPIO_Pin_5);
						GPIO_ResetBits(GPIOB ,GPIO_Pin_7);
					}
					else
						GPIO_ResetBits(GPIOB ,GPIO_Pin_7|GPIO_Pin_5);
				}
			}
		}
	

//  CanWriteData(0x1314);
//  		//ASCII����ʾ
//
//		char power[]="";
//		char speed[]="";
//		Asc12_24(72,50 ,"Emotion") ;  // 12*24����ASCII��  Every english character takes up 8
//		delay_ms(1864);					   // ��ʱ
//	 	delay_ms(1864);
//		Fill_Block(0x00,0x00,0x3F,0x00,0x7F); //��ȫ��
//
// 	  Asc5_8( 45,50 ,"Speed:") ;	   // 5*8����ASCII��
	Asc8_16( 8*8,58 ,"voltage:") ;
// 	  Asc8_16( 84,45 ,"8*16 ASCII") ;	   // 8*16����ASCII��
		Asc20_40( 8*8+9*8,40 ,a) ;  //
		Asc8_16( 8*8+9*8+24*3,58 ,"V") ;	   // 6*12����ASCII��
		Asc8_16( 16*8,16*6 ,"speed:") ;	   //
		Asc8_16( 16*8+8*6,16*6 ,b) ;
		Asc8_16( 16*8+8*6+8*2,16*6 ,"r") ;
//		delay_ms(1864);					   // ��ʱ
//	 	delay_ms(1864);
//		Fill_Block(0x00,0x00,0x3F,0x00,0x7F); //��ȫ��
//
//		Asc20_40( 0,43 ,"20*40ASCII") ;  // 20*40����ASCII��
//  		delay_ms(1864);
//		delay_ms(1864);
//		Fill_Block(0x00,0x00,0x3F,0x00,0x7F); //��ȫ��
//
//		//������ʾ
//	Show_HZ12_12(4,12,0,0,3);	// 12*12����
// 	  Show_HZ16_16(64,44,4,0,3);	// ���꣨64��17�����ּ��4���ֿ��0~3�����¿Ƽ�����16*16����
//		Show_HZ24_24(140,80,4,0,3); //24*24����
//		delay_ms(1864);
//		delay_ms(1864);
//		delay_ms(1864);
//		Fill_Block(0x00,0x00,0x3F,0x00,0x7F); //��ȫ��
  		//ͼƬ��ʾ
//		Show_Pattern(&lcdsoc256128[0],0,63,0,127);	 //��ʾ��ɫͼ��ȡģ����16�Ҷ�ȡģ
//	 	delay_ms(1864);
//		delay_ms(1864);
//		Fill_Block(0x00,0x00,0x3F,0x00,0x7F); //��ȫ��
//		delay_ms(1864);
}

//		Grayscale(CS1);	  //�ϰ����Ҷ���ʾ
//		delay_ms(1864);
//		Fill_Block(0x00,0x00,0x3F,0x00,0x7F); //��ȫ��
//		Grayscale(CS2);	//�°����Ҷ���ʾ
//	 	delay_ms(1864);
//		Fill_Block(0x00,0x00,0x3F,0x00,0x7F); //��ȫ��
 }
void Int_To_Str(int x,char *Str)
{
  int t;
	char *Ptr,Buf[5];
	int i = 0;
	Ptr = Str;
	if(x < 10)
	{
		*Ptr ++ = '0';
		*Ptr ++ = x+0x30;
	}
	else
	{
		while(x > 0)
		{
			t = x % 10;
			x = x / 10;
			Buf[i++] = t+0x30;	// ?????????ASCII???
		}
		i -- ;
		for(;i >= 0;i --) 		// ?????????
		{
			*(Ptr++) = Buf[i];
		}
	}
	*Ptr = '\0';
}


