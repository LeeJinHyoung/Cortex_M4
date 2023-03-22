#include "stm32f4xx.h"
#include "uart2.h"
#include "fnd.h"
#include <stdio.h>
#include <stdlib.h>


extern volatile unsigned char rx2Flag;
extern char rx2Data[50];

static void Delay(const uint32_t Count)   //스태틱은 그 함수 내에서만 호출할 때 써야하는데 글로벌 함수가 되었기 때문에 스태틱을 뺸다
{
  __IO uint32_t index = 0; 
  for(index = (16800 * Count); index != 0; index--);
} 

void FND_init();
void display_digit(int positon,int number);

// delay 함수



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

