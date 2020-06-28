//////////////////////////////////////////////////////////////////////////////////	 
// M47S1322_01_V1 FOR STM32 201505 WWW.LCDSOC.COM
//********************************************************************************
//�ӿڶ������oled.h�ļ�
 
#include "oled.h"
#include "stdlib.h"
#include "delay.h"
//����ASCII��
#include "Asc5x8.h" 	   //ascii 8*16�ֿ�
#include "Asc6x12.h" 	   //ascii 8*16�ֿ�
#include "Asc8x16.h" 	   //ascii 8*16�ֿ�
#include "Asc12x24.h" 	   //ascii 12*24�ֿ�
#include "Asc20x40.h" 	   //ascii 20*40�ֿ�
//�Զ��庺�ֿ�
#include "HZ12X12_S.h" 	   //12*12�����Զ��庺�ֿ�
#include "HZ16X16_S.h" 	   //16*16�����Զ��庺�ֿ�
#include "HZ24X24_S.h" 	   //24*24�����Զ��庺�ֿ�
// #include "HZ24X24_H.h" 	   //24*24�����Զ��庺�ֿ�
// #include "HZ24X24_K.h" 	   //24*24�����Զ��庺�ֿ�

   
 
//��SSD1322д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(u8 dat,u8 cmd,u8 cs)
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();

   	if(cs==2)
	{
	OLED_CS2_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS2_Set();
	OLED_DC_Set();  
	} 	
	  		  
	if(cs==1)
	{
	OLED_CS1_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS1_Set();
	OLED_DC_Set();  
	} 	
   
} 

//-----------------OLED�˿ڶ��� 4��SPI----------------  					   
/* ����IO��ģ��SPIͨѶ��ʽ����oledģ��
CS1    ~ PE6
RES ~ PE5
DC   ~ PE4
CLK  ~ PE3
DIN ~ PE2
CS2 ~ PE1
*/

//��ʼ��SSD1322				    
void OLED_Init(void)
{ 	
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��E�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_4|GPIO_Pin_1;	 //PE4,PE6,PE1    CS1 DC  CS2
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOE, &GPIO_InitStructure);	  //��ʼ��PE4,PE6
 	GPIO_SetBits(GPIOE,GPIO_Pin_6|GPIO_Pin_4|GPIO_Pin_1);	//PE4,PE6,PE1�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_2;				 //PE2,PE3,PE5�������	 RES CLK DIN
 	GPIO_Init(GPIOE, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOE,GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_2);						 //PE2,PE3,PE5 �����
 
    OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(100);
	OLED_RST_Set(); 
	
	//cs1 ��ʼ��
	OLED_WR_Byte(0xAE,OLED_CMD,CS1); //	Display Off

	OLED_WR_Byte(0xFD,OLED_CMD,CS1); // Set Command Lock
	OLED_WR_Byte(0x12,OLED_DATA,CS1); //
	
	OLED_WR_Byte(0xB3,OLED_CMD,CS1); // Set Clock as 80 Frames/Sec
	OLED_WR_Byte(0x91,OLED_DATA,CS1); //  

   	OLED_WR_Byte(0xCA,OLED_CMD,CS1); // Set Multiplex Ratio
	OLED_WR_Byte(0x3F,OLED_DATA,CS1); // 1/64 Duty (0x0F~0x5F)
   
	OLED_WR_Byte(0xA2,OLED_CMD,CS1); // Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_DATA,CS1); //	
	
	OLED_WR_Byte(0xA1,OLED_CMD,CS1); // Set Mapping RAM Display Start Line (0x00~0x7F)
	OLED_WR_Byte(0x00,OLED_DATA,CS1); //	

	
	OLED_WR_Byte(0xA0,OLED_CMD,CS1); //Set Column Address 0 Mapped to SEG0 
	OLED_WR_Byte(0x04,OLED_DATA,CS1);      	//   Default => 0x40
						                //     Horizontal Address Increment
						//     Column Address 0 Mapped to SEG0
						//     Disable Nibble Remap
						//     Scan from COM0 to COM[N-1]
						//     Disable COM Split Odd Even
	OLED_WR_Byte(0x11,OLED_DATA,CS1);      	//    Default => 0x01 (Disable Dual COM Mode)

	
	OLED_WR_Byte(0xB5,OLED_CMD,CS1); //  Disable GPIO Pins Input
	OLED_WR_Byte(0x00,OLED_DATA,CS1); //	
	
    OLED_WR_Byte(0xAB,OLED_CMD,CS1); //   Enable Internal VDD Regulator
	OLED_WR_Byte(0x01,OLED_DATA,CS1); //

	OLED_WR_Byte(0xB4,OLED_CMD,CS1); //  Display Enhancement  
	OLED_WR_Byte(0xA0,OLED_DATA,CS1); // Enable External VSL
	OLED_WR_Byte(0xFD,OLED_DATA,CS1); // Enhance Low Gray Scale Display Quality

    OLED_WR_Byte(0xC1,OLED_CMD,CS1); //  Set Contrast Current 
	OLED_WR_Byte(0xEF,OLED_DATA,CS1); //  Default => 0x7F

    OLED_WR_Byte(0xC7,OLED_CMD,CS1); //  Master Contrast Current Control 
	OLED_WR_Byte(Brightness,OLED_DATA,CS1); //  Default => 0x0f (Maximum)

    Set_Gray_Scale_Table(CS1);		   // Set Gray Scale Table

	OLED_WR_Byte(0xB1,OLED_CMD,CS1); //   Set Phase 1 as 5 Clocks & Phase 2 as 14 Clocks
	OLED_WR_Byte(0xE2,OLED_DATA,CS1); //  Default => 0x74 (7 Display Clocks [Phase 2] / 9 Display Clocks [Phase 1])

    OLED_WR_Byte(0xD1,OLED_CMD,CS1); //    Enhance Driving Scheme Capability (0x00/0x20)
	OLED_WR_Byte(0xA2,OLED_DATA,CS1); //   Normal
	OLED_WR_Byte(0x20,OLED_DATA,CS1); //

   
    OLED_WR_Byte(0xBB,OLED_CMD,CS1); //     Set Pre-Charge Voltage Level as 0.60*VCC
	OLED_WR_Byte(0x1F,OLED_DATA,CS1); //

    OLED_WR_Byte(0xB6,OLED_CMD,CS1); //  // Set Second Pre-Charge Period as 8 Clocks 
	OLED_WR_Byte(0x08,OLED_DATA,CS1); //

    OLED_WR_Byte(0xBE,OLED_CMD,CS1); //  Set Common Pins Deselect Voltage Level as 0.86*VCC 
	OLED_WR_Byte(0x07,OLED_DATA,CS1); //  Default => 0x04 (0.80*VCC)

    OLED_WR_Byte(0xA6,OLED_CMD,CS1); //     Normal Display Mode (0x00/0x01/0x02/0x03)
	 								  //   Default => 0xA4
						//     0xA4  => Entire Display Off, All Pixels Turn Off
						//     0xA5  => Entire Display On, All Pixels Turn On at GS Level 15
						//     0xA6  => Normal Display
						//     0xA7  => Inverse Display
	 
	 OLED_WR_Byte(0xA9,OLED_CMD,CS1); //   Disable Partial Display
     	 	
	

		//CS2 ��ʼ��
	OLED_WR_Byte(0xAE,OLED_CMD,CS2); //	Display Off

	OLED_WR_Byte(0xFD,OLED_CMD,CS2); // Set Command Lock
	OLED_WR_Byte(0x12,OLED_DATA,CS2); //
	
	OLED_WR_Byte(0xB3,OLED_CMD,CS2); // Set Clock as 80 Frames/Sec
	OLED_WR_Byte(0x91,OLED_DATA,CS2); //  

   	OLED_WR_Byte(0xCA,OLED_CMD,CS2); // Set Multiplex Ratio
	OLED_WR_Byte(0x3F,OLED_DATA,CS2); // 1/64 Duty (0x0F~0x5F)
   
	OLED_WR_Byte(0xA2,OLED_CMD,CS2); // Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_DATA,CS2); //	
	
	OLED_WR_Byte(0xA1,OLED_CMD,CS2); // Set Mapping RAM Display Start Line (0x00~0x7F)
	OLED_WR_Byte(0x00,OLED_DATA,CS2); //	

	
	OLED_WR_Byte(0xA0,OLED_CMD,CS2); //Set Column Address 0 Mapped to SEG0 
	OLED_WR_Byte(0x16,OLED_DATA,CS2);      	//   Default => 0x40
						                //     Horizontal Address Increment
						//     Column Address 0 Mapped to SEG0
						//     Disable Nibble Remap
						//     Scan from COM0 to COM[N-1]
						//     Disable COM Split Odd Even
	OLED_WR_Byte(0x11,OLED_DATA,CS2);      	//    Default => 0x01 (Disable Dual COM Mode)

	
	OLED_WR_Byte(0xB5,OLED_CMD,CS2); //  Disable GPIO Pins Input
	OLED_WR_Byte(0x00,OLED_DATA,CS2); //	
	
    OLED_WR_Byte(0xAB,OLED_CMD,CS2); //   Enable Internal VDD Regulator
	OLED_WR_Byte(0x01,OLED_DATA,CS2); //

	OLED_WR_Byte(0xB4,OLED_CMD,CS2); //  Display Enhancement  
	OLED_WR_Byte(0xA0,OLED_DATA,CS2); // Enable External VSL
	OLED_WR_Byte(0xFD,OLED_DATA,CS2); // Enhance Low Gray Scale Display Quality

    OLED_WR_Byte(0xC1,OLED_CMD,CS2); //  Set Contrast Current 
	OLED_WR_Byte(0xEF,OLED_DATA,CS2); //  Default => 0x7F

    OLED_WR_Byte(0xC7,OLED_CMD,CS2); //  Master Contrast Current Control 
	OLED_WR_Byte(Brightness,OLED_DATA,CS2); //  Default => 0x0f (Maximum)

	Set_Gray_Scale_Table(CS2);		   // Set Gray Scale Table

	OLED_WR_Byte(0xB1,OLED_CMD,CS2); //   Set Phase 1 as 5 Clocks & Phase 2 as 14 Clocks
	OLED_WR_Byte(0xE2,OLED_DATA,CS2); //  Default => 0x74 (7 Display Clocks [Phase 2] / 9 Display Clocks [Phase 1])

    OLED_WR_Byte(0xD1,OLED_CMD,CS2); //    Enhance Driving Scheme Capability (0x00/0x20)
	OLED_WR_Byte(0xA2,OLED_DATA,CS2); //   Normal
	OLED_WR_Byte(0x20,OLED_DATA,CS2); //

   
    OLED_WR_Byte(0xBB,OLED_CMD,CS2); //     Set Pre-Charge Voltage Level as 0.60*VCC
	OLED_WR_Byte(0x1F,OLED_DATA,CS2); //

    OLED_WR_Byte(0xB6,OLED_CMD,CS2); //  // Set Second Pre-Charge Period as 8 Clocks 
	OLED_WR_Byte(0x08,OLED_DATA,CS2); //

    OLED_WR_Byte(0xBE,OLED_CMD,CS2); //  Set Common Pins Deselect Voltage Level as 0.86*VCC 
	OLED_WR_Byte(0x07,OLED_DATA,CS2); //  Default => 0x04 (0.80*VCC)

    OLED_WR_Byte(0xA6,OLED_CMD,CS2); //     Normal Display Mode (0x00/0x01/0x02/0x03)
	 								  //   Default => 0xA4
						//     0xA4  => Entire Display Off, All Pixels Turn Off
						//     0xA5  => Entire Display On, All Pixels Turn On at GS Level 15
						//     0xA6  => Normal Display
						//     0xA7  => Inverse Display
	 
	 OLED_WR_Byte(0xA9,OLED_CMD,CS2); //   Disable Partial Display
	
	
	Fill_RAM(0x00,CS1);
	Fill_RAM(0x00,CS2);
	OLED_WR_Byte(0xAF,OLED_CMD,CS2); //	Display On
	OLED_WR_Byte(0xAF,OLED_CMD,CS1); //	Display On
 
	
}  

void Set_Column_Address(unsigned char a, unsigned char b ,unsigned char cs)
{
	OLED_WR_Byte(0x15,OLED_CMD,cs);			// Set Column Address
	OLED_WR_Byte(a,OLED_DATA,cs);			//   Default => 0x00
	OLED_WR_Byte(b,OLED_DATA,cs);			//   Default => 0x77
}


void Set_Row_Address(unsigned char a, unsigned char b, unsigned char cs)
{
	OLED_WR_Byte(0x75,OLED_CMD,cs);			// Set Row Address
	OLED_WR_Byte(a,OLED_DATA,cs);			//   Default => 0x00
	OLED_WR_Byte(b,OLED_DATA,cs);			//   Default => 0x7F
}

void Set_Write_RAM(unsigned char cs)
{
 
	OLED_WR_Byte(0x5C,OLED_CMD,cs);			// Enable MCU to Write into RAM
}


void Set_Read_RAM(unsigned char cs)
{
	OLED_WR_Byte(0x5D,OLED_CMD,cs);			// Enable MCU to Read from RAM
}



 void Set_Remap_Format(unsigned char d ,unsigned char cs)
{
	 OLED_WR_Byte(0xA0,OLED_CMD,cs);  			// Set Re-Map / Dual COM Line Mode
  OLED_WR_Byte(d,OLED_DATA,cs);  				//   Default => 0x40
						//     Horizontal Address Increment
						//     Column Address 0 Mapped to SEG0
						//     Disable Nibble Remap
						//     Scan from COM0 to COM[N-1]
						//     Disable COM Split Odd Even
    OLED_WR_Byte(0x11,OLED_DATA,cs); 	 			//   Default => 0x01 (Disable Dual COM Mode)
}


 void Set_Partial_Display(unsigned char a, unsigned char b, unsigned char c,unsigned char cs)
{
 
	OLED_WR_Byte((0xA8|a),OLED_CMD,cs);
						// Default => 0x8F
						//   Select Internal Booster at Display On
	if(a == 0x00)
	{
	    OLED_WR_Byte(b,OLED_DATA,cs); 
		OLED_WR_Byte(c,OLED_DATA,cs); 
	 
	}
}

void Set_Display_Offset(unsigned char d,unsigned char cs)
{
	OLED_WR_Byte(0xA2,OLED_CMD,cs);
 	OLED_WR_Byte(d,OLED_DATA,cs);
 
}

void Set_Start_Line(unsigned char d,unsigned char cs)
{
	OLED_WR_Byte(0xA1,OLED_CMD,cs);	// Set Vertical Scroll by RAM
 	OLED_WR_Byte(d,OLED_DATA,cs);
 
}

void Set_Master_Current(unsigned char d,unsigned char cs)
{
   OLED_WR_Byte(0xC7,OLED_CMD,cs);	//  Master Contrast Current Control
 	OLED_WR_Byte(d,OLED_DATA,cs);

}

void Set_Linear_Gray_Scale_Table(unsigned char cs)
{
   	OLED_WR_Byte(0xB9,OLED_CMD,cs); //	Set Default Linear Gray Scale Table
 
}
  

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Gray Scale Table Setting  
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Set_Gray_Scale_Table(unsigned char cs)
{

    OLED_WR_Byte(0xB8,OLED_CMD,cs); //  	// Set Gray Scale Table 
	OLED_WR_Byte(0x0C,OLED_DATA,cs); //
	OLED_WR_Byte(0x18,OLED_DATA,cs); //
	OLED_WR_Byte(0x24,OLED_DATA,cs); //
	OLED_WR_Byte(0x30,OLED_DATA,cs); //
	OLED_WR_Byte(0x3C,OLED_DATA,cs); //
	OLED_WR_Byte(0x48,OLED_DATA,cs); //
	OLED_WR_Byte(0x54,OLED_DATA,cs); //
	OLED_WR_Byte(0x60,OLED_DATA,cs); //
	OLED_WR_Byte(0x6C,OLED_DATA,cs); //
	OLED_WR_Byte(0x78,OLED_DATA,cs); //
	OLED_WR_Byte(0x84,OLED_DATA,cs); //
	OLED_WR_Byte(0x90,OLED_DATA,cs); //
	OLED_WR_Byte(0x9C,OLED_DATA,cs); //
	OLED_WR_Byte(0xA8,OLED_DATA,cs); //
	OLED_WR_Byte(0xB4,OLED_DATA,cs); //
 	OLED_WR_Byte(0x00,OLED_CMD,cs); 	// Enable Gray Scale Table

}
 
//��������,������,������Ļ�Ǻڵ�	  
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (��������)
 
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_RAM(unsigned char Data,unsigned char cs)
{
unsigned char i,j;

	Set_Column_Address(0x00,0x77,cs);
	Set_Row_Address(0x00,0x7F,cs);
	Set_Write_RAM(cs);

	for(i=0;i<128;i++)
	{
		for(j=0;j<120;j++)
		{
			OLED_WR_Byte(Data,OLED_DATA,cs);
		    OLED_WR_Byte(Data,OLED_DATA,cs); 
 
		}
	}
}
 
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//   ȫ�����߲�����䣬����Dataֵ�����Ҷȱ���ʾ��0x00Ϊ������0xffΪ����
//
//    a: Column Address of Start
//    b: Column Address of End (Total Columns Devided by 4)
//    c: Row Address of Start
//    d: Row Address of End
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_Block(unsigned char Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d )
{
unsigned char i,j;
 
	if (d <= 63 )	   //c<=d
	{
		Set_Column_Address(Shift+a,Shift+b,CS1);
		Set_Row_Address(c,d,CS1);
		Set_Write_RAM(CS1);

		for(i=0;i<(d-c+1);i++)
		{
			for(j=0;j<(b-a+1);j++)
			{
				OLED_WR_Byte(Data,OLED_DATA,CS1);
			    OLED_WR_Byte(Data,OLED_DATA,CS1); 
			}
		}
	 }

	else 
		if ( c<=63 )  // d>=64
			{
			Set_Column_Address(Shift+a,Shift+b,CS1);
			Set_Row_Address(c,63,CS1);
			Set_Write_RAM(CS1);
	
			for(i=0;i<(63-c+1);i++)
			{
				for(j=0;j<(b-a+1);j++)
				{
				OLED_WR_Byte(Data,OLED_DATA,CS1);
			    OLED_WR_Byte(Data,OLED_DATA,CS1);
						}
			}
			Set_Column_Address(Shift+a,Shift+b,CS2);
			Set_Row_Address(0,(d-63),CS2);
			Set_Write_RAM(CS2);
		
			for(i=0;i<(d-63+1);i++)
			{  
			  for(j=0;j<(b-a+1);j++)
				{
				OLED_WR_Byte(Data,OLED_DATA,CS2);
			    OLED_WR_Byte(Data,OLED_DATA,CS2);
							}
			}

		 }
	 else //	�м�λ��
	   	 	Set_Column_Address(Shift+a,Shift+b,CS2);
			Set_Row_Address((c-64),(d-64),CS2);
			Set_Write_RAM(CS2);
				for(i=0;i<(d-c+1);i++)
			{
				for(j=0;j<(b-a+1);j++)
				{
				OLED_WR_Byte(Data,OLED_DATA,CS2);
		    	OLED_WR_Byte(Data,OLED_DATA,CS2);
				}
		   	}
}
 
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Gray Scale Bar (����)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Grayscale(unsigned char cs)
{
	
	 if(cs==1) //�ϰ���
	  {
	// Level 16 => Column 1~16
		Fill_Block(0xFF,0x00,0x03,0x00,0x3F);
	// Level 15 => Column 17~32
		Fill_Block(0xEE,0x04,0x07,0x00,0x3F);
   	// Level 14 => Column 33~48
		Fill_Block(0xDD,0x08,0x0B,0x00,0x3F);
   	// Level 13 => Column 49~64
		Fill_Block(0xCC,0x0C,0x0F,0x00,0x3F);
	// Level 12 => Column 65~80
		Fill_Block(0xBB,0x10,0x13,0x00,0x3F);
   	// Level 11 => Column 81~96
		Fill_Block(0xAA,0x14,0x17,0x00,0x3F);
	// Level 10 => Column 97~112
		Fill_Block(0x99,0x18,0x1B,0x00,0x3F);
	// Level 9 => Column 113~128
		Fill_Block(0x88,0x1C,0x1F,0x00,0x3F);
	// Level 8 => Column 129~144
		Fill_Block(0x77,0x20,0x23,0x00,0x3F);
	// Level 7 => Column 145~160
		Fill_Block(0x66,0x24,0x27,0x00,0x3F);
	// Level 6 => Column 161~176
		Fill_Block(0x55,0x28,0x2B,0x00,0x3F);
	// Level 5 => Column 177~192
		Fill_Block(0x44,0x2C,0x2F,0x00,0x3F);
	// Level 4 => Column 193~208
		Fill_Block(0x33,0x30,0x33,0x00,0x3F);
	// Level 3 => Column 209~224
		Fill_Block(0x22,0x34,0x37,0x00,0x3F);
	// Level 2 => Column 225~240
		Fill_Block(0x11,0x38,0x3B,0x00,0x3F);
	// Level 1 => Column 241~256
		Fill_Block(0x00,0x3C,Max_Column,0x00,0x3F);
}

	 if(cs==2) //�°���
	  {
	// Level 16 => Column 1~16
		Fill_Block(0xFF,0x00,0x03,0x40,0x7F);
	// Level 15 => Column 17~32
		Fill_Block(0xEE,0x04,0x07,0x40,0x7F);
	// Level 14 => Column 33~48
		Fill_Block(0xDD,0x08,0x0B,0x40,0x7F);
	// Level 13 => Column 49~64
		Fill_Block(0xCC,0x0C,0x0F,0x40,0x7F);
	// Level 12 => Column 65~80
		Fill_Block(0xBB,0x10,0x13,0x40,0x7F);
	// Level 11 => Column 81~96
		Fill_Block(0xAA,0x14,0x17,0x40,0x7F);
	// Level 10 => Column 97~112
		Fill_Block(0x99,0x18,0x1B,0x40,0x7F);
	// Level 9 => Column 113~128
		Fill_Block(0x88,0x1C,0x1F,0x40,0x7F);
	// Level 8 => Column 129~144
		Fill_Block(0x77,0x20,0x23,0x40,0x7F);
	// Level 7 => Column 145~160
		Fill_Block(0x66,0x24,0x27,0x40,0x7F);
	// Level 6 => Column 161~176
		Fill_Block(0x55,0x28,0x2B,0x40,0x7F);
	// Level 5 => Column 177~192
		Fill_Block(0x44,0x2C,0x2F,0x40,0x7F);
	// Level 4 => Column 193~208
		Fill_Block(0x33,0x30,0x33,0x40,0x7F);
	// Level 3 => Column 209~224
		Fill_Block(0x22,0x34,0x37,0x40,0x7F);
	// Level 2 => Column 225~240
		Fill_Block(0x11,0x38,0x3B,0x40,0x7F);
	// Level 1 => Column 241~256
		Fill_Block(0x00,0x3C,Max_Column,0x40,0x7F);
}
	}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Pattern (Partial or Full Screen)
//
//    a: Column Address of Start
//    b: Column Address of End (Total Columns Devided by 4)
//    c: Row Address of Start
//    d: Row Address of End
// �Ҷ�ģʽ����ʾһ��ͼƬ
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Pattern(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
			 
unsigned char *Src_Pointer;
unsigned char i,j;
 
  //ȡģʱ����������	�����ܷ�����2.7��ͬ��
    Src_Pointer=Data_Pointer;
	
	if (d <= 63 )	   //c<=d
		   	 {
			   	Set_Column_Address(Shift+a,Shift+b,CS1);
				Set_Row_Address(c,d,CS1);
				Set_Write_RAM(CS1);
			
				for(i=0;i<(d-c+1);i++)
				{
					for(j=0;j<(b-a+1);j++)
					{
				    OLED_WR_Byte(*Src_Pointer,OLED_DATA,CS1);
					Src_Pointer++;
					OLED_WR_Byte(*Src_Pointer,OLED_DATA,CS1);
					Src_Pointer++;
					}
				}
			}
	else 
	if ( c<=63 )  // d>=64
			 {
			   	Set_Column_Address(Shift+a,Shift+b,CS1);
				Set_Row_Address(c,63,CS1);
				Set_Write_RAM(CS1);
			
				for(i=0;i<(63-c+1);i++)
				{
					for(j=0;j<(b-a+1);j++)
					{
					OLED_WR_Byte(*Src_Pointer,OLED_DATA,CS1);
					Src_Pointer++;
					OLED_WR_Byte(*Src_Pointer,OLED_DATA,CS1);
					Src_Pointer++;
					}
			   	}

			   Set_Column_Address(Shift+a,Shift+b,CS2);
				Set_Row_Address(0,(d-63),CS2);
				Set_Write_RAM(CS2);
			
				for(i=0;i<(d-63+1);i++)
				{
					for(j=0;j<(b-a+1);j++)
					{
     				OLED_WR_Byte(*Src_Pointer,OLED_DATA,CS2);
					Src_Pointer++;
					OLED_WR_Byte(*Src_Pointer,OLED_DATA,CS2);
					Src_Pointer++;
					}
			   	}

		   	}  
	   else  // ���м�
		{
	     	Set_Column_Address(Shift+a,Shift+b,CS2);
			Set_Row_Address((c-63),(d-63),CS2);
			Set_Write_RAM(CS2);
		
			for(i=0;i<(d-c+1);i++)
			{
				for(j=0;j<(b-a+1);j++)
				{
     				OLED_WR_Byte(*Src_Pointer,OLED_DATA,CS2);
					Src_Pointer++;
					OLED_WR_Byte(*Src_Pointer,OLED_DATA,CS2);
					Src_Pointer++;
				}
		   	}
		  }
	

}
 

/**************************************
  ����ת�����򣺽�2λ�ֳ�1���ֽڴ����Դ棬����1��seg��ʾ4��������Ҫͬʱд2���ֽڼ�4������
  uchar DATA��ȡģ������ģ����
****************************************/
void Con_4_byte(unsigned char DATA,unsigned char cs)
{
   unsigned char d1_4byte[4],d2_4byte[4];
   unsigned char i;
   unsigned char d,k1,k2;
   d=DATA;
 
  for(i=0;i<2;i++)   // һ��λ�ķ�ʽд��  2*4=8λ
   {
     k1=d&0xc0;     //��i=0ʱ ΪD7,D6λ ��i=1ʱ ΪD5,D4λ

     /****��4�ֿ��ܣ�16���Ҷ�,һ���ֽ����ݱ�ʾ�������أ�һ�����ض�Ӧһ���ֽڵ�4λ***/

     switch(k1)
       {
	 case 0x00:
           d1_4byte[i]=0x00;
		   
         break;
     case 0x40:  // 0100,0000
           d1_4byte[i]=0x0f;
		   
         break;	
	 case 0x80:  //1000,0000
           d1_4byte[i]=0xf0;
		   
         break;
     case 0xc0:   //1100,0000
           d1_4byte[i]=0xff;
		  
         break;	 
     default:
      	 break;
	   }
     
	   d=d<<2;
	  k2=d&0xc0;     //��i=0ʱ ΪD7,D6λ ��i=1ʱ ΪD5,D4λ

     /****��4�ֿ��ܣ�16���Ҷ�,һ���ֽ����ݱ�ʾ�������أ�һ�����ض�Ӧһ���ֽڵ�4λ***/

     switch(k2)
       {
	 case 0x00:
           d2_4byte[i]=0x00;
		   
         break;
     case 0x40:  // 0100,0000
           d2_4byte[i]=0x0f;
		   
         break;	
	 case 0x80:  //1000,0000
           d2_4byte[i]=0xf0;
		 
         break;
     case 0xc0:   //1100,0000
           d2_4byte[i]=0xff;
		  
         break;	 
     default:
      	 break;
	   }
	  
	  d=d<<2;                                //������λ
      
	 OLED_WR_Byte(d1_4byte[i],OLED_DATA,cs);	    //дǰ2��
	 OLED_WR_Byte(d2_4byte[i],OLED_DATA,cs);               //д��2��	  ����4��
   }

}


/*======================================================================*/
/*======================������ʾ�Ӻ�������=====================================*/
/*======================================================================*/

/***************************************************************
//  ��ʾ12*12������ 2015-05���²���ͨ��
//  ȡģ��ʽΪ������ȡģ���λ,��������:�����Ҵ��ϵ���    16�� 12�� 
//   num���������ֿ��е�λ��
//   x: Start Column  ��ʼ�� ��Χ 0~��256-16��
//   y: Start Row   ��ʼ�� 0~127-12 
***************************************************************/
void HZ12_12( unsigned char x, unsigned char y, unsigned char num)
{
	unsigned int j;
	unsigned char x1;
	x1=x/4; 
	if (y <=52)	//63-11
	{
	Set_Column_Address(Shift+x1,Shift+x1+3,CS1); // ���������꣬shiftΪ��ƫ������1322������3Ϊ16/4-1
	Set_Row_Address(y,y+11,CS1);  //
	Set_Write_RAM(CS1);	 //	д�Դ�
	for(j=0;j<24;j++)
    	{
		Con_4_byte(HZ12X12_S[num*24+j],CS1);
				}
  	 }
	 else
	 	if (y >=64 )
		 	{
			Set_Column_Address(Shift+x1,Shift+x1+3,CS2); // ���������꣬shiftΪ��ƫ������1322������3Ϊ16/4-1
			Set_Row_Address((y-63),(y-63+11),CS2);  //
			Set_Write_RAM(CS2);	 //	д�Դ�
	 	    for(j=0;j<24;j++)
        	{
	         	Con_4_byte(HZ12X12_S[num*24+j],CS2);
			    	}
  			}		   			 			  
			 
		  else	 //�����м�λ��
		  {	//�����ϰ�������
			Set_Column_Address(Shift+x1,Shift+x1+3,CS1); // ���������꣬shiftΪ��ƫ������1322������3Ϊ16/4-1
			Set_Row_Address(y,63,CS1);  //
			Set_Write_RAM(CS1);	 //	дCS1�Դ�
		 	for(j=0;j<(64-y)*2;j++)
		    {
			  	Con_4_byte(HZ12X12_S[num*24+j],CS1);
					 }
			//�����°�������
			Set_Column_Address(Shift+x1,Shift+x1+3,CS2); // ���������꣬shiftΪ��ƫ������1322������3Ϊ16/4-1
			Set_Row_Address(0,(y+11-64),CS2);  //
			Set_Write_RAM(CS2);	 //	дCS2�Դ�
		   	for(j=(64-y)*2;j<24;j++)
			 {
			   	Con_4_byte(HZ12X12_S[num*24+j],CS2);
					 }
			}
	 }
 
//****************************************************
//   д��һ��12*12���� www.lcdsoc.com	x������ּ��dҪΪ4�ı���
//    num1,num2���������ֿ��е�λ��	 ��num1��ʾ��num2
//    x: Start Column  ��ʼ�� ��Χ 0~��255-16��
//    y: Start Row    ��ʼ�� 0~127-16
//    d:�ּ���� 0Ϊ�޼�� 
//*****************************************************
void Show_HZ12_12(unsigned char  x,unsigned char  y, unsigned char  d,unsigned char num1,unsigned char num2)
{
  unsigned char  i,d1;
  d1=d+16;
  for(i=num1;i<num2+1;i++)
  {
  if(x>(239-d)) {x=0;y=y+12;}  //����
  HZ12_12(x,y,i);
  x=x+d1;             
  }
}
  

//****************************************************
//   д��һ��16*16���� www.lcdsoc.com
//    num1,num2���������ֿ��е�λ��	 ��num1��ʾ��num2
//    x: Start Column  ��ʼ�� ��Χ 0~��255-16��
//    y: Start Row    ��ʼ�� 0~127
//    d:�ּ���� 0Ϊ�޼�� 
//    x������ּ��dҪΪ4�ı���
//*****************************************************
void Show_HZ16_16(unsigned char  x,unsigned char  y, unsigned char  d,unsigned char num1,unsigned char num2)
{
  unsigned char  i,d1;
  d1=d+16;
  for(i=num1;i<num2+1;i++)
  {
  if(x>(239-d)) {x=0;y=y+16;}  //����
  HZ16_16(x,y,i);
  x=x+d1;             
  }
}
 
/***************************************************************
//  ��ʾ16*16������ 2015-05���²���ͨ��
//  ȡģ��ʽΪ������ȡģ���λ,��������:�����Ҵ��ϵ���    16�� 16�� 
//   num���������ֿ��е�λ��
//   x: Start Column  ��ʼ�� ��Χ 0~��256-16��
//   y: Start Row   ��ʼ�� 0~127-16
***************************************************************/
void HZ16_16( unsigned char x, unsigned char y, unsigned char num)
{
    

	unsigned int j;
	unsigned char x1;
	x1=x/4; 
	if (y <=48)	//63-15
	{
	Set_Column_Address(Shift+x1,Shift+x1+3,CS1); // ���������꣬shiftΪ��ƫ������1322������3Ϊ16/4-1
	Set_Row_Address(y,y+15,CS1);  //
	Set_Write_RAM(CS1);	 //	д�Դ�
	for(j=0;j<32;j++)
    	{
		 Con_4_byte(HZ16X16_S[num*32+j],CS1);
				}
  
	 }
	 else
	 	if (y >=64 )
		 	{
			Set_Column_Address(Shift+x1,Shift+x1+3,CS2); // ���������꣬shiftΪ��ƫ������1322������3Ϊ16/4-1
			Set_Row_Address((y-63),(y-63+15),CS2);  //
			Set_Write_RAM(CS2);	 //	д�Դ�
		    for(j=0;j<32;j++)
	        {
		      Con_4_byte(HZ16X16_S[num*32+j],CS2);
				
					}
  			 			  
			 }
		  else	 //�����м�λ��
		  {	//�����ϰ�������
			Set_Column_Address(Shift+x1,Shift+x1+3,CS1); // ���������꣬shiftΪ��ƫ������1322������3Ϊ16/4-1
			Set_Row_Address(y,63,CS1);  //
			Set_Write_RAM(CS1);	 //	дCS1�Դ�
		 	for(j=0;j<(64-y)*2;j++)
		    {
			    Con_4_byte(HZ16X16_S[num*32+j],CS1);
					 }
			//�����°�������
			Set_Column_Address(Shift+x1,Shift+x1+3,CS2); // ���������꣬shiftΪ��ƫ������1322������3Ϊ16/4-1
			Set_Row_Address(0,(y+15-64),CS2);  //
			Set_Write_RAM(CS2);	 //	дCS2�Դ�
		   	for(j=(64-y)*2;j<32;j++)
			 {
			    Con_4_byte(HZ16X16_S[num*32+j],CS2);
					 }
						 
			 }
	

	 }
 
 /***************************************************************
//  ��ʾ24*24������ 2015-05���²���ͨ��
//  ȡģ��ʽΪ������ȡģ���λ,��������:�����Ҵ��ϵ���    24�� 24�� 
//   num���������ֿ��е�λ��
//   x: Start Column  ��ʼ�� ��Χ 0~��255-16��
//   y: Start Row   ��ʼ�� 0~127-24
***************************************************************/
void HZ24_24( unsigned char x, unsigned char y, unsigned char num)
{
   	unsigned int j;
	unsigned char x1;
	x1=x/4; 
   
	if (y <=40)	//63-23
	{
	Set_Column_Address(Shift+x1,Shift+x1+5,CS1); // ���������꣬shiftΪ��ƫ������1322������5Ϊ24/4-1
	Set_Row_Address(y,y+23,CS1);  //
	Set_Write_RAM(CS1);	 //	д�Դ�CS1
	for(j=0;j<72;j++)
    	{
		 Con_4_byte(HZ24X24_S[num*72+j],CS1);	//����24*24 ������������Ҫ����24*24��HZ24X24_K.h�� ,����24*24��HZ24X24_H.h��������Ӧ��ͷ�ļ����޸Ĵ˴�������
				}
  
	 }
	 else
	 	if (y >=64 )
		 	{
			Set_Column_Address(Shift+x1,Shift+x1+5,CS2); // ���������꣬shiftΪ��ƫ������1322������5Ϊ24/4-1
			Set_Row_Address((y-63),(y-63+23),CS2);  //
			Set_Write_RAM(CS2);	 //	д�Դ�CS2
		    for(j=0;j<72;j++)
	        {
		      Con_4_byte(HZ24X24_S[num*72+j],CS2);	//����24*24 ������������Ҫ����24*24��HZ24X24_K.h�� ,����24*24��HZ24X24_H.h��������Ӧ��ͷ�ļ����޸Ĵ˴�������
				
					}
  			 			  
			 }
		  else	 //�����м�λ��
		  {	//�����ϰ�������
			Set_Column_Address(Shift+x1,Shift+x1+5,CS1); // ���������꣬shiftΪ��ƫ������1322������5Ϊ24/4-1
			Set_Row_Address(y,63,CS1);  //
			Set_Write_RAM(CS1);	 //	дCS1�Դ�
		 	for(j=0;j<(64-y)*3;j++)
		    {
			    Con_4_byte(HZ24X24_S[num*72+j],CS1);	//����24*24 ������������Ҫ����24*24��HZ24X24_K.h�� ,����24*24��HZ24X24_H.h��������Ӧ��ͷ�ļ����޸Ĵ˴�������
								 }
			//�����°�������
			Set_Column_Address(Shift+x1,Shift+x1+5,CS2); // ���������꣬shiftΪ��ƫ������1322������5Ϊ24/4-1
			Set_Row_Address(0,(y+23-64),CS2);  //
			Set_Write_RAM(CS2);	 //	дCS2�Դ�
		   	for(j=(64-y)*3;j<72;j++)
			 {
			    Con_4_byte(HZ24X24_S[num*72+j],CS2);	//����24*24 ������������Ҫ����24*24��HZ24X24_K.h�� ,����24*24��HZ24X24_H.h��������Ӧ��ͷ�ļ����޸Ĵ˴�������
					 }
						 
			 }
	 }
 
//****************************************************
//   д��һ��24*24���� www.lcdsoc.com
//    num1,num2���������ֿ��е�λ��	 ��num1��ʾ��num2
//    x: Start Column  ��ʼ�� ��Χ 0~��255-16��
//    y: Start Row    ��ʼ�� 0~127-24
//    d:�ּ���� 0Ϊ�޼�� 
//    x������ּ��dҪΪ4�ı���
//*****************************************************
void Show_HZ24_24(unsigned char  x,unsigned char  y, unsigned char  d,unsigned char num1,unsigned char num2)
{
  unsigned char  i,d1;
  d1=d+24;
  for(i=num1;i<num2+1;i++)
  {
  	if(x>(231-d)) {x=0;y=y+24;}  //����
  HZ24_24(x,y,i);
  x=x+d1;             
  }
}

/*======================================================================*/
/*======================ASCII�Ӻ�������=====================================*/
/*======================================================================*/
 
 //==============================================================
//����������д��һ���׼ASCII�ַ���	 5x8  ʵ�ʵ���8*8
//��������ʾ��λ�ã�x,y����ch[]Ҫ��ʾ���ַ���
//���أ���
//==============================================================  
void Asc5_8(unsigned char x,unsigned char y,unsigned char ch[])
{
  unsigned char x1,c=0, i=0,j;      
  while (ch[i]!='\0')
  {    
    x1=x/4;
	c =ch[i]-32;
	i++;  //��һ���ַ�
    	
	if(x1>61)  
	   {x=0;
	   x1=x/4;
	   y=y+8;
	   }  //����		  
			 
	if (y <=48)	//63-15
		{
		Set_Column_Address(Shift+x1,Shift+x1+1,CS1); // ���������꣬shiftΪ��ƫ������1322������1Ϊ8/4-1
		Set_Row_Address(y,y+15,CS1);  //
		Set_Write_RAM(CS1);	 //	д�Դ�
		for(j=0;j<8;j++)
	    	{
			  Con_4_byte(ASC5X8[c*8+j],CS1);	//����ת��
		 		}
		 } 
	else
	 	if (y >=64 )
		 {
			Set_Column_Address(Shift+x1,Shift+x1+1,CS2); // ���������꣬shiftΪ��ƫ������1322������1Ϊ8/4-1
			Set_Row_Address((y-63),(y-63+7),CS2);  //
			Set_Write_RAM(CS2);	 //	д�Դ�
			for(j=0;j<8;j++)
             {
		        Con_4_byte(ASC5X8[c*8+j],CS2);	//����ת��
	 	       	}
        	 }

		  else	 //�����м�λ��
		  {	//�����ϰ�������
			Set_Column_Address(Shift+x1,Shift+x1+1,CS1); // ���������꣬shiftΪ��ƫ������1322������1Ϊ8/4-1
			Set_Row_Address(y,63,CS1);  //
			Set_Write_RAM(CS1);	 //	дCS1�Դ�
		 	for(j=0;j<(64-y);j++)
		    {
			    Con_4_byte(ASC5X8[c*8+j],CS1);	//����ת��
					 }
			//�����°�������
			Set_Column_Address(Shift+x1,Shift+x1+1,CS2); // ���������꣬shiftΪ��ƫ������1322������1Ϊ8/4-1
			Set_Row_Address(0,(y+7-64),CS2);  //
			Set_Write_RAM(CS2);	 //	дCS2�Դ�
		   	for(j=(64-y);j<8;j++)
			 {
			     Con_4_byte(ASC5X8[c*8+j],CS2);	//����ת��
					 }
						 
			 }
			x=x+8;	//�¸��ַ�x����
				 }  
  	}

 //==============================================================
//����������д��һ���׼ASCII�ַ���	 6x12 ʵ�ʵ���8*12
//��������ʾ��λ�ã�x,y����ch[]Ҫ��ʾ���ַ���
//���أ���
//==============================================================  
void Asc6_12(unsigned char x,unsigned char y,unsigned char ch[])
{
  unsigned char x1,c=0, i=0,j;      
  while (ch[i]!='\0')
  {    
    x1=x/4;
	c =ch[i]-32;
	i++;  //��һ���ַ�
    	
	if(x1>61)  
	   {x=0;
	   x1=x/4;
	   y=y+12;
	   }  //����		  
			 
	if (y <=48)	//63-15
		{
		Set_Column_Address(Shift+x1,Shift+x1+1,CS1); // ���������꣬shiftΪ��ƫ������1322������1Ϊ8/4-1
		Set_Row_Address(y,y+11,CS1);  //
		Set_Write_RAM(CS1);	 //	д�Դ�
		for(j=0;j<12;j++)
	    	{
			  Con_4_byte(ASC6X12[c*12+j],CS1);	//����ת��
		 		}
		 } 
	else
	 	if (y >=64 )
		 {
			Set_Column_Address(Shift+x1,Shift+x1+1,CS2); // ���������꣬shiftΪ��ƫ������1322������1Ϊ8/4-1
			Set_Row_Address((y-63),(y-63+11),CS2);  //
			Set_Write_RAM(CS2);	 //	д�Դ�
			for(j=0;j<12;j++)
             {
		        Con_4_byte(ASC6X12[c*12+j],CS2);	//����ת��
	 	       	}
        	 }

		  else	 //�����м�λ��
		  {	//�����ϰ�������
			Set_Column_Address(Shift+x1,Shift+x1+1,CS1); // ���������꣬shiftΪ��ƫ������1322������1Ϊ8/4-1
			Set_Row_Address(y,63,CS1);  //
			Set_Write_RAM(CS1);	 //	дCS1�Դ�
		 	for(j=0;j<(64-y);j++)
		    {
			    Con_4_byte(ASC6X12[c*12+j],CS1);	//����ת��
					 }
			//�����°�������
			Set_Column_Address(Shift+x1,Shift+x1+1,CS2); // ���������꣬shiftΪ��ƫ������1322������1Ϊ8/4-1
			Set_Row_Address(0,(y+11-64),CS2);  //
			Set_Write_RAM(CS2);	 //	дCS2�Դ�
		   	for(j=(64-y);j<12;j++)
			 {
			     Con_4_byte(ASC6X12[c*12+j],CS2);	//����ת��
					 }
						 
			 }
			x=x+8;	//�¸��ַ�x����
	 }  
  
}



//==============================================================
//����������д��һ���׼ASCII�ַ���	 8x16
//��������ʾ��λ�ã�x,y����ch[]Ҫ��ʾ���ַ���
//    x: Start Column  ��ʼ�� ��Χ 0~��255-8��
//    y: Start Row    ��ʼ�� 0~127-16
//==============================================================  
void Asc8_16(unsigned char x,unsigned char y,unsigned char ch[])
{
  unsigned char x1,c=0, i=0,j;      
  while (ch[i]!='\0')
  {    
    x1=x/4;
	c =ch[i]-32;
	i++;  //��һ���ַ�
    	
	if(x1>61)  
	   {x=0;
	   x1=x/4;
	   y=y+16;
	   }  //����		  
			 
	if (y <=48)	//63-15
		{
		Set_Column_Address(Shift+x1,Shift+x1+1,CS1); // ���������꣬shiftΪ��ƫ������1322������1Ϊ8/4-1
		Set_Row_Address(y,y+15,CS1);  //
		Set_Write_RAM(CS1);	 //	д�Դ�
		for(j=0;j<16;j++)
	    	{
			  Con_4_byte(ASC8X16[c*16+j],CS1);	//����ת��
		 		}
		 } 
	else
	 	if (y >=64 )
		 {
			Set_Column_Address(Shift+x1,Shift+x1+1,CS2); // ���������꣬shiftΪ��ƫ������1322������1Ϊ8/4-1
			Set_Row_Address((y-63),(y-63+15),CS2);  //
			Set_Write_RAM(CS2);	 //	д�Դ�
			for(j=0;j<16;j++)
             {
		        Con_4_byte(ASC8X16[c*16+j],CS2);	//����ת��
	 	       	}
        	 }

		  else	 //�����м�λ��
		  {	//�����ϰ�������
			Set_Column_Address(Shift+x1,Shift+x1+1,CS1); // ���������꣬shiftΪ��ƫ������1322������1Ϊ8/4-1
			Set_Row_Address(y,63,CS1);  //
			Set_Write_RAM(CS1);	 //	дCS1�Դ�
		 	for(j=0;j<(64-y);j++)
		    {
			    Con_4_byte(ASC8X16[c*16+j],CS1);	//����ת��
					 }
			//�����°�������
			Set_Column_Address(Shift+x1,Shift+x1+1,CS2); // ���������꣬shiftΪ��ƫ������1322������1Ϊ8/4-1
			Set_Row_Address(0,(y+15-64),CS2);  //
			Set_Write_RAM(CS2);	 //	дCS2�Դ�
		   	for(j=(64-y);j<16;j++)
			 {
			     Con_4_byte(ASC8X16[c*16+j],CS2);	//����ת��
					 }
						 
			 }
			x=x+8;	//�¸��ַ�x����
				 }  
}

 
//==============================================================
//����������д��һ���׼ASCII�ַ���	 12x24	   ʵ�ʵ���16*24
//��������ʾ��λ�ã�x,y����ch[]Ҫ��ʾ���ַ���
//���أ���
//==============================================================  
void Asc12_24(unsigned char x,unsigned char y,unsigned char ch[])
{
  unsigned char x1,c=0, i=0,j;      
  while (ch[i]!='\0')
  {    
    x1=x/4;
	c =ch[i]-32;
	i++;  //��һ���ַ�
    	
	if(x1>59)  
	   {x=0;
	   x1=x/4;
	   y=y+24;
	   }  //����		  
			 
	if (y <=48)	//63-15
		{
		Set_Column_Address(Shift+x1,Shift+x1+3,CS1); // ���������꣬shiftΪ��ƫ������1322������1Ϊ8/4-1
		Set_Row_Address(y,y+23,CS1);  //
		Set_Write_RAM(CS1);	 //	д�Դ�
		for(j=0;j<48;j++)
	    	{
			  Con_4_byte(ASC12X24[c*48+j],CS1);	//����ת��
		 		}
		 } 
	else
	 	if (y >=64 )
		 {
			Set_Column_Address(Shift+x1,Shift+x1+3,CS2); // ���������꣬shiftΪ��ƫ������1322������1Ϊ8/4-1
			Set_Row_Address((y-63),(y-63+23),CS2);  //
			Set_Write_RAM(CS2);	 //	д�Դ�
			for(j=0;j<48;j++)
             {
		        Con_4_byte(ASC12X24[c*48+j],CS2);	//����ת��
	 	       	}
        	 }

		  else	 //�����м�λ��
		  {	//�����ϰ�������
			Set_Column_Address(Shift+x1,Shift+x1+3,CS1); // ���������꣬shiftΪ��ƫ������1322������1Ϊ8/4-1
			Set_Row_Address(y,63,CS1);  //
			Set_Write_RAM(CS1);	 //	дCS1�Դ�
		 	for(j=0;j<(64-y)*2;j++)
		    {
			    Con_4_byte(ASC12X24[c*48+j],CS1);	//����ת��
					 }
			//�����°�������
			Set_Column_Address(Shift+x1,Shift+x1+3,CS2); // ���������꣬shiftΪ��ƫ������1322������1Ϊ8/4-1
			Set_Row_Address(0,(y+23-64),CS2);  //
			Set_Write_RAM(CS2);	 //	дCS2�Դ�
		   	for(j=(64-y)*2;j<48;j++)
			 {
			     Con_4_byte(ASC12X24[c*48+j],CS2);	//����ת��
					 }
						 
			 }
			x=x+16;	//�¸��ַ�x����
				 }  
   
}
 
//==============================================================
//����������д��һ���׼ASCII�ַ���	 20x40	ʵ�ʵ���24*40 
//��������ʾ��λ�ã�x,y����ch[]Ҫ��ʾ���ַ���
//���أ���
//==============================================================  
void Asc20_40(unsigned char x,unsigned char y,unsigned char ch[])
{

  unsigned char x1,c=0, i=0,j;      
  while (ch[i]!='\0')
  {    
    x1=x/4;
	c =ch[i]-32;
	i++;  //��һ���ַ�
     	
	if(x1>58)  
	   {x=0;
	   x1=x/4;
	   y=y+40;
	   }  //���� 	  
			 
	if (y <=24)	//63-23
		{
		Set_Column_Address(Shift+x1,Shift+x1+5,CS1); // ���������꣬shiftΪ��ƫ������1322������1Ϊ24/4-1
		Set_Row_Address(y,y+39,CS1);  //
		Set_Write_RAM(CS1);	 //	д�Դ�
		for(j=0;j<120;j++)
	    	{
			  Con_4_byte(ASC20X40[c*120+j],CS1);	//����ת��
		 		}
		 } 
	else
	 {
	 	if (y >=64 )
		 {
			Set_Column_Address(Shift+x1,Shift+x1+5,CS2); // ���������꣬ 
			Set_Row_Address((y-63),(y-63+39),CS2);  //
			Set_Write_RAM(CS2);	 //	д�Դ�
			for(j=0;j<120;j++)
             {
		        Con_4_byte(ASC20X40[c*120+j],CS2);	//����ת��
	 	      }
          }

		else	 //�����м�λ��
		  {	//�����ϰ�������
			Set_Column_Address(Shift+x1,Shift+x1+5,CS1); // ���������� 
			Set_Row_Address(y,63,CS1);  //
			Set_Write_RAM(CS1);	 //	дCS1�Դ�
		 	for(j=0;j<(64-y)*3;j++)
		    {
			   Con_4_byte(ASC20X40[c*120+j],CS1);	//����ת��
			}
			//�����°�������
			Set_Column_Address(Shift+x1,Shift+x1+5,CS2); // ���������� 
			Set_Row_Address(0,(y+39-63),CS2);  //															   
			Set_Write_RAM(CS2);	 //	дCS2�Դ�
		   	for(j=((64-y)*3);j<120;j++)
			 {
			     Con_4_byte(ASC20X40[c*120+j],CS2);	//����ת��
			 }
						 
		  }
		}
		
			x=x+24;	//�¸��ַ�x����
	 }  
}	  