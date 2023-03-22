#include "stm32f4xx.h"
#include <stdio.h> 
#include "lcd.h"
#include "key.h"
#include <string.h>
typedef struct
{
  int msec;
  int hour;
  int min;
  int sec;
  char apm[3];
  char star;
}TIME;

TIME T;
char time_ary[16];
int al_hour = 00; int al_min = 0; int al_sec = 0; ; char al_star; char al_apm[2];
char alarm_ary[16];
extern int key;
int apmFlag = 0;

void clock_display();
void clock_set();
void clock_calc();

volatile int t_cnt = 0;
volatile unsigned long systick_count = 0;
volatile unsigned long systick_sec=0;
volatile int systick_secFlag = 0;

int main()
{
  T.hour = 12; T.min = 0; T.sec = 0; T.star = NULL; char al_star = '*';
  
  GPIO_InitTypeDef   GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
  NVIC_InitTypeDef   NVIC_InitStructure; 
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
                                GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  TIM_TimeBaseStructure.TIM_Prescaler = 84-1;         //(168Mhz/2)/84 = 1MHz(1us)
  TIM_TimeBaseStructure.TIM_Period = 1000-1;          // 1uS * 4000 = 4ms
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
  
  TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
  TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM7, ENABLE);
 
  Key_Init();
  LcdPort_Init();
 
  
  
  while(1)
  {
    clock_calc();
    clock_display();
    if(key != 0)
      clock_set();
    else return 0;
    
    if(al_hour==T.hour && al_min == T.min && al_sec == T.sec)
    GPIO_SetBits(GPIOC, 0xFF);
    else return 0;

  }
  
}

void TIM7_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    t_cnt++ ;
    systick_count++;
    if(t_cnt >= 60)
    {
      t_cnt = 0;
      systick_sec++; 
      systick_secFlag = 1;
    }
  }
}

void clock_set()
{
  if(key == 1)
  {
    al_hour++;
  }
  else if(key == 2)
  {
    al_min++;
  }
  else if(key == 3)
  {
    al_sec++;
  }
  else if(key == 4)
  {
    if(apmFlag ==0)
    {
      al_star = NULL;
      T.star = '*';
      apmFlag = 1;
    }
    else if(apmFlag == 1)
    {
      T.star = NULL;
      al_star = '*';
      apmFlag = 0;
    }
   }
 }

void clock_display()
{
 sprintf(time_ary, "%d:%d:%d AM %c", T.hour, T.min, T.sec, T.star);
 lcd(0,0, time_ary);
 sprintf(alarm_ary, "%d:%d:%d PM %c", al_hour,al_min,al_sec, al_star);
 lcd(0,1, alarm_ary);
 clock_calc();
}

void clock_calc()
{
  T.sec += systick_count;
  if(T.sec == 60)
  { 
    T.min++;
    T.sec = 0;
  }
  if(T.min == 60)
  {
    T.hour++;
    T.min = 0;
  }
  if(T.hour == 13)
  {
    T.hour = 1;
  }
  
  if(al_sec == 60)
  { 
    al_min++;
    al_sec = 0;
  }
  if(al_min == 60)
  {
    al_hour++;
    al_min = 0;
  }
  if(al_hour == 13)
  {
    al_hour = 1;
  }

}
