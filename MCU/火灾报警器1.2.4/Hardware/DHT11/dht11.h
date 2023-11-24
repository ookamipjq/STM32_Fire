#ifndef __DHT11_H
#define __DHT11_H	 
#include "sys.h"
#include "delay.h"

//��һ��
#define DHT11PORT	GPIOB	//����IO�ӿ�
#define DHT11_IO	GPIO_Pin_12	//����IO�ӿ�

//�ڶ���
#define DHT11PORT2	GPIOB	//����IO�ӿ�
#define DHT11_IO2	GPIO_Pin_13	//����IO�ӿ�

//������
#define DHT11PORT3	GPIOB	//����IO�ӿ�
#define DHT11_IO3	GPIO_Pin_14	//����IO�ӿ�


//IO��������
//#define DHT11_IO_IN()  {GPIOA->CRL&=0XFFFFF0FF;GPIOA->CRL|=8<<3;}
//#define DHT11_IO_OUT() {GPIOA->CRL&=0XFFFFF0FF;GPIOA->CRL|=3<<3;}
////IO��������											   
//#define	DHT11_DQ_OUT PAout(3) //���ݶ˿�	PA2 
//#define	DHT11_DQ_IN  PAin(3)  //���ݶ˿�	PA2

//u8 DHT11_Init(void);//��ʼ��DHT11
//u8 DHT11_Read_Data(u8 *temp,u8 *humi);//��ȡ��ʪ��
//u8 DHT11_Read_Byte(void);//����һ���ֽ�
//u8 DHT11_Read_Bit(void);//����һ��λ
//u8 DHT11_Check(void);//����Ƿ����DHT11
//void DHT11_Rst(void);//��λDHT11 
//��һ��
void DHT11_IO_OUT (void);
void DHT11_IO_IN (void);
void DHT11_RST (void);
u8 Dht11_Check(void); 	   
u8 Dht11_ReadBit(void); 
u8 Dht11_ReadByte(void); 
u8 DHT11_Init (void);
u8 DHT11_ReadData(u8 *temp,u8 *humi);
//�ڶ���
void DHT11_IO_OUT2 (void);
void DHT11_IO_IN2 (void);
void DHT11_RST2 (void);
u8 Dht11_Check2(void); 	   
u8 Dht11_ReadBit2(void); 
u8 Dht11_ReadByte2(void); 
u8 DHT11_Init2 (void);
u8 DHT11_ReadData2(u8 *temp,u8 *humi);

//������
void DHT11_IO_OUT3 (void);
void DHT11_IO_IN3 (void);
void DHT11_RST3 (void);
u8 Dht11_Check3(void); 	   
u8 Dht11_ReadBit3(void); 
u8 Dht11_ReadByte3(void); 
u8 DHT11_Init3 (void);
u8 DHT11_ReadData3(u8 *temp,u8 *humi);
		 				    
#endif
