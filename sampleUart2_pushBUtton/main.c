/******************************************************************************
  ��Ʈ ����:
    1)  ARM Cortex-M4 ����� ��ƮC (PC0 ~ PC7)�� 8�� ���̺� �̿��ؼ�
        LED����� LED0 ~ LED7�� �����Ѵ�. (LED0�� PC0�� ����Ǿ� �Ѵ�.)
    2)  ARM Cortex-M4 ����� ��ƮA (PA0 ~ PA7)�� 8�� ���̺� �̿��ؼ�
        Dip Switch����� DIP0 ~ DIP7�� �����Ѵ�. (DIP0�� PA0�� ����Ǿ� �Ѵ�.)
******************************************************************************/
// stm32f4xx�� �� �������͵��� ������ �������
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
                                             GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; // C ��Ʈ ���� 8��Ʈ�� ������� �����Ѵ�.
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|
                                             GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; // C ��Ʈ ���� 8��Ʈ�� ������� �����Ѵ�.

                                      // A ��Ʈ ���� 8��Ʈ�� �Է����� �����Ѵ�.
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;      
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  UART2_init();                                                      
  printf("Start Main()\n\r");
  

  while(1)
  {
    GPIO_Write(GPIOC,GPIO_ReadInputData(GPIOB)&0xFF00);// Dip ����ġ �Է��� ��Ʈ E�� LED�� ���� ����Ѵ�.
    printf("%x", GPIO_ReadInputData(GPIOB));    
  }
}

