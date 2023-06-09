/******************************************************************************
  포트 연결:
    1)  ARM Cortex-M4 모듈의 포트C (PC0 ~ PC7)를 8핀 케이블를 이용해서
        Text LCD모듈의 D0 ~ D7에 연결한다. (D0가 PC0로 연결되야 한다.)
    2)  ARM Cortex-M4 모듈의 포트B (PB0 ~ PB2)를 4핀 케이블를 이용해서
        Text LCD모듈의 RS, RW E에 연결한다. (RS가 PB0로 연결되야 한다.)
******************************************************************************/
// stm32f4xx의 각 레지스터들을 정의한 헤더파일
#include "stm32f4xx.h"
#include "uart2.h"
#include <stdio.h>
#include <string.h>

#define TLCD_RS  GPIO_Pin_0                  // LCD RS 신호 
#define TLCD_RW  GPIO_Pin_1                  // LCD RW 신호 
#define TLCD_E   GPIO_Pin_2                  // LCD E 신호 
#define TLCD_EN { GPIO_ResetBits(GPIOB, TLCD_E); GPIO_SetBits(GPIOB, TLCD_E); }
#define DATA     GPIOC                       // LCD 데이터 신호 

#define ON      1
#define OFF     2

#define RIGHT   1
#define LEFT    2


extern volatile unsigned char rx2Flag; //uart에서 한 바이트가 수신되면 \n이 수신되면 rxflag2를 true로 만듬.
extern char rx2Data[50];

// delay 함수
static void Delay(const uint32_t Count)
{
  __IO uint32_t index = 0; 
  for(index = (16800 * Count); index != 0; index--);
}

static void Delay_us(const uint32_t Count)
{
  __IO uint32_t index = 0; 
  for(index = (16 * Count); index != 0; index--);
}

void Port_Init(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
                                             GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}
// LCD 에 모든 명령을 전송한후 반드시 Enable Pulse를 주어야 한다.
void E_Pulse(void)
{
  GPIO_SetBits(GPIOB, TLCD_E);  //enable 을 high로 보냄
  Delay(5);
  GPIO_ResetBits(GPIOB, TLCD_E); //enable을 low로 보냄.
}
  
// LCD에 데이터를 전송하는 함수이다   
void TLCD_DATA(unsigned char data)
{
  GPIO_Write(DATA, data);
  TLCD_EN;    // 이 부분에서는  E_Pulse() 함수 대신 사용한다.
} 

void Func_Set(void)
{
  GPIO_ResetBits(GPIOB, TLCD_RW);        // RW = 0, //1이면 읽기 0이면 쓰기 RS = 0 일 때 IR쓰기 명령
  GPIO_ResetBits(GPIOB, TLCD_RS); //rs = 0이면 명령을 데이터 버스에 보낸다.
  GPIO_Write(DATA, 0x38);               // 초기화를 위한 Function Set, lcd 기능 설정
  E_Pulse();                            // Enable Pulse
}

// LCD 초기화 함수
void Init_LCD(void)
{
  GPIO_ResetBits(GPIOB, TLCD_E);  //데이터시트에 있는대로 초기화를 하는 것
  Delay(15);                    // 약 15ms 시간 지연
  Func_Set();
  Delay(10);                    // 약 10ms 시간 지연
  Func_Set();
  Delay_us(150);                // 약 150us 시간 지연
  Func_Set();
  GPIO_Write(DATA, 0x0f);       // 디스플레이 ON, 커서 ON  0000 1***로 시작하면 명령어임
  E_Pulse();                                   //어떤 명령인지 찾아야함. 디스플레이 on/off
  GPIO_Write(DATA, 0x06);       // 글자를 오른쪽으로 시프트 시키며 출력 
  E_Pulse();                                   //엔트리 모드 세트
  GPIO_Write(DATA, 0x01);       // 화면 클리어
  E_Pulse();                                    //클리어 디스플레이 명령어
}
  
  /******************************************
     lcd_char() : 1 문자를 디스플레이 하는 함수
  *******************************************/
void lcd_char(char s)
{
  GPIO_SetBits(GPIOB, TLCD_RS); // 1이니까 데이터를 보냄
  GPIO_Write(DATA, s); //B의 아스키코드 0x41
  E_Pulse();
}

void lcd_disp(char x, char y)
{
  GPIO_ResetBits(GPIOB, TLCD_RS); //명령보내야하니가 rs에 0
  GPIO_ResetBits(GPIOB, TLCD_RW);
  if(y==0) GPIO_Write(DATA, x+0x80); // d d ram 어드레스 설정.
  else if(y==1) GPIO_Write(DATA, x+0xc0);
  E_Pulse();
 }
 
void move_disp(char p )
{
  GPIO_ResetBits(GPIOB, TLCD_RS);
  GPIO_ResetBits(GPIOB, TLCD_RW);

  if(p==LEFT ) GPIO_Write(DATA, 0x18);
  else if(p==RIGHT) GPIO_Write(DATA, 0x1c);

  E_Pulse();
}

void disp_ON_OFF(char d, char c, char b)
{
  char disp = 0x08;

  GPIO_ResetBits(GPIOB, TLCD_RS);
  GPIO_ResetBits(GPIOB, TLCD_RW);

  if (d==ON) d = 0x04;
  else         d = 0x00;

  if(c == ON) c = 0x02;
  else          c = 0x00;

  if(b == ON) b = 0x01;
  else          b = 0x00;

  GPIO_Write(DATA, disp | d | c| b);  //0000 1011
  E_Pulse();
            
}   
 
void clrscr(void)
{
  GPIO_ResetBits(GPIOB, TLCD_RS);  //명령
  GPIO_ResetBits(GPIOB, TLCD_RW); //write
  GPIO_Write(DATA, 0x01); // 화면 클리어
  E_Pulse();

  Delay(10);
} 


 /***************************************************************
    LCD 에 위치를 지정하여 문자열을 출력할 수 있도록 만든 함수이다 
 ****************************************************************/
void lcd(char x, char y, char *str)
{
  lcd_disp(x, y);
  while(*str) lcd_char(*str++);
}


int main()
{
  int i;
  Port_Init();
  Init_LCD();
  int len = 0;
  
  UART2_init();
  printf("Start Main()\n\r");
  
  while(1)
  {
    if(rx2Flag)
    {
      len =  strlen(rx2Data);  //40에서 len 뺀만큼 for를 돌리면 된다.
      if(strlen(rx2Data)<=16)
        lcd(0,0,rx2Data;
            else if(len<=40)
            {
              lcd(0,0,rx2Data);
              move_disp(LEFT);
            }
            rx2Flag = 0;
    }
    
   /* clrscr();  //  화면 클리어 
    lcd(0,0,"HBE-Multi II");    // 문자열 출력
    lcd(0,1,"BASE PROGRAM"); // 문자열 출력
    Delay(1000);           // 일정시간 지연

    // 디스플레이 ON/OFF를 번갈아 가면서 5번 수행 
    for(i = 0; i < 5 ; i++ )
    {
      disp_ON_OFF(OFF, ON, ON);  // 디스플레이 OFF 
      Delay(1000);

      disp_ON_OFF(ON, ON, ON);  // 디스플레이 ON 
      Delay(1000);
    }  

    // 문자열 전체를 오른쪽/왼쪽으로 각 10회씩 이동 
    for ( i = 0 ; i < 20 ; i ++) 
    {
      if(i < 10) move_disp(RIGHT); // 디스플레이 오른쪽 시프트 
      else      move_disp(LEFT); // 디스플레이 왼쪽 시프트 
      Delay(1000);                
    }
    lcd_disp(0,0);   // 커서위치 초기화 

    disp_ON_OFF(ON,ON,OFF); // 커서 깜빡임 
    Delay(1000);            // 일정 시간 지연 

    disp_ON_OFF(ON, ON, ON); // 커서 깜빡임 ON
    Delay(1000);             // 일정 시간 지연 

    clrscr();                     // 화면 클리어

    lcd(0,1," End of Demo ");    // 종료 문자열 출력 
    Delay(1000);             // 약 1초간 시간 지연 */
  }
    
}