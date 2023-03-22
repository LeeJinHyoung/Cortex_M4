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
uint16_t adc_data;
volatile uint16_t ADCConvertedValues[2];

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  /* Configure ADC1 Channel0 & 1 pins as analog input ******************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
//******************************************************************************
void ADC1_Configuration(void)
{
  ADC_InitTypeDef ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  DMA_InitTypeDef DMA_InitStructure;
  /* Enable peripheral clocks *************************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  /* DMA2_Stream0 channel0 configuration **************************************/
  
  //주변장치끼리 데이터를 주고받는 방식
  DMA_DeInit(DMA2_Stream0);
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;
  //읽어올 주소를 직접 적어놓은 것임.
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
  //어디에 쓸 것인지
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADCConvertedValues[0];
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 2;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  /* DMA2_Stream0 enable */
  DMA_Cmd(DMA2_Stream0, ENABLE);
  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);
  /* ADC1 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 2;
  ADC_Init(ADC1, &ADC_InitStructure);
  /* ADC1 regular channel configuration ******************************/
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_480Cycles); // PA0
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_480Cycles); // PA1
  /* Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);
  /* Enable ADC1 **************************************************************/
  ADC_Cmd(ADC1, ENABLE);
  /* Start ADC1 Software Conversion */
  ADC_SoftwareStartConv(ADC1);
}
//******************************************************************************
int main(void)
{
  GPIO_Configuration();
  ADC1_Configuration();
  UART2_init();
  printf("ADC Testing\n\r");
  while(1)
  {
    //while문이 굉장히 빠르기 때문에 값을 읽어왔을 때만 출력
    if (ADCConvertedValues[1] != 0xFFFF)
    {
      printf("vres ; %d cds : %d\n\r", ADCConvertedValues[0], ADCConvertedValues[1]);
      ADCConvertedValues[1] = 0xFFFF;
    }
  }
}