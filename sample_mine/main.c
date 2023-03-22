
  #include "stm32f4xx.h"

  static void Delay(const uint32_t Count) 
  {
    __IO uint32_t index = 0; 
    for(index = (16800 * Count); index != 0; index--); 
  }
  int main() 
  {
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
      GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; 
    
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
    
    
    while(1)
    {
      GPIO_WriteBit(GPIOB, GPIO_Pin_0,Bit_SET); //  한개씩 제어할때
      Delay(500);
      GPIO_WriteBit(GPIOB, GPIO_Pin_0,Bit_RESET);
      Delay(500);
      
    }
  }
//
//  #include "stm32f4xx.h"
//
//  static void Delay(const uint32_t Count) 
//  {
//    __IO uint32_t index = 0; 
//    for(index = (16800 * Count); index != 0; index--); 
//  }
//  int main() 
//  {
//    GPIO_InitTypeDef GPIO_InitStructure;
//    
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
//    
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
//    
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
//      GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; 
//    
//    GPIO_Init(GPIOC, &GPIO_InitStructure); 
//    GPIO_Write(GPIOC, 0x55);
//    
//    while(1)
//    {
//      GPIO_ToggleBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
//      GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
//      Delay(500);
//    
//    }
//  }

//
//  #include "stm32f4xx.h"
//
//  static void Delay(const uint32_t Count) 
//  {
//    __IO uint32_t index = 0; 
//    for(index = (16800 * Count); index != 0; index--); 
//  }
//  int main() 
//  {
//    GPIO_InitTypeDef GPIO_InitStructure;
//    
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
//    
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
//    
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
//      GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; 
//    
//    GPIO_Init(GPIOC, &GPIO_InitStructure); 
//    
//    while(1)
//    {
//     
//     int ledVal =0x01;
//     for(int i =0; i<8; i++)
//     {
//        GPIO_Write(GPIOC, ledVal<<i);
//        Delay(100);
//     }
//     GPIO_Write(GPIOC, 0x00);
//     Delay(100);
//     ledVal =0x80;
//        for(int i =0; i<8; i++)
//     {
//        GPIO_Write(GPIOC, ledVal>>i);
//        Delay(100);
//     }
//          GPIO_Write(GPIOC, 0x00);
//     Delay(100);
//    }
//  }



//
//#include "stm32f4xx.h"
//
//static void Delay(const uint32_t Count) 
//{
//  __IO uint32_t index = 0; 
//  for(index = (16800 * Count); index != 0; index--); 
//}
//int main() 
//{
//  GPIO_InitTypeDef GPIO_InitStructure;
//  
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
//  
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
//  
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
//    GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; 
//  
//  GPIO_Init(GPIOC, &GPIO_InitStructure); 
//  uint8_t led_state = 0b00000001;
//  
//  while(1)
//  {
//    static int cnt = -7;
//  
//  GPIO_ResetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
//    GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); 
//    
//  GPIO_SetBits(GPIOC,led_state<<abs(cnt++)); 
//    
//    Delay(100); 
//    
//    if(cnt == 7)
//    {  GPIO_ResetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
//    GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
//    Delay(200); 
//
//    cnt=-7;}
//    
//  }
//}


//#include "stm32f4xx.h"
//
//static void Delay(const uint32_t Count) 
//{
//  __IO uint32_t index = 0; 
//  for(index = (16800 * Count); index != 0; index--); 
//}
//int main() 
//{
//  GPIO_InitTypeDef GPIO_InitStructure;
//  
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
//  
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
//  
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
//    GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; 
//  
//  GPIO_Init(GPIOC, &GPIO_InitStructure); 
//  uint8_t led_state = 0b00000001;
//  
//  while(1)
//  {
//    GPIO_Write(GPIOC, GPIO_Pin_0); // LED 0을 ON 시킨다. 
//    Delay(500); // 0.5초 딜레이 
//
//    GPIO_Write(GPIOC, GPIO_Pin_1); // LED 1을 ON 시킨다. 
//    Delay(500); // 0.5초 딜레이 
//
//    GPIO_Write(GPIOC, GPIO_Pin_2); // LED 2를 ON 시킨다. 
//    Delay(500); // 0.5초 딜레이 
//
//    GPIO_Write(GPIOC, GPIO_Pin_3); // LED 3을 ON 시킨다. 
//    Delay(500); // 0.5초 딜레이 
//
//    GPIO_Write(GPIOC, GPIO_Pin_4); // LED 4를 ON 시킨다. 
//    Delay(500); // 0.5초 딜레이 
//
//    GPIO_Write(GPIOC, GPIO_Pin_5); // LED 5를 ON 시킨다. 
//    Delay(500); // 0.5초 딜레이 
//
//    GPIO_Write(GPIOC, GPIO_Pin_6); // LED 6을 ON 시킨다. 
//    Delay(500); // 0.5초 딜레이 
//
//    GPIO_Write(GPIOC, GPIO_Pin_7); // LED 7을 ON 시킨다. 
//    Delay(500); // 0.5초 딜레이
//  }
//}

//// stm32f4xx의 각 레지스터들을 정의한 헤더 파일 
//#include "stm32f4xx.h"
//// delay 함수
//static void Delay(const uint32_t Count) 
//{
//__IO uint32_t index = 0; //__IO에 volatile 포함되어있음 10클럭이 나와야 딜레이함수가 잘돌아가니깐 S_LAM 에 잡아서 내 의도대로써라 
//for(index = (16800 * Count); index != 0; index--); 
//}
//int main() 
//{
//  //register int delay = 500; //컴파일러가 레지스터에 공간있는지 파악하고 최적화해버림 (레지스터 변수)
//  //volatile int delay = 500; //지역변수로 씀 인터럽트 서비스루틴 메인함수간에 공유하는 변수에 자주 씀 레지스터변수를 쓰면 인터럽트가 걸린 변수가 스택에 접근할수없을 수 있음메인함수의 변수ㅜ와 인터럽트가 걸린함수가 동기화가 안된다
//GPIO_InitTypeDef GPIO_InitStructure;//20바이트 4바이트 다섯개할당 메모리 잡음 쓰레기값들어가있는 상태
//
//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//클럭을 공급해주겠다 고속버스에 연결됨 AHB1Periph 
//
//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//모터같은 출력할때는  OUT IN은 인풋값
//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Push Pull0 아니면 1을 (0v, 5v)보내야할때 디지털방식, Ofen Drain : 외부전원12v 3.3V면 끊어버림  0001
//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//핀 동작속도 고속일수록 노이즈에 약함 값을 적절히 설정해야함 일반적으로100MHz 0003
//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //오픈은 블로킹상태이므로 풀업풀다운 항상 필요 0000
//// C 포트 하위 8비트를 출력으로 선언한다.
//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
//GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; 
//
//GPIO_Init(GPIOC, &GPIO_InitStructure); //더빠르게 처리하기위해 주소를 보냄(포인터로 이루어짐) // MCU메모리 내가 원하는 비트열을 모두읽어와서 앤드로 0으로 만들고 원하는 비트를 오아시킴
//while(1)
//{
//// C 포트 하위 8비트에 ‘high'를 출력한다.
//GPIO_SetBits(GPIOC, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3| 
//GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); 
//// 500 ms 지연 
//Delay(500); 
//// C 포트 하위 8비트에 ‘low'를 출력한다.
//GPIO_ResetBits(GPIOC, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3| 
//GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); 
//// 500 ms 지연 
//Delay(500); 
//}
//}