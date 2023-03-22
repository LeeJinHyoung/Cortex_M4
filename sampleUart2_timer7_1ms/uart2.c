#include "stm32f4xx.h"
#include "uart2.h"

volatile unsigned char rx2Flag = 0; //uart에서 한 바이트가 수신되면 \n이 수신되면 rxflag2를 true로 만듬.
char rx2Data[50];



void UART2_init()  //메인에서 복사하고 초기화해줘야함.
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;


  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;  //rx tx를 위한 두 개의 핀 모두 정의했음. 둘 중 하나만 필요한 경우 하나만 해도 됨
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);     //USART2_TX
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);    //USART3_RX

  //인터럽트 enable 및 Priority 설정.
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);       //이미 인터럽트가 처리되는 중에도 더 높은 인터럽트가 발생이되면 지금 발생하는 포트를 멈추고 더 높은 인터럽트 순위 발동. 이를 선점이라고 함.
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; //인터럽트 루틴, 인터럽트 채널을 할당해줌.
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //선점에 대한 우선순위
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; //같은 우선순위가 동시에 발생했을 때의 우선순위
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;  //홀수냐 짝수냐
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 하드웨어 흐름제어 없음. sync를 맞추기 위함인데 비동기식에서  잘 사용하지 않음
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);

  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // USART2 Interrupt enable
  USART_Cmd(USART2, ENABLE); //실제 동작시키는거 커맨드날려서 초기화 동작  UART2 활성화 시킴.

}

void Serial2_Send(unsigned char t)
{
  USART_SendData(USART2, t); // 한 바이트 전송
  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); //다 전송이 됐는지 확인.RESET이 되면 비어있다는 것. 내가 데이터를 다 보낼 때 까지 잠시 대기
}

void Serial2_Send_String(char* s)
{
  while( *s != '\0')
  {
   Serial2_Send((unsigned char)(*s)); //널문자가 나올 때 까지 한 바이트 보냄
   s++;   //s = s + 1; 그 다음 주소로 이동
  }
}

int putchar(int ch)  //  한 바이트를 보내는 함수 Seiral send를 썼지만 printf 사용할 땐 putchar를 먼저 호출함
{ 
  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
  USART_SendData(USART2,ch);
  return ch;
}

void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
	static int i=0;
	rx2Data[i] = USART_ReceiveData(USART2);
	if(rx2Data[i] == '\r')  // putty에서 \r이 먼저 들어오니까 이렇게 작성했지만 바꿔야할 가능성이 있다. 
	{
		rx2Data[i] = '\0';
		rx2Flag = 1;
		i = 0;
	}
	else
	{
		i++;
	}
  }
}

//디버깅할 때나 쓰는거고 printf 너무 무거운 함수라 사용하지 않는 것이 권장된다.
void print_2d1(double number)		        /* floating-point number xx.x */
{ 
        unsigned int i, j;

	j = (int)(number*10. + 0.5);
	i = j / 100;					// 10^1
	if(i == 0) Serial2_Send(' ');
	else       Serial2_Send(i + '0');

	j = j % 100;					// 10^0
	i = j / 10;
	Serial2_Send(i + '0');
	Serial2_Send('.');

	i = j % 10;					// 10^-1
	Serial2_Send(i + '0');
}