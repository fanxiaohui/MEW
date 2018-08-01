#include "mwstm32.h"
#include "stm32f10x.h"


static uint64_t delayStart = 0;

mwSTM32Handle_t mwSTM32;

__weak void mwSTM32COM1RecvByteHook(uint8_t byte)
{
}

__weak void mwSTM32COM3RecvByteHook(uint8_t byte)
{
}

void SysTick_Handler(void)
{
	mwSTM32.Nowticks ++;
}

void USART1_IRQHandler(void)
{
	uint8_t byte;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		byte = USART_ReceiveData(USART1);    
		mwSTM32COM1RecvByteHook(byte);
	}
	if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET)
  {
		USART_ReceiveData(USART1);
	}
}

void USART3_IRQHandler(void)
{
	uint8_t byte;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		byte = USART_ReceiveData(USART3);    
		mwSTM32COM3RecvByteHook(byte);
	}
	if(USART_GetITStatus(USART3, USART_IT_IDLE) == SET)
  {
		USART_ReceiveData(USART3);
	}
}


static void stm32DelayMS(uint32_t span)
{
	delayStart = mwSTM32.Nowticks;
	while(mwSTM32.Nowticks - delayStart < span);
}

static void stm32SystickInit(void)
{
	mwSTM32.Nowticks = 0;
  if(SysTick_Config(SystemCoreClock / 1000))
  { 
    /* Capture error */ 
    while (1);
  }
}

static void stm32PinM26DTR(uint8_t on)
{
	if(on)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_11);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_11);
	}
}

static void stm32PinM26PK(uint8_t on)
{
	if(on)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_8);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	}
}

static void stm32PinM26NET(uint8_t on)
{
	if(on)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_9);
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);
	}
}
static void stm32PinLEDSTA(uint8_t on)
{
	if(on)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_6);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_6);
	}
}

static void stm32PinLEDRXD(uint8_t on)
{
	if(on)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_7);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_7);
	}
}

static void stm32PinLEDTXD(uint8_t on)
{
	if(on)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_0);
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	}
}

static void stm32PinBUSY(uint8_t on)
{
	if(on)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_1);
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	}
}

static void stm32PinsInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
  
  // pb10 = uart3 tx  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// PB11 = UART3 RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// PA9 = uart1 tx  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// PA10 = UART1 RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	// PA11 = M26 DTR  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	// PA12 = M26 RI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// PA8 = M26 PK
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	// PB9 = M26 NET
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  

	// PB1 = BUSY
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
	
	// PA1 = ADC
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	// PA4 = DAC
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	// PA6 = LED TXD
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  

	// PA7 = LED RXD
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  

	// PB0 = LED STA
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
}

static void stm32RCCInit()
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
}
static void stm32ADC1Init(void)
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

static void stm32UART1Init(void)
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

static void stm32UART1SendByte(uint8_t byte)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET);
	USART_SendData(USART1, byte);
}

static void stm32UART3Init(void)
{
	USART_InitTypeDef USART_InitStructure;
    
  NVIC_InitTypeDef NVIC_InitStructure;
	
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init( USART3, & USART_InitStructure );

  USART_Cmd( USART3 , ENABLE );

  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 8;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure );  
  
  USART_ITConfig(USART3, USART_IT_IDLE , ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE , ENABLE);
}

static void stm32UART3SendByte(uint8_t byte)
{
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) != SET);
	USART_SendData(USART3, byte);
}

static void stm32DACWrite(uint16_t val)
{
}

static uint16_t stm32ADCRead(void)
{
	uint16_t val;
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	//delay_ms(10);
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	val = ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
	
	return val;
}

void mwSTM32Init(void)
{
	mwSTM32.DelayMS = stm32DelayMS;

	mwSTM32.PinM26PK = stm32PinM26PK;
	mwSTM32.PinM26NET = stm32PinM26NET;
	mwSTM32.PinM26DTR = stm32PinM26DTR;
	mwSTM32.COM1SendByte = stm32UART1SendByte;
	mwSTM32.COM3SendByte = stm32UART3SendByte;
	mwSTM32.DACWrite = stm32DACWrite;
	mwSTM32.ADCRead = stm32ADCRead;

	mwSTM32.PinLEDTXD = stm32PinLEDTXD;
	mwSTM32.PinLEDRXD = stm32PinLEDRXD;
	mwSTM32.PinLEDSTA = stm32PinLEDSTA;
	mwSTM32.PinBUSY = stm32PinBUSY;
	
	stm32RCCInit();
	stm32PinsInit();
	stm32UART1Init();
	stm32UART3Init();
	stm32SystickInit();
	stm32ADC1Init();
}
