#include "stm32f4xx.h"
#include "uart2.h"  //현재 디렉토리에서 찾게 하려면 ""로 묶어줘야함. <>는 c파일 내부에서 찾음.
#include <stdio.h>  //경고도 에러야! 컴파일러가 문제가 없다는거지 정상동작을 위해선 경고도 잡아줘야함.
/******************************************************************************
  포트 연결:
    1)  ARM Cortex-M4 모듈의 포트C (PC0 ~ PC7)를 8핀 케이블를 이용해서
        Array FND모듈의 SA_A ~ SA_H에 연결한다. (SA_A가 PC0로 연결되야 한다.)
    2)  ARM Cortex-M4 모듈의 포트C (PC8 ~ PC11)를 4핀 케이블를 이용해서
        Array FND모듈의 C0 ~ C3에 연결한다. (C0가 PC8로 연결되야 한다.)
    3) Cortex-M4모듈의 포트A의 0번핀(PA0)를 가변저항모듈의 VRES핀에 2핀케이블로
       연결한다.
******************************************************************************/
volatile uint16_t adc_data = 0; // 인터럽트 핸들러와 메인함수가 공유
volatile int adc1Flag = 0; //인터럽트 발생하면 true로 바꿔줌

// delay 함수
static void Delay(const uint32_t Count)
{
  __IO uint32_t index = 0; 
  for(index = (16800 * Count); index != 0; index--);
}

// 7-세그먼트 폰트를 배열로 지정한다.
unsigned char Font[18] = {0x3F, 0X06, 0X5B, 0X4F, 
                                         0X66, 0X6D, 0X7C, 0X07,
                                         0X7F, 0X67, 0X77, 0X7C, 
                                         0X39, 0X5E, 0X79, 0X71, 
                                         0X08, 0X80};

void ADC_IRQHandler(void)
{ 
  if(ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET) 
  {
    ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
    adc_data = ADC_GetConversionValue(ADC1);
    adc1Flag = 1;
  }
}

void Segment ( int N )  // Segment 함수 선언
{
  int  i ;
  unsigned char N1000, N100, N10, N1 ;
  int Buff ;

  N1000 = N /1000;  // 세그먼트에서 사용하는 천의 자리를 추출
  Buff = N % 1000 ;
  N100 = Buff / 100 ; // 세그먼트에서 사용하는 백의자리 추출
  Buff = Buff % 100;
  N10 = Buff /10 ;     // 세그먼트에서 사용하는 십의 자리 추출
  N1 =  Buff % 10 ;    // 세그먼트에서 사용하는 일의 자리 추출      

  for( i = 0 ; i < 30; i++ )
  {
    GPIO_Write(GPIOC, Font[N1000]|0x0e00); // 왼쪽 첫 번째 세그먼트를 ON하고, 천의 자리  숫자를 출력해 준다 
    Delay(1);

    GPIO_Write(GPIOC, Font[N100]|0x0d00); // 왼쪽 두 번째 세그먼트를 ON하고, 백의 자리  숫자를 출력해 준다 
    Delay(1); 

    GPIO_Write(GPIOC, Font[N10]|0x0b00);  // 왼쪽 세 번째 세그먼트를 ON하고, 십의 자리  숫자를 출력해 준다 
    Delay(1);

    GPIO_Write(GPIOC, Font[N1]|0x0700);   // 왼쪽 네 번째 세그먼트를 ON하고, 일의 자리  숫자를 출력해 준다 
    Delay(1); 
  }
}


int main()
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
                                GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|
                                GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
                                                                                                                                                                                     
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;             // ADC123_IN0
  //모드 자체에 아날로그가 있기 때문에 굳이 alternate function config가 필요가 없다.
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  //인터럽터 넘버 공유
  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  //MCU가 고급일수록 adc 설정할 수 있는 방식이 많다.
  //adc를 독립적으로 동작시키겠다
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  //클럭. p358에 나와있음. 시간에 민감하다면 손댈만 하다. 타이머는 시간이 정확해야하기에 계산하지만 adc는 손대지 않아도 어느정도 돌아감.
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  //adc가 채널이 많아지면 값을 받아오기 복잡함. 만약 perriph에서 읽어와서 타이머에 쓴다면. DMA를 쓰면 CPU가 간섭을 안함.즉 주변장치끼리 다이렉트로 정보를 전송가능함
  //즉 CPU는 resource를 세이브할 수 있음.
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  //분해능 조절 가능
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  // 채널이 여러개일 때 사용
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  //enable하면 내가 멈출 때 까지 계속 변환한다.
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  //
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  //16비트 자료형의 좌측이냐 우측이냐 정렬
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  //채널과 순서 번호임.
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_3Cycles);     // ADC1의 채널0 사용

  ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
  ADC_Cmd(ADC1, ENABLE);
  UART2_init();
  printf("start main()");
  ADC_SoftwareStartConv(ADC1);

  while(1)
  {
    //소프트웨어적으로 스타트 시키는 방법.
    //타이머로 구동시키는 방식도 있다.
    //변환이 됐는지 안됐는지 체크하는 bit = adc_eoc, 변환이 완료되면 0이 되어서 반복문을 빠져나온다.
   // while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
    //변수 초기화
  //  ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
    int adc_val = 0;
    //adc값 읽어오기
    if(adc1Flag)
    {
      adc_val= adc_data;
      printf("adc: %d, volt : %f\r\n", adc_val, 3.3/4096*adc_val);
      adc1Flag = 0;
    }
    Segment(adc_data);
  }
}
