#include "stm32f4xx.h"
#include "uart2.h"

volatile unsigned char rx2Flag = 0; //uart���� �� ����Ʈ�� ���ŵǸ� \n�� ���ŵǸ� rxflag2�� true�� ����.
char rx2Data[50];



void UART2_init()  //���ο��� �����ϰ� �ʱ�ȭ�������.
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
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;  //rx tx�� ���� �� ���� �� ��� ��������. �� �� �ϳ��� �ʿ��� ��� �ϳ��� �ص� ��
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);     //USART2_TX
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);    //USART3_RX

  //���ͷ�Ʈ enable �� Priority ����.
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);       //�̹� ���ͷ�Ʈ�� ó���Ǵ� �߿��� �� ���� ���ͷ�Ʈ�� �߻��̵Ǹ� ���� �߻��ϴ� ��Ʈ�� ���߰� �� ���� ���ͷ�Ʈ ���� �ߵ�. �̸� �����̶�� ��.
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; //���ͷ�Ʈ ��ƾ, ���ͷ�Ʈ ä���� �Ҵ�����.
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //������ ���� �켱����
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; //���� �켱������ ���ÿ� �߻����� ���� �켱����
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;  //Ȧ���� ¦����
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // �ϵ���� �帧���� ����. sync�� ���߱� �����ε� �񵿱�Ŀ���  �� ������� ����
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);

  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // USART2 Interrupt enable
  USART_Cmd(USART2, ENABLE); //���� ���۽�Ű�°� Ŀ�ǵ峯���� �ʱ�ȭ ����  UART2 Ȱ��ȭ ��Ŵ.

}

void Serial2_Send(unsigned char t)
{
  USART_SendData(USART2, t); // �� ����Ʈ ����
  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); //�� ������ �ƴ��� Ȯ��.RESET�� �Ǹ� ����ִٴ� ��. ���� �����͸� �� ���� �� ���� ��� ���
}

void Serial2_Send_String(char* s)
{
  while( *s != '\0')
  {
   Serial2_Send((unsigned char)(*s)); //�ι��ڰ� ���� �� ���� �� ����Ʈ ����
   s++;   //s = s + 1; �� ���� �ּҷ� �̵�
  }
}

int putchar(int ch)  //  �� ����Ʈ�� ������ �Լ� Seiral send�� ������ printf ����� �� putchar�� ���� ȣ����
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
	if(rx2Data[i] == '\r')  // putty���� \r�� ���� �����ϱ� �̷��� �ۼ������� �ٲ���� ���ɼ��� �ִ�. 
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

//������� ���� ���°Ű� printf �ʹ� ���ſ� �Լ��� ������� �ʴ� ���� ����ȴ�.
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