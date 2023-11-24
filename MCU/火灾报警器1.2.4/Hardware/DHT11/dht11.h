#ifndef __DHT11_H
#define __DHT11_H	 
#include "sys.h"
#include "delay.h"

//第一个
#define DHT11PORT	GPIOB	//定义IO接口
#define DHT11_IO	GPIO_Pin_12	//定义IO接口

//第二个
#define DHT11PORT2	GPIOB	//定义IO接口
#define DHT11_IO2	GPIO_Pin_13	//定义IO接口

//第三个
#define DHT11PORT3	GPIOB	//定义IO接口
#define DHT11_IO3	GPIO_Pin_14	//定义IO接口


//IO方向设置
//#define DHT11_IO_IN()  {GPIOA->CRL&=0XFFFFF0FF;GPIOA->CRL|=8<<3;}
//#define DHT11_IO_OUT() {GPIOA->CRL&=0XFFFFF0FF;GPIOA->CRL|=3<<3;}
////IO操作函数											   
//#define	DHT11_DQ_OUT PAout(3) //数据端口	PA2 
//#define	DHT11_DQ_IN  PAin(3)  //数据端口	PA2

//u8 DHT11_Init(void);//初始化DHT11
//u8 DHT11_Read_Data(u8 *temp,u8 *humi);//读取温湿度
//u8 DHT11_Read_Byte(void);//读出一个字节
//u8 DHT11_Read_Bit(void);//读出一个位
//u8 DHT11_Check(void);//检测是否存在DHT11
//void DHT11_Rst(void);//复位DHT11 
//第一个
void DHT11_IO_OUT (void);
void DHT11_IO_IN (void);
void DHT11_RST (void);
u8 Dht11_Check(void); 	   
u8 Dht11_ReadBit(void); 
u8 Dht11_ReadByte(void); 
u8 DHT11_Init (void);
u8 DHT11_ReadData(u8 *temp,u8 *humi);
//第二个
void DHT11_IO_OUT2 (void);
void DHT11_IO_IN2 (void);
void DHT11_RST2 (void);
u8 Dht11_Check2(void); 	   
u8 Dht11_ReadBit2(void); 
u8 Dht11_ReadByte2(void); 
u8 DHT11_Init2 (void);
u8 DHT11_ReadData2(u8 *temp,u8 *humi);

//第三个
void DHT11_IO_OUT3 (void);
void DHT11_IO_IN3 (void);
void DHT11_RST3 (void);
u8 Dht11_Check3(void); 	   
u8 Dht11_ReadBit3(void); 
u8 Dht11_ReadByte3(void); 
u8 DHT11_Init3 (void);
u8 DHT11_ReadData3(u8 *temp,u8 *humi);
		 				    
#endif
