/**
******************************************************************************
* @file    sst26vf032b.h
* @author  LL
* @version V0.0.0
* @date    2017-06-07
* @brief   
******************************************************************************
******************************************************************************
***/


#ifndef __SST26VF032B_H__
#define	__SST26VF032B_H__

#include <stdint.h>

void SST26_ULBRP(void);//ȫ�ֽ���
uint32_t SST26_ReadJedecID(void);//��ȡSST��ID
void SST26_ChipErase(void);//����оƬ
void SST26_4KByteSectorErase(uint32_t addr);//������ָ����4KB����
void SST26_32KByteSectorErase(uint32_t addr);//������ָ����32KB����
void SST26_64KByteSectorErase(uint32_t addr);//������ָ����64KB����
uint8_t SST26_Write(uint32_t addr,uint8_t* p_data,uint32_t no);//д����ֽ�
uint8_t SST26_EraseWrite(uint32_t addr,const uint8_t* p_data,uint32_t no);
void SST26_HRead(uint32_t addr,uint8_t* p_data,uint32_t no);//���ٶ� ���ں�����5��оƬ
void SST26_LRead(uint32_t addr,uint8_t* p_data,uint32_t no);//���ٶ�
uint8_t SST26_LReadByte(uint32_t addr);//���ٶ�һ���ֽ�
void SST26_Init(void);
void SST26_Deinit(void);
void SST26_Test(void);
#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* __SST26VF032B_H__ */

