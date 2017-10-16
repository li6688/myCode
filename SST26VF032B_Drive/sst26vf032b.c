/**
******************************************************************************
* @file    sst26vf032b.c
* @author  LL
* @version V0.0.0
* @date    2017-06-07
* @brief   
******************************************************************************
******************************************************************************
***/

#include "sst26vf032b.h"
#include "SPIX.h"

#define SST26_DELAY_TIME 160000

static uint8_t SST26_BY(void);// ���æ
static void SSTCmd1(uint8_t cmd);//дһ��SST����
//static void SSTCmd2(uint8_t cmd,uint8_t data);//дһ��������һ������
//static void SSTCmd4(uint8_t cmd,uint8_t *addr);//дһ���������д3����ַ�ֽ�
//static uint8_t SSTCmdb1b(uint8_t cmd);//дһ��SST�������1�ֽ�����
static uint8_t* SSTCmd4bs(uint8_t cmd,uint8_t* addr,uint8_t* data,uint32_t no);//д1�������֣�3����ַ�ֽڣ����ض���ֽ�
static void SST26_WREN(void);//����д����
static void SST26_WRDI(void);//����д����


static uint8_t SST_ReadWriteByte(uint8_t data)
{
    return SST26SPI_ReadWriteByte(data);
}
/*****************************************
*������:SSTCmd1/2/4
*����:дһ��SST����/дһ��������һ������/дһ���������д3������
*ע������:����һ�������ĵ����������������
*****************************************/
void SSTCmd1(uint8_t cmd)//дһ��SST����
{
    SST26_CE_CLR;//ʹ��Ƭѡ
    SST_ReadWriteByte(cmd);
    SST26_CE_SET;//��ֹƬѡ
}
void SSTCmd2(uint8_t cmd,uint8_t data)//дһ��������һ������
{
    SST26_CE_CLR;//ʹ��Ƭѡ
    SST_ReadWriteByte(cmd);
    SST_ReadWriteByte(data);
    SST26_CE_SET;//��ֹƬѡ
}
void SSTCmd4(uint8_t cmd,uint8_t *addr)//дһ���������д3����ַ�ֽ�
{
    SST26_CE_CLR;//ʹ��Ƭѡ
    SST_ReadWriteByte(cmd); //������
    SST_ReadWriteByte(*addr++);
    SST_ReadWriteByte(*addr++);
    SST_ReadWriteByte(*addr);
    SST26_CE_SET;//��ֹƬѡ
}
/****************************************
*������:SSTCmdb1b/SSTCmd4bs
*����:дһ��SST�������1�ֽ�����/д1�������֣�3����ַ�֣����ض���ֽ�
*����ʹ���ڶ����ϵ�
****************************************/
uint8_t SSTCmdb1b(uint8_t cmd)//дһ��SST�������1�ֽ�����
{
    uint8_t tmp;
    SST26_CE_CLR;//ʹ��Ƭѡ
    SST_ReadWriteByte(cmd);
    tmp=SST_ReadWriteByte(0xFF);
    SST26_CE_SET;//��ֹƬѡ
    return(tmp);
}
uint8_t* SSTCmd4bs(uint8_t cmd,uint8_t* addr,uint8_t* data,uint32_t no)//д1�������֣�3����ַ�ֽڣ����ض���ֽ�
{
    SST26_CE_CLR;//ʹ��Ƭѡ
    SST_ReadWriteByte(cmd); //������
    SST_ReadWriteByte(*addr++);
    SST_ReadWriteByte(*addr++);
    SST_ReadWriteByte(*addr);
    for(;no>0;no--)
    {
        *data++=SST_ReadWriteByte(0xFF);
    }
    SST26_CE_SET;//��ֹƬѡ
    return(data);
}
/***************************************
  SST26_WREN  ����д����
***************************************/
void SST26_WREN(void)
{
    SSTCmd1(0x06);
}
/***********************************
  SST26_WRDI  ����д����
***********************************/
void SST26_WRDI(void)
{
    SSTCmd1(0x04);
}
/**********************************
  Read_StatusRegister ��״̬�Ĵ���
**********************************/
uint8_t Read_StatusRegister()
{
	uint8_t byte = 0;
    SST26_CE_CLR;//ʹ��Ƭѡ
	SST_ReadWriteByte(0x05);		/* send RDSR command */
	byte = SST_ReadWriteByte(0xFF);		/* receive byte */
    SST26_CE_SET;//��ֹƬѡ
	return byte;
}

/**********************************
  SST26_BY  ���æ,����0����æ״̬��1����æ״̬
**********************************/
uint8_t SST26_BY(void)
{
    uint8_t i=0;
	uint8_t sta;
    uint32_t rty = SST26_DELAY_TIME;

	do
	{
		sta = Read_StatusRegister();
	}while (((sta & 0x80) == 0x80)&&(rty--));	/* waste time until not busy */
       
    if(0 == rty)
        i=0x00;
    else
        i=0x01;
	
    return i;
}

/***********************************
  SST26_WRSR  д״̬�Ĵ���
***********************************/
void SST26_WRSR(uint8_t data)
{
    SST26_WREN();
    SST26_CE_CLR;//ʹ��Ƭѡ
    SST_ReadWriteByte(0x01);
    SST_ReadWriteByte(0xFF);
    SST_ReadWriteByte(data);
    SST26_CE_SET;//��ֹƬѡ
}

/***********************************
  SST26_RDCR �����üĴ���
***********************************/
uint8_t SST26_RDCR(void)
{
    uint8_t byte = 0;
    SST26_CE_CLR;//ʹ��Ƭѡ
    SST_ReadWriteByte(0x35);
    byte = SST_ReadWriteByte(0xFF);
    SST26_CE_SET;//��ֹƬѡ
    return byte;
}

/***********************************
  SST26_ULBRP ȫ�ֽ���
***********************************/
void SST26_ULBRP(void)
{
    SST26_WREN();
    SST26_CE_CLR;//ʹ��Ƭѡ
    SST_ReadWriteByte(0x98);
    SST26_CE_SET;//��ֹƬѡ
    SST26_WRDI();
}

/**********************************
 * SST26_ReadJedecID ��ȡSST��JedecID 
**********************************/
uint32_t SST26_ReadJedecID(void)
{
    uint32_t temp;
    temp = 0;
    
    SST26_CE_CLR;//ʹ��Ƭѡ
	SST_ReadWriteByte(0x9F);			/* send JEDEC ID command (9Fh) */
    temp = (temp | SST_ReadWriteByte(0xFF)) << 8;	/* receive byte */
	temp = (temp | SST_ReadWriteByte(0xFF)) << 8;	
	temp = (temp | SST_ReadWriteByte(0xFF)); 		/* temp value = 0xBF2642 */
	SST26_CE_SET;//��ֹƬѡ

	return temp;
}
/**********************************
 * ����ָ��������
**********************************/
void SST26_SectorErase(uint32_t addr,uint8_t capacity)
{
    
    uint8_t ad[3];
    ad[0]=(addr>>16)&0xFF;
    ad[1]=(addr>>8)&0xFF;
    ad[2]=addr&0xFF;
    
    SST26_WREN();
    SST26_CE_CLR;//ʹ��Ƭѡ
    SST_ReadWriteByte(capacity);
    SST_ReadWriteByte(ad[0]);
    SST_ReadWriteByte(ad[1]);
    SST_ReadWriteByte(ad[2]);
    SST26_CE_SET;//��ֹƬѡ
    SST26_BY();
    SST26_WRDI();
}
/***********************************
 * SST26_4KByteSectorErase  ����4KB���� 
 * �õ���4kb��С �����ַ��0~4095 ֮����ô
 * ��֮��ĵ�ַ�������
 * ��Ȼ�Ҹ� 4096 �Ļ�4096��4096+4095 ֮�䶼�����
***********************************/
void SST26_4KByteSectorErase(uint32_t addr)
{
    SST26_SectorErase(addr,0x20);
}

/***********************************
 * SST26_8KByteSectorErase  ����8KB���� 
 * �õ���8kb��С �����ַ��0~8191 ֮����ô
 * ��֮��ĵ�ַ�������
 * ��Ȼ�Ҹ� 8193 �Ļ�8192��8192+8191 ֮�䶼�����
***********************************/
void SST26_8KByteSectorErase(uint32_t addr)
{
    SST26_SectorErase(addr,0xD8);
}


/***********************************
 * SST26_32KByteSectorErase  ����32KB���� 
 * �õ���32kb��С �����ַ��0~32767 ֮����ô
 * ��֮��ĵ�ַ�������
 * ��Ȼ�Ҹ� 32768 �Ļ�32768��32768+32767 ֮�䶼�����
***********************************/
void SST26_32KByteSectorErase(uint32_t addr)
{
    SST26_SectorErase(addr,0xD8);
}
/***********************************
 * SST26_64KByteSectorErase  ����64KB���� 
 * �õ���64kb��С �����ַ��0~65535 ֮����ô
 * ��֮��ĵ�ַ�������
 * ��Ȼ�Ҹ� 65536 �Ļ�65536��65536+65535 ֮�䶼�����
***********************************/
void SST26_64KByteSectorErase(uint32_t addr)
{ 
    SST26_SectorErase(addr,0xD8);
}

/**********************************
 * SST26_ChipErase ������Ƭ
**********************************/
void SST26_ChipErase(void)
{
    SST26_WREN();
    SSTCmd1(0xC7);
    SST26_BY();
    SST26_WRDI();
}

/***********************************
 * SST26_PageProgram ҳ��̣�һ�α��һ����ҳ256�ֽ�
 * SST26VF032B����ֻ��ҳ��̣����Ա��1-256�ֽ�
 * ����256�ֽ�ʱ�����ֱ�̣�����256�ֽ�ʱֻ���256�ֽ� 
 ��̳ɹ����� 1�����ɹ�����0
***********************************/
uint8_t SST26_PageProgram(uint32_t addr,const uint8_t* p_data,uint32_t no)
{
	uint16_t i;
	
	if (no != 256) return 0;//һ�α��256���ֽ�
    
    SST26_WREN();//ʹ��д
    
    SST26_CE_CLR;//ʹ��Ƭѡ
	SST_ReadWriteByte(0x02); 			/* send Byte Program command */
	SST_ReadWriteByte((addr >> 16) & 0xFF);	/* send 3 address bytes */
	SST_ReadWriteByte((addr >> 8) & 0xFF);
	SST_ReadWriteByte(addr & 0xFF);
	for (i=0;i<256;i++)
	{	
		SST_ReadWriteByte(p_data[i]);		/* send byte to be programmed */
	}  
    SST26_CE_SET;//��ֹƬѡ
   
    if (0==SST26_BY()) return 0;
    SST26_WRDI();//WP����
    
   return 1; 
}


/***********************************
 * SST26_Write д����ֽ�
 * SST26VF032B����ֻ��ҳ��̣����Ա��1-256�ֽ�
 * ����256�ֽ�ʱ�����ֱ�̣�����256�ֽ�ʱֻ���256�ֽ� 
 ��̳ɹ����� 1�����ɹ�����0
***********************************/
uint8_t SST26_Write(uint32_t addr,uint8_t* p_data,uint32_t no)
{
	uint16_t i;
    if (no == 0) return 0;
	if (no > 256) return 0;//�����256���ֽ�
    
    SST26_WREN();//ʹ��д
    
    SST26_CE_CLR;//ʹ��Ƭѡ
	SST_ReadWriteByte(0x02); 			/* send Byte Program command */
	SST_ReadWriteByte((addr >> 16) & 0xFF);	/* send 3 address bytes */
	SST_ReadWriteByte((addr >> 8) & 0xFF);
	SST_ReadWriteByte(addr & 0xFF);
	for (i=0;i<no;i++)
	{	SST_ReadWriteByte(p_data[i]);		/* send byte to be programmed */
	}  
    SST26_CE_SET;//��ֹƬѡ
   
    if (0==SST26_BY()) return 0;
    SST26_WRDI();//WP����
    
   return 1; 
}

/***********************************
 * SST26_EraseWrite �Ȳ����ٱ�̣�д���ҳ
 * SST26VF032B����ֻ��ҳ��̣����Ա��1-256�ֽ�
 * ����256�ֽ�ʱ�����ֱ�̣�����256�ֽ�ʱֻ���256�ֽ� 
 * ��̳ɹ����� 1�����ɹ�����0
***********************************/
uint8_t SST26_EraseWrite(uint32_t addr,const uint8_t* p_data,uint32_t no)
{
	uint16_t temp,temp1;
	uint16_t startbz;//��̵�һ����Ĵ�С
	uint16_t endbz;//��̽�����Ĵ�С
	uint16_t bz;//������Ĵ�С
	uint32_t i;
    uint32_t temp2;
	uint32_t addrs;
	uint8_t  bufft[4096];
    
    if (no == 0) return 0;//û������
	if (0!= (no%256)) return 0;//���ݸ�����������ҳ
	if ((addr%256)) return 0;//��ַ����ҳ�����

	temp= (addr%4096); 
	if (temp!=0)//��ʼ��ַ����4Kbytes������
	{
		temp1 = 4096-temp;
		if (no<=temp1)//д�������С�ڵ�ǰ���ʣ���С
		{
			startbz = no;
			bz = 0;
			endbz = 0;
		}
		else //�����
		{
			startbz = temp1;
			bz = (no - startbz)/4096;
			bz *= 4096;
			endbz = no - startbz - bz;
		}
	}
	else //��ʼ��ַ��ҳ�����
	{
		if (no<4096)//С��һ����
		{
			startbz = no;
			bz = 0;
			endbz = 0;
		}
		else
		{
			startbz = 0;
			bz = no/4096;
			bz *= 4096;
			endbz = no - bz;
		}
	}
    
    /*
	����д����ʱ��Ҫд��ĵ�ַ����Ϊ0xFF,����д��֮ǰ��Ҫ�Ȳ���������
	��Ϊ��������С��λ��4KBytes,��д���������һ����512bytes,����д��֮ǰ��Ҫ�ȶ�������
	Ȼ���޸�֮����д���ȥ��
	*/

	if (startbz)
	{
		//��Ҫ�������ݣ���д��
        addrs = addr & (~0xFFF);
        SST26_LRead(addrs,bufft, 4096);
        for(i=0; i<startbz; i++)
        {
            bufft[i+ addr-addrs] = p_data[i];
        }
        SST26_4KByteSectorErase(addrs);
        for(i=0; i<16; i++)
        {
            if (0==SST26_PageProgram(addrs+i*256,bufft+i*256,256)) break;
        }
        if (i<16) return 0;
       
		addr += startbz;
	}
    temp2 = 0; 
	while (bz--)
	{
		//����Ҫ��������
        addrs = addr & (~0xFFF);
        SST26_4KByteSectorErase(addrs);
        for(i=0; i<16; i++)
        {
            if (0==SST26_PageProgram(addrs+i*256,p_data+startbz+temp2+i*256,256)) break;
        }
        if (i<16) return 0;
		addr += 4096;
        bz -= 4096;
        temp2 += 4096;
	}
    
    if (endbz)
	{
		//��Ҫ�������ݣ���д��
        addrs = addr & (~0xFFF);
        SST26_LRead(addrs,bufft, 4096);
        for(i=0; i<startbz; i++)
        {
            bufft[i] = p_data[i];
        }
        SST26_4KByteSectorErase(addrs);
        for(i=0; i<16; i++)
        {
            if (0==SST26_PageProgram(addrs+i*256,bufft+i*256,256)) break;
        }
        if (i<16) return 0;
	}
    
   return 1; 
}

/*************************************
 * SST26_HRead ���ٶ� ���ں�����5��оƬ���ɵ��ô˺���������ָ����ַ����ָ�����ֽڣ�
*************************************/
void SST26_HRead(uint32_t addr,uint8_t* p_data,uint32_t no)
{
    SST26_CE_CLR;//ʹ��Ƭѡ
    SST_ReadWriteByte(0x0b);
    SST_ReadWriteByte(addr>>16);
    SST_ReadWriteByte(addr>>8);
    SST_ReadWriteByte(addr);
    SST_ReadWriteByte(0xff);
    for(;no>0;no--)
        *p_data++=SST_ReadWriteByte(0xFF);
    SST26_CE_SET;//��ֹƬѡ
}
/****************************************
SST26_LRead  ���ٶ�����ָ����ַ����ָ�����ֽڣ�
****************************************/
void SST26_LRead(uint32_t addr,uint8_t* p_data,uint32_t no)
{
    uint8_t ad[3];
    ad[0]=(addr>>16)&0xff;
    ad[1]=(addr>>8)&0xff;
    ad[2]=addr&0xff;
    SSTCmd4bs(0x03,ad,p_data,no);
}
/****************************************
SST26_LReadByte  ���ٶ�һ���ֽ�
****************************************/
uint8_t SST26_LReadByte(uint32_t addr)
{
	uint8_t byte = 0;	
    
    SST26_CE_CLR;//ʹ��Ƭѡ
	SST_ReadWriteByte(0x03); 		/* read command */
	SST_ReadWriteByte(((addr & 0xFFFFFF) >> 16));	/* send 3 address bytes */
	SST_ReadWriteByte(((addr & 0xFFFF) >> 8));
	SST_ReadWriteByte(addr & 0xFF);
	byte = SST_ReadWriteByte(0xFF);
	SST26_CE_SET;//��ֹƬѡ
	return byte;			/* return one byte read */
}

/****************************************
SST26_Init
��ʼ��SST26�ӿ�
****************************************/
void SST26_Init(void)
{
	SST26SPI_Initialization();
}

/****************************************
SST26_Deinit
�ر�SST26�ӿ�
****************************************/
void SST26_Deinit(void)
{
	SST26SPI_DeInitialization();
}


/*******************************************************************************
* Function Name  : SST26_Test
* Description    : ���Ժ���                 
* Input          : None
* Output         : None
* Return         : None
* Date           : 2017-06-08
* Author         : LL
*******************************************************************************/
void SST26_Test(void)
{	
    uint16_t i;
    uint8_t buf[300];
    uint8_t bufd[300];
    for(i=0; i<300; i++)
    {
        buf[i] = i+1;
        bufd[i] = 0;
    }
    
    buf[0] = 'a';
    SST26_4KByteSectorErase(0);
    //SST26_ChipErase();
    if (0==SST26_Write(0,buf,256))
    {
        
    }
    SST26_LRead(0,bufd,256);
   
	
}
