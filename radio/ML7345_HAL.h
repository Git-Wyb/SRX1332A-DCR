#ifndef __ML7345_HAL_H__
#define __ML7345_HAL_H__

#include "ML7345_defs.h"
#include "Pin_define.h"
#include "spi.h"
#include "timer.h"
#include "ram.h"
#include "ML7345.h"


typedef enum{
    Deep_Sleep = 0x01,  /* DEEP_Sleep mode: Powers for all blocks except IO pins are turned off */
    /* Sleep mode: Main regulator and 24MHz oscillation circuits are tured off. But sub-regulator is turned-on */
    Sleep1_Mode,        /* Sub regulator ON */
    Sleep2_Mode         /* Sub regulator ON;RC ON;Low clock timer ON */
}SleepMode_ENUM;

typedef enum{
    DATA_RATE_1_2K = 0x00,   /* Data rate 1.2 kbps */
    DATA_RATE_2_4K = 0x11,   /* Data rate 2.4 kbps */
    DATA_RATE_4_8K = 0x22,   /* Data rate 4.8 kbps */
    DATA_RATE_9_6K = 0x33    /* Data rate 9.6 kbps */
}DataRate_ENUM;

typedef enum{
    RF_TxDone_Int,
    RF_RxDone_Int,
    RF_TX_FIFO,
    RF_RX_FIFO
}RF_TRX_ENUM;

typedef enum{
    Get_Sta = 0,
    TX_ON = 0x09,           /* TX Enable*/
    RX_ON = 0x06,           /* RX Enable */
    TRX_OFF = 0x08,         /* RF OFF*/
    Force_TRX_OFF = 0x03    /* force RF OFFsetting,*Not waiting for the completion of the state transition*/
}RF_StatusSet_ENUM;

typedef enum{
    Power_0dBm,
    Power_1dBm,
    Power_1_5dBm, //1.5dBm
    Power_2dBm,
    Power_3dBm,
    Power_4dBm,
    Power_5dBm,
    Power_6dBm,
    Power_7dBm,
    Power_8dBm,
    Power_9dBm,
    Power_10dBm,
    Power_11dBm,
    Power_12dBm,
    Power_13dBm
}TransmitPower_ENUM;

void ML7345_AutoStateTransition_Set(u8 sta);
void ML7345_StateFlag_Clear(u8 sta);
void ML7345_DataRate_Set(DataRate_ENUM rate);
u8 ML7345_RSSI_Val(void);
u8 ML7345_SetAndGet_State(RF_StatusSet_ENUM sta);
void ML7345_GoSleep(SleepMode_ENUM mode);
void ML7345_Frequency_Set(u8 *freq,u8 radio_type);
void ML7345_RESETN_SET(void);
void ML7345_TRX_Int_Config(u8 GpioCtrlAddr,RF_TRX_ENUM Intnum,u8 Inten);
void ML7345_AllStateFlag_Clear(void);
void ML7345_TransmitPower_Set(TransmitPower_ENUM Power);
void ML7345_GPIO1RxDoneInt_Enable(void);
void ML7345_GPIO1TxDoneInt_Enable(void);
void ML7345_GPIO2RxDoneInt_Enable(void);
void ML7345_GPIO2TxDoneInt_Enable(void);
u8 WaitStatus_Complete(void);
void ML7345_ReadRx_Pack(u8 *pbuf,u8 len);
void ML7345_AutoTx_Data(u8 *pbuf,u8 len);
u8 RF_SyncWord_DONE(void);
void ML7345_DataRate_Set_4_8k(void);
void ML7345_DataRate_Set_1_2k(void);
void ML7345_DataRate_Set_2_4k(void);
void ML7345_DataRate_Set_9_6k(void);
void ML7345_Frequency_Calcul(u32 Freq,u8 *pbuf);
void SetReg_Rssi(void);


#endif
