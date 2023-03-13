#ifndef __ML7345_H__
#define __ML7345_H__

#include "spi.h"
#include "ML7345_defs.h"
#include "ML7345_HAL.h"

#define RF_RX_DONE() (ML7345_Read_Reg(ADDR_INT_SOURCE_GRP2) & 0x01)
#define RF_TX_DONE() (ML7345_Read_Reg(ADDR_INT_SOURCE_GRP3) & 0x01)

#define TX_PACKET_COUNT 3

#define FREQ_PLL     12000000 //Extern TCXO / 2 = 24MHz / 2
#define CONST_COEFF  1048576  //常数2^20
#define VCO_LowerLimit_FREQ 400000  //400kHz

void RF_ML7345_Init(u8* freq,u8 sync,u8 rx_len);
void Tx_Data_Test(u8 mode);
void ML7345_MeasurBER_Init(void);
void RF_ML7345_TEST(void);
void DataReceive(void);
void RF_Ber_Test(void);
void ML7345_TRX_Del(void);
void APP_TX_PACKET(void);

void RF_BER_TEST(void);
void Uart_RF_Ber_Test(void);

void ML7345d_Change_Channel(void);
void ML7345D_Freq_Scanning(void);
void ML7345D_RF_test_mode(void);
void TX_DataLoad_HighSpeed(u32 IDCache, Wireless_Body CtrCmd, u8 *Packet);
void ML7345D_Error_Detect(void);
void Ber_PinExit_Init(void);

extern u8 Tx_Done;
extern u8 Rx_Done;
extern u8 reg1_val[128];
extern u8 reg2_val[128];

extern u8 Fre_426_075[8];
extern u8 Fre_426_750[8];
extern u8 Fre_429_550[8];
extern u8 Fre_429_350[8];
extern u8 Fre_429_175[8];
extern u8 Fre_429_200[8];
extern u8 Freq_SetBuff[8];
extern u8 rf_offset;

extern u8 tx_pack_cnt;

extern u8 ADF7030_REST_Cache;
extern u8 Receiver_vent_Cache;
extern u8 RX_COUNT;
extern u8 SPI_RECEIVE_BUFF[SPI_REV_BUFF_LONG];
extern u32 SPI_Receive_DataForC[7];
extern u32 ADF7030_RESIGER_VALUE_READ;
extern u8 ADF7030_Read_OneByte;
extern u16 Head_0x5515_or_0x5456;

extern UINT32 DATA_Packet_ID;
extern UINT8 DATA_Packet_Control;
extern UINT8 DATA_Packet_Contro_buf; //2015.3.24����

extern u8 Radio_Date_Type;
extern u8 Radio_Date_Type_bak;

#endif


