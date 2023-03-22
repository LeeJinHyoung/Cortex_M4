#include "stm32f4xx.h"
#include "uart2.h"
#include "fnd.h"
#include <stdio.h>
#include <stdlib.h>


extern volatile unsigned char rx2Flag;
extern char rx2Data[50];

static void Delay(const uint32_t Count)   //����ƽ�� �� �Լ� �������� ȣ���� �� ����ϴµ� �۷ι� �Լ��� �Ǿ��� ������ ����ƽ�� �A��
{
  __IO uint32_t index = 0; 
  for(index = (16800 * Count); index != 0; index--);
} 

void FND_init();
void display_digit(int positon,int number);

// delay �Լ�



int main()
{

  int count = 100;
  
  FND_init();
  UART2_init();
  printf("Start main()\n\r");
  
  while(1)
  {
    if(rx2Flag)
    {
      printf("%s\r\n",rx2Data);
      count = atoi(rx2Data);
      rx2Flag = 0;
    }
    display_fnd(count); 
    Delay(10);
    count--;
    if(count < 0)
    {
      count = -count;
    }
  }
}

