/******************************************************************************
  포트 연결:
    1)  ARM Cortex-M4 모듈의 포트C (PC0 ~ PC7)를 8핀 케이블를 이용해서
        LED모듈의 LED0 ~ LED7에 연결한다. (LED0가 PC0로 연결되야 한다.)
    2)  ARM Cortex-M4 모듈의 포트A (PA0 ~ PA7)를 8핀 케이블를 이용해서
        Dip Switch모듈의 DIP0 ~ DIP7에 연결한다. (DIP0가 PA0로 연결되야 한다.)
******************************************************************************/
// stm32f4xx의 각 레지스터들을 정의한 헤더파일
#include "stm32f4xx.h"
#include <stdio.h>
#include "uart2.h"

int main()
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
                                             GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; // C 포트 하위 8비트를 출력으로 선언한다.
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|
                                             GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; // C 포트 하위 8비트를 출력으로 선언한다.

                                      // A 포트 하위 8비트를 입력으로 선언한다.
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;      
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  UART2_init();                                                      
  printf("Start Main()\n\r");
  

  while(1)
  {
    GPIO_Write(GPIOC,GPIO_ReadInputData(GPIOB)&0xFF00);// Dip 스위치 입력을 포트 E의 LED로 직접 출력한다.
    printf("%x", GPIO_ReadInputData(GPIOB));    
  }
}

