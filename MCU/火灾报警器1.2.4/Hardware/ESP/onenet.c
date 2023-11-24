//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"
 
//�����豸
#include "esp32.h"
 
//Э���ļ�
#include "onenet.h"
#include "mqttkit.h"
 
//Ӳ������
#include "Serial.h"
 
//C��
#include <string.h>
#include <stdio.h>
 
//ȫ�ֱ���

 
#define PROID		"fire&k0f1gOAOPId"   //��ƷID
 
#define AUTH_INFO	"43fd3fc704f57f5603a05fc074cb4d291ad232059d1ed7342801b2ce941b8b55"		//��Ȩ��Ϣ
 
#define DEVID		"k0f1gOAOPId.fire|securemode=2,signmethod=hmacsha256,timestamp=1698907334413|"	//�豸ID
 
 
extern unsigned char esp8266_buf[128];
 
extern unsigned char esp8266_buf[128];
 
//==========================================================
//	�������ƣ�	OneNet_DevLink
//
//	�������ܣ�	��onenet��������
//
//	��ڲ�����	��
//
//	���ز�����	1-�ɹ�	0-ʧ��
//
//	˵����		��onenetƽ̨��������
//==========================================================
_Bool OneNet_DevLink(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//Э���
 
	unsigned char *dataPtr;
	
	_Bool status = 1;
	
	printf("OneNet_DevLink\r\nPROID: %s,	AUIF: %s,	DEVID:%s\r\n", PROID, AUTH_INFO, DEVID);
	
	if(MQTT_PacketConnect(PROID, AUTH_INFO, DEVID, 256, 0, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//�ϴ�ƽ̨
		
		dataPtr = ESP8266_GetIPD(250);									//�ȴ�ƽ̨��Ӧ
		if(dataPtr != NULL)
		{
			if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK)
			{
				switch(MQTT_UnPacketConnectAck(dataPtr))
				{
					case 0:printf("Tips\r\n");
					status = 0;
					//LED2_ON;						//�����ɹ�
					;break;
					
					case 1:printf("WARN:	����ʧ�ܣ�Э�����\r\n");break;
					case 2:printf("WARN:	����ʧ�ܣ��Ƿ���clientid\r\n");break;
					case 3:printf("WARN:	����ʧ�ܣ�������ʧ��\r\n");break;
					case 4:printf("WARN:	����ʧ�ܣ��û������������\r\n");break;
					case 5:printf("WARN:	����ʧ�ܣ��Ƿ�����(����token�Ƿ�)\r\n");break;
					
					default:printf("ERR:	����ʧ�ܣ�δ֪����\r\n");break;
				}
			}
		}
		
		MQTT_DeleteBuffer(&mqttPacket);								//ɾ��
	}
	else
		printf("WARN:	MQTT_PacketConnect Failed\r\n");
	
	return status;
	
}
u8 velue0 = 0;
u8 velue1 = 0;
unsigned char OneNet_FillBuf(char *buf)
{
	char text[32];
	
	memset(text, 0, sizeof(text));
	
	strcpy(buf, ",;");
		
	memset(text, 0, sizeof(text));
	sprintf(text, "Blue_Led,%d;", velue0);
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "Yellow_Led,%d;", velue1);
	strcat(buf, text);
	
	return strlen(buf);
}
 
//==========================================================
//	�������ƣ�	OneNet_SendData
//
//	�������ܣ�	�ϴ����ݵ�ƽ̨
//
//	��ڲ�����	type���������ݵĸ�ʽ
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void OneNet_SendData(char *Tip)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};												//Э���
	
	char buf[128];
	
	short body_len = 0, i = 0;
	
	//printf("Tips:%s\r\n",Tip);
	
	memset(buf, 0, sizeof(buf));
	
	body_len = OneNet_FillBuf(buf);																	//��ȡ��ǰ��Ҫ���͵����������ܳ���
	
	if(body_len)
	{
		if(MQTT_PacketSaveData(DEVID, body_len, NULL, 5, &mqttPacket) == 0)							//���
		{
			for(; i < body_len; i++)
				mqttPacket._data[mqttPacket._len++] = buf[i];
			
			ESP8266_SendData(mqttPacket._data, mqttPacket._len);									//�ϴ����ݵ�ƽ̨
			//printf("Send %d Bytes\r\n", mqttPacket._data);
			
			MQTT_DeleteBuffer(&mqttPacket);															//ɾ��
		}
		else
			printf("WARN:	EDP_NewBuffer Failed\r\n");
	}
	
}
//==========================================================
//	�������ƣ�	OneNet_Publish
//
//	�������ܣ�	������Ϣ
//
//	��ڲ�����	topic������������
//				msg����Ϣ����
//
//	���ز�����	0-�ɹ�	1-��Ҫ����
//
//	˵����		
//==========================================================
_Bool OneNet_Publish(const char *topic, const char *msg)
{
 
	//Delay_ms(100);
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};							//Э���
	
	//printf( "Publish Topic: %s, Msg: %s\r\n", topic, msg);
	
	if(MQTT_PacketPublish(MQTT_PUBLISH_ID, topic, msg, strlen(msg), MQTT_QOS_LEVEL2, 0, 1, &mqttPacket) != 1)
	{
 
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);					//��ƽ̨���Ͷ�������
		MQTT_DeleteBuffer(&mqttPacket);											//ɾ��
	}
	return 0;
 
}
//==========================================================
//	�������ƣ�	OneNet_Subscribe
//
//	�������ܣ�	����
//
//	��ڲ�����	topics�����ĵ�topic
//				topic_cnt��topic����
//
//	���ز�����	SEND_TYPE_OK-�ɹ�	SEND_TYPE_SUBSCRIBE-��Ҫ�ط�
//
//	˵����		
//==========================================================
_Bool OneNet_Subscribe(const char *topics[], unsigned char topic_cnt)
{
	
	unsigned char i = 0;
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};							//Э���
	
	for(; i < topic_cnt; i++)
		printf( "Subscribe Topic: %s\r\n", topics[i]);
	
	if(MQTT_PacketSubscribe(MQTT_SUBSCRIBE_ID, MQTT_QOS_LEVEL2, topics, topic_cnt, &mqttPacket) == 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);					//��ƽ̨���Ͷ�������
		
		MQTT_DeleteBuffer(&mqttPacket);											//ɾ��
	}
  return 0;
}
 
//==========================================================
//	�������ƣ�	OneNet_HeartBeat
//
//	�������ܣ�	�������
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void OneNet_HeartBeat(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};
	
	unsigned char sCount = 3;
	
//---------------------------------------------����һ�����---------------------------------------------
	if(MQTT_PacketPing(&mqttPacket))
		return;
	
	while(sCount--)
	{
//---------------------------------------------���������������-----------------------------------------
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);
	  //while(OneNet_DevLink(Tips))
//---------------------------------------------��������������������-------------------------------------
//		if(MQTT_UnPacketRecv(cmd) == MQTT_PKT_PINGRESP)
//		{
//			printf( "Tips:	HeartBeat OK\r\n");
//			
//			break;
//		}
//		else
//		{
//			//ESP8266_Init();					//��ʼ��ESP8266
//			printf("Check Device\r\n");
//		}
		
		Delay_ms(10);
	}
	
//---------------------------------------------�����ģ�ɾ��---------------------------------------------
	MQTT_DeleteBuffer(&mqttPacket);
 
}
/**
  * @brief  ����ַ����Ȳ���ȡ͸��
  * @param  ��
  * @retval ��
  */
char my_strlen(int8 **payload)//?const?????
{
	static char cmd[200];
	int i=0;
	while(**payload)
	{
		cmd[i] = **payload;
		i++;
		payload++;
		printf("%d\n",cmd[i]);
	}
 
	printf("%c\n",cmd[100]);
	printf("%c%c%c%c\n",cmd[111],cmd[112],cmd[113],cmd[114]);
	return cmd[11];
}
//==========================================================
//	�������ƣ�	OneNet_RevPro
//
//	�������ܣ�	ƽ̨�������ݼ��
//
//	��ڲ�����	dataPtr��ƽ̨���ص�����
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void OneNet_RevPro(unsigned char *cmd)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};								//Э���
	
	char *req_payload = NULL;
	char *cmdid_topic = NULL;
	
	unsigned short topic_len = 0;
	unsigned short req_len = 0;
	
	unsigned char type = 0;
	unsigned char qos = 0;
	static unsigned short pkt_id = 0;
	
	short result = 0;
 
	char *dataPtr = NULL;
	char numBuf[10];
	int num = 0;
	uint16_t ID = 0;
	type = MQTT_UnPacketRecv(cmd);
	switch(type)
	{
 
		case MQTT_PKT_CMD:															//�����·�
			
			result = MQTT_UnPacketCmd(cmd, &cmdid_topic, &req_payload, &req_len);	//���topic����Ϣ��
			if(result == 0)
			{
				printf( "cmdid: %s, req: %s, req_len: %d\r\n", cmdid_topic, req_payload, req_len);
				
				if(MQTT_PacketCmdResp(cmdid_topic, req_payload, &mqttPacket) == 0)	//����ظ����
				{
					printf( "Tips:	Send CmdResp\r\n");
					
					ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//�ظ�����
					MQTT_DeleteBuffer(&mqttPacket);									//ɾ��
				}
			}
		
		break;
			
		case MQTT_PKT_PUBLISH:														//���յ�Publish��Ϣ
		
			result = MQTT_UnPacketPublish(cmd, &cmdid_topic, &topic_len, &req_payload, &req_len, &qos, &pkt_id);
			if(result == 0)
			{
				printf( "topic: %s, topic_len: %d, payload: %s, payload_len: %d\r\n",
																	cmdid_topic, topic_len, req_payload, req_len);
	
				switch(qos)
				{
					case 1:															//�յ�publish��qosΪ1���豸��Ҫ�ظ�Ack
					
						if(MQTT_PacketPublishAck(pkt_id, &mqttPacket) == 0)
						{
							printf( "Tips:	Send PublishAck\r\n");
							ESP8266_SendData(mqttPacket._data, mqttPacket._len);
							MQTT_DeleteBuffer(&mqttPacket);
						}
					
					break;
					
					case 2:															//�յ�publish��qosΪ2���豸�Ȼظ�Rec
																					//ƽ̨�ظ�Rel���豸�ٻظ�Comp
						if(MQTT_PacketPublishRec(pkt_id, &mqttPacket) == 0)
						{
							printf( "Tips:	Send PublishRec\r\n");
							ESP8266_SendData(mqttPacket._data, mqttPacket._len);
							MQTT_DeleteBuffer(&mqttPacket);
						}
					
					break;
					
					default:
						break;
				}
			}
		
		break;
			
		case MQTT_PKT_PUBACK:														//����Publish��Ϣ��ƽ̨�ظ���Ack
		
			if(MQTT_UnPacketPublishAck(cmd) == 0)
				printf( "Tips:	MQTT Publish Send OK\r\n");
			
		break;
			
		case MQTT_PKT_PUBREC:														//����Publish��Ϣ��ƽ̨�ظ���Rec���豸��ظ�Rel��Ϣ
		
			if(MQTT_UnPacketPublishRec(cmd) == 0)
			{
				printf( "Tips:	Rev PublishRec\r\n");
				if(MQTT_PacketPublishRel(MQTT_PUBLISH_ID, &mqttPacket) == 0)
				{
					printf( "Tips:	Send PublishRel\r\n");
					ESP8266_SendData(mqttPacket._data, mqttPacket._len);
					MQTT_DeleteBuffer(&mqttPacket);
				}
			}
		
		break;
			
		case MQTT_PKT_PUBREL:														//�յ�Publish��Ϣ���豸�ظ�Rec��ƽ̨�ظ���Rel���豸���ٻظ�Comp
			
			if(MQTT_UnPacketPublishRel(cmd, pkt_id) == 0)
			{
				printf( "Tips:	Rev PublishRel\r\n");
				if(MQTT_PacketPublishComp(MQTT_PUBLISH_ID, &mqttPacket) == 0)
				{
					printf( "Tips:	Send PublishComp\r\n");
					ESP8266_SendData(mqttPacket._data, mqttPacket._len);
					MQTT_DeleteBuffer(&mqttPacket);
				}
			}
		
		break;
		
		case MQTT_PKT_PUBCOMP:														//����Publish��Ϣ��ƽ̨����Rec���豸�ظ�Rel��ƽ̨�ٷ��ص�Comp
		
			if(MQTT_UnPacketPublishComp(cmd) == 0)
			{
				printf( "Tips:	Rev PublishComp\r\n");
			}
		
		break;
			
		case MQTT_PKT_SUBACK:														//����Subscribe��Ϣ��Ack
		
			if(MQTT_UnPacketSubscribe(cmd) == 0)
				printf( "Tips:	MQTT Subscribe OK\r\n");
			else
				printf( "Tips:	MQTT Subscribe Err\r\n");
		
		break;
			
		case MQTT_PKT_UNSUBACK:														//����UnSubscribe��Ϣ��Ack
		
			if(MQTT_UnPacketUnSubscribe(cmd) == 0)
				printf( "Tips:	MQTT UnSubscribe OK\r\n");
			else
				printf( "Tips:	MQTT UnSubscribe Err\r\n");
		
		break;
		
		default:
			result = -1;
		break;
	}
	
	ESP8266_Clear();									//��ջ���
	
	if(result == -1)
		return;
	
		dataPtr = strchr(req_payload, ':');					//����':'
 
	if(dataPtr != NULL && result != -1)					//����ҵ���
	{
		dataPtr++;
		printf("Yes");
		while(*dataPtr >= '0' && *dataPtr <= '9')		//�ж��Ƿ����·��������������
		{
			numBuf[num++] = *dataPtr++;
		}
		numBuf[num] = 0;
		
		num = atoi((const char *)numBuf);				//תΪ��ֵ��ʽ
		printf( "num:%d\r\n",num);
		if(strstr((char *)req_payload, "LED"))		//����"redled"
		{
			//LED
		
			//Mqtt_LED(Tips,num);
		}
		else if(strstr((char *)req_payload, "TOUCH"))
		{
			//ָ��
			dataPtr = strchr(req_payload, '-');					//����'-'
			if(dataPtr != NULL && result != -1)					//����ҵ���
			{
				dataPtr++;
				while(*dataPtr >= '0' && *dataPtr <= '9')		//�ж��Ƿ����·��������������
				{
					numBuf[ID++] = *dataPtr++;
				}
				numBuf[ID] = 0;
				ID = atoi((const char *)numBuf);				//תΪ��ֵ��ʽ
				printf("ID:%d\r\n",ID);
				//Mqtt_Task( Tips, num, ID);
			}else{
				//Mqtt_Task(Tips, num, ID);
			}
			
		}
		else if(strstr((char *)req_payload, "DOOR"))
		{
			//�������
			//Door(Tips,num);
		}
		
 
	}
 
	if(type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
	{
		MQTT_FreeBuffer(cmdid_topic);
		MQTT_FreeBuffer(req_payload);
	}
 
}