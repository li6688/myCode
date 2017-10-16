/*
 * File:   SPI.c
 * Author: lsj-pc
 *
 * Created on 2016��7��17��, ����3:27
 */


#include "SPIX.h"

//RF4
#define SST_CE_TRI TRISF
#define SST_CE_PORT LATF
#define SST_CE_ANS  ANSF
#define SST_CE_BIT (1<<4)
#define SST_CE_OUT  {SST_CE_ANS &= ~SST_CE_BIT; SST_CE_TRI &= ~SST_CE_BIT; Nop();}
#define SST_CE_SET  {SST_CE_PORT |= SST_CE_BIT;}
#define SST_CE_CLR  {SST_CE_PORT &= ~SST_CE_BIT;}


/********************************
* SPI��ʼ�����ú���
* SST26SPI_InitializationSET()
* ��������SPI����
*********************************/
void SST26SPI_Initialization(void)
{
	/* Attach SPI2 module to I/O pads */
	// Unlock Registers
   asm volatile (   "push w1 \n"
                    "push w2 \n"
                    "push w3 \n"
                    "mov #OSCCON, w1 \n"
                    "mov #0x46, w2 \n"
                    "mov #0x57, w3 \n"
                    "mov.b w2, [w1] \n"
                    "mov.b w3, [w1] \n"
                    "bclr OSCCON, #6 \n"
                    "pop w3 \n"
                    "pop w2 \n"
                    "pop w1");
    //���ö˿�ӳ��
    // Assign SPI2DI To Pin RPI43
    RPINR22bits.SDI2R = 43;
    // Assign SPI2DO To Pin RP5
    RPOR2bits.RP5R = 10;
    // Assign SPI2CLK To Pin RP14
    RPOR7bits.RP14R = 11;
   
    // Lock Registers
   asm volatile (   "push w1 \n"
                    "push w2 \n"
                    "push w3 \n"
                    "mov #OSCCON, w1 \n"
                    "mov #0x46, w2 \n"
                    "mov #0x57, w3 \n"
                    "mov.b w2, [w1] \n"
                    "mov.b w3, [w1] \n"
                    "bset OSCCON, #6 \n"
                    "pop w3 \n"
                    "pop w2 \n"
                    "pop w1");
    //�ر����ŵ�ģ�⹦��
    ANSEbits.ANSE9 = 0;
    ANSBbits.ANSB4 = 0;
    //�������ŵķ���
    TRISEbits.TRISE9 = 1;
    TRISBbits.TRISB4 = 0;

	SST_CE_OUT;
	SST_CE_SET;
	
    SPI2STAT=0;
    SPI2CON1=0;
    SPI2CON1bits.MSTEN=1;//��ģʽ
    SPI2CON1bits.MODE16=0;//ͨ��Ϊ�ֿ�8 λ��
    SPI2CON1bits.CKP=0;//����״̬ʱ���ź�Ϊ�͵�ƽ����Ч״̬Ϊ�ߵ�ƽ
    SPI2CON1bits.CKE=1;//�������������ʱ������Ч״̬��Ϊ����״̬ʱ�ı�
    SPI2CON1bits.SMP=1;//���������ʱ���ĩβ������������
    SPI2CON2=0;
    SPI2CON1bits.SPRE=0b111;//����Ԥ��Ƶ��Ϊ1:1
    SPI2CON1bits.PPRE=0b11;//��Ԥ��Ƶ��Ϊ16:1
    SPI2STATbits.SPIEN=1;//ʹ��ģ�鲢��SCKx��SDOx��SDIx ��SSx ����Ϊ���ж˿�����
}
/********************************
* SPI�ر�
* SST26SPI_Off
*********************************/
void SST26SPI_DeInitialization(void)
{
	SPI2STATbits.SPIEN = 0;/* Disable SPI2 */
}
/*******************************************************************************
* ������:SPI2Put
* ����:��SPI2����һ���ֽ�����
******************************************************************************/
uint8_t SST26SPI_ReadWriteByte(uint8_t data)
{ 
    IFS2bits.SPI2IF=0;
    // Wait for free buffer
    while(SPI2STATbits.SPITBF);
    SPI2BUF = data;
    // Wait for data byte
    while(!SPI2STATbits.SPIRBF);
    return SPI2BUF;
}