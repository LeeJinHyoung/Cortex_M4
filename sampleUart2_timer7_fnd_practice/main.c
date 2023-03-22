#include "stm32f4xx.h"
#include "uart2.h"  //현재 디렉토리에서 찾게 하려면 ""로 묶어줘야함. <>는 c파일 내부에서 찾음.
#include <stdio.h>  //경고도 에러야! 컴파일러가 문제가 없다는거지 정상동작을 위해선 경고도 잡아줘야함.
#include "tim7_fnd.h"
#include <stdlib.h>
#include "key.h"
#include "lcd.h"
#include <string.h>

/******************************************************************************
* Basic timers TIM7

포트 연결:
1)  ARM Cortex-M4 모듈의 포트C (PC0 ~ PC7)를 8핀 케이블를 이용해서
Array FND모듈의 SA_A ~ SA_H에 연결한다. (SA_A가 PC0로 연결되야 한다.)
2)  ARM Cortex-M4 모듈의 포트C (PC8 ~ PC11)를 4핀 케이블를 이용해서
Array FND모듈의 C0 ~ C3에 연결한다. (C0가 PC8로 연결되야 한다.)
******************************************************************************/

// 7-세그먼트 폰트를 배열로 지정한다.
extern volatile unsigned char rx2Flag ; //uart에서 한 바이트가 수신되면 \n이 수신되면 rxflag2를 true로 만듬.
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

