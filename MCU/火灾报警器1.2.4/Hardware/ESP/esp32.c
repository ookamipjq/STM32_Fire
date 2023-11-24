//单片机头文件
#include "stm32f10x.h"
 
//网络设备驱动
#include "esp32.h"
 
/* FreeRTOS头文件 */
#include "Serial.h"
 #include "delay.h"
 #include "cJson.h"
//C库
#include <string.h>
#include <stdio.h>
 
//#define ESP8266_WIFI_INFO		"AT+CWJAP=\"KIRA 4575\",\"12345678\"\r\n"

#define ESP8266_WIFI_INFO		"AT+CWJAP=\"WIFI名称\",\"WIFI密码\"\r\n"
 
#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"改为项目的生成的域名\",1883\r\n"
 
 
#define AT_MQTTUSERCFG	"AT+MQTTUSERCFG=0,1,\"devices\",\"连接参数的username\",\"连接参数的password\",0,0,\"\r\n"
//client_id随便写，username和password填生成的参数
 
#define AT_MQTTCLIENTID "AT+MQTTCLIENTID=0,\"332211|securemode=3\\,signmethod=hmacsha1\\,timestamp=112233|\"\r\n"
//ClientId填生成的参数，注意要在每个逗号前加分隔符"\"
 
#define AT_MQTTCONN	"AT+MQTTCONN=0,\"域名填写\",1883,1\r\n"
//连接域名填上面生成的，注意要去掉端口号1883，因为后面已经设置了


//#define ALI_TOPIC_SET "/sys/填入ProductKey/fire/thing/event/property/post\",\"{\"params\":{\"temp\":80}}\",1,0\r\n"
//#define AT_MQTTSUB "AT+MQTTSUB=0,\"/sys/填入ProductKey/fire/thing/service/property/set\",1\r\n"
 
#define      ALI_TOPIC_SET          "	/sys/填入ProductKey/fire/thing/service/property/set"
#define      ALI_TOPIC_POST         "/sys/填入ProductKey/fire/thing/event/property/post"
 
unsigned char esp8266_buf[128];
unsigned short esp8266_cnt = 0, esp8266_cntPre = 0;
 
 
//==========================================================
//	函数名称：	ESP8266_Clear
//
//	函数功能：	清空缓存
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP8266_Clear(void)
{
 
	memset(esp8266_buf, 0, sizeof(esp8266_buf));
	esp8266_cnt = 0;
 
}
 
//==========================================================
//	函数名称：	ESP8266_WaitRecive
//
//	函数功能：	等待接收完成
//
//	入口参数：	无
//
//	返回参数：	REV_OK-接收完成		REV_WAIT-接收超时未完成
//
//	说明：		循环调用检测是否接收完成
//==========================================================
_Bool ESP8266_WaitRecive(void)
{
 
	if(esp8266_cnt == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
		return REV_WAIT;
		
	if(esp8266_cnt == esp8266_cntPre)				//如果上一次的值和这次相同，则说明接收完毕
	{
		esp8266_cnt = 0;							//清0接收计数
			
		return REV_OK;								//返回接收完成标志
	}
		
	esp8266_cntPre = esp8266_cnt;					//置为相同
	
	return REV_WAIT;								//返回接收未完成标志
 
}
 
//==========================================================
//	函数名称：	ESP8266_SendCmd
//
//	函数功能：	发送命令
//
//	入口参数：	cmd：命令
//				res：需要检查的返回指令
//
//	返回参数：	0-成功	1-失败
//
//	说明：		
//==========================================================
_Bool ESP8266_SendCmd(char *cmd, char *res, u16 time)
{	
	Usart2_SendString((unsigned char *)cmd, strlen((const char *)cmd));
 
	while(time--)
	{
		if(ESP8266_WaitRecive() == REV_OK)							//如果收到数据
		{
			if(strstr((const char *)esp8266_buf, res) != NULL)		//如果检索到关键词
			{
				ESP8266_Clear();									//清空缓存
				
				return 0;
			}
		}
		
		Delay_ms(10);
	}
	
	return 1;
 
}
 
//==========================================================
//	函数名称：	ESP8266_SendData
//
//	函数功能：	发送数据
//
//	入口参数：	data：数据
//				len：长度
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP8266_SendData(unsigned char *data, unsigned short len)
{
 
	char cmdBuf[32];
	
	ESP8266_Clear();								//清空接收缓存
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//发送命令
	if(!ESP8266_SendCmd(cmdBuf, ">", 200))				//收到‘>’时可以发送数据
	{
		Usart2_SendString(data, len);		//发送设备连接请求数据
	}
 
}
 
//==========================================================
//	函数名称：	ESP8266_GetIPD
//
//	函数功能：	获取平台返回的数据
//
//	入口参数：	等待的时间(乘以10ms)
//
//	返回参数：	平台返回的原始数据
//
//	说明：		不同网络设备返回的格式不同，需要去调试
//				如ESP8266的返回格式为	"+IPD,x:yyy"	x代表数据长度，yyy是数据内容
//==========================================================
unsigned char *ESP8266_GetIPD(unsigned short timeOut)
{
 
	char *ptrIPD = NULL;
	
	do
	{
		if(ESP8266_WaitRecive() == REV_OK)								//如果接收完成
		{
			ptrIPD = strstr((char *)esp8266_buf, "IPD,");				//搜索“IPD”头
			if(ptrIPD == NULL)											//如果没找到，可能是IPD头的延迟，还是需要等待一会，但不会超过设定的时间
			{
				//printf("\"IPD\" not found\r\n");
			}
			else
			{
				ptrIPD = strchr(ptrIPD, ':');							//找到':'
				if(ptrIPD != NULL)
				{
					ptrIPD++;
					return (unsigned char *)(ptrIPD);
				}
				else
					return NULL;
				
			}
		}
		
		Delay_ms(5);													//延时等待
	} while(timeOut--);
	
	return NULL;														//超时还未找到，返回空指针
 
}
//==========================================================
//	函数名称：	USART2_IRQHandler
//
//	函数功能：	串口2收发中断
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void USART2_IRQHandler(void)
{
 
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收中断
	{
		if(esp8266_cnt >= sizeof(esp8266_buf))	esp8266_cnt = 0; //防止串口被刷爆
		esp8266_buf[esp8266_cnt++] = USART2->DR;
		
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}
 
}
//==========================================================
//	函数名称：	ESP8266_Init
//
//	函数功能：	初始化ESP8266
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP8266_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
 
	//ESP8266复位引脚
	GPIO_Initure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initure.GPIO_Pin = GPIO_Pin_9;					//GPIOB1-复位
	GPIO_Initure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_Initure);
	
	GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_RESET);
	Delay_ms(100);
	GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_SET);
	Delay_ms(100);
	
	ESP8266_Clear();
 
	UsartPrintf(USART1,"AT\r\n");
	while(ESP8266_SendCmd("AT\r\n\r", "OK", 200))
		Delay_ms(500);
	
	UsartPrintf(USART1,"CWMODE\r\n");
	while(ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK", 200))
		Delay_ms(500);
	
	UsartPrintf(USART1,"AT+CWDHCP\r\n");
	while(ESP8266_SendCmd("AT+CWDHCP=1,1\r\n", "OK", 200))
		Delay_ms(500);
	
	UsartPrintf(USART1,"CWJAP\r\n");
	while(ESP8266_SendCmd(ESP8266_WIFI_INFO, "GOT IP", 200))
		Delay_ms(500);
	UsartPrintf(USART1,"5\r\n");
	while(ESP8266_SendCmd(AT_MQTTUSERCFG, "OK", 200))
		Delay_ms(500);
	UsartPrintf(USART1,"6\r\n");
	while(ESP8266_SendCmd(AT_MQTTCLIENTID, "OK", 200))
		Delay_ms(500);
	UsartPrintf(USART1,"7\r\n");
	while(ESP8266_SendCmd(AT_MQTTCONN, "OK", 200))
		Delay_ms(500);
	
	while(ESP8266_SendCmd("AT+MQTTSUB=0,\""ALI_TOPIC_SET"\",0\r\n","OK",200)){}
		
	while(ESP8266_SendCmd("AT+MQTTSUB=0,\""ALI_TOPIC_POST"\",0\r\n","OK",200)){}
//	UsartPrintf(USART1,"8\r\n");
//	while(ESP8266_SendCmd(AT_MQTTSUB, "OK", 200))
//		Delay_ms(500);
//	UsartPrintf(USART1,"CIPSTART\r\n");
//	while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT", 200))
//		Delay_ms(500);
	
	UsartPrintf(USART1,"ESP8266 Init OK\r\n");
}
 

void ESP8266_Updata(char *cmd)
{
	UsartPrintf(USART1,"success\r\n");
	Usart2_SendString((unsigned char *)cmd, strlen((const char *)cmd));
 
	ESP8266_Clear();									//清空缓存
	Delay_ms(10);
	
}
 
/*
************************************************************
*	函数名称：	Usart2_Init
*
*	函数功能：	串口2初始化
*
*	入口参数：	baud：设定的波特率
*
*	返回参数：	无
*
*	说明：		TX-PA2		RX-PA3
************************************************************
*/
void Usart2_Init(unsigned int baud)
{
 
	GPIO_InitTypeDef gpio_initstruct;
	USART_InitTypeDef usart_initstruct;
	NVIC_InitTypeDef nvic_initstruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//PA2	TXD
	gpio_initstruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_initstruct.GPIO_Pin = GPIO_Pin_2;
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_initstruct);
	
	//PA3	RXD
	gpio_initstruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_initstruct.GPIO_Pin = GPIO_Pin_3;
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_initstruct);
	
	usart_initstruct.USART_BaudRate = baud;
	usart_initstruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控
	usart_initstruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//接收和发送
	usart_initstruct.USART_Parity = USART_Parity_No;									//无校验
	usart_initstruct.USART_StopBits = USART_StopBits_1;								//1位停止位
	usart_initstruct.USART_WordLength = USART_WordLength_8b;							//8位数据位
	USART_Init(USART2, &usart_initstruct);
	
	USART_Cmd(USART2, ENABLE);														//使能串口
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);									//使能接收中断
	
	nvic_initstruct.NVIC_IRQChannel = USART2_IRQn;
	nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
	nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_initstruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvic_initstruct);
 
}
 
 
 
 
 
/*
************************************************************
*	函数名称：	Usart_SendString
*
*	函数功能：	串口数据发送
*
*	入口参数：	USARTx：串口组
*				str：要发送的数据
*				len：数据长度
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void Usart2_SendString(unsigned char *str, unsigned short len)
{
 
	unsigned short count = 0;
	
	for(; count < len; count++)
	{
		USART_SendData(USART2, *str++);									//发送数据
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);		//等待发送完成
	}
}
 
 

 
 


