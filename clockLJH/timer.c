#include "stm32f4xx.h"
#include "timer.h"
volatile int t_cnt = 0;                                      //4ms
volatile unsigned long systick_count = 0;        //4ms
volatile unsigned long systick_sec=0;            //1sec
volatile int systick_secFlag = 0;

void TIM_INTERUPT_INIT()
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
  NVIC_InitTypeDef   NVIC_InitStructure; 
  
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
}

void TIM7_IRQHandler(void)              //4ms
{

  if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    t_cnt++ ;
    systick_count++;
 
    if(t_cnt >= 100)             //1s
    {
      t_cnt = 0;
      systick_sec++; 
      systick_secFlag = 1;
    }
  }
}