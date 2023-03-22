
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
      GPIO_WriteBit(GPIOB, GPIO_Pin_0,Bit_SET); //  �Ѱ��� �����Ҷ�
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
//    GPIO_Write(GPIOC, GPIO_Pin_0); // LED 0�� ON ��Ų��. 
//    Delay(500); // 0.5�� ������ 
//
//    GPIO_Write(GPIOC, GPIO_Pin_1); // LED 1�� ON ��Ų��. 
//    Delay(500); // 0.5�� ������ 
//
//    GPIO_Write(GPIOC, GPIO_Pin_2); // LED 2�� ON ��Ų��. 
//    Delay(500); // 0.5�� ������ 
//
//    GPIO_Write(GPIOC, GPIO_Pin_3); // LED 3�� ON ��Ų��. 
//    Delay(500); // 0.5�� ������ 
//
//    GPIO_Write(GPIOC, GPIO_Pin_4); // LED 4�� ON ��Ų��. 
//    Delay(500); // 0.5�� ������ 
//
//    GPIO_Write(GPIOC, GPIO_Pin_5); // LED 5�� ON ��Ų��. 
//    Delay(500); // 0.5�� ������ 
//
//    GPIO_Write(GPIOC, GPIO_Pin_6); // LED 6�� ON ��Ų��. 
//    Delay(500); // 0.5�� ������ 
//
//    GPIO_Write(GPIOC, GPIO_Pin_7); // LED 7�� ON ��Ų��. 
//    Delay(500); // 0.5�� ������
//  }
//}

//// stm32f4xx�� �� �������͵��� ������ ��� ���� 
//#include "stm32f4xx.h"
//// delay �Լ�
//static void Delay(const uint32_t Count) 
//{
//__IO uint32_t index = 0; //__IO�� volatile ���ԵǾ����� 10Ŭ���� ���;� �������Լ��� �ߵ��ư��ϱ� S_LAM �� ��Ƽ� �� �ǵ���ν�� 
//for(index = (16800 * Count); index != 0; index--); 
//}
//int main() 
//{
//  //register int delay = 500; //�����Ϸ��� �������Ϳ� �����ִ��� �ľ��ϰ� ����ȭ�ع��� (�������� ����)
//  //volatile int delay = 500; //���������� �� ���ͷ�Ʈ ���񽺷�ƾ �����Լ����� �����ϴ� ������ ���� �� �������ͺ����� ���� ���ͷ�Ʈ�� �ɸ� ������ ���ÿ� �����Ҽ����� �� ���������Լ��� �����̿� ���ͷ�Ʈ�� �ɸ��Լ��� ����ȭ�� �ȵȴ�
//GPIO_InitTypeDef GPIO_InitStructure;//20����Ʈ 4����Ʈ �ټ����Ҵ� �޸� ���� �����Ⱚ���ִ� ����
//
//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//Ŭ���� �������ְڴ� ��ӹ����� ����� AHB1Periph 
//
//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���Ͱ��� ����Ҷ���  OUT IN�� ��ǲ��
//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Push Pull0 �ƴϸ� 1�� (0v, 5v)�������Ҷ� �����й��, Ofen Drain : �ܺ�����12v 3.3V�� �������  0001
//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�� ���ۼӵ� ����ϼ��� ����� ���� ���� ������ �����ؾ��� �Ϲ�������100MHz 0003
//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //������ ���ŷ�����̹Ƿ� Ǯ��Ǯ�ٿ� �׻� �ʿ� 0000
//// C ��Ʈ ���� 8��Ʈ�� ������� �����Ѵ�.
//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
//GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; 
//
//GPIO_Init(GPIOC, &GPIO_InitStructure); //�������� ó���ϱ����� �ּҸ� ����(�����ͷ� �̷����) // MCU�޸� ���� ���ϴ� ��Ʈ���� ����о�ͼ� �ص�� 0���� ����� ���ϴ� ��Ʈ�� ���ƽ�Ŵ
//while(1)
//{
//// C ��Ʈ ���� 8��Ʈ�� ��high'�� ����Ѵ�.
//GPIO_SetBits(GPIOC, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3| 
//GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); 
//// 500 ms ���� 
//Delay(500); 
//// C ��Ʈ ���� 8��Ʈ�� ��low'�� ����Ѵ�.
//GPIO_ResetBits(GPIOC, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3| 
//GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); 
//// 500 ms ���� 
//Delay(500); 
//}
//}