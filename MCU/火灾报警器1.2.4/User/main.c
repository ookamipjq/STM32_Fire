#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Serial.h"
#include "oled.h"		//OLED显示
#include "led.h"		//LED初始化
#include "fire.h"		//火焰传感器
#include "dht11.h"	//温度传感器
#include "beep.h"		//蜂鸣器
#include "MQ2.h"		//烟雾传感器
#include "esp32.h"	//阿里云连接配置

//#define AT_MQTTPUB "AT+MQTTPUB=0,\"/sys/填入ProductKey/fire/thing/event/property/post\",\"{\\\"params\\\":{\\\"temperature\\\":80}}\",1,0\r\n"
//#define AT_MQTTSUB "AT+MQTTSUB=0,\"/sys/iknjuQejQ6d/name/thing/service/property/set\",1\r\n"

char mqtt_message[512];	//MQTT的上报消息缓存

//功能函数
static void led_run(void);
//static void atanaction(float ad_f,uint8_t temp, uint8_t humi);
static void main_run(void);		//核心代码函数


uint8_t mq_y = 12;
uint8_t dh_y = 14;

uint8_t time = 0;

int main(void)
{

	LED_Init();
	Fire_Init();	Fire_Init2();	Fire_Init3();		//火焰传感器初始化
	DHT11_Init();	DHT11_Init2(); DHT11_Init3();	//温湿度传感器初始化
	BEEP_Init();		//蜂鸣器初始化
	Delay_ms(200);
	MQ2_Init();	MQ2_Init2();	MQ2_Init3();			//烟雾传感器初始化
	OLED_Init();		//OLED初始化
	Serial_Init();	//串口通信初始化
	Usart2_Init(115200); 	//串口2，驱动ESP8266用
	OLED_Clear();		//OLED清屏
	
	ESP8266_Init();					

	//房间号
	
	OLED_ShowCHinese(40,1,6);//房
	OLED_ShowCHinese(56,1,7);//间

	
	//烟雾传感器信息
	OLED_ShowCHinese(0,mq_y,0);//烟
	OLED_ShowCHinese(16,mq_y,1);//雾
	OLED_ShowChar(32,mq_y,':',16);
	
	
	//温湿度传感器信息
	OLED_ShowCHinese(0,dh_y,4);//温
	OLED_ShowCHinese(16,dh_y,3);//度
	OLED_ShowChar(32,dh_y,':',16);
	OLED_ShowCHinese(64,dh_y,5);//湿
	OLED_ShowCHinese(80,dh_y,3);//度
	OLED_ShowChar(96,dh_y,':',16);
	
	Delay_s(3);
	while (1)
	{
		main_run();
		
	}
}

static void led_run(void)
{
	//LED闪烁表示系统正常运行
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		Delay_ms(500);
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		Delay_ms(500);
		time++;
	if(time>12)	time = 0;
}

static void main_run(void)
{
	
	//温湿度传感器
	uint8_t temp = 0,humi = 0;
	uint8_t temp2 = 0,humi2 = 0;
	uint8_t temp3 = 0,humi3 = 0;
	
	//烟雾传感器
	uint16_t ad = 0,ad2 = 0,ad3 = 0;
	float ad_f,ad_f2,ad_f3;

	//火焰传感器
	uint16_t fire = 0,fire2 = 0,fire3 = 0;
	float fire_f,fire_f2,fire_f3;
	
	ad = ADC_Data();	//获取烟雾传感器信息
	ad_f = ad * 99 /4096.0;	//进行百分比换算
	
	ad2 = ADC_Data2();	//获取烟雾传感器信息
	ad_f2 = ad2 * 99 /4096.0;	//进行百分比换算
	
	ad3 = ADC_Data3();	//获取烟雾传感器信息
	ad_f3 = ad3 * 99 /4096.0;	//进行百分比换算
	
	fire = Fire_Data();
	fire_f = fire * 99 / 4096.0;
	
	fire2 = Fire_Data2();
	fire_f2 = fire2 * 99 / 4096.0;
	
	fire3 = Fire_Data3();
	fire_f3 = fire3 * 99 / 4096.0;
	
	DHT11_ReadData(&temp,&humi);
	DHT11_ReadData2(&temp2,&humi2);
	DHT11_ReadData3(&temp3,&humi3);
	
	//火焰传感器信息
	OLED_ShowCHinese(80,mq_y,8);//正
	OLED_ShowCHinese(96,mq_y,10);//常
	
	//数据刷新
	if( time < 4 )
	{
		OLED_ShowNum(72,1,1,1,16);
		OLED_ShowBNum(40,mq_y,ad_f,3,16);
//		OLED_ShowBNum(104,mq_y,fire_f,3,8);
		OLED_ShowNum(40,dh_y,temp,2,16);
		OLED_ShowNum(104,dh_y,humi,2,16);
	}		
	else if( time >= 4 && time <8)
	{
		OLED_ShowNum(72,1,2,1,16);
		OLED_ShowBNum(40,mq_y,ad_f2,3,16);
//		OLED_ShowBNum(104,mq_y,fire_f2,3,8);
		OLED_ShowNum(40,dh_y,temp2,2,16);
		OLED_ShowNum(104,dh_y,humi2,2,16);
	}		
	else
	{
		OLED_ShowNum(72,1,3,1,16);
		OLED_ShowBNum(40,mq_y,ad_f3,3,16);
//		OLED_ShowBNum(104,mq_y,fire_f3,3,8);
		OLED_ShowNum(40,dh_y,temp3,2,16);
		OLED_ShowNum(104,dh_y,humi3,2,16);
	}		
	
	Delay_s(1);
	UsartPrintf(USART2,"AT+MQTTPUB=0,\"/sys/填入ProductKey/fire/thing/event/property/post\",\"{\\\"params\\\":{\\\"temperature\\\":%d\\,\
		\\\"humidity\\\":%d\\,\\\"yanwu\\\":%.1f\\,\\\"temp2\\\":%d\\,\\\"humi2\\\":%d\\,\\\"yanwu2\\\":%.1f\\,\
		\\\"temp3\\\":%d\\,\\\"humi3\\\":%d\\,\\\"yanwu3\\\":%.1f\\}}\",1,0\r\n",
		temp,humi,ad_f,temp2,humi2,ad_f2,temp3,humi3,ad_f3);
		
	led_run();
	
	//报警
	while(ad_f > 50 || temp >50 || humi <45 || fire_f > 2000)
	{
		ad = ADC_Data();	//获取烟雾传感器信息	
		ad_f = ad * 99 /4096.0;	//进行百分比换算
		DHT11_ReadData(&temp,&humi);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		TIM_SetCompare2(TIM2,50);
		//火焰传感器信息
		OLED_ShowCHinese(80,mq_y,9);//异
		OLED_ShowCHinese(96,mq_y,10);//常
		Delay_ms(500);
		TIM_SetCompare2(TIM2,0);
		Delay_ms(500);
	}
	
	while(ad_f2 > 50 || temp2 >50 || humi2 <45 || fire_f2 > 2000)
	{
		ad2 = ADC_Data();	//获取烟雾传感器信息
		ad_f2 = ad2 * 99 /4096.0;	//进行百分比换算
		DHT11_ReadData(&temp2,&humi2);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		TIM_SetCompare2(TIM2,50);
		//火焰传感器信息
		OLED_ShowCHinese(80,mq_y,9);//异
		OLED_ShowCHinese(96,mq_y,10);//常
		Delay_ms(500);
		TIM_SetCompare2(TIM2,0);
		Delay_ms(500);
	}
	
	while(ad_f3 > 50 || temp3 >50 || humi3 <45 || fire_f3 > 2000)
	{
		ad3 = ADC_Data();	//获取烟雾传感器信息
		ad_f3 = ad3 * 99 /4096.0;	//进行百分比换算
		DHT11_ReadData(&temp3,&humi3);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		TIM_SetCompare2(TIM2,50);
		//火焰传感器信息
		OLED_ShowCHinese(80,mq_y,9);//异
		OLED_ShowCHinese(96,mq_y,10);//常
		Delay_ms(500);
		TIM_SetCompare2(TIM2,0);
		Delay_ms(500);
	}
	
	//串口打印调试
		UsartPrintf(USART1,"烟雾1:1 = %d,2 = %.3f %\r\n",ad,ad_f);
		UsartPrintf(USART1,"烟雾2:1 = %d,2 = %.3f %\r\n",ad2,ad_f2);
		UsartPrintf(USART1,"烟雾3:1 = %d,2 = %.3f %\r\n",ad3,ad_f3);
		UsartPrintf(USART1,"温度1: %d,湿度: %d %\r\n",temp,humi);  
		UsartPrintf(USART1,"温度2: %d,湿度2: %d %\r\n",temp2,humi2);  
		UsartPrintf(USART1,"温度3: %d,湿度3: %d %\r\n",temp3,humi3);  
		UsartPrintf(USART1,"火焰传感1: %.f\r\n",fire_f );
		UsartPrintf(USART1,"火焰传感2: %.f\r\n",fire_f2 );
		UsartPrintf(USART1,"火焰传感3: %.f\r\n",fire_f3 );
		
}

//static void atanaction(float ad_f,uint8_t temp, uint8_t humi)
//{

//	
//}
