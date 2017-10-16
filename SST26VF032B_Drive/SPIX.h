
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SPIX_H
#define	SPIX_H


#include <xc.h> // include processor files - each processor file is guarded. 
#include <stdbool.h> 
#include <stdint.h>

//#define SST26_CE              LATFbits.LATF4
#define SST26_CE_SET          LATFbits.LATF4=1//�ߵ�ƽ
#define SST26_CE_CLR          LATFbits.LATF4=0//�͵�ƽ



void SST26SPI_Initialization(void);//SPI��ʼ������
void SST26SPI_DeInitialization(void);//�ر�SPI
uint8_t SST26SPI_ReadWriteByte(uint8_t data);//SPI�շ��͵��ֽ�����

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

