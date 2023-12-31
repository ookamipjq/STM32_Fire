
#include "dht11.h"
#include "delay.h"

void DHT11_IO_OUT2 (void){ //端口变为输出
	EXTI_InitTypeDef  EXTI_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitStructure.GPIO_Pin = DHT11_IO2; //选择端口号（0~15或all）                        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //选择IO接口工作方式       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）   	
	GPIO_Init(DHT11PORT2, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource12);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
}

void DHT11_IO_IN2 (void){ //端口变为输入
		GPIO_InitTypeDef  GPIO_InitStructure; 	
    GPIO_InitStructure.GPIO_Pin = DHT11_IO2; //选择端口号（0~15或all）                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //选择IO接口工作方式       
		GPIO_Init(DHT11PORT2, &GPIO_InitStructure);
}

void DHT11_RST2 (void){ //DHT11端口复位，发出起始信号（IO发送）
	DHT11_IO_OUT2();
	GPIO_ResetBits(DHT11PORT2,DHT11_IO2); //	
	Delay_ms(20); //拉低至少18ms						
	GPIO_SetBits(DHT11PORT2,DHT11_IO2); //							
	Delay_us(30); //主机拉高20~40us
}

u8 Dht11_Check2(void){ //等待DHT11回应，返回1:未检测到DHT11，返回0:成功（IO接收）	   
    u8 retry=0;
    DHT11_IO_IN2();//IO到输入状态	 
    while (GPIO_ReadInputDataBit(DHT11PORT2,DHT11_IO2)&&retry<100){//DHT11会拉低40~80us
        retry++;
        Delay_us(1);
    }	 
    if(retry>=100)return 1; else retry=0;
    while (!GPIO_ReadInputDataBit(DHT11PORT2,DHT11_IO2)&&retry<100){//DHT11拉低后会再次拉高40~80us
        retry++;
        Delay_us(1);
    }
    if(retry>=100)return 1;	    
    return 0;
}

u8 Dht11_ReadBit2(void){ //从DHT11读取一个位 返回值：1/0
    u8 retry=0;
    while(GPIO_ReadInputDataBit(DHT11PORT2,DHT11_IO2)&&retry<100){//等待变为低电平
        retry++;
        Delay_us(1);
    }
    retry=0;
    while(!GPIO_ReadInputDataBit(DHT11PORT2,DHT11_IO2)&&retry<100){//等待变高电平
        retry++;
        Delay_us(1);
    }
    Delay_us(40);//等待40us	//用于判断高低电平，即数据1或0
    if(GPIO_ReadInputDataBit(DHT11PORT2,DHT11_IO2))return 1; else return 0;		   
}

u8 Dht11_ReadByte2(void){  //从DHT11读取一个字节  返回值：读到的数据
    u8 i,dat;
    dat=0;
    for (i=0;i<8;i++){ 
        dat<<=1; 
        dat|=Dht11_ReadBit2();
    }						    
    return dat;
}

u8 DHT11_Init2 (void){	//DHT11初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE); //APB2外设时钟使能      
	DHT11_RST2();//DHT11端口复位，发出起始信号
	return Dht11_Check2(); //等待DHT11回应
}
/*
u8 DHT11_ReadData2(u8 *h){ //读取一次数据//湿度值(十进制，范围:20%~90%) ，温度值(十进制，范围:0~50°)，返回值：0,正常;1,失败 
    u8 buf[5];
    u8 i;
    DHT11_RST2();//DHT11端口复位，发出起始信号
    if(Dht11_Check2()==0){ //等待DHT11回应
        for(i=0;i<5;i++){//读取5位数据
            buf[i]=Dht11_ReadByte2(); //读出数据
        }
        if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4]){	//数据校验
            *h=buf[0]; //将湿度值放入指针1
						 h++;
            *h=buf[2]; //将温度值放入指针2
        }
    }else return 1;	
    return 0;	    
}
 */

u8 DHT11_ReadData2(u8 *temp,u8 *humi){ //读取一次数据//湿度值(十进制，范围:20%~90%) ，温度值(十进制，范围:0~50°)，返回值：0,正常;1,失败 
    u8 buf[5];
    u8 i;
    DHT11_RST2();//DHT11端口复位，发出起始信号
    if(Dht11_Check2()==0){ //等待DHT11回应
        for(i=0;i<5;i++){//读取5位数据
            buf[i]=Dht11_ReadByte2(); //读出数据
        }
        if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4]){	//数据校验
            *humi=buf[0]; //将湿度值放入指针1
						 //h++;
            *temp=buf[2]; //将温度值放入指针2
        }
    }else return 1;	
    return 0;	    
}







