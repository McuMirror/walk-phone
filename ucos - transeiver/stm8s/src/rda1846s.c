/***************************************************************************/
#include <iostm8s105k4.h>
#include <intrinsics.h>
#include "includes.h"

#define NOP          for(u8 i = 0;i < 16 ; i ++) {asm("NOP");asm("NOP"); asm("NOP"); asm("NOP");}

  
   


unsigned char rda1846TX[3] @0x60 = {0x00,0x48,0x26};          //����Ƶ��
unsigned char rda1846RX[3] @0X63 = {0x00,0x48,0x26};          //����Ƶ��
unsigned char rdaCTCSSTX[2] @0X66 = {0x22,0x68};         //����ģ������
unsigned int  rdaCTCSSRX @0X68;            //����ģ������
unsigned char rdaDTCTX[3] @0X6A ;          //������������
unsigned int  rdaDTCRX    @0X6D;              //������������
unsigned char rda1846r30r @0x6f;           //����ר�üĴ���
unsigned char rda1846r58r @0x70 = 0x00;            //����CTCSS(rda1846r58r=0x03)CDCSS(rda1846r58r=0x00)
unsigned char rda1846r45r @0x71 = 0x86;            //����CTCSS(rda1846r45r=0x85)CDCSS(rda1846r58r=0x86)
unsigned char rda1846r45t @0x72 = 0x80;            //�����������ƼĴ���
unsigned char rda1846rt05lt @0x73;           //����Ƶ�ʷ���ר�üĴ���
unsigned char rda1846rt05ht @0x74;           //����Ƶ�ʷ���ר�üĴ���
unsigned char rda1846rt64lt @0x75;           //����Ƶ�ʷ���ר�üĴ���
unsigned char txpram @0x76 = 0x3F;//���书�ʵ����Ĵ���
unsigned char BZRAM @0x77;//���ղ����Ĵ���

unsigned char V8RAM1 = 0;//������ר�üĴ�����

//unsigned char rda1846r05t ;           //������ƼĴ���

//unsigned char rda1846r64t[2] @0x6f;         //������ƼĴ���


/***************************************************************************/
#define RDASQ        PC_ODR_ODR1
#define RDA1846_O    PD_ODR_ODR3 
#define RDA1846_I    PD_IDR_IDR3 
#define RDA1846CLK   PD_ODR_ODR2
#define RDA1846SEN   PD_ODR_ODR0
#define RDA1846IOC   PC_ODR_ODR4 //�������� IOC ��ɶ��˼

//#define RDA1846PDN   _pa2 
/***************************************************************************/        

/*
void Delays4ms(unsigned char number)                        //��ʱ4MS����
{
unsigned char temp,temp1;
for(;number!=0;number--)        //ѭ��
{ 
for(temp=80;temp!=0;temp--)                //��ѭ��
{
for(temp1=10;temp1!=0;temp1--)
{
                }
                }
        }
}*/
void rda_spi_write_Bytes( unsigned char wdata)
{
   //set the IO input
   PD_DDR_DDR3 = 1;
   
  for(unsigned char i = 8; i !=0 ; i--)
  {
    if (wdata & 0x80)
    {
      RDA1846_O = 1;
    }
    else 
    {
      RDA1846_O = 0;
    }
    
    RDA1846CLK = 1;
    
    NOP 
    RDA1846CLK=0;
    wdata <<= 1;
  }

}

unsigned char rda_spi_read_Bytes()
{
  unsigned char  rdaRead = 0;
  //set the IO input
    //SDIO 
  PD_DDR_DDR3 = 0;
  PD_CR1_C13  = 1;
  PD_CR2_C23  = 0; 
  
  for(unsigned char i= 8;i!=0;i--)
  {
    rdaRead =rdaRead<< 1;
    RDA1846CLK=1;
    
    NOP

    if(RDA1846_I) 
    {
      rdaRead|= 0x1;
    }
    RDA1846CLK=0;
  }

  return rdaRead;
}
unsigned int Readrda1846(unsigned char rdarom) //����RDA�Ĵ�������
{
  unsigned  int rdaRead;

  RDA1846SEN=1;
//  RDA1846IOC=0;
  RDA1846CLK=0;
  RDA1846SEN=0;
  rdarom|=0x80;
  rda_spi_write_Bytes(rdarom);
  rdaRead = rda_spi_read_Bytes();
  rdaRead <<= 8;
  rdaRead |= rda_spi_read_Bytes();
  
  RDA1846IOC=0;
  RDA1846SEN=1;
  RDA1846_O=0;
  return rdaRead;
}        

void Writerda1846(unsigned char rdarom,unsigned char a,unsigned char b)
{

  RDA1846SEN=1;
 // RDA1846IOC=0;
  RDA1846CLK=0;
  RDA1846SEN=0;
  rda_spi_write_Bytes(rdarom);
  rda_spi_write_Bytes(a);
  rda_spi_write_Bytes(b);
  RDA1846SEN=1;
  RDA1846_O=0;
  
}        
/*25KHZ��ʼ��RDA����*/
void RDA1846_Init(void)
{
  //_clrwdt();//������Ź�
  Writerda1846(0x30,0x00,0x01);
  
 //  unsigned int data = Readrda1846(0x01);
//  data = Readrda1846(0x01);
//  data = Readrda1846(0x03);
//  data = Readrda1846(0x04);
//  data = Readrda1846(0x08);
//  data = Readrda1846(0x09);
//  data = Readrda1846(0x0a);

  Writerda1846(0x30,0x00,0x04);
  unsigned int data = Readrda1846(0x30);
  //Writerda1846(0x04,0x0f,0xd1);//13M��12.8M
  Writerda1846(0x04,0x0f,0xd0);//26M��25.6M
  Writerda1846(0x08,0x02,0xA0);
  Writerda1846(0x09,0x03,0xAC);
  //Writerda1846(0x08,0x42,0xA0);
  //Writerda1846(0x09,0x03,0x2C);
  Writerda1846(0x0B,0x1A,0x10);
  //Writerda1846(0x0F,0x00,0x24);//400m=0X0024;200m=0X00A4;100m=0X00E4;
  Writerda1846(0x0F,0x00,0xE4);
  Writerda1846(0x11,0x3E,0x37);//���ſƼ��ṩ�ο�����
  Writerda1846(0x2B,0x32,0xc8);//26M
  //Writerda1846(0x2B,0x32,0x00);//12.8M��25.6M
  Writerda1846(0x2C,0x19,0x64);//26M
  //Writerda1846(0x2C,0x19,0x00);//12.8M��25.6M
  //Writerda1846(0x32,0x62,0x7C);//UHF
  Writerda1846(0x32,0x62,0xBC);//VHF
  Writerda1846(0x33,0x0A,0xF2);
  //Writerda1846(0x41,r[1],r[0]);//���ؿ���
  //Writerda1846(0x42,r[3],r[2]);//���عر�
  Writerda1846(0x47,0x2C,0x2F);
  Writerda1846(0x4E,0x29,0x3A);
  Writerda1846(0x54,0x1D,0x4C);
  Writerda1846(0x56,0x06,0x52);
  Writerda1846(0x6E,0x06,0x2D);
  Writerda1846(0x70,0x10,0x29);
  Writerda1846(0x7F,0x00,0x01);
  Writerda1846(0x05,0x00,0x1F);
  Writerda1846(0x7F,0x00,0x00);
  Writerda1846(0x30,0x30,0xA4);
  Writerda1846(0x30,0x30,0xA6);
  //_clrwdt();//������Ź�
  OSTimeDly(10);//10*10ms ��ʱ100MS
  Writerda1846(0x30,0x30,0x06);
  //_clrwdt();//������Ź�
  OSTimeDly(10);;//��ʱ100MS
}



void RDA1846RX(unsigned char rama)
{
  unsigned char SQRAM;
  SQRAM=BZRAM-0x18;
  // _clrwdt();//������Ź�
  Writerda1846(0x30,0x30,0x06);//�ر��շ�
  OSTimeDly(5);//delay 50ms
  Writerda1846(0x05,0x07,0x63);//
  Writerda1846(0x64,0x70,0x3A);//
  Writerda1846(0x29,0x00,rda1846RX[2]);//�趨����Ƶ��
  Writerda1846(0x2A,rda1846RX[1],rda1846RX[0]);//�趨����Ƶ��
  Writerda1846(0x1F,0x55,0x01);//�趨GPIO��
  Writerda1846(0x44,0x00,0xFE);//AF�����С����
  Writerda1846(0x48,0x00,BZRAM);//�趨����SQֵ
  Writerda1846(0x49,0x00,SQRAM);//�趨�ر�SQֵ= ����SQֵ��0x18
  Writerda1846(0x4A,0X22,0X68);//=0x2268��0x1740
  Writerda1846(0x58,rda1846r58r,0x00);//CDCSS 0x0000  CTCSSʱWriterda1846(0x58,0x03,0x00) CTCSS(rda1846r58r=0x03)CDCSS(rda1846r58r=0x00)
  Writerda1846(0x45,0x0A,rda1846r45r);//CDCSS 0x0a86  CTCSSʱWriterda1846(0x45,0x0A,0x85) CTCSS(rda1846r45r=0x85)CDCSS(rda1846r58r=0x86)
  //Writerda1846(0x58,0X03,0x00);//CDCSS 0x0000  CTCSSʱWriterda1846(0x58,0x03,0x00) CTCSS(rda1846r58r=0x03)CDCSS(rda1846r58r=0x00)
  //Writerda1846(0x45,0x0A,0X85);//CDCSS 0x0a86  CTCSSʱWriterda1846(0x45,0x0A,0x85) CTCSS(rda1846r45r=0x85)CDCSS(rda1846r58r=0x86)
  Writerda1846(0x30,0x30,0x06);//���ģʽ խ��ʱWriterda1846(0x30,0x00,0x06)
  OSTimeDly(3);//delay 30ms
  Writerda1846(0x30,0x30,rama);//���ģʽ=0x302eʱΪ�źŵ���SQֵʱAF�����ź��������Ϊ���ź����
  // խ��ʱWriterda1846(0x30,0x00,0x2E)
  //���ģʽ=OX3026ʱΪһֱ���ź����
  //���ģʽ=0X30A6ʱΪ����ģʽ
}
void RDA1846TX(void)
{
  if(V8RAM1==1)//�������Ƿ��ڿ���״̬
  {        
    unsigned char ram[2];
    unsigned char *p;
    ram[0]=0x82;//0X2H=0x8280ʱΪ����ģʽ
    ram[1]=0x80;
    p=ram;
    //???    WriteRDA58XX(p,0x02,2);
    
    //txpram=0x3f;
    V8RAM1=0;//���������״̬��־
    OSTimeDly(2);;
  }
  //_clrwdt();//������Ź�
  Writerda1846(0x30,0x30,0x06);//�ر��շ�
  OSTimeDly(2); //12ms
  Writerda1846(0x05,rda1846rt05ht,rda1846rt05lt);
  Writerda1846(0x64,0x70,rda1846rt64lt);
  Writerda1846(0x29,0x00,rda1846TX[2]);//�趨����Ƶ��
  Writerda1846(0x2A,rda1846TX[1],rda1846TX[0]);//�趨����Ƶ��
  Writerda1846(0x0A,0x04,txpram);//tx���ʵ�ѹ����(���0x043f����С��0x0400)
  Writerda1846(0x1F,0x55,0x01);//�趨GPIO��
  Writerda1846(0x43,0x12,0x0F);//�趨����Ƶƫ���������ȴ�С[12:6]����������������[5:0]������������
  Writerda1846(0x4A,rdaCTCSSTX[0],rdaCTCSSTX[1]);//�趨��������Ƶ  4aH[15:0](ctcss_sentreg)=0.0974*(2^16) = 6383   
  //��������ʱ����0X2268;
  Writerda1846(0x4B,0x00,rdaDTCTX[0]);//�趨������������Ƶ
  Writerda1846(0x4C,rdaDTCTX[1],rdaDTCTX[2]);//�趨������������Ƶ
  Writerda1846(0x45,0x0A,rda1846r45t);//�趨����Ƶ���ƼĴ���(������Ϊ0x80;ģ������0x81,����0x82)
  Writerda1846(0x30,0x30,0x06);//���ģʽ խ��ʱWriterda1846(0x30,0x00,0x06)
  OSTimeDly(3);// 24ms
  Writerda1846(0x30,0x30,0x46);//��VOXʱ���ģʽ խ��ʱWriterda1846(0x30,0x00,0x56)                        
}