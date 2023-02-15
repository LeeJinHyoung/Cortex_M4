/******************************************************************************
  ��Ʈ ����:
    1)  ARM Cortex-M4 ����� ��ƮA�� 9~10������(PA9~PA10) 2�� ���̺��
        UART����� RXD, TXD �ɿ� �����Ѵ�.
******************************************************************************/
// stm32f4xx�� �� �������͵��� ������ �������
#include "stm32f4xx.h"// �⺻������ ���ԵǾ����

#include <stdio.h>
#include <string.h>
#include <stdlib.h>//atoi�Լ�

#define CMD_SIZE 50//�ѹ��� ���� ������ ��
#define ARR_CNT 5  //����Ʈ�迭�� �ε�������


volatile unsigned char rx1Flag = 0;//���ͷ�Ʈ���� ��ƾ�� �����Լ����� �����ϴ� ���� ���������� �� �����ؾ��� volatile�� ��� �ü���� ���⶧���� 
char rx1Data[50];//����Ʈ��ȣ�� ���� �μ��� ������ ������
//�Լ������� �����Ϸ��� ������ ���� �����ؼ� ������Ͽ� ������� 
void Serial_Event(); //������ ���鼭 �齽���ÿ����� �����͸� ó���ϱ����� 
void Serial_Send(unsigned char t);//�ѹ���Ʈ�� �����ϴ� �Լ�
void Serial_Send_String(char* s);// ���ڿ��� �����ϴ� �Լ� �ι��ڳ��ö����� �����ϴ� ������ ȣ��
int putchar(int ch);//����Ʈ������������ ���� �Լ�
void print_2d1(double number);// ����Ʈ���� �ϳ����� 20K�޸𸮰� ���� ������ ���� ������Ҷ��� ���� �������� ������ ���ڸ� ����ϰ� ������ �Լ��� �������
//�������ڸ� �Ҽ����ڸ�
int main()
{
  GPIO_InitTypeDef   GPIO_InitStructure; //GPIO�ʱⰪ����
  USART_InitTypeDef USART_InitStructure;//����Ʈ�� �ʱ�ȭ�ϱ����� ����ü
  NVIC_InitTypeDef   NVIC_InitStructure; //��ø������ 
//�ʱ�ȭ���ϸ� �����Ⱚ

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //��� ��� Ŭ������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                   //LED���Ŵϱ�
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                 //0�̳� 1�̳�
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;         //����������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;            //����Ǯ��Ǯ�ٿ� �� ����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;        
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;// �ΰ���ɾ�����
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�����ϰ� �Ȱ��� �Ǿ��־ �ʱ�ȭ�� �ʿ����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�⺻���°� ���̿��� Ǯ��Ǯ�ٿ�
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure); //���� �ΰ������ �������� �ؿ��� �Ҵ��Ұ�
  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);     //USART1_TX  <- ����Ʈ���� GPIO�� ���ľ��ؼ� �ٽ��ʱ�ȭ�ϴ°���
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);    //USART1_RX
  
//���ͷ�Ʈ ��Ʈ�ѷ� NVIC �� �����ֺ���ġ���� �� ���ͷ�Ʈ�� �Ҵ�������� ����Ʈ���ͷ�Ʈ�� ����ũ������ũ �������� ������ 
  //�켱������ ���� ���ͷ�Ʈ�� �����θ��� �����̶���� �׷��� ���ͷ�Ʈ��ø����
  //���ͷ�Ʈ enable �� Priority ����.
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�켱���� ��������������� �׷쳪���°� NVIC_PriorityGroup_2 : 2��Ʈ ���� 2��Ʈ ���� �� ���ڴ�
  //�����׷��̸� ��������. ����� ���ÿ� ���ͷ�Ʈ�� �߻������� ���������̾�Ƽ �켱������ ������(���� 0 ) ���� 10 01 10 01  
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; //�̹� ���ͷ�Ʈ���񽺷�ƾ�� �����ǵǾ����� STM32F4XX.H ����� ���ǵǾ�����
  //STM3214XX_IT.C �ھ�� �߻��ϴ� �ڵ鷯 ���� ���� ���������� ���ͷ�Ʈ 2�� : ���͸��ٵǸ� ������ �˸��� �������ϰ� �����ϴϱ�,  
  //STARTUP_STM3214_41XXX.S ���ͷ�Ʈ ���� ���̺� DCD 0������ 4�� �� 8����// ��� ���ͷ�Ʈ �ּ� ���ǵǾ�����
  // DCD     USART1_IRQHandler                 ; USART1   �Լ��̸��� ��� ������ ù��°�ּҰ��� ȣ��� NVIC�� ���ּҷ� ���� 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; // ����
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; // ���� ���ÿ� �߻������� �켱���� �����ָ� ���ٴ�.
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 
  NVIC_Init(&NVIC_InitStructure);//

  USART_InitStructure.USART_BaudRate = 115200; //����ü ��� ���� ����� ���Ǵ�� �����ִ°� //1�ʿ� 115200��Ʈ ����
  USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�ϳ��� ������ 5-9��Ʈ������ ���� �����Ҽ� �մµ� �����ϱ��������� ����Ʈ8 8��Ʈ�� ����
  USART_InitStructure.USART_StopBits = USART_StopBits_1; // ��ž��Ʈ�� �ӵ����� 1
  USART_InitStructure.USART_Parity = USART_Parity_No; //�丮Ʈ ��Ʈ�� ��ž��Ʈ �տ�����, �丮Ʈ ��Ʈ�� �߰��ؼ� 1�ǰ��� ¦��Ȥ�� Ȧ���� �������  �Ѻ�Ʈ�� ������ ������������ üũ�ϴ� ���
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;// PC�� �� 
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�ΰ��� ���尡 ����ϴµ� �ѹ������� �ϰٴ� ���̺����ϰų� �ʿ信������// ���̵���Ź��: �Ѵ� ����ϰٴٻ���
  USART_Init(USART1, &USART_InitStructure);

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // USART1 Interrupt enable //USART_ITConfig�������ͷ�Ʈ RX�ҽ��� ���ͷ�Ʈ�ϰڴ�
  USART_Cmd(USART1, ENABLE);//?

  Serial_Send_String("Start Main()\n\r");
  while(1)//168MG
  {
    if(rx1Flag)  // '\r' ���� �ԷµǸ�
      Serial_Event();
    //�÷��׸� ������ ���
    //RPOS �̿�
  }
}

void Serial_Event() //�µ���
{
  int i=0;
  int num = 0;
  char * pToken;
  char * pArray[ARR_CNT]={0};//ĳ������ �ּ�����
  char recvBuf[CMD_SIZE]={0};   //rx�� ���� �����͸� ����  �����߿� ������ ������ �ȵǴϱ�    
  strcpy(recvBuf,rx1Data); 

  rx1Flag = 0; // �ٽ� Rflag �� 0���� ���´�. ���ó���� ������ ��������
  Serial_Send_String(recvBuf);  //��ݹ��� ��ɾ �״�� ������
  Serial_Send_String("\n\r");//s�� 200���� "S"�������� �ι��ھƴ� //���������� \n(�ٹٲ�)���� �س��� \r ĳ��������
  printf("rx : %s\r\n",recvBuf); //������
  printf("test : %f\r\n",12.3); //������
  printf("\n\r");//������
  print_2d1(12.3);//������
  pToken = strtok(recvBuf,"[@]"); //���ڿ� @�� ����(strrok�Լ�)  �ι��ڷ� ��ü�ع��� �� �ּҰ� ���� ����ġ�ܿ� �ٽþ�������?

  while(pToken != NULL) //�����ͺ��� 0x01�� ���̳�
  {
    pArray[i] =  pToken; //0�ε����� 0x01�ּҰ��ְٴ�
    if(++i >= ARR_CNT)  //
      break;
    pToken = strtok(NULL,"[@]");//��Ӻи��Ҷ��� ���̿;��� //PArayy[0] : 0x01 "RHJ_ARM" ù��° ���� ���̱⶧���� �ƹ��͵� ���Ͼȵ�?
  }
       
  if(!strcmp(pArray[1],"LED")) //�ι��ڳ��ö��������ڿ�
  { //�����ϸ� 0����
    if(pArray[3] != NULL)              
      num = atoi(pArray[3]); //0�̶�� ���ڿ��� ���ڷ� �ٲ� 2����Ʈ (�ι�������)
    else
      num = 0;
    if(!strcmp(pArray[2],"ON"))
    {
     GPIO_SetBits(GPIOC, 0x01 << num); //Ű��?
    }
    else 
    {
      GPIO_ResetBits(GPIOC, 0x01 << num); 
    }
  }  
}

void Serial_Send(unsigned char t)
{
  USART_SendData(USART1, t); //��������Ʈ 8��Ʈ�� ��������?
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); //1�ϵɶ��� ���Ϲ����ٴ� ���ۿ� ���������� ��ñ�ٸ��ٴ� RESET 0 SET1 ��������
}

void Serial_Send_String(char* s)
{
  while( *s != '\0') //201���� ����Ÿ ----->���̸� ����
  {
   Serial_Send((unsigned char)(*s)); //�ٽù��� ����ȯ
   s++;   //s = s + 1;
  }
}

int putchar(int ch)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART1,ch);
	return ch;
}

void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//���ͷ�Ʈ�߻��ߴ��� Ȯ�� 8��Ʈ�� �餷���� ���ͷ�Ʈ�߻� �츮�� �׷��� ������ ����Ƽ�Ѵ��� Ȯ��
  {
	static int i=0; //ó���ѹ��� �ʱ�ȭ ���������� �������ؾ��ؼ�
	rx1Data[i] = USART_ReceiveData(USART1); //�Ѻ�Ʈ �о�� 0���� S����
	if(rx1Data[i] == '\r')   //���ϴ� ��ɾ ���о����� 
	{
		rx1Data[i] = '\0'; //���ڿ� �ϼ���Ŵ 50�������� ����
		rx1Flag = 1; //�ϳ��� ��ɾ ���ſϷ�ƴٰ� ǥ��
		i = 0;//�ٽ� 0���ͽ����ϵ���
	}
	else
	{
		i++;
	}
  }
}

void print_2d1(double number) //numer:<-12.34		        /* floating-point number xx.x */
{ 
        unsigned int i, j;

	j = (int)(number*10. + 0.5); //�ݿø��ϱ����� 123.9 INT����ȯ 123
	i = j / 100;	//i=1 				// 10^1
	if(i == 0) Serial_Send(' '); // ����
	else       Serial_Send(i + '0'); //������ �ƽ�Ű���ڷ� ��������� ���

	j = j % 100;	//�ι�° �ڸ� ��� i=2				// 10^0
	i = j / 10;
	Serial_Send(i + '0'); //2���
	Serial_Send('.'); //�Ҽ������

	i = j % 10;	//3���				// 10^-1
	Serial_Send(i + '0');
      //�Ҽ� ��°�ڸ� 100������ 0.005���ϱ� 
}