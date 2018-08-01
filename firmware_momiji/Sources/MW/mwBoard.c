#include "mwboard.h"

#define PRESS_TIME 100

uint8_t SW1, SW2, SW3, SW4;
uint8_t running = 0;
uint8_t keyEnable = 0;

static void _delayMS_noob(uint32_t time)
{
   uint32_t i=0;   
   while(time--) 
   { 
      i=12000;
      while(i--) ;     
   } 
}

static void _delayMS(uint32_t time)
{
	static uint64_t delayStart;
	delayStart = mwBoard.Nowticks;
	while(mwBoard.Nowticks - delayStart < time);
}

static void pinKey1(uint8_t on)
{
	if(on)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_1);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
	}
}
static void pinKey2(uint8_t on)
{
	if(on)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_10);
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	}
}
static void pinKey3(uint8_t on)
{
	if(on)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_12);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_12);
	}
}
static void pinKey4(uint8_t on)
{
	if(on)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_11);
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_11);
	}
}

void pressKey1(void)
{
	pinKey1(1);
	mwBoard.DelayMS(PRESS_TIME);
	pinKey1(0);
}

void pressKey2(void)
{
	pinKey2(1);
	mwBoard.DelayMS(PRESS_TIME);
	pinKey2(0);
}

void pressKey3(void)
{
	pinKey3(1);
	mwBoard.DelayMS(PRESS_TIME);
	pinKey3(0);
}

void pressKey4(void)
{
	pinKey4(1);
	mwBoard.DelayMS(PRESS_TIME);
	pinKey4(0);
}

//////////////////////////////////////////////////


mwBoard_t mwBoard;

//static uint64_t delayStart;

__weak void mwBoardCOM1RecvByteHook(uint8_t byte)
{
}

__weak void mwBoardCOM2RecvByteHook(uint8_t byte)
{
}

__weak void mwBoardCOM2RecvStopHook(void)
{
}

void SysTick_Handler(void)
{
	mwBoard.Nowticks ++;
}

void USART1_IRQHandler(void)
{
	uint8_t byte;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		byte = USART_ReceiveData(USART1);    
		mwBoardCOM1RecvByteHook(byte);
	}
	if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET)
  {
		USART_ReceiveData(USART1);
	}
}

void USART2_IRQHandler(void)
{
	uint8_t byte;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		byte = USART_ReceiveData(USART2);    
		mwBoardCOM2RecvByteHook(byte);
	}
	if(USART_GetITStatus(USART2, USART_IT_IDLE) == SET)
  {
		USART_ReceiveData(USART2);
		mwBoardCOM2RecvStopHook();
	}
}
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line11) == SET)
	{
		if(running)
		{
			running = 0;
			
			pinKey4(1);
			_delayMS_noob(70);
			pinKey4(0);
			
		}
		else
		{
			if(SW3)
			{
				NVIC_SystemReset();			
			}
			else
			{
				if(keyEnable)
				{
					SW4 = 1;
				}
			}
		}
		//_delay_ms(200);
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
	if(EXTI_GetITStatus(EXTI_Line13) == SET)
	{
		if(keyEnable)
		{
			SW1 = 1;
		}
		//_delay_ms(200);
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	if(EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		if(keyEnable)
		{
			SW2 = 1;
		}
		//_delay_ms(200);
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	if(EXTI_GetITStatus(EXTI_Line15) == SET)
	{
		if(keyEnable)
		{
			SW3 = 1;
		}
		//_delay_ms(200);
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
//	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == RESET)
//	{
//		SW4 = 1;
//		//_delay_ms(200);
//		EXTI_ClearITPendingBit(EXTI_Line11);
//	}
//	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == RESET)
//	{
//		SW1 = 1;
//		//_delay_ms(200);
//		EXTI_ClearITPendingBit(EXTI_Line13);
//	}
//	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) == RESET)
//	{
//		SW2 = 1;
//		//_delay_ms(200);
//		EXTI_ClearITPendingBit(EXTI_Line14);
//	}
//	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) == RESET)
//	{
//		SW3 = 1;
//		//_delay_ms(200);
//		EXTI_ClearITPendingBit(EXTI_Line15);
//	}
	
}
//static void _delayMS(uint32_t span)
//{
//	delayStart = mwBoard.Nowticks;
//	while(mwBoard.Nowticks - delayStart < span);
//}

static void _SYSTickInit(void)
{
	mwBoard.Nowticks = 0;
  if(SysTick_Config(SystemCoreClock / 1000))
  { 
    /* Capture error */ 
    while (1);
  }
}

static void _RCCInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	
	
//	PWR_BackupAccessCmd(ENABLE);//�����޸�RTC �ͺ󱸼Ĵ���

//  RCC_LSICmd(DISABLE);//�ر��ⲿ�����ⲿʱ���źŹ��� ��PC14 PC15 �ſ��Ե���ͨIO�á�
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;            //ѡ���ж�ͨ��3
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //��ռʽ�ж����ȼ�����Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 15;  //��Ӧʽ�ж����ȼ�����Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                   //ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);
	
	

	//ѡ���жϹܽ�PC.2 PC.3 PC.5
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource14);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource15);
	EXTI_InitStructure.EXTI_Line = EXTI_Line11 | EXTI_Line13 | EXTI_Line14 | EXTI_Line15; //ѡ���ж���·2 3 5
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //����Ϊ�ж����󣬷��¼�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�����жϴ�����ʽΪ���½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                           //�ⲿ�ж�ʹ��
	EXTI_Init(&EXTI_InitStructure);
}

static void _PINInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	// for lcd db
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	// for lcd control
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	// for adc4 5 6 7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  	
	
	// for virtual key
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// for key
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 |  GPIO_Pin_14 | GPIO_Pin_15;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	// for uart2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_10;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
}

static void _UART1Init(void)
{
	USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;	

  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init( USART1, & USART_InitStructure );
  
  USART_Cmd( USART1 , ENABLE );
  
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 9;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure );  
  
  USART_ITConfig(USART1, USART_IT_IDLE , ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE , ENABLE);
}

static void _UART2Init(void)
{
	USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;	

  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init( USART2, & USART_InitStructure );
  
  USART_Cmd( USART2 , ENABLE );
  
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 8;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure );  
  
  USART_ITConfig(USART2, USART_IT_IDLE , ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE , ENABLE);
}


static void _ADC1Init(void)
{
	ADC_InitTypeDef ADC_InitStructure; 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

  
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
}

static uint16_t _ADC1Read(uint8_t ch)
{	
	uint16_t val;
	
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	//delay_ms(10);
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	val = ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
	
	return val;
}

static void _UART1SendByte(uint8_t byte)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET);
	USART_SendData(USART1, byte);
}

static void _UART1SendBuff(uint8_t *buff, uint16_t len)
{
	uint16_t i;
	for(i = 0; i < len; i ++)
	{
		_UART1SendByte(buff[i]);
	}
}

static void _UART2SendByte(uint8_t byte)
{
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET);
	USART_SendData(USART2, byte);
}

static void _UART2SendBuff(uint8_t *buff, uint16_t len)
{
	uint16_t i;
	for(i = 0; i < len; i ++)
	{
		_UART2SendByte(buff[i]);
	}
}
void boardInit(void)
{
	mwBoard.DelayMS = _delayMS;
	mwBoard.ADCRead = _ADC1Read;
	mwBoard.COM1SendByte = _UART1SendByte;
	mwBoard.COM2SendByte = _UART2SendByte;
	mwBoard.COM1SendBuff = _UART1SendBuff;
	mwBoard.COM2SendBuff = _UART2SendBuff;
	
	
	_RCCInit();
	_PINInit();
	
	_UART1Init();
	_UART2Init();
	
	_ADC1Init();
	
	_SYSTickInit();
	
	SW1 = 0;
	SW2 = 0;
	SW3 = 0;
	SW4 = 0;
}
