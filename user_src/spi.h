#ifndef __SPI_H__
#define __SPI_H__
#include "iostm8l151g4.h"
#include "type_def.h"

#define SPI_CS PB_ODR_ODR4     //CS
#define SPI_CS_DDR PB_DDR_DDR4 //方向
#define SPI_CS_CR1 PB_CR1_C14  //类型
#define SPI_CS_CR2 PB_CR2_C24  //输出频率

#define SPI_CS_LOW() (SPI_CS = 0)
#define SPI_CS_HIGH() (SPI_CS = 1)
#define SPI_CS_TOGGLE() (SPI_CS = ~SPI_CS)

void SPI_conf();
unsigned char SPI_SendByte(unsigned char byte);
void SPI_SendString(u16 Length, const u8 *TX_Cache, u8 *RX_Cache);
void WAIT_SPI_IDEL(void);
u8 SPISendADF7030CFG(u8 *RF_CONFIG, u32 SIZE);



//-------------------------------ML7345D-------------------------------------
#define SPI_CS      PB_ODR_ODR4 //CS
#define SPI_CS_DDR  PB_DDR_DDR4 //方向
#define SPI_CS_CR1  PB_CR1_C14  //类型
#define SPI_CS_CR2  PB_CR2_C24  //输出频率

#define SPI1_CS_L (SPI_CS = 0)
#define SPI1_CS_H (SPI_CS = 1)
#define SPI_CS_TOGGLE() (SPI_CS = ~SPI_CS)

#define SPI1_SCK_DDR    PB_DDR_DDR5 /* SPI1 SCK */
#define SPI1_SCK_CR1    PB_CR1_C15
#define SPI1_SCK_CR2    PB_CR2_C25

#define SPI1_MOSI_DDR   PB_DDR_DDR6 /* SPI1 MOSI*/
#define SPI1_MOSI_CR1   PB_CR1_C16
#define SPI1_MOSI_CR2   PB_CR2_C26

#define SPI1_MISO_DDR   PB_DDR_DDR7 /* SPI1 MISO*/
#define SPI1_MISO_CR1   PB_CR1_C17
#define SPI1_MISO_CR2   PB_CR2_C27

void spi_delay(u16 n);
void SPI_Config_Init(void);
void ML7345_Write_Reg(u8 addr,u8 dat);
u8 ML7345_Read_Reg(u8 addr);
void ML7345_Write_Fifo(u8 addr,u8 *pbuf,u8 len);
void ML7345_Read_Fifo(u8 addr,u8 *pbuf,u8 len);

#endif
