#include "ML7345_HAL.h"


/*
Function: RF auto status transition control
Parameter: AUTO_TX_EN,AUTO_TXDONE_GORX,AUTO_TXDONE_CONTINUE_RX
Return: Null
*/

void ML7345_AutoStateTransition_Set(u8 sta)
{
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL);  //set bank0
    if(sta == AUTO_TX_EN || sta == AUTO_TXDONE_GORX || sta == AUTO_TXDONE_CONTINUE_RX)
        ML7345_Write_Reg(ADDR_RF_STATUS_CTRL,sta);
}


/*
Function: RF state setting and status indication
Parameter: TX_ON,RX_ON,TRX_OFF, Force_TRX_OFF
Return: RF status

Description: RF_STATUS Register 高4位指示RF状态，低四位设置RF状态
*1 During TX operation, setting RX_ON is possible.
In this case, after TX completion, move to RX_ON state automatically.
*2 During RX operation, setting TX_ON is possible.
In this case, after RX completion, move to TX_ON state automatically.
*3 If TRX_OFF is selected during TX or RX operation, after TX or RX operation completed, RF is turned off.
If Force_TRX_OFF is selected during TX or RX operation, RF is turned off immediately
*/
u8 ML7345_SetAndGet_State(RF_StatusSet_ENUM sta)
{
    u8 status = 0;

    ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL);  //set bank0
    status = ML7345_Read_Reg(ADDR_RF_STATUS) >> 4;
    if(status != sta)
    {
        if(sta == TX_ON || sta == RX_ON || sta == TRX_OFF || sta == Force_TRX_OFF)
        {
            ML7345_Write_Reg(ADDR_RF_STATUS,sta);
            status = WaitStatus_Complete();
            if(status != 0 && sta != Force_TRX_OFF)
            {
                ML7345_Frequency_Calcul(PROFILE_CH_FREQ_32bit_200002EC,Freq_SetBuff);
                ClearWDT();
                if(PROFILE_CH_FREQ_32bit_200002EC < 429000000)  RF_ML7345_Init(Freq_SetBuff,0x55,12);
                else    RF_ML7345_Init(Freq_SetBuff,0x55,28);
                ClearWDT();
                ML7345_Write_Reg(ADDR_RF_STATUS,sta);
                WaitStatus_Complete();
            }
            status = ML7345_Read_Reg(ADDR_RF_STATUS) >> 4;
        }
    }
    return status;
}

/* 等待RF状态转换完成 */
u8 WaitStatus_Complete(void)
{
    u16 count = 0;
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL);  //set bank0
    while(1)
    {
        if(ML7345_Read_Reg(ADDR_INT_SOURCE_GRP1) & 0x08)
        {
            ML7345_Write_Reg(ADDR_INT_SOURCE_GRP1,0x00); //clear
            return 0;
        }
        if(count++ > 5000)    return 1;
        ClearWDT();
    }
}

/*
Function:State clear control
Parameter: TX_FIFO_POINTER,RX_FIFO_POINTER,PHY_STATE
Return: Null
*/
void ML7345_StateFlag_Clear(u8 sta)
{
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL);  //set bank0
    if(sta == TX_DONE_FLAG)
    {
        ML7345_Write_Reg(ADDR_INT_SOURCE_GRP3,0x00);
        ML7345_Write_Reg(ADDR_STATE_CLR,TX_FIFO_POINTER);
    }
    else if(sta == RX_DONE_FLAG)
    {
        ML7345_Write_Reg(ADDR_INT_SOURCE_GRP2,0x00);
        ML7345_Write_Reg(ADDR_STATE_CLR,RX_FIFO_POINTER);
    }
}
/*
Function:Interrupt Status Clear for INT0 to INT23
Parameter: Null
Return: Null
*/

void ML7345_AllStateFlag_Clear(void)
{
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL);  //set bank0
    ML7345_Write_Reg(ADDR_INT_SOURCE_GRP1,0x00);
    ML7345_Write_Reg(ADDR_INT_SOURCE_GRP2,0x00);
    ML7345_Write_Reg(ADDR_INT_SOURCE_GRP3,0x00);
}


/*
Function:Data rate set
Parameter: DATA_RATE_1_2K,DATA_RATE_2_4K,DATA_RATE_4_8K,DATA_RATE_9_6K
Return: Null
*/
void ML7345_DataRate_Set_4_8k(void)//(DataRate_ENUM rate)
{
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL);
    ML7345_Write_Reg(0x06,0x22);
    ML7345_Write_Reg(0x07,0x15);
    ML7345_Write_Reg(0x66,0x1E);
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK1_SEL);
    ML7345_Write_Reg(0x0C,0x93);
    ML7345_Write_Reg(0x0D,0x35);
    ML7345_Write_Reg(0x11,0x27);
    ML7345_Write_Reg(0x13,0x0C);
    ML7345_Write_Reg(0x2F,0x08);
    ML7345_Write_Reg(0x56,0x50);
    ML7345_Write_Reg(0x57,0x40);
    ML7345_Write_Reg(0x58,0x01);
    ML7345_Write_Reg(0x59,0x14);
    ML7345_Write_Reg(0x5A,0x00);
    ML7345_Write_Reg(0x5B,0x00);
    ML7345_Write_Reg(0x5C,0x15);
    ML7345_Write_Reg(0x5D,0x0D);
    ML7345_Write_Reg(0x5E,0x05);
    ML7345_Write_Reg(0x5F,0x67);
    ML7345_Write_Reg(0x60,0x09);
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK3_SEL);
    ML7345_Write_Reg(0x0C,0x41);
    ML7345_Write_Reg(0x0D,0x33);
    ML7345_Write_Reg(0x0E,0x54);
    ML7345_Write_Reg(0x14,0x10);
    ML7345_Write_Reg(0x24,0x03);
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL);
}

void ML7345_DataRate_Set_1_2k(void)
{
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL);
    ML7345_Write_Reg(0x06,0x00);
    ML7345_Write_Reg(0x66,0x1E);
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK1_SEL);
    ML7345_Write_Reg(0x0C,0x93);
    ML7345_Write_Reg(0x0D,0x35);
    ML7345_Write_Reg(0x11,0x27);
    ML7345_Write_Reg(0x13,0x0C);
    ML7345_Write_Reg(0x2F,0x08);
    ML7345_Write_Reg(0x57,0x40);
    ML7345_Write_Reg(0x58,0x01);
    ML7345_Write_Reg(0x59,0x14);
    ML7345_Write_Reg(0x5A,0x00);
    ML7345_Write_Reg(0x5B,0x00);
    ML7345_Write_Reg(0x5C,0x15);
    ML7345_Write_Reg(0x5D,0x1B);
    ML7345_Write_Reg(0x5E,0x05);
    ML7345_Write_Reg(0x5F,0x67);
    ML7345_Write_Reg(0x60,0x0C);
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK3_SEL);
    ML7345_Write_Reg(0x0C,0x41);
    ML7345_Write_Reg(0x0D,0x33);
    ML7345_Write_Reg(0x0E,0x54);
    ML7345_Write_Reg(0x14,0x10);
    ML7345_Write_Reg(0x24,0x03);
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL);
}

void ML7345_DataRate_Set_2_4k(void)//(DataRate_ENUM rate)
{
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL);
    ML7345_Write_Reg(0x06,0x11);
    ML7345_Write_Reg(0x07,0x15);
    ML7345_Write_Reg(0x66,0x1E);
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK1_SEL);
    ML7345_Write_Reg(0x0C,0x93);
    ML7345_Write_Reg(0x0D,0x35);
    ML7345_Write_Reg(0x11,0x27);
    ML7345_Write_Reg(0x13,0x0C);
    ML7345_Write_Reg(0x2F,0x08);
    ML7345_Write_Reg(0x56,0x50);
    ML7345_Write_Reg(0x57,0x40);
    ML7345_Write_Reg(0x58,0x01);
    ML7345_Write_Reg(0x59,0x14);
    ML7345_Write_Reg(0x5A,0x00);
    ML7345_Write_Reg(0x5B,0x00);
    ML7345_Write_Reg(0x5C,0x15);
    ML7345_Write_Reg(0x5D,0x0D);
    ML7345_Write_Reg(0x5E,0x05);
    ML7345_Write_Reg(0x5F,0x67);
    ML7345_Write_Reg(0x60,0x09);
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK3_SEL);
    ML7345_Write_Reg(0x0C,0x41);
    ML7345_Write_Reg(0x0D,0x33);
    ML7345_Write_Reg(0x0E,0x54);
    ML7345_Write_Reg(0x14,0x10);
    ML7345_Write_Reg(0x24,0x03);
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL);
}

void ML7345_DataRate_Set_9_6k(void)//(DataRate_ENUM rate)
{
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL);
    ML7345_Write_Reg(0x06,0x33);
    ML7345_Write_Reg(0x07,0x15);
    ML7345_Write_Reg(0x66,0x1E);
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK1_SEL);
    ML7345_Write_Reg(0x0C,0x93);
    ML7345_Write_Reg(0x0D,0x35);
    ML7345_Write_Reg(0x11,0x27);
    ML7345_Write_Reg(0x13,0x0C);
    ML7345_Write_Reg(0x2F,0x08);
    ML7345_Write_Reg(0x56,0x50);
    ML7345_Write_Reg(0x57,0x04);
    ML7345_Write_Reg(0x58,0x01);
    ML7345_Write_Reg(0x59,0x14);
    ML7345_Write_Reg(0x5A,0x00);
    ML7345_Write_Reg(0x5B,0x00);
    ML7345_Write_Reg(0x5C,0x15);
    ML7345_Write_Reg(0x5D,0x0D);
    ML7345_Write_Reg(0x5E,0x04);
    ML7345_Write_Reg(0x5F,0x5D);
    ML7345_Write_Reg(0x60,0x0C);
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK3_SEL);
    ML7345_Write_Reg(0x0C,0x41);
    ML7345_Write_Reg(0x0D,0x33);
    ML7345_Write_Reg(0x0E,0x54);
    ML7345_Write_Reg(0x14,0x10);
    ML7345_Write_Reg(0x24,0x03);
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL);
}


/*
Function: FIFO readout setting
Parameter: 0:RX FIFO read when reading FIFO(ADDR_RD_FIFO); 1: TX FIFO read when reading FIFO(ADDR_RD_FIFO)
Return: Null
*/
void ML7345_FIFO_ReadOut_Set(u8 select)
{
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL);
    if(select == 0x00)  ML7345_Write_Reg(ADDR_FIFO_SET,0x00);
    else if(select == 0x01) ML7345_Write_Reg(ADDR_FIFO_SET,0x01);
}


/*
Function: Get ED value
Parameter: Null
Return: ED value
*/
u8 ML7345_RSSI_Val(void)
{
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL);
    return ML7345_Read_Reg(ADDR_ED_RSLT);
}


/*
Function: Soft Reset
Parameter: Null
Return: Null
*/
void ML7345_SoftReset(void)
{
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL);
    ML7345_Write_Reg(ADDR_RST_SET,0xff);
}

/*
Function: Hardware Reset
Parameter: Null
Return: Null
*/
void ML7345_RESETN_SET(void)
{
    ML7345_RESETN = 0;
    system_delay(5);
    ML7345_RESETN = 1;
}

/*
Function: Sleep Mode Set
Parameter: Deep_Sleep,Sleep1_Mode,Sleep2_Mode
Return: Null
*/
void ML7345_GoSleep(SleepMode_ENUM mode)
{
    switch(mode)
    {
        case Deep_Sleep:
            //RF_WORKE_MODE = 1;
            break;

        case Sleep1_Mode:
            ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL); /* Bank0 Set */
            ML7345_Write_Reg(ADDR_CLK_SET2,ML7345_Read_Reg(ADDR_CLK_SET2) & 0xF7);
            ML7345_Write_Reg(ADDR_SLEEP_WU_SET,0x07);
            break;

        case Sleep2_Mode:
            ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL); /* Bank0 Set */
            ML7345_Write_Reg(ADDR_CLK_SET2,ML7345_Read_Reg(ADDR_CLK_SET2) | 0x08);
            ML7345_Write_Reg(ADDR_SLEEP_WU_SET,0x37);
            break;

        default:
            break;
    }
}


/*
Function: Frequency Set
Parameter: Frequency
Return: Null
*/
void ML7345_Frequency_Set(u8 *freq,u8 radio_type)
{
    Flag_set_freq = 1;
    if(ML7345_SetAndGet_State(Get_Sta) != TRX_OFF)
        ML7345_SetAndGet_State(Force_TRX_OFF);
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK1_SEL); /* Bank1 Set */
    ML7345_Write_Reg(ADDR_TXFREQ_I,freq[0]);
    ML7345_Write_Reg(ADDR_TXFREQ_FH,freq[1]);
    ML7345_Write_Reg(ADDR_TXFREQ_FM,freq[2]);
    ML7345_Write_Reg(ADDR_TXFREQ_FL,freq[3]);
    ML7345_Write_Reg(ADDR_RXFREQ_I,freq[0]);
    ML7345_Write_Reg(ADDR_RXFREQ_FH,freq[1]);
    ML7345_Write_Reg(ADDR_RXFREQ_FM,freq[2]);
    ML7345_Write_Reg(ADDR_RXFREQ_FL,freq[3]);

    ML7345_Write_Reg(ADDR_VCO_CAL_MIN_I,freq[4]);      //P107
    ML7345_Write_Reg(ADDR_VCO_CAL_MIN_FH,freq[5]);
    ML7345_Write_Reg(ADDR_VCO_CAL_MIN_FM,freq[6]);
    ML7345_Write_Reg(ADDR_VCO_CAL_MIN_FL,freq[7]);

    ML7345_Write_Reg(0x00,0x22);    /* Bank1 Set */
    ML7345_Write_Reg(0x25,0x08);    /* SyncWord length setting */
    ML7345_Write_Reg(0x2a,0x55);

    if(radio_type == 1) ML7345_DataRate_Set_1_2k();
    else if(radio_type == 2)  ML7345_DataRate_Set_4_8k();

    ML7345_Write_Reg(0x6f, 0x01);     /* VCO_CAL_START(CAL start) */
    while(1){
        if(ML7345_Read_Reg(0x0Du)&0x02u){   /* Wait VCO calibration completion */
            break;
        }
    }

    ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL); /* Bank0 Set */
    Flag_set_freq = 0;
}


/*
Function: ML7345 TX RX INT CONFIG
Parameter: GpioCtrlAddr:GPIO ADDR; Intnum:Tx Done Interrupt or Rx Done Interrupt; Inten:1 Enable,0 Disable
Return: Null
*/
void ML7345_TRX_Int_Config(u8 GpioCtrlAddr,RF_TRX_ENUM Intnum,u8 Inten)
{
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL);    /* BANK 0 SEL*/
    if(Inten)
    {
        ML7345_Write_Reg(GpioCtrlAddr,GPIO_INTOUTPUT_ENABLE);    /* GPIO 中断输出(SINTN) */

        if(Intnum == RF_TxDone_Int)  ML7345_Write_Reg(ADDR_INT_EN_GRP3,0x01);    /* 使能中断事件16,发送完成中断 */
        else if(Intnum == RF_RxDone_Int) ML7345_Write_Reg(ADDR_INT_EN_GRP2,0x21);/* 0x21:使能中断事件8/13,接收完成中断/同步字检测中断;0x01:使能中断事件8,接收完成中断 */
    }
    else
    {
        ML7345_Write_Reg(GpioCtrlAddr,GPIO_INTOUTPUT_DISABLE);    /* GPIO 禁止中断输出(SINTN) */

        if(Intnum == RF_TxDone_Int)  ML7345_Write_Reg(ADDR_INT_EN_GRP3,0x00);    /* 禁止中断事件16,发送完成中断 */
        else if(Intnum == RF_RxDone_Int) ML7345_Write_Reg(ADDR_INT_EN_GRP2,0x00);/* 禁止中断事件8,接收完成中断 */
    }
}

void ML7345_GPIO1TxDoneInt_Enable(void)
{
    ML7345_TRX_Int_Config(ADDR_GPIO1_CTRL,RF_TxDone_Int,1);
}
void ML7345_GPIO1TxDoneInt_Disable(void)
{
    ML7345_TRX_Int_Config(ADDR_GPIO1_CTRL,RF_TxDone_Int,0);
}

void ML7345_GPIO1RxDoneInt_Enable(void)
{
    ML7345_TRX_Int_Config(ADDR_GPIO1_CTRL,RF_RxDone_Int,1);
}
void ML7345_GPIO1RxDoneInt_Disable(void)
{
    ML7345_TRX_Int_Config(ADDR_GPIO1_CTRL,RF_RxDone_Int,0);
}

void ML7345_GPIO2TxDoneInt_Enable(void)
{
    ML7345_TRX_Int_Config(ADDR_GPIO2_CTRL,RF_TxDone_Int,1);
}
void ML7345_GPIO2TxDoneInt_Disable(void)
{
    ML7345_TRX_Int_Config(ADDR_GPIO2_CTRL,RF_TxDone_Int,0);
}

void ML7345_GPIO2RxDoneInt_Enable(void)
{
    ML7345_TRX_Int_Config(ADDR_GPIO2_CTRL,RF_RxDone_Int,1);
}
void ML7345_GPIO2RxDoneInt_Disable(void)
{
    ML7345_TRX_Int_Config(ADDR_GPIO2_CTRL,RF_RxDone_Int,0);
}


/*
Function: Transmit Power Setting
Parameter: Frequency
Return: Null
*/
void ML7345_TransmitPower_Set(TransmitPower_ENUM Power)
{
    if(ML7345_SetAndGet_State(Get_Sta) != TRX_OFF)
        ML7345_SetAndGet_State(TRX_OFF);
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL); /* Bank0 Set */
    switch(Power)
    {
        case Power_0dBm:
            ML7345_Write_Reg(ADDR_PA_MODE,0x02);
            ML7345_Write_Reg(ADDR_PA_REG_FINE_ADJ,0x10);
            ML7345_Write_Reg(ADDR_PA_ADJ,0x00);
            break;
        case Power_1dBm:
            ML7345_Write_Reg(ADDR_PA_MODE,0x03);
            ML7345_Write_Reg(ADDR_PA_REG_FINE_ADJ,0x10);
            ML7345_Write_Reg(ADDR_PA_ADJ,0x01);
            break;
        case Power_1_5dBm:
            ML7345_Write_Reg(ADDR_PA_MODE,0x06);
            ML7345_Write_Reg(ADDR_PA_REG_FINE_ADJ,0x10);
            ML7345_Write_Reg(ADDR_PA_ADJ,0x01);
            break;
        case Power_2dBm:
            ML7345_Write_Reg(ADDR_PA_MODE,0x07);
            ML7345_Write_Reg(ADDR_PA_REG_FINE_ADJ,0x10);
            ML7345_Write_Reg(ADDR_PA_ADJ,0x01);
            break;
        case Power_3dBm:
            ML7345_Write_Reg(ADDR_PA_MODE,0x06);
            ML7345_Write_Reg(ADDR_PA_REG_FINE_ADJ,0x10);
            ML7345_Write_Reg(ADDR_PA_ADJ,0x03);
            break;
        case Power_4dBm:
            ML7345_Write_Reg(ADDR_PA_MODE,0x07);
            ML7345_Write_Reg(ADDR_PA_REG_FINE_ADJ,0x10);
            ML7345_Write_Reg(ADDR_PA_ADJ,0x04);
            break;
        case Power_5dBm:
            ML7345_Write_Reg(ADDR_PA_MODE,0x07);
            ML7345_Write_Reg(ADDR_PA_REG_FINE_ADJ,0x10);
            ML7345_Write_Reg(ADDR_PA_ADJ,0x06);
            break;
        case Power_6dBm:
            ML7345_Write_Reg(ADDR_PA_MODE,0x06);
            ML7345_Write_Reg(ADDR_PA_REG_FINE_ADJ,0x10);
            ML7345_Write_Reg(ADDR_PA_ADJ,0x09);
            break;
        case Power_7dBm:
            ML7345_Write_Reg(ADDR_PA_MODE,0x07);
            ML7345_Write_Reg(ADDR_PA_REG_FINE_ADJ,0x10);
            ML7345_Write_Reg(ADDR_PA_ADJ,0x0B);
            break;
        case Power_8dBm:
            ML7345_Write_Reg(ADDR_PA_MODE,0x13);
            ML7345_Write_Reg(ADDR_PA_REG_FINE_ADJ,0x10);
            ML7345_Write_Reg(ADDR_PA_ADJ,0x00);
            break;
        case Power_9dBm:
            ML7345_Write_Reg(ADDR_PA_MODE,0x16);
            ML7345_Write_Reg(ADDR_PA_REG_FINE_ADJ,0x10);
            ML7345_Write_Reg(ADDR_PA_ADJ,0x00);
            break;
        case Power_10dBm:
            ML7345_Write_Reg(ADDR_PA_MODE,0x15);
            ML7345_Write_Reg(ADDR_PA_REG_FINE_ADJ,0x10);
            ML7345_Write_Reg(ADDR_PA_ADJ,0x01);
            break;
        case Power_11dBm:
            ML7345_Write_Reg(ADDR_PA_MODE,0x16);
            ML7345_Write_Reg(ADDR_PA_REG_FINE_ADJ,0x10);
            ML7345_Write_Reg(ADDR_PA_ADJ,0x02);
            break;
        case Power_12dBm:
            ML7345_Write_Reg(ADDR_PA_MODE,0x16);
            ML7345_Write_Reg(ADDR_PA_REG_FINE_ADJ,0x10);
            ML7345_Write_Reg(ADDR_PA_ADJ,0x03);
            break;
        case Power_13dBm:
            ML7345_Write_Reg(ADDR_PA_MODE,0x15);
            ML7345_Write_Reg(ADDR_PA_REG_FINE_ADJ,0x10);
            ML7345_Write_Reg(ADDR_PA_ADJ,0x05);
            break;
        default:
            break;
    }
}

/*
Function: Read receive packet
Parameter: pbuf: RX buffer; len: Read length,No more than 64 bytes
Return: Null
*/
void ML7345_ReadRx_Pack(u8 *pbuf,u8 len)
{
    if(len > 64)    len = 64;
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL);
    ML7345_Read_Fifo(ADDR_RD_FIFO,pbuf,len);
}

/*
Function: send data
Parameter: pbuf: data buffer; len: data length,No more than 64 bytes
Return: Null
*/
void ML7345_AutoTx_Data(u8 *pbuf,u8 len)
{
    if(len > 64)    len = 64;
    CG2214M6_USE_T;
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL);  //set bank0
    ML7345_Write_Reg(ADDR_TX_PKT_LEN_L,len);    //发送包长度低八位
    ML7345_AutoStateTransition_Set(AUTO_TX_EN);  //设置为自动发送模式
    ML7345_Write_Fifo(ADDR_WR_TX_FIFO,pbuf,len);
}

u8 RF_SyncWord_DONE(void)
{
    u8 sync = 0;
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL);  //set bank0
    sync = ML7345_Read_Reg(ADDR_INT_SOURCE_GRP2);
    return sync;
}

void ML7345_Frequency_Calcul(u32 Freq,u8 *pbuf)  //将频率计算为ML7345寄存器设定值,存入pbuf中
{
    u8 integer = 0;
    float f_dec = 0.0;
    u32 cal_val = 0;
    integer = (u8)(Freq / FREQ_PLL);
    f_dec = ((float)(Freq / (float)FREQ_PLL) - integer);
    cal_val = (u32)(f_dec *  CONST_COEFF);
    pbuf[0] = integer;
    pbuf[1] = (cal_val >> 16) & 0xff;
    pbuf[2] = (cal_val >> 8) & 0xff;
    pbuf[3] = cal_val & 0xff;

    Freq = Freq - VCO_LowerLimit_FREQ;
    integer = (u8)(Freq / FREQ_PLL);
    f_dec = ((float)(Freq / (float)FREQ_PLL) - integer);
    cal_val = (u32)(f_dec *  CONST_COEFF);
    pbuf[4] = integer;
    pbuf[5] = (cal_val >> 16) & 0xff;
    pbuf[6] = (cal_val >> 8) & 0xff;
    pbuf[7] = cal_val & 0xff;
}

void SetReg_Rssi(void)
{
    ML7345_Write_Reg(0x00,0x22);  //Bank 1
    ML7345_Write_Reg(0x07,0x5A);
    ML7345_Write_Reg(0x0C,0x8A);
    ML7345_Write_Reg(0x0D,0x26);
    ML7345_Write_Reg(0x10,0x03);
    ML7345_Write_Reg(0x15,0x83);
    ML7345_Write_Reg(0x00,0x11);  //Bank 0
}

/*
void SetReg_RssiTest(void)
{
    ML7345_Write_Reg(0x00,0x22);  //Bank 1
    ML7345_Write_Reg(0x07,0x5A);
    ML7345_Write_Reg(0x10,0x03);
    ML7345_Write_Reg(0x12,0x77);
    ML7345_Write_Reg(0x15,0x83);

    ML7345_Write_Reg(0x00,0x44);  //Bank 2
    ML7345_Write_Reg(0x09,0x09);
    ML7345_Write_Reg(0x0C,0x7D);
    ML7345_Write_Reg(0x0E,0x00);
    ML7345_Write_Reg(0x16,0x23);
    ML7345_Write_Reg(0x1C,0x23);
    ML7345_Write_Reg(0x2A,0x0F);
    ML7345_Write_Reg(0x4E,0x70);

    ML7345_Write_Reg(0x00,0x88);  //Bank 3
    ML7345_Write_Reg(0x31,0x00);
    ML7345_Write_Reg(0x32,0x00);
    ML7345_Write_Reg(0x33,0x00);
    ML7345_Write_Reg(0x34,0x00);
    ML7345_Write_Reg(0x35,0x00);
    ML7345_Write_Reg(0x36,0x00);
    ML7345_Write_Reg(0x37,0x00);
    ML7345_Write_Reg(0x38,0x00);

    ML7345_Write_Reg(0x00,0x11);  //Bank 0
}*/
