#include "stm32f4xx.h"
#include "uart2.h"  //���� ���丮���� ã�� �Ϸ��� ""�� ���������. <>�� c���� ���ο��� ã��.
#include <stdio.h>  //��� ������! �����Ϸ��� ������ ���ٴ°��� �������� ���ؼ� ��� ��������.
#include "tim7_fnd.h"
#include <stdlib.h>
#include "key.h"
#include "lcd.h"
#include <string.h>

/******************************************************************************
* Basic timers TIM7

��Ʈ ����:
1)  ARM Cortex-M4 ����� ��ƮC (PC0 ~ PC7)�� 8�� ���̺� �̿��ؼ�
Array FND����� SA_A ~ SA_H�� �����Ѵ�. (SA_A�� PC0�� ����Ǿ� �Ѵ�.)
2)  ARM Cortex-M4 ����� ��ƮC (PC8 ~ PC11)�� 4�� ���̺� �̿��ؼ�
Array FND����� C0 ~ C3�� �����Ѵ�. (C0�� PC8�� ����Ǿ� �Ѵ�.)
******************************************************************************/

// 7-���׸�Ʈ ��Ʈ�� �迭�� �����Ѵ�.
extern volatile unsigned char rx2Flag ; //uart���� �� ����Ʈ�� ���ŵǸ� \n�� ���ŵǸ� rxflag2�� true�� ����.
extern char rx2Data[50];
extern volatile int fndNumber;
extern volatile unsigned long systick_count ;        //4ms
int hour = 20; int min = 39; int sec = 0;
char time[16];
int main()
{
  TIM7_Fnd_Init();
  GPIOAB_LCD_Init();
  while(1)
  {

    sprintf(time , "TIME %d:%d:%d", hour, min, sec);
    
    lcd(0,0, time);
    fndNumber = 0000;
  }

