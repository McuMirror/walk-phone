/*********************************************
ϱ��������Ͷ�������ˣ����ٺ��������㾲��������
koing2010  @2015/4/7
**********************************************/
#ifndef __MB1504_H__
#define __MB1504_H__
#include "iostm8s105k4.h"

//ʹ�õ���Ӳ������
#define  MB1504_LE     PD_ODR_ODR5
#define  MB1504_DATA   PD_ODR_ODR6
#define  MB1504_CLK    PD_ODR_ODR7

#define _nop_( )  asm("NOP");asm("NOP")

typedef struct MB1504_T
{
char a;
}MB1504_INIT,*PMB1504_INIT;

void FrequenceToSend( unsigned int FrequenceD );
void SendReferF();
#endif