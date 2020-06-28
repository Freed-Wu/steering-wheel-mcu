//////////////////////////////////////////////////////////////////////////////////	 
 //使用SysTick的普通计数模式对延迟进行管理
   //********************************************************************************
#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    	

#define OLED_MODE 0
#define SIZE 16
#define Max_Column		0x3F			// 256/4-1
#define	Brightness	0xFF 
#define	Shift		0x1C
	    						  
//-----------------OLED端口定义 4线SPI----------------  					   
/* 采用IO口模拟SPI通讯方式驱动oled模块
CS1    ~ PE6
RES ~ PE5
DC   ~ PE4
CLK  ~ PE3
DIN ~ PE2
CS2 ~ PE1
*/
#define OLED_CS1_Clr()  GPIO_ResetBits(GPIOE,GPIO_Pin_6)//CS1   PE6
#define OLED_CS1_Set()  GPIO_SetBits(GPIOE,GPIO_Pin_6)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_5)//RES  PE5
#define OLED_RST_Set() GPIO_SetBits(GPIOE,GPIO_Pin_5)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_4)//DC	  PE4
#define OLED_DC_Set() GPIO_SetBits(GPIOE,GPIO_Pin_4)

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_3)//CLK  PE3
#define OLED_SCLK_Set() GPIO_SetBits(GPIOE,GPIO_Pin_3)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_2)//DIN  PE2
#define OLED_SDIN_Set() GPIO_SetBits(GPIOE,GPIO_Pin_2)

#define OLED_CS2_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_1)//CS2  PE1
#define OLED_CS2_Set() GPIO_SetBits(GPIOE,GPIO_Pin_1)

 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define CS1 1	//片选1322控制器 1
#define CS2 2	//片选1322控制器 2
//OLED控制用函数
 
 
#endif  
void OLED_WR_Byte(u8 dat,u8 cmd,u8 cs);
void OLED_Init(void);
void Set_Column_Address(unsigned char a, unsigned char b, unsigned char cs);
void Set_Row_Address(unsigned char a, unsigned char b , unsigned char cs);
void Set_Write_RAM(unsigned char cs);
void Set_Read_RAM(unsigned char cs);
void Set_Remap_Format(unsigned char d, unsigned char cs);
void Set_Partial_Display(unsigned char a, unsigned char b, unsigned char c,unsigned char cs);
void Set_Display_Offset(unsigned char d,unsigned char cs);
void Set_Start_Line(unsigned char d,unsigned char cs);
void Set_Master_Current(unsigned char d,unsigned char cs);
void Set_Linear_Gray_Scale_Table(unsigned char cs);
void Set_Gray_Scale_Table(unsigned char cs);
 void Grayscale(unsigned char cs);
 
void Fill_RAM(unsigned char Data,unsigned char cs);
void Fill_Block(unsigned char Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d );
void Show_Pattern(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d);
void Con_4_byte(unsigned char DATA,unsigned char cs);

 //以下ASCII程序
void Asc5_8(unsigned char x,unsigned char y,unsigned char ch[]);
void Asc6_12(unsigned char x,unsigned char y,unsigned char ch[]);
void Asc8_16(unsigned char x,unsigned char y,unsigned char ch[]);
void Asc12_24(unsigned char x,unsigned char y,unsigned char ch[]);
void Asc20_40(unsigned char x,unsigned char y,unsigned char ch[]);
 //以下汉字程序
void HZ16_16( unsigned char x, unsigned char y, unsigned char num);
void HZ12_12( unsigned char x, unsigned char y, unsigned char num);
void HZ24_24( unsigned char x, unsigned char y, unsigned char num);
void Show_HZ16_16(unsigned char  x,unsigned char  y, unsigned char  d,unsigned char num1,unsigned char num2);
void Show_HZ12_12(unsigned char  x,unsigned char  y, unsigned char  d,unsigned char num1,unsigned char num2);
void Show_HZ24_24(unsigned char  x,unsigned char  y, unsigned char  d,unsigned char num1,unsigned char num2);

