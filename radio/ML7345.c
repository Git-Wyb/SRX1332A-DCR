#include "ML7345.h"

//#include <stdlib.h>
//#include <stdbool.h> /* For true/false definition                      */
//#include <stdio.h>
#include "spi.h"
#include "Pin_define.h" // 芯片引脚定于
#include "ram.h"
#include "ID_Decode.h"
#include "type_def.h"
#include "uart.h"

//ROHM demo板 main fre=426.25;Vco Cal Range:425.85 - 431.85.MHz
//TCXO = 24M
u8 Fre_426_075[8] = {0x23,0x08,0x19,0x99,0x23,0x07,0x91,0x11}; //426.075MHz频率设置
u8 Fre_429_350[8] = {0x23,0x0C,0x77,0x77,0x23,0x0B,0xEE,0xEF}; //429.350MHz频率设置
u8 Fre_429_550[8] = {0x23,0x0C,0xBB,0xBC,0x23,0x0C,0x33,0x33}; //429.550MHz频率设置
u8 Fre_426_750[8] = {0x23,0x09,0x00,0x00,0x23,0x08,0x77,0x77}; //426.750MHz频率设置
u8 Fre_429_175[8] = {0x23,0x0c,0x3b,0xbc,0x23,0x0b,0xb3,0x33}; //429.175MHz频率设置
u8 Fre_429_200[8] = {0x23,0x0c,0x44,0x44,0x23,0x0b,0xbb,0xbc}; //429.200MHz频率设置

u8 FilterChar[2][1] = {
    {' '},
    {'G'}};
/**ADF7030_REST**/ u8 ADF7030_REST_Cache = 0;
/**Receiver_vent**/ u8 Receiver_vent_Cache = 0;
u8 SPI_SEND_BUFF[SPI_SEND_BUFF_LONG] = {0X55};
u8 SPI_RECEIVE_BUFF[SPI_REV_BUFF_LONG] = {0};
u32 SPI_Receive_DataForC[7]; //C郿
u16 Head_0x5515_or_0x5456 = 0;
u32 ADF7030_RESIGER_VALUE_READ = 0;
u8 ADF7030_Read_OneByte = 0;
u8 RX_COUNT = 0;
u8 Radio_State = 0;
u32 CCA_READBACK_Cache = 0;
/************Address: 0x20000500, Name: GENERIC_PKT_FRAME_CFG1***************************************/
u8 TRX_IRQ1_TYPE = 0x80;
u8 TRX_IRQ0_TYPE = 0x04;
u8 GENERIC_PKT_FRAME_CFG1_13_15 = 0;
u8 PREAMBLE_UNIT = 1;
u16 PAYLOAD_SIZE = 0x0C;

u8 BREState = 0;
u8 Radio_Date_Type = 1;
u8 tx_pack_cnt = 0;
u8 read_reg = 0;
u8 Radio_Date_Type_bak=1;
u8 APP_TX_freq=0;

u8 TX_Scan_step=0;
u8 First_TX_Scan=0;
u8 TX_Scan_count=0;
u8 rf_offset = 0;

u8 Freq_SetBuff[8] = {0};
/*
Function: RF Init
Parameter: Null
Return: Null
*/
/* Initialize: 426MHz General Packet(TCXO_ChStep=12.5kHz_DR=1.2kbps_Dev=0.6kHz_GFSK)
  */
void RF_ML7345_Init(u8* freq,u8 sync,u8 rx_len)
{
    ML7345_RESETN_SET();    /* Hardware Reset */
    while(1){
        if(ML7345_Read_Reg(0x0Du)&0x01u){   /* Wait Clock stabilization completion */
            break;
        }
    }
    ML7345_Write_Reg(0x00,0x11);    /* Bank0 Set */
    ML7345_Write_Reg(0x01,0xff);    /* Software Reset */
    ML7345_Write_Reg(0x02,0x9f);    /* Clk Enable */
    ML7345_Write_Reg(0x03,0xc3);    /* TCXO_EN */
    ML7345_Write_Reg(0x04,0x03);    /* MSB first,Format D (non Wireless M-Bus, general purpose format) */
    ML7345_Write_Reg(0x05,0x10);    /* CRC Disable,Length field 1-byte mode */
    ML7345_Write_Reg(0x06,0x00);    /* Data Rate: 0x00 1.2kbps; 0x22 4.8kbps */
    ML7345_Write_Reg(0x07,0x15);    /* GFSK,NRZ */
    ML7345_Write_Reg(0x08,0x00);    /* Sync word pattern 1,Manchester Do not inverse polarity */
    ML7345_Write_Reg(0x09,0x00);    /* CH 0 */
    ML7345_Write_Reg(0x0a,0x18);    /* AUTO_TX_EN */
    ML7345_Write_Reg(0x0b,0x88);    /* TRX_OFF */
    ML7345_Write_Reg(0x0c,0x00);    /* FIFO mode */
    ML7345_Write_Reg(0x0d,0x08);    /* [INT_SOURCE_GRP1],Interrupt status for INT0 to INT7 */
    ML7345_Write_Reg(0x0e,0x00);    /* [INT_SOURCE_GRP2],Interrupt status for INT8 to INT15 (RX) */
    ML7345_Write_Reg(0x0f,0x00);    /* [INT_SOURCE_GRP3],Interrupt status for INT16 to INT23 (TX) */
    ML7345_Write_Reg(0x10,0x00);    /* masking interrupt,[INT_EN_GRP1]Interupt mask for INT0 to INT7 */
    ML7345_Write_Reg(0x11,0x00);    /* masking interrupt,[INT_EN_GRP2]Interupt mask for INT8 to INT15 */
    ML7345_Write_Reg(0x12,0x00);    /* masking interrupt,[INT_EN_GRP3]Interupt mask for INT16 to INT23 */
    ML7345_Write_Reg(0x13,0x00);    /* [CRC_ERR_H]CRC error status (high byte) */
    ML7345_Write_Reg(0x14,0x00);    /* [CRC_ERR_M]CRC error status (middle byte)*/
    ML7345_Write_Reg(0x15,0x00);    /* [CRC_ERR_L]CRC error status (low byte)*/
    ML7345_Write_Reg(0x16,0x00);    /* State clear control */
    ML7345_Write_Reg(0x17,0x00);    /* TX FIFO-Full level setting */
    ML7345_Write_Reg(0x18,0x00);    /* TX FIFO-Empty level setting and TX trigger level setting in FAST_TX mode */
    ML7345_Write_Reg(0x19,0x00);    /* RX FIFO-Full level disable and level setting */
    ML7345_Write_Reg(0x1a,0x00);    /* RX FIFO-Empty level disable and level setting (high byte) */

    ML7345_Write_Reg(0x2d,0x06);    /* SLEEP execution and Wake-up operation setting */
    /* Preamble */
    ML7345_Write_Reg(0x3f,0x05);    /* Preamble pattern setting,p198*/

    ML7345_Write_Reg(0x41,0x8B);    /* Enable ED value calculation,bit3=0 ED value constantly updated,bit3=1 ED value acquired at SyncWord detection timing*/

    ML7345_Write_Reg(0x42,0x00);    //TX前导码长度高八位
    ML7345_Write_Reg(0x43,0x34);    //TX前导码长度低八位,不等少于16个位,TX preamble length = (specified value x2) bits
    //-----------------------------------------------------------------------------------------------------
    ML7345_Write_Reg(0x45,0x08);    //接收前导码长度(bit) RX preamble setting and ED threshold check setting
    //-----------------------------------------------------------------------------------------------------
    ML7345_Write_Reg(0x4e,0x00);    /* GPIO0 [output] “L” level */
    ML7345_Write_Reg(0x4f,0x00);    /* GPIO1 [output] “L” level,Upon reset,disable GPIO1 pin is CLK_OUT function */
    ML7345_Write_Reg(0x50,0x00);    /* GPIO2 [output] “L” level */
    ML7345_Write_Reg(0x51,0x00);    /* GPIO3 [output] “L” level */

    ML7345_Write_Reg(0x54,0x14);    /* Channel filter bandwidth setting */

    ML7345_Write_Reg(0x60,0x06); /* Decimation gain setting 提高灵敏度 */

    ML7345_Write_Reg(0x63,0x88);    /* Fine adjustment of load capacitance for oscillation circuits */

    ML7345_Write_Reg(0x67,0x16);    /* 功率设置12dbm */
    ML7345_Write_Reg(0x68,0x10);    /* PA regulator fine adjustment */
    ML7345_Write_Reg(0x69,0x03);    /* PA gain adjustment */

    ML7345_Write_Reg(0x6e,0x5b);    /*5b VCO calibration setting or status indication */

    ML7345_Write_Reg(0x7a,0x00);    //发送包长度高八位
    ML7345_Write_Reg(0x7b,0x0c);    //发送包长度低八位

    ML7345_Write_Reg(0x7d,0x00);    //接收包长度高八位
    ML7345_Write_Reg(0x7e,rx_len);    //接收包长度低八位

    /***************************************************************************************************/

    ML7345_Write_Reg(0x00,0x22);    /* Bank1 Set */
    ML7345_Write_Reg(0x02,0x07);    /* TX data rate conversion setting (high 4bits) */
    ML7345_Write_Reg(0x03,0xd0);    /* TX data rate conversion setting (low byte) */
    ML7345_Write_Reg(0x04,0x00);    /* RX data rate concversion setting 1 (high 4bits) */
    ML7345_Write_Reg(0x05,0x50);    /* RX data rate cnvesrion setting 1 (low byte) */

    ML7345_Write_Reg(0x11,0x27);    /* RSSI offset value setting during low gain operation */

    ML7345_Write_Reg(0x13,0x0c);    /* Scale factor setting for ED value conversion */

    ML7345_Write_Reg(0x1a,0x02);    /* Divide 2,PLL frequency division setting */
    /* 426.075MHz */
    ML7345_Write_Reg(0x1b,freq[0]);    /* TX frequency setting (I counter),p67-69 */
    ML7345_Write_Reg(0x1c,freq[1]);    /* TX frequency setting (F counter high 4bit) */
    ML7345_Write_Reg(0x1d,freq[2]);    /* TX frequency setting (F counter middle byte) */
    ML7345_Write_Reg(0x1e,freq[3]);    /* TX frequency setting (F counter low byte) */
    ML7345_Write_Reg(0x1f,freq[0]);    /* RX frequency setting (I counter) */
    ML7345_Write_Reg(0x20,freq[1]);    /* RX frequency setting (F counter high 4bit)  */
    ML7345_Write_Reg(0x21,freq[2]);    /* RX frequency setting (F counter middle byte) */
    ML7345_Write_Reg(0x22,freq[3]);    /* RX frequency setting (F counter low byte) */

    ML7345_Write_Reg(0x23,0x08);    /* 25KHz,Channel space setting (high byte) */
    ML7345_Write_Reg(0x24,0x88);    /* Channel space setting (low byte) */
    /* SyncWord 0x00005515 */
    ML7345_Write_Reg(0x25,0x08);    /* SyncWord length setting */
    ML7345_Write_Reg(0x26,0x0f);    /* SyncWord enable setting */
    ML7345_Write_Reg(0x27,0x00);    /* SyncWord #1 setting (bit24 to 31) */
    ML7345_Write_Reg(0x28,0x00);    /* SyncWord #1 setting (bit16 to 23) */
    ML7345_Write_Reg(0x29,0x00);    /* SyncWord #1 setting (bit8 to 15) */
    ML7345_Write_Reg(0x2a,sync);    /* SyncWord #1 setting (bit0 to 7) */

    ML7345_Write_Reg(0x2f,0x08);    /* GFSK clock setting x8 clock,GFSK/FSK modulation timing resolution setting */
    ML7345_Write_Reg(0x30,0x00);    /* GFSK frequency deviation setting (high 6bits) */
    ML7345_Write_Reg(0x31,0xAE);    /* P71  GFSK frequency deviation:2.0KHz,GFSK frequency deviation setting (low byte) */

    /*(Channel#0 frequency-400khz)426.075:0x23 0x07 0x91 0x11;
    426.25:0x23 0x07 0xcc 0xcc */
    ML7345_Write_Reg(0x4d,freq[4]);    /* VCO calibration low limit frequency setting (I counter),p254 */
    ML7345_Write_Reg(0x4e,freq[5]);    /* VCO calibration low limit frequency setting (F counter high 4bits) */
    ML7345_Write_Reg(0x4f,freq[6]);    /* VCO calibration low limit frequency setting (F counter middle byte) */
    ML7345_Write_Reg(0x50,freq[7]);    /* VCO calibration low limit frequency setting (F counter low byte) */
    ML7345_Write_Reg(0x51,0x04);    /* 6 MHz,VCO calibration upper limit frequency setting */
    ML7345_Write_Reg(0x52,0x5a);    /* VCO calibration low limit value indication and setting */
    ML7345_Write_Reg(0x53,0x66);    /* VCO calibration upper limit value indication and setting */

    ML7345_Write_Reg(0x5c,0x15);    /* 0x10 Demodulator configulation 6 */
    ML7345_Write_Reg(0x5d,0x1b);    /* 0x1b Demodulator configulation 7 */
    ML7345_Write_Reg(0x5e,0x05);    /* 0x05 Demodulator configuration 8 */
    ML7345_Write_Reg(0x5f,0x67);    /* 0x67 Demodulator configulation 9 */
    ML7345_Write_Reg(0x60,0x0c);    /* Demodulator configulation 10 */

    SetReg_Rssi();  //2021.11.17 ROHM提出必要设置,否则读取RSSI偏差大。

    ML7345_Write_Reg(0x00,0x44); //bank2
    ML7345_Write_Reg(0x2a,0x0f); //2019.11.25 ROHM提出设置,设置之后灵敏度提升了2dBm

    /* 以下寄存器值为发送功率必要设置，不然发送功率偏差大 */
    ML7345_Write_Reg(0x00,0x44); //bank2
    ML7345_Write_Reg(0x4f,0x00);

    ML7345_Write_Reg(0x00,0x88); //bank3
    ML7345_Write_Reg(0x2f,0x3f);
    ML7345_Write_Reg(0x0c,0x41);
    ML7345_Write_Reg(0x0d,0x33);
    ML7345_Write_Reg(0x0e,0x54);
    ML7345_Write_Reg(0x14,0x10);
    ML7345_Write_Reg(0x24,0x03);
    ML7345_Write_Reg(0x7e,0x00);

    if((PROFILE_CH_FREQ_32bit_200002EC == 429350000) || (PROFILE_CH_FREQ_32bit_200002EC == 429550000)) ML7345_DataRate_Set_4_8k();
    else    ML7345_DataRate_Set_1_2k();
    /*****************************************************/

    ML7345_Write_Reg(0x00, 0x11);     /* BANK_SEL(BANK0) */
    ML7345_Write_Reg(0x6f, 0x01);     /* VCO_CAL_START(CAL start) */
    while(1){
        if(ML7345_Read_Reg(0x0Du)&0x02u){   /* Wait VCO calibration completion */
            break;
        }
    }
    ML7345_AllStateFlag_Clear(); //清除所有标志
}

void ML7345_MeasurBER_Init(void)
{
    ML7345_Write_Reg(ADDR_BANK_SEL,BANK1_SEL);
    ML7345_Write_Reg(0x0e,0x00);

    ML7345_Write_Reg(ADDR_BANK_SEL,BANK3_SEL);
    ML7345_Write_Reg(0x0d,0x33);

    ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL);
    ML7345_Write_Reg(0x0c,0x40); //50   /* DIO_SET, continuous output mode,DCLK is constantly output,no OUTPUT (NOT stop output)*/
    ML7345_Write_Reg(0x4d,0x80); //80

    ML7345_Write_Reg(0x4f,0x05);    //ML7345D GPIO1 DCLK
    ML7345_Write_Reg(0x51,0x04);    //ML7345D GPIO3 DIO
}




void Ber_PinExit_Init(void)
{
    ML7345_DATA_DDR = 0;
    ML7345_DATA_CR1 = 1;
    ML7345_DATA_CR2 = 0;

    ML7345_CLK_DDR = 0;
    ML7345_CLK_CR1 = 0; // Floating input
    ML7345_CLK_CR2 = 1;
    EXTI_CR2 &= (~MASK_EXTI_CR2_P6IS);
    //EXTI_CR2 |= 0x08 << 2;   // 下降沿触发
    EXTI_CR2 |= 0x04 << 2;  //上升沿
}

void Ber_Exit_UnInit(void)
{
    ML7345_DATA_DDR = 0;
    ML7345_DATA_CR1 = 1;
    ML7345_DATA_CR2 = 0;

    ML7345_CLK_DDR = 0;
    ML7345_CLK_CR1 = 1;
    ML7345_CLK_CR2 = 0;
}

void Tx_Data_Test(u8 mode)
{
    ML7345_Write_Reg(0x00, 0x11);     /* BANK_SEL(BANK0) */
    switch(mode)
    {
        /* 发送：载波 */
        case 0: ML7345_Write_Reg(ADDR_RF_TEST_MODE,0x21);break;

        /* 发送：0101 */
        case 1:ML7345_Write_Reg(ADDR_RF_TEST_MODE,0x11);break;

        /* 发送：0 */
        case 2:ML7345_Write_Reg(ADDR_RF_TEST_MODE,0x09);break;

        /* 发送：1 */
        case 3: ML7345_Write_Reg(ADDR_RF_TEST_MODE,0x05);break;

         /* 发送：随机 */
        default:
            ML7345_Write_Reg(ADDR_RF_TEST_MODE,0x03);
         break;
    }
    ML7345_SetAndGet_State(TX_ON);
}

void DataReceive(void)
{
    static u8 StateCache = 0;
    static u8 Cache = 0;
    static u8 X_HISbyte = 0;
    switch (StateCache)
    {
        case 0:
        {
            Cache <<= 1;
            if (ML7345_DATA)
                Cache++;
            if (Cache == 0x55)
            {
                StateCache = 1;
                X_HISbyte = 0;
                Cache = 0;
            }
        }
        break;
        case 1:
        {
            if (ML7345_DATA != X_HISbyte)
                X_ERR++;
            X_COUNT++;
            X_HISbyte ^= 1;
            if (X_COUNT >= 1000)
                StateCache = 2;
        }
        break;
        case 2:
            if (X_COUNT == 0)
                StateCache = 0;
            break;
        default:
            break;
    }
    EXTI_SR1_P6F = 1;
}

void RF_Ber_Test(void)
{
    if (X_COUNT >= 1000)
    {
        if (X_ERR >= 50) Receiver_LED_RX = 0;
        else             Receiver_LED_RX = 1;
        X_ERR = 0;
        X_COUNT = 0;
        X_ERRTimer = 1250;
    }
    if (X_ERRTimer == 0)
        Receiver_LED_RX = 0;
}

void Uart_RF_Ber_Test(void)
{
    if (X_COUNT >= 1000)
    {
        if(X_ERR > 255)  X_ERR_CNT = 255;
        else X_ERR_CNT = X_ERR;

        if (X_ERR >= 50) Receiver_LED_RX = 0;
        else             Receiver_LED_RX = 1;

        X_ERR = 0;
        X_COUNT = 0;
        X_ERRTimer = 1250;
    }
    if (X_ERRTimer == 0)
        Receiver_LED_RX = 0;
}

void APP_TX_PACKET(void)
{
    char rssi;
	 u8 i=2,j=0;

	 //if((TP3==0)&&(FLAG_Key_TP3==0))FLAG_Key_TP3=1;

      if((Time_error_read_timeout==0)&&(Flag_ERROR_Read==1))
      {
	  	Flag_ERROR_Read=0;
		FLAG_APP_TX_fromUART_err_read=0;
      }
      if( (Flag_FREQ_Scan==0) && ((PROFILE_CH_FREQ_32bit_200002EC == PROFILE_CH1_FREQ_32bit_429HighSpeed)||(PROFILE_CH_FREQ_32bit_200002EC == PROFILE_CH2_FREQ_32bit_429HighSpeed)) &&
	  	  ((FLAG_APP_TX_fromUART_err_read==1) && (Flag_ERROR_Read==1) && (Time_error_read_gap==0) && ((FLAG_APP_RX==1)||(FLAG_APP_RXstart==1)) && (Radio_Date_Type_bak==2))
	  	   )
      {
         for(j=0;j<8;j++)Last_Uart_Struct_DATA_Packet_Contro.data[j/2].uc[j%2]=0x00;

         Last_Uart_Struct_DATA_Packet_Contro.Fno_Type.UN.type=0x1f;
		 Last_Uart_Struct_DATA_Packet_Contro.data[0].uc[0]=0x98;
		 ERROR_Read_sendTX_packet++;
		 Last_Uart_Struct_DATA_Packet_Contro.data[0].uc[1]=ERROR_Read_sendTX_packet;
		 while(ERROR_Read_sendTX_count<UART_DATA_ID98[1]-4)
		 {
		     Last_Uart_Struct_DATA_Packet_Contro.data[i/2].uc[i%2]=UART_DATA_ID98[ERROR_Read_sendTX_count+4];
			 ERROR_Read_sendTX_count++;
			 i++;
			 if(i==8)break;
		 }
		 if(ERROR_Read_sendTX_count==UART_DATA_ID98[1]-4)
		 {
		 	Last_Uart_Struct_DATA_Packet_Contro.data[0].uc[1]= Last_Uart_Struct_DATA_Packet_Contro.data[0].uc[1] |0x80;
			FLAG_APP_TX_fromUART_err_read=0;
			Flag_ERROR_Read=0;
		 }

		 DEF_APP_TX_freq=3;
		 FLAG_APP_TX=1;
		 FLAG_APP_RX=0;
		 //FLAG_APP_RXstart=0;
		 APP_TX_freq=0;
		 TX_Scan_step=1;
		 First_TX_Scan=0;

	  }
      else if((Flag_FREQ_Scan==0)&&((PROFILE_CH_FREQ_32bit_200002EC == PROFILE_CH1_FREQ_32bit_429HighSpeed)||(PROFILE_CH_FREQ_32bit_200002EC == PROFILE_CH2_FREQ_32bit_429HighSpeed))&&
	  	  (((FLAG_APP_TX_fromOUT==1)&&(TIME_APP_TX_fromOUT==0))||(FLAG_Key_TP3==1)||
	  	   //((FLAG_APP_TX_fromUART==1)&&(((TIME_APP_TX_fromOUT==0)&&(Radio_Date_Type_bak==2))||((TIMER300ms==0)&&(Radio_Date_Type_bak==1)))&&(Uart_Struct_DATA_Packet_Contro.data[0].ui!=Last_Uart_Struct_DATA_Packet_Contro.data[0].ui))
	  	   ((FLAG_APP_TX_fromUART==1)&&(((TIME_APP_TX_fromOUT==0)&&(Radio_Date_Type_bak==2))||((TIMER300ms==0)&&(Radio_Date_Type_bak==1))))
		  )
	  	)
	  {
					  FLAG_APP_TX_fromUART=0;
					  FLAG_Key_TP3=0;
					  Last_Uart_Struct_DATA_Packet_Contro=Uart_Struct_DATA_Packet_Contro;
					  Last_Uart_Struct_DATA_Packet_Contro.Fno_Type.UN.type=1;
					  rssi = (RAM_RSSI_AVG - 255)/3 - 51;
                      rssi = -rssi;
					  if(Radio_Date_Type_bak==2)    rssi= rssi | 0x80;
					  Last_Uart_Struct_DATA_Packet_Contro.data[1].uc[0] = rssi;
					  if(FLAG_APP_TX_fromOUT==1) Last_Uart_Struct_DATA_Packet_Contro.Fno_Type.UN.fno= Struct_DATA_Packet_Contro_fno;
					  FLAG_APP_TX_fromOUT=0;
					  TIME_APP_TX_fromOUT=0;

					  DEF_APP_TX_freq=3;
					  FLAG_APP_TX=1;
					  FLAG_APP_RX=0;
					  APP_TX_freq=0;
					  TX_Scan_step=1;
					  First_TX_Scan=0;

                      RSSI_Read_Counter = 0;
                      RAM_RSSI_SUM = 0;
	  }
    if(FLAG_APP_TX == 1)
    {
        if(TX_Scan_step==1) TX_Scan_step=2;
		   if(TX_Scan_step==2)
		   {
				if(APP_TX_freq==0)// && Flag_tx_en == 0)
				{
                    Flag_tx_en = 1;
                    TIMER18ms = 300;
				    Receiver_LED_TX = 1;
					TX_DataLoad_HighSpeed(ID_SCX1801_DATA,Last_Uart_Struct_DATA_Packet_Contro, &CONST_TXPACKET_DATA_20000AF0[0]);
                    ML7345_SetAndGet_State(Force_TRX_OFF);
                    ML7345_Write_Reg(0x00,0x22);    // Bank1 Set
                    ML7345_Write_Reg(0x2a,0x15);    //sync
                    ML7345_Write_Reg(ADDR_BANK_SEL,BANK0_SEL);
                    ML7345_GPIO2TxDoneInt_Enable();
                    ML7345_AutoTx_Data(CONST_TXPACKET_DATA_20000AF0,28);
                    Time_Tx_Out = 100;
					APP_TX_freq=1; //1
				}
				else if((APP_TX_freq < DEF_APP_TX_freq) && (Flag_TxDone == 1) && (Time_APP_blank_TX == 0))
				{
                    Flag_TxDone = 0;
                    ML7345_AutoTx_Data(CONST_TXPACKET_DATA_20000AF0,28);
                    Time_Tx_Out = 100;
					APP_TX_freq++;
				}
				else if((APP_TX_freq == DEF_APP_TX_freq) && (Flag_TxDone == 1) && (Time_APP_blank_TX == 0))
				{
                    Flag_TxDone = 0;
					APP_TX_freq++;
				   FLAG_APP_RXstart=1;
				   FLAG_APP_TX=0;
				   Time_APP_RXstart=1;
				   Receiver_LED_TX = 0;
				   FLAG_APP_TX_once=0;
                   Flag_tx_en = 0;
				}
		   }
    }
    if((FLAG_APP_RXstart==1)&&(Time_APP_RXstart==0)&&(FLAG_APP_TX_fromUART_err_read==0))
    {
        FLAG_APP_RXstart = 0;
        FLAG_APP_RX = 1;
    }
    else if(Flag_tx_en == 1 && Time_Tx_Out == 0 && Flag_TxDone == 0 && FLAG_APP_RXstart == 0)
    {
        if(PROFILE_CH_FREQ_32bit_200002EC == 429350000) RF_ML7345_Init(Fre_429_350,0x15,28);
        else if(PROFILE_CH_FREQ_32bit_200002EC == 429550000) RF_ML7345_Init(Fre_429_550,0x15,28);
        ML7345_GPIO2TxDoneInt_Enable();
        ML7345_AutoTx_Data(CONST_TXPACKET_DATA_20000AF0,28);
        Time_Tx_Out = 100;
    }
}

//--------------------------------------------------------------------------------------------------
void ML7345D_RF_test_mode(void)
{
    u8 Flag_TP4 = 0;
    u8 test_time_Base10ms = 0;
    Flag_test_mode = 0;
    Flag_test_pc = 0;
    while (Receiver_test == 0)
    {
        if(Flag_test_mode == 0)
        {
            UART1_INIT_TestMode();
            Receiver_LED_OUT = 0;
            BEEP_Module(500,1);
        }
        Flag_test_mode = 1;
        ClearWDT();   // Service the WDT
        if(Flag_test_pc == 0)
        {
            if((TP4 == 0)&&(Flag_TP4==0))   //不使用TP3，因为测试模式TP3与工作模式换气输出有冲突，冲突为三极管导致TP3的高电平只有0.8V
            {
                if (FG_10ms==1)
                {
                    FG_10ms = 0;
                    test_time_Base10ms++;
                    if (test_time_Base10ms>5)
                    {
                        test_time_Base10ms = 5;
                        Flag_TP4 = 1;
                        Tx_Rx_mode++;
                        if (Tx_Rx_mode == 2)
                            Tx_Rx_mode = 3; //屏蔽mode 2，mode 2暂时不使用
                        if (Tx_Rx_mode == 3)
                            X_COUNT = 0;   //切换成mode3时，RX误码率有时会亮
                        if (Tx_Rx_mode > 3)
                            Tx_Rx_mode = 0;
                    }
                }
            }
            else if(TP4 == 1)
            {
                Flag_TP4 = 0;
                test_time_Base10ms = 0;
            }
            if ((Tx_Rx_mode == 0) || (Tx_Rx_mode == 1))
            {
                CG2214M6_USE_T;
                FG_test_rx = 0;
                Receiver_LED_RX = 0;
                FG_test_tx_off = 0;
                Flag_test_rssi = 0;
                PROFILE_CH_FREQ_32bit_200002EC = 429175000;
                if (Tx_Rx_mode == 0) //发载波，无调制信叿
                {
                    Receiver_LED_OUT = 1;
                    FG_test_mode = 0;
                    FG_test_tx_1010 = 0;
                    if (FG_test_tx_on == 0)
                    {
                        FG_test_tx_on = 1;
                        ML7345_SetAndGet_State(Force_TRX_OFF);
                        ML7345_Frequency_Set(Fre_429_175,1);
                        Tx_Data_Test(0);
                    }
                }
                else //发载波，有调制信叿
                {
                    if (TIMER1s == 0)
                    {
                        TIMER1s = 500;
                        Receiver_LED_OUT = !Receiver_LED_OUT;
                    }
                    FG_test_mode = 1;
                    FG_test_tx_on = 0;
                    if (FG_test_tx_1010 == 0)
                    {
                        FG_test_tx_1010 = 1;
                        ML7345_SetAndGet_State(Force_TRX_OFF);
                        ML7345_Frequency_Set(Fre_429_175,1);
                        Tx_Data_Test(1);
                    }
                }
            }
            //else  {           //test  RX
            if ((Tx_Rx_mode == 2) || (Tx_Rx_mode == 3))
            {
                CG2214M6_USE_R;
                FG_test_rx = 1;
                Receiver_LED_OUT = 0;
                FG_test_mode = 0;
                FG_test_tx_on = 0;
                FG_test_tx_1010 = 0;
                if (FG_test_tx_off == 0)
                {
                    FG_test_tx_off = 1;
                    ML7345_SetAndGet_State(Force_TRX_OFF);
                    PROFILE_CH_FREQ_32bit_200002EC = 426750000;
                    ML7345_Frequency_Set(Fre_426_750,1);
                    ML7345_MeasurBER_Init();
                    ML7345_SetAndGet_State(RX_ON);
                }
                if (Tx_Rx_mode == 2) //packet usart out put RSSI
                {
                    if (TIMER1s == 0)
                    {
                        TIMER1s = 500;
                        Receiver_LED_RX = !Receiver_LED_RX;
                    }
                    //SCAN_RECEIVE_PACKET(); //扫描接收数据
                }
                if (Tx_Rx_mode == 3) //packet usart out put BER
                {
                    RF_Ber_Test();
                }
            }
        }
        PC_PRG();
    }
    if(Flag_test_mode == 1)
    {
        UART1_INIT();
        ML7345_SetAndGet_State(Force_TRX_OFF);
        RF_ML7345_Init(Fre_426_075,0x55,12);
    }
    PROFILE_CH_FREQ_32bit_200002EC = 426075000;
    Ber_Exit_UnInit();
    OUT_VENT_Init();
    Flag_test_mode = 0;
    Flag_test_pc = 0;
    ML7345_GPIO2RxDoneInt_Enable(); /* 开启接收完成中断,ML7345D GPIO2中断输出 */
    ML7345_GPIO2TxDoneInt_Enable(); /* 开启发送完成中断,ML7345D GPIO2中断输出 */
    FG_test_rx = 0;
    TIMER1s = 0;
    Receiver_LED_TX = 0;
    Receiver_LED_RX = 0;
    FG_Receiver_LED_RX = 0;
    //Receiver_LED_OUT = 0;
    Time_rf_init = 100;
    FLAG_APP_RX = 1;
    TIME_Fine_Calibration = 900;
    TIME_EMC = 10;
}

void ML7345d_Change_Channel(void)
{
    if ((FLAG_ID_Erase_Login == 1) || (FLAG_ID_Login == 1) ||(FLAG_ID_SCX1801_Login==1) || (Receiver_429MHz_mode==0))
    {
        PROFILE_CH_FREQ_32bit_200002EC = 426075000;
        Radio_Date_Type=1;
        Channels=1;
        ML7345_Frequency_Set(Fre_426_075,Radio_Date_Type);
    }
    else
    {
        switch(Channels)
        {
            case 1:
                    Radio_Date_Type = 1;
                    PROFILE_CH_FREQ_32bit_200002EC = 426075000;
                    ML7345_Frequency_Set(Fre_426_075,Radio_Date_Type);       //加上VCO校准后用时5ms，不加1.2ms
                    if(ID_SCX1801_DATA == 0) Channels = 1;
                    else Channels = 2;
                    break;

            case 2:
                    Radio_Date_Type = 2;
                    PROFILE_CH_FREQ_32bit_200002EC = PROFILE_CH1_FREQ_32bit_429HighSpeed;
                    ML7345_Frequency_Set(Fre_429_350,Radio_Date_Type);
                    Channels = 3;
                    break;

            case 3:
                    Radio_Date_Type = 2;
                    PROFILE_CH_FREQ_32bit_200002EC = PROFILE_CH2_FREQ_32bit_429HighSpeed;
                    ML7345_Frequency_Set(Fre_429_550,Radio_Date_Type);
                    Channels = 4;
                    break;

             case 4:
                    Radio_Date_Type = 1;
                    PROFILE_CH_FREQ_32bit_200002EC = 426075000;
                    ML7345_Frequency_Set(Fre_426_075,Radio_Date_Type);
                    Channels = 1;
                    break;

             default:
                break;
        }
    }
}

void ML7345D_Freq_Scanning(void)
{
    if(TIMER18ms == 0)
    {
        if(Flag_FREQ_Scan)  return;
        ML7345d_Change_Channel();
        if(Time_rf_init == 0)
        {
            Time_rf_init = 100;
            if(PROFILE_CH_FREQ_32bit_200002EC == 426075000) RF_ML7345_Init(Fre_426_075,0x55,12);
            else if(PROFILE_CH_FREQ_32bit_200002EC == 429350000) RF_ML7345_Init(Fre_429_350,0x55,28);
            else if(PROFILE_CH_FREQ_32bit_200002EC == 429550000) RF_ML7345_Init(Fre_429_550,0x55,28);
            ML7345_GPIO2RxDoneInt_Enable();
        }
        ML7345_SetAndGet_State(RX_ON);
        CG2214M6_USE_R;

        if(Radio_Date_Type==1)
            TIMER18ms = 18;
        else if(Radio_Date_Type > 1)
            TIMER18ms = 18;
        Flag_rx_pream = 0;
        Flag_tx_en = 0;
        RSSI_Read_Counter = 0;
        RAM_RSSI_SUM = 0;

    }else Flag_FREQ_Scan = 0;
}

void ML7345_TRX_Del(void)
{
    u8 reg = 0;
    if(Flag_tx_en == 0) reg = RF_SyncWord_DONE();
    if(reg & 0x20)
    {
        TIMER18ms = 550;
        Flag_FREQ_Scan = 1;
        if(Flag_rx_pream == 0 && Flag_set_freq == 0)
        {
            FG_Receiver_LED_RX = 1;
            ML7345_SetAndGet_State(Force_TRX_OFF);
            if(PROFILE_CH_FREQ_32bit_200002EC == 426075000)
            {
                TIMER300ms = 600;
                if(Flag_TX_ID_load == 0)    RF_ML7345_Init(Fre_426_075,0x15,12);
                else                        RF_ML7345_Init(Fre_426_075,0x15,24);
            }
            else if(PROFILE_CH_FREQ_32bit_200002EC == 429350000)   {RF_ML7345_Init(Fre_429_350,0x15,28); TIMER300ms = 100;}
            else if(PROFILE_CH_FREQ_32bit_200002EC == 429550000)   {RF_ML7345_Init(Fre_429_550,0x15,28); TIMER300ms = 100;}
            ML7345_GPIO2RxDoneInt_Enable();
            ML7345_SetAndGet_State(RX_ON);
            CG2214M6_USE_R;
        }
        Flag_rx_pream = 1;
        ML7345_Write_Reg(ADDR_INT_SOURCE_GRP2,0x00); //清接收完成标志
    }
    if(reg & 0x01)
    {
        if(TIMER18ms < 50) TIMER18ms = 50;
        Flag_RxDone = 1;
        ML7345_Write_Reg(ADDR_INT_SOURCE_GRP2,0x00); //清接收完成标志
        //ML7345_StateFlag_Clear(RX_DONE_FLAG); //清除后接收的数据会被清0
    }
    if(RF_TX_DONE())
    {
        Flag_TxDone = 1;
        Time_APP_blank_TX = 6;
        ML7345_StateFlag_Clear(TX_DONE_FLAG);
    }
    EXTI_SR1_P5F = 1;
}

void RX_ANALYSIS(void)
{
    u8 i;
    for (i = 0; i < 7; i++)
    {
        SPI_Receive_DataForC[i] = (u32)SPI_RECEIVE_BUFF[i * 4 + 3] |
                                  (u32)SPI_RECEIVE_BUFF[i * 4 + 2] << 8 |
                                  (u32)SPI_RECEIVE_BUFF[i * 4 + 1] << 16 |
                                  (u32)SPI_RECEIVE_BUFF[i * 4 + 0] << 24;
    }
	if(Flag_ERROR_Read==0)
	{
       FLAG_Receiver_IDCheck = 1;
       ID_Decode_IDCheck();
	}
}

void SCAN_RECEIVE_PACKET(void)
{
    short Cache;
    if(Flag_RxDone)
    {
        Flag_RxDone = 0;
        if(PROFILE_CH_FREQ_32bit_200002EC == 426075000)
        {
            if(Flag_TX_ID_load == 1)    ML7345_ReadRx_Pack(SPI_RECEIVE_BUFF,24);
            else                        ML7345_ReadRx_Pack(SPI_RECEIVE_BUFF,12);
        }
        else    ML7345_ReadRx_Pack(SPI_RECEIVE_BUFF,28);
        ML7345_StateFlag_Clear(RX_DONE_FLAG);
        RX_ANALYSIS();
        Flag_FREQ_Scan = 0;

        ML7345_SetAndGet_State(Force_TRX_OFF);
        Cache = ML7345_Read_Reg(ADDR_ED_RSLT);
        RAM_RSSI_SUM += Cache;
        RSSI_Read_Counter++;
        RAM_RSSI_AVG = RAM_RSSI_SUM / RSSI_Read_Counter;
        ML7345_Write_Reg(0x00,0x22);    // Bank1 Set
        ML7345_Write_Reg(0x2a,0x55);    //sync
        ML7345_Write_Reg(0x00,0x11);    // Bank0 Set
        ML7345_SetAndGet_State(RX_ON);
        CG2214M6_USE_R;
        Flag_rx_pream = 0;
        TIMER18ms = 28;
    }
}

void ML7345D_Error_Detect(void)
{
    if(ML7345_Read_Reg(ADDR_INT_SOURCE_GRP1) & 0x04)    //PLL unlock interrupt
    {
        ML7345_Write_Reg(ADDR_INT_SOURCE_GRP1,0xFB);
        while(ML7345_SetAndGet_State(Get_Sta) != TRX_OFF);
        ML7345_AutoTx_Data(CONST_TXPACKET_DATA_20000AF0,28);
    }
    else if(ML7345_Read_Reg(ADDR_INT_SOURCE_GRP3) & 0x10)    //TX FIFO access error interrupt
    {
        ML7345_SetAndGet_State(Force_TRX_OFF);
        ML7345_StateFlag_Clear(TX_DONE_FLAG);
        ML7345_AutoTx_Data(CONST_TXPACKET_DATA_20000AF0,28);
    }
}


void TX_DataLoad(u32 IDCache, u8 CtrCmd, u8 *Packet)
{
    u8 i;
    u16 CRCTemp = 0;
    CRCTemp = (IDCache & 0xffff) + (((IDCache >> 16) & 0xff) + ((u16)CtrCmd << 8));
    for (i = 0; i < 24; i++)
    {
        *(Packet + (i / 4)) <<= 2;
        *(Packet + (i / 4)) |= ((IDCache & ((u32)1 << i)) ? 2 : 1);
    }
    for (i = 24; i < 32; i++)
    {
        *(Packet + (i / 4)) <<= 2;
        *(Packet + (i / 4)) |= ((CtrCmd & ((u8)1 << (i - 24))) ? 2 : 1);
    }
    for (i = 32; i < 48; i++)
    {
        *(Packet + (i / 4)) <<= 2;
        *(Packet + (i / 4)) |= ((CRCTemp & ((u16)1 << (i - 32))) ? 2 : 1);
    }
    ClearWDT();
}

void TX_DataLoad_HighSpeed(u32 IDCache, Wireless_Body CtrCmd, u8 *Packet)
{
    u8 i;
    u16 CRCTemp = 0;

    CRCTemp = (IDCache & 0xffff) + (((IDCache >> 16) & 0xff) + ((u16)CtrCmd.Fno_Type.byte << 8));
	for(i=0;i<4;i++)
		CRCTemp+=CtrCmd.data[i].ui;

    for (i = 0; i < 24; i++)
    {
        *(Packet + (i / 4)) <<= 2;
        *(Packet + (i / 4)) |= ((IDCache & ((u32)1 << i)) ? 2 : 1);
    }
    for (i = 24; i < 32; i++)
    {
        *(Packet + (i / 4)) <<= 2;
        *(Packet + (i / 4)) |= ((CtrCmd.Fno_Type.byte & ((u8)1 << (i - 24))) ? 2 : 1);
    }
	ClearWDT();
    for (i = 32; i < 48; i++)
    {
        *(Packet + (i / 4)) <<= 2;
        *(Packet + (i / 4)) |= ((CtrCmd.data[0].ui & ((u16)1 << (i - 32))) ? 2 : 1);
    }
    for (i = 48; i < 64; i++)
    {
        *(Packet + (i / 4)) <<= 2;
        *(Packet + (i / 4)) |= ((CtrCmd.data[1].ui & ((u16)1 << (i - 48))) ? 2 : 1);
    }
	ClearWDT();
    for (i = 64; i < 80; i++)
    {
        *(Packet + (i / 4)) <<= 2;
        *(Packet + (i / 4)) |= ((CtrCmd.data[2].ui & ((u16)1 << (i - 64))) ? 2 : 1);
    }
    for (i = 80; i < 96; i++)
    {
        *(Packet + (i / 4)) <<= 2;
        *(Packet + (i / 4)) |= ((CtrCmd.data[3].ui & ((u16)1 << (i - 80))) ? 2 : 1);
    }
    ClearWDT();
    for (i = 96; i < 112; i++)
    {
        *(Packet + (i / 4)) <<= 2;
        *(Packet + (i / 4)) |= ((CRCTemp & ((u16)1 << (i - 96))) ? 2 : 1);
    }
}
