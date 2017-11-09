/**********************************************************************************

File_name:      init_peripher.c
Version:		   0.0
Revised:        $Date:2014-7-  ; $
Description:    the main file of this project.
Notes:          This version targets the stm8
Editor:		    Mr.Kong

**********************************************************************************/


/*================================================================================
@ Include files
*/
#include "init_peripher.h"


/*================================================================================
@ Global variable
*/


/*================================================================================
@ All functions  as follow
*/

/*********************************************************************************
Function:  void Delay_Ms(u16 z)
Description:
delay 1ms @  8MHz

Input:        ByteData you want to send.
Output:       None
Return:       None
Others:
*********************************************************************************/
void Delay_Ms(u16 z)
{
  u16 x,y;
  for(x=z;x>0;x--)
  { for(y=1600;y>0;y--);}
  
  
}

/*********************************************************************************
**���ƣ�void EEPROM_WriteByte(uchar EEPROM_Data,uint EEPROM_Add)
**
**���ܣ���EEPROM д��һ�ֽ�
**
**˵�����洢һ�ֽ����� 0-639
**
**ʱ�䣺2013.5.6
**********************************************************************************/
void EEPROM_WriteByte(uchar EEPROM_Data,u16 EEPROM_Add)
{
  uchar *Address;
  Address=(uchar *)(EEPROM_Add+0x4000);
  do
  {
    FLASH_DUKR=0xae; // д���һ����Կ
    FLASH_DUKR=0x56; // д��ڶ�����Կ
  }
  while((FLASH_IAPSR&0x08)==0); // ������δ�ɹ�������������
  *Address =EEPROM_Data;     // д���һ���ֽ�
  while((FLASH_IAPSR&0x04)==0); // �ȴ�д�����ɹ�
}




/*********************************************************************************
**���ƣ�uchar EEPROM_ReadByte(uint EEPROM_Add)
**
**���ܣ���EEPROM ��ȡһ�ֽ�
**
**˵������ȡһ�ֽ�����
**
**ʱ�䣺2013.5.6
*********************************************************************************/
uchar EEPROM_ReadByte(u16 EEPROM_Add)
{
  uchar EEPROM_Data;
  EEPROM_Data = *((uchar*) (EEPROM_Add+0x4000));
  
  return EEPROM_Data;
}


/********************************************************************************
Function:  void UART2_SendBytE
Description:
send a byte by UART2
Input:        ByteData you want to send.
Output:       None
Return:       None
Others:
*********************************************************************************/
void UART2_SendByte(uchar ByteData)
{
  
  while(!UART2_SR_TXE);
  UART2_DR=ByteData;     //��USART1����һ�ֽ�����
  
  while(!UART2_SR_TC);   //�Ƿ������
  
}

/********************************************************************************
Function:       TIM1_PWM_Init
Description:
configurate PWM generator
Input:        CarrierFrq  you set
Output:       None
Return:       None
Others:
*********************************************************************************/
void TIM1_PWM_Init(u8 CarrierFrq)

{
  
  u16  TIM1_ARR;
  
  TIM1_CCMR1=TIM1_CCMR1|0x70;
  TIM1_CCER1=TIM1_CCER1|0x03;
  
  TIM1_ARR  = ( 16000 / CarrierFrq ) -1 ;// calculate the timer1 count.
  TIM1_ARRH =( TIM1_ARR >> 8) & 0xFF;//
  TIM1_ARRL = TIM1_ARR & 0XFF;
  
  
  TIM1_ARR = (TIM1_ARR +1 )/ 3 -1 ;//calculate the timer1 compare data.
  TIM1_CCR1H = (TIM1_ARR >> 8) & 0xFF;//
  TIM1_CCR1L = TIM1_ARR & 0xFF;
  
  TIM1_PSCRH = 0x00;
  TIM1_PSCRL = 0x00;
  //TIM1_CCMR1_OC1M = 100;
  // TIM1_CR1=TIM1_CR1|0X01;
  TIM1_BKR=0x80;
  DISABLE_TIM1_PWM;
  
  
  
}
/********************************************************************************
Function: void SendFrame(u8 *data,u8 Length)
Description: send a frame of data by UART2

Input:     u8 *data    pointer  of a series of data you want to send.
u8 Length  the number of this series of

data.
Output:       None
Return:       None
Others:
*********************************************************************************/
void SendFrame(u8 *data,u8 Length)
{
  
  while( Length>0 )
  {
    
    UART2_SendByte(*data);
    data++;
    Length--;
    
  }
}

/********************************************************************************
**���ƣ�void TIM4_Init(void)
**
**���ܣ���ʱ��4 ��ʼ��
**
**˵������ʱ��4 ����ģ�⴮��  ������Ϊ9600
**
**ʱ�䣺2013.04.26
*********************************************************************************/
void TIM4_Init(void)
{
  TIM4_IER=0x00; // ��ֹ�ж�
  TIM4_EGR=0x01; // ������������¼� // ���ÿ�������������ʱ��
  
  TIM4_PSCR = 0x00;//0.0625us
  
  TIM4_ARR  = 0xEF; // 15us
  TIM4_CNTR = 0XEF;
  
  //TIM4_CR1=0x01; // b0=1,�������������
  TIM4_IER=0x01; // �����ж�
  DISABLE_TIM4 ;
  
  
}
/********************************************************************************
**���ƣ�void IWDG_Init(void)
**
**���ܣ��������Ź���ʼ��
**
**˵����
**
**ʱ�䣺2013.09.27
*********************************************************************************/
void IWDG_Init(void)
{
  
  IWDG_KR=0xcc;
  IWDG_KR=0x55;
  IWDG_PR=0x06;
  IWDG_RLR=0xff;
  IWDG_KR=0xaa;
  
}




/********************************************************************************
Function:  All_Iint
Description:  external device will be configurated in this sector.
Input:        None
Output:       None
Return:       None
Others:
*********************************************************************************/
void All_Init(void)
{
  
  
  /***************MCU clock config*********/
  CLK_CKDIVR = 0x00;     //RC��������Ƶ   CPU��Ƶ M
  CLK_SWCR = 0x00;      //��ֹʱ���л�
  // CLK_ICKR_LSIEN = 1;   //�������Ź�ʱ�ӣ�
  //  while(!CLK_ICKR_LSIRDY);
  
  
  //  IWDG_Init();//in
  
  
  //Rx Power ennable
  PD_DDR_DDR6 = 1;
  PD_CR1_C16  = 1;
  PD_CR2_C26  = 0;
  
  //SQL
  PF_DDR_DDR4 = 1;
  PF_CR1_C14  = 1;
  PF_CR2_C24  = 0;
  //Tx Power ennable
  PB_DDR_DDR0 = 1;
  PB_CR1_C10  = 1;
  PB_CR2_C20  = 0;
  
  
  //SDIO 
  PD_DDR_DDR3 = 1;
  PD_CR1_C13  = 1;
  PD_CR2_C23  = 0; 
  
  //SCLK
  PD_DDR_DDR2 = 1;
  PD_CR1_C12  = 1;
  PD_CR2_C22  = 0; 
  
  //SEBN
  PD_DDR_DDR0 = 1;
  PD_CR1_C10  = 1;
  PD_CR2_C20  = 0; 
  PD_ODR_ODR0 = 1;
//  //stled316s IO all is pp out
//  PA_DDR_DDR2 = 1;
//  PA_CR1_C12  = 1;
//  PA_CR2_C22  = 0;
//  
//  PA_DDR_DDR1 = 1;
//  PA_CR1_C11  = 1;
//  PA_CR2_C21  = 0;
//  
//  PF_DDR_DDR4 = 1;
//  PF_CR1_C14  = 1;
//  PF_CR2_C24  = 0; 
  
  //key input
  PB_DDR_DDR1 = 0;
  PB_CR1_C11  = 1;
  PB_CR2_C21  = 0;
  PB_ODR_ODR1 = 1;
  
  PD_DDR_DDR7 = 0;//
  PD_CR1_C17  = 1;
  PD_CR2_C27  = 0;
  PD_ODR_ODR7 = 1;
  
  //MB1504 IO
//  PD_DDR_DDR5 = 1 ;
//  PD_CR1_C15 =  1 ;
//  PD_CR2_C25 =  0 ;
//  
//  PD_DDR_DDR6 = 1 ;
//  PD_CR1_C16 =  1 ;
//  PD_CR2_C26 =  0 ;
//  
//  PD_DDR_DDR7 = 1 ;
//  PD_CR1_C17 =  1 ;
//  PD_CR2_C27 =  0 ;
//  
//  PC_DDR_DDR7 = 1 ;
//  PC_CR1_C17 =  1 ;
//  PC_CR2_C27 =  0 ;
//  
//  PB_DDR_DDR5 = 1 ;
//  PB_CR1_C15 =  1 ;
//  PB_CR2_C25 =  0 ;
  /*******serial_port_config************/
  
  //   UART2_BRR2= 0x01;
  //   UART2_BRR1= 0x34;
  //   UART2_CR2 = 0x2c;//������գ����ͣ��������ж�
  
  //UART2_CR2=0x0c;//�رս����ж�
  
  /*************TIM2_config************/
  
  //��Ҫ����50ms��ʱ
  TIM2_IER = 0x00; // ��ֹ�ж�
  TIM2_EGR = 0x01; // ������������¼� // ���ÿ�������������ʱ��
  
  TIM2_PSCR = 0x08;//������ʱ��=��ʱ��/2^8�൱�ڼ���������Ϊ32us
  
  TIM2_ARRH = 0x02; // �趨��װ��ʱ�ļĴ���ֵ��
  TIM2_ARRL = 0x70;
  TIM2_CNTRH = 0x02;// �趨�������ĳ�ֵ
  TIM2_CNTRL = 0x70;// ��ʱ����=(ARR+1)*32uS=125mS;
  TIM2_CR1 = 0x01; // b0=1,�������������
  TIM2_IER = 0x01; // �����ж�
  //TIM1_PWM_Init( CarrierFreqency);
  
  
  
  /*************TIM4_config***********/
  //��ҪΪ�������ݼ�ʱ��
  //   TIM4_CR1=0x01;//ʹ�ܼ�����
  //
  //   TIM4_IER=0x01;//�����ж�ʹ��
  //   TIM4_EGR=0x01;
  //   TIM4_CNTR=255;//������ֵ
  //   TIM4_ARR=255;//�Զ���װ��ֵ
  //   TIM4_PSCR=0x07;//128��Ƶֵ
  //   TIM4_CR1=0x01;  //���� TIM4;
  
}




/*@*****************************end of file*************************************@*/
