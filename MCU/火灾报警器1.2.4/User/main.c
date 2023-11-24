#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Serial.h"
#include "oled.h"		//OLED��ʾ
#include "led.h"		//LED��ʼ��
#include "fire.h"		//���洫����
#include "dht11.h"	//�¶ȴ�����
#include "beep.h"		//������
#include "MQ2.h"		//��������
#include "esp32.h"	//��������������

//#define AT_MQTTPUB "AT+MQTTPUB=0,\"/sys/����ProductKey/fire/thing/event/property/post\",\"{\\\"params\\\":{\\\"temperature\\\":80}}\",1,0\r\n"
//#define AT_MQTTSUB "AT+MQTTSUB=0,\"/sys/iknjuQejQ6d/name/thing/service/property/set\",1\r\n"

char mqtt_message[512];	//MQTT���ϱ���Ϣ����

//���ܺ���
static void led_run(void);
//static void atanaction(float ad_f,uint8_t temp, uint8_t humi);
static void main_run(void);		//���Ĵ��뺯��


uint8_t mq_y = 12;
uint8_t dh_y = 14;

uint8_t time = 0;

int main(void)
{

	LED_Init();
	Fire_Init();	Fire_Init2();	Fire_Init3();		//���洫������ʼ��
	DHT11_Init();	DHT11_Init2(); DHT11_Init3();	//��ʪ�ȴ�������ʼ��
	BEEP_Init();		//��������ʼ��
	Delay_ms(200);
	MQ2_Init();	MQ2_Init2();	MQ2_Init3();			//����������ʼ��
	OLED_Init();		//OLED��ʼ��
	Serial_Init();	//����ͨ�ų�ʼ��
	Usart2_Init(115200); 	//����2������ESP8266��
	OLED_Clear();		//OLED����
	
	ESP8266_Init();					

	//�����
	
	OLED_ShowCHinese(40,1,6);//��
	OLED_ShowCHinese(56,1,7);//��

	
	//����������Ϣ
	OLED_ShowCHinese(0,mq_y,0);//��
	OLED_ShowCHinese(16,mq_y,1);//��
	OLED_ShowChar(32,mq_y,':',16);
	
	
	//��ʪ�ȴ�������Ϣ
	OLED_ShowCHinese(0,dh_y,4);//��
	OLED_ShowCHinese(16,dh_y,3);//��
	OLED_ShowChar(32,dh_y,':',16);
	OLED_ShowCHinese(64,dh_y,5);//ʪ
	OLED_ShowCHinese(80,dh_y,3);//��
	OLED_ShowChar(96,dh_y,':',16);
	
	Delay_s(3);
	while (1)
	{
		main_run();
		
	}
}

static void led_run(void)
{
	//LED��˸��ʾϵͳ��������
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		Delay_ms(500);
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		Delay_ms(500);
		time++;
	if(time>12)	time = 0;
}

static void main_run(void)
{
	
	//��ʪ�ȴ�����
	uint8_t temp = 0,humi = 0;
	uint8_t temp2 = 0,humi2 = 0;
	uint8_t temp3 = 0,humi3 = 0;
	
	//��������
	uint16_t ad = 0,ad2 = 0,ad3 = 0;
	float ad_f,ad_f2,ad_f3;

	//���洫����
	uint16_t fire = 0,fire2 = 0,fire3 = 0;
	float fire_f,fire_f2,fire_f3;
	
	ad = ADC_Data();	//��ȡ����������Ϣ
	ad_f = ad * 99 /4096.0;	//���аٷֱȻ���
	
	ad2 = ADC_Data2();	//��ȡ����������Ϣ
	ad_f2 = ad2 * 99 /4096.0;	//���аٷֱȻ���
	
	ad3 = ADC_Data3();	//��ȡ����������Ϣ
	ad_f3 = ad3 * 99 /4096.0;	//���аٷֱȻ���
	
	fire = Fire_Data();
	fire_f = fire * 99 / 4096.0;
	
	fire2 = Fire_Data2();
	fire_f2 = fire2 * 99 / 4096.0;
	
	fire3 = Fire_Data3();
	fire_f3 = fire3 * 99 / 4096.0;
	
	DHT11_ReadData(&temp,&humi);
	DHT11_ReadData2(&temp2,&humi2);
	DHT11_ReadData3(&temp3,&humi3);
	
	//���洫������Ϣ
	OLED_ShowCHinese(80,mq_y,8);//��
	OLED_ShowCHinese(96,mq_y,10);//��
	
	//����ˢ��
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
	UsartPrintf(USART2,"AT+MQTTPUB=0,\"/sys/����ProductKey/fire/thing/event/property/post\",\"{\\\"params\\\":{\\\"temperature\\\":%d\\,\
		\\\"humidity\\\":%d\\,\\\"yanwu\\\":%.1f\\,\\\"temp2\\\":%d\\,\\\"humi2\\\":%d\\,\\\"yanwu2\\\":%.1f\\,\
		\\\"temp3\\\":%d\\,\\\"humi3\\\":%d\\,\\\"yanwu3\\\":%.1f\\}}\",1,0\r\n",
		temp,humi,ad_f,temp2,humi2,ad_f2,temp3,humi3,ad_f3);
		
	led_run();
	
	//����
	while(ad_f > 50 || temp >50 || humi <45 || fire_f > 2000)
	{
		ad = ADC_Data();	//��ȡ����������Ϣ	
		ad_f = ad * 99 /4096.0;	//���аٷֱȻ���
		DHT11_ReadData(&temp,&humi);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		TIM_SetCompare2(TIM2,50);
		//���洫������Ϣ
		OLED_ShowCHinese(80,mq_y,9);//��
		OLED_ShowCHinese(96,mq_y,10);//��
		Delay_ms(500);
		TIM_SetCompare2(TIM2,0);
		Delay_ms(500);
	}
	
	while(ad_f2 > 50 || temp2 >50 || humi2 <45 || fire_f2 > 2000)
	{
		ad2 = ADC_Data();	//��ȡ����������Ϣ
		ad_f2 = ad2 * 99 /4096.0;	//���аٷֱȻ���
		DHT11_ReadData(&temp2,&humi2);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		TIM_SetCompare2(TIM2,50);
		//���洫������Ϣ
		OLED_ShowCHinese(80,mq_y,9);//��
		OLED_ShowCHinese(96,mq_y,10);//��
		Delay_ms(500);
		TIM_SetCompare2(TIM2,0);
		Delay_ms(500);
	}
	
	while(ad_f3 > 50 || temp3 >50 || humi3 <45 || fire_f3 > 2000)
	{
		ad3 = ADC_Data();	//��ȡ����������Ϣ
		ad_f3 = ad3 * 99 /4096.0;	//���аٷֱȻ���
		DHT11_ReadData(&temp3,&humi3);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		TIM_SetCompare2(TIM2,50);
		//���洫������Ϣ
		OLED_ShowCHinese(80,mq_y,9);//��
		OLED_ShowCHinese(96,mq_y,10);//��
		Delay_ms(500);
		TIM_SetCompare2(TIM2,0);
		Delay_ms(500);
	}
	
	//���ڴ�ӡ����
		UsartPrintf(USART1,"����1:1 = %d,2 = %.3f %\r\n",ad,ad_f);
		UsartPrintf(USART1,"����2:1 = %d,2 = %.3f %\r\n",ad2,ad_f2);
		UsartPrintf(USART1,"����3:1 = %d,2 = %.3f %\r\n",ad3,ad_f3);
		UsartPrintf(USART1,"�¶�1: %d,ʪ��: %d %\r\n",temp,humi);  
		UsartPrintf(USART1,"�¶�2: %d,ʪ��2: %d %\r\n",temp2,humi2);  
		UsartPrintf(USART1,"�¶�3: %d,ʪ��3: %d %\r\n",temp3,humi3);  
		UsartPrintf(USART1,"���洫��1: %.f\r\n",fire_f );
		UsartPrintf(USART1,"���洫��2: %.f\r\n",fire_f2 );
		UsartPrintf(USART1,"���洫��3: %.f\r\n",fire_f3 );
		
}

//static void atanaction(float ad_f,uint8_t temp, uint8_t humi)
//{

//	
//}
