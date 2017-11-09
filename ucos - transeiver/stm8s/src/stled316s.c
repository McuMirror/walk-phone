
#include "stled316s.h"
//�������ʾ������
//                  			0 1 2 3 4 5 6 7 8 9 A B C D E F - �� N  t L H r o
const uchar SegA[] ={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,
			         0x77,0x7c,0x39,0x5e,0x79,0x71,0x40,0x00,0x37,0x78,0x38,0x76,0x50,0x3E,0x5c};    //����


/*  STLED316S */
void Delay_STLED316S(void)  //��ʱ
{
 uint i;
 for( i=0;i<10;i++ )
   {;}
}
/****************************************************************************
����:		�������ݴ���
����: 	    ��stled316s��������
����:		data
����ֵ:		��
��ע:		
******************************************************************************/
void SPI_SendData( uchar data )              
{
	uchar i,l;
	for(i=0; i<8 ;i++)
		{
	      CLK=0;
          if(data&0x01==1)			
	      DIN=1;
          else  DIN=0;	
          data>>=1;		
		  for( l=0 ; l<5 ; l++);
		  CLK=1;		
		  for( l=0 ; l<5 ; l++);					
		}
}
/****************************************************************************
����:		stled316s��ʼ��
����: 	    ������ܵĸ��������ȼ�led�����Ƚ��г�ʼ��	
����:		Brightness 
����ֵ:		��
��ע:		
******************************************************************************/
void InitSTLED316( uchar Brightness )   //��ʼ��
{  
	STB=1;	
	Delay_STLED316S();												
	STB=0;
	Delay_STLED316S();					
    SPI_SendData(0x30);
    Delay_STLED316S();		
	STB=1;	
	Delay_STLED316S();												
	STB=0;
	Delay_STLED316S();	
    SPI_SendData(0x30);  //��ַ
    SPI_SendData(0xE4); //��������   E5��6��      
	Delay_STLED316S();												
	STB=1;	
	Delay_STLED316S();												
	STB=0;
	Delay_STLED316S();
  	SPI_SendData(0x11);	       	
	Delay_STLED316S();
   	STB=1;	
	Delay_STLED316S();												
	STB=0;
	Delay_STLED316S();	
    SPI_SendData(0x11);//��ַ
	SPI_SendData( Brightness );//��������	         
	SPI_SendData( Brightness );							 
    SPI_SendData( Brightness );							 															
	STB=1;	
	Delay_STLED316S();												
	STB=0;
	Delay_STLED316S();
    SPI_SendData(0x18);	       	
	Delay_STLED316S();
    STB=1;	
	Delay_STLED316S();												
	STB=0;
	Delay_STLED316S();	
    SPI_SendData(0x18);     //��ַ
	SPI_SendData(0x00);					         
	SPI_SendData(0x00);	
    SPI_SendData(0x00);		//��������				 	
}
/****************************************************************************
����:		д�����
����: 	    �����������ʾ״̬
����:		data1,data2,data3,data4,data5,data6
����ֵ:		��
��ע:		
******************************************************************************/
void Write_8LED( uchar oneSecondsFlag,uchar	data1,uchar data2,uchar	data3,uchar	data4)
{
STB=0;
	Delay_STLED316S();					
    SPI_SendData(0x00);		  	
	Delay_STLED316S();
	STB=1; 
	
	Delay_STLED316S();												
	STB=0;
	Delay_STLED316S();				
    SPI_SendData(0x00);   //��ַ

//���������*******************
//�˴�������޸�
switch(oneSecondsFlag)
{
case 0://С����ȫ����
{	

	SPI_SendData(SegA[data1]);
	SPI_SendData(SegA[data2]);
	SPI_SendData(SegA[data3]);
	SPI_SendData(SegA[data4]);
	break;
}
case 1://�м�ʱ��ð�ŵ���
{	
	SPI_SendData(SegA[data1]|0x80);
	SPI_SendData(SegA[data2]);
	SPI_SendData(SegA[data3]);
	SPI_SendData(SegA[data4]);

	break;
}

case 2://data1С������
{
	SPI_SendData(SegA[data1]);
	SPI_SendData(SegA[data2]|0x80);
	SPI_SendData(SegA[data3]);
	SPI_SendData(SegA[data4]);

	break;
}
case 3://data2С������
{
        SPI_SendData(SegA[data1]);
	SPI_SendData(SegA[data2]);
	SPI_SendData(SegA[data3]|0x80);
	SPI_SendData(SegA[data4]);

	break;
}

case 4://data4С������
{
	SPI_SendData(SegA[data1]);
	SPI_SendData(SegA[data2]);
	SPI_SendData(SegA[data3]);
	SPI_SendData(SegA[data4]|0x80);

	break;
}
/*
case 5://data5С������
{
	SPI_SendData(SegA[data1]);
	SPI_SendData(SegA[data2]);
	SPI_SendData(SegA[data3]);
	SPI_SendData(SegA[data4]);
	SPI_SendData(SegA[data5]|0x80);
	SPI_SendData(SegA[data6]);
	break;
}

//���ڴ������ʾ�������磺
case 6://data6С������
{
	SPI_SendData(SegA[data1]);
	SPI_SendData(SegA[data2]);
	SPI_SendData(SegA[data3]);
	SPI_SendData(SegA[data4]);
	SPI_SendData(SegA[data5]);
 	SPI_SendData(SegA[data6]|0x80);
	break;
}
*/
//ע��Ҫ�ں������������data6��
}

//**********************************************
    Delay_STLED316S();
    STB=1;
	Delay_STLED316S();														
	STB=0;
	Delay_STLED316S();			 										
	SPI_SendData(0x0d);				//������ʾ����
	Delay_STLED316S();
	STB=1;	
}
/****************************************************************************
����:		����led
����: 	    ����ĳһλled��״̬
����:		data 
����ֵ:		��
��ע:		
******************************************************************************/
void	Write_LED(uchar	data)
{
		STB=0;
		Delay_STLED316S();
		SPI_SendData(0x28);
		SPI_SendData(data);
		Delay_STLED316S();
		STB=1;
		Delay_STLED316S();
		STB=0;
		Delay_STLED316S();
    	SPI_SendData(0xD);  //Display On
    	Delay_STLED316S();
    	STB=1;
		Delay_STLED316S();		
}