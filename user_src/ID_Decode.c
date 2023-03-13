/***********************************************************************/
/*  FILE        :ID_Decode.c                                           */
/*  DATE        :Mar, 2014                                             */
/*  Programmer	:xiang 'R                                              */
/*  CPU TYPE    :STM8L151G6     Crystal: 16M HSI                       */
/*  DESCRIPTION :                                                      */
/*  Mark        :ver 1.0                                               */
/***********************************************************************/
#include <iostm8l151g4.h> // CPUÂûãÂè∑
#include "Pin_define.h"   // ÁÆ°ËÑöÂÆö‰πâ
#include "initial.h"      // ÂàùÂßãÂå? È¢ÑÂÆö‰π?
#include "ram.h"          // RAMÂÆö‰πâ
#include "ID_Decode.h"
#include "eeprom.h" // eeprom
#include "uart.h"   // uart
#include "ML7345.h"

UINT16 TIME_BEEP_on;
UINT16 TIME_BEEP_off;
UINT16 BASE_TIME_BEEP_on;
UINT16 BASE_TIME_BEEP_off;
UINT16 TIME_BEEP_freq;
UINT16 BASE_TIME_BEEP_freq;
UINT8 FG_beep_on_Motor;
UINT8 FG_beep_off_Motor;
UINT16 TIME_BEEP_on2;
UINT16 TIME_BEEP_off2;
UINT16 BASE_TIME_BEEP_on2;
UINT16 BASE_TIME_BEEP_off2;
UINT16 TIME_BEEP_freq2;
UINT8 FG_beep_on_Motor2;
UINT8 FG_beep_off_Motor2;

void BEEP_function(void);

//void EXIT_init(void)
//{
//    EXTI_CR1 = 0x20;          //PORT B2  ÁöÑ‰∏≠Êñ≠Ëß¶Âèë‰Ωç
//    ADF7021_DATA_CLK_CR2 = 1; //‰ΩøËÉΩËØ•I/OÂè£‰∏≠Êñ? PA1
//    //   EXTI_CR2=0X00;
//    //   PIN_PD7_CR2=1;
//}


void ID_Decode_IDCheck(void)
{

    if (FLAG_Receiver_IDCheck)
    {
        FLAG_Receiver_IDCheck = 0;
        if(Radio_Date_Type==1) Signal_DATA_Decode(0);
		else if(Radio_Date_Type==2) Signal_DATA_Decode(2);
        if (FLAG_Signal_DATA_OK == 1)
        {
            Radio_Date_Type_bak=Radio_Date_Type;
            eeprom_IDcheck();
            if ((FLAG_ID_Erase_Login == 1) || (FLAG_ID_Login == 1) ||(FLAG_ID_SCX1801_Login==1))
            {
                if ((FLAG_ID_Login_OK == 0) && (DATA_Packet_Contro_buf != 0x40) && (DATA_Packet_ID != 0)&&(Radio_Date_Type_bak==1)) //2015.4.1‰øÆÊ≠£ Âú®ÁôªÂΩïÊ®°Âºè‰∏ã ‰∏çÂÖÅËÆ∏Ëá™Âä®È?Å‰ø°ÁôªÂΩïÔºåÂè™ÂÖÅËÆ∏ÊâãÂä®ÈÄÅ‰ø°ÁôªÂΩï
                {
                    if ((FLAG_ID_Erase_Login == 1) || (((FLAG_ID_Login == 1) ||(FLAG_ID_SCX1801_Login==1))&&(DATA_Packet_ID != 0xFFFFFE)))
                    	{
	                    FLAG_ID_Login_OK = 1;
	                    ID_Receiver_Login = DATA_Packet_ID;
                    	}
                }
            }
            else if ((FLAG_IDCheck_OK == 1) || (DATA_Packet_ID == 0xFFFFFE))
            {
                FLAG_IDCheck_OK = 0;
			    if(Radio_Date_Type_bak==1)
			    {
		                if (DATA_Packet_ID == 0xFFFFFE)
		                    DATA_Packet_Control = DATA_Packet_Contro_buf; //2015.3.24‰øÆÊ≠£ ControlÁºìÂ≠òËµ?IDÂà§Êñ≠ÊòØÂê¶Â≠¶‰π†ËøáÂêéÊâçËÉΩ‰ΩøÁî®

		                if ((SPI_Receive_DataForC[1] & 0x0000FFFF) == 0x5556)
		                {
							Flag_TX_ID_load=1;
		                    Signal_DATA_Decode(1);
		                    if (FLAG_Signal_DATA_OK == 1)
		                    {
		                        eeprom_IDcheck();
		                        if (DATA_Packet_Contro_buf == 0xFF)
		                        { //2015.4.2‰øÆÊ≠£  ÈÄÅ‰ø°Âô®ÊóÅËæπÁöÑÁôªÂΩïÈî?ËøΩÂä†ÁôªÂΩï‰∏çË°å
		                            if (FLAG_IDCheck_OK == 1)
		                                FLAG_IDCheck_OK = 0;
                                    else if (ID_DATA_PCS < ID_Max_PCS)
                                    {
		                                BEEP_and_LED();
		                                ID_Receiver_Login = DATA_Packet_ID;
		                                ID_EEPROM_write();
		                            } //end else
		                        }
		                        else if (DATA_Packet_Control == 0x00)
		                        {
		                            if (FLAG_IDCheck_OK == 1)
		                            {
		                                FLAG_IDCheck_OK = 0;
		                                BEEP_and_LED();
										if(ID_SCX1801_DATA==DATA_Packet_ID)
                                        {
                                            ID_SCX1801_DATA = 0;
                                            ID_SCX1801_EEPROM_write(0x00);
                                            //if (ID_Receiver_DATA[0] != 0)
                                            if (ID_Receiver_DATA_READ(ID_Receiver_DATA[0]) != 0)
                                            {
                                                //ID_SCX1801_DATA = ID_Receiver_DATA[0];
                                                ID_SCX1801_DATA = ID_Receiver_DATA_READ(ID_Receiver_DATA[0]);
                                                ID_SCX1801_EEPROM_write(ID_SCX1801_DATA);
                                                Delete_GeneralID_EEPROM(ID_SCX1801_DATA);
                                            }
                                        }
                                        else
                                            Delete_GeneralID_EEPROM(DATA_Packet_ID);
                                    }
		                        }
		                    }
		                }
		                else
		                {
							Flag_TX_ID_load=0;
                            static u32 DATA_Packet_ID_auto = 0;
                            if ((DATA_Packet_Control == 0x40) && (Manual_override_TIMER == 0))
		                    {
                                if ((TIME_auto_useful > 0) && (TIME_auto_useful < 27000)) //27000*10ms=5min*60*90
                                {
                                    if (FREQ_auto_useful_count==0)
                                    {
                                        if ((DATA_Packet_ID_auto != DATA_Packet_ID) || (FREQ_auto_useful_continuous == 0))
                                            FREQ_auto_useful = 0;
                                        DATA_Packet_ID_auto = DATA_Packet_ID;
                                        FREQ_auto_useful++;
                                        FREQ_auto_useful_count = 90;//90=1s
                                        FREQ_auto_useful_continuous = 477; //477=5.3*90=5.3s
                                    }
                                }
                                else {
                                    FREQ_auto_useful=0;
                                    FREQ_auto_useful_count=0;
                                }
                                if ((TIME_auto_useful == 0) || (TIME_auto_useful >= 27000) || ((TIME_auto_useful < 27000) && (FREQ_auto_useful >= 2)))
                                {
                                    TIME_auto_useful = 0;
                                    FREQ_auto_useful = 0;
                                    FG_auto_manual_mode = 1;
                                    if(TIME_auto_out==0)
                                        TIME_auto_out = 7300; //time*90; time=80s
                                    else if(TIME_auto_out<2700)
                                        TIME_auto_out = 2700; //time*90; time=30s
                                    if (FG_First_auto == 0)
                                    {
                                        FG_First_auto = 1;
                                        TIMER1s = 3000; //2500
                                    }
                                }
		                    }
		                    else if (DATA_Packet_Control == 0x40)
		                        ;
		                    else
		                    {
		                        FG_auto_out = 0;
                                TIME_auto_useful = 0;
                                TIME_auto_close = 0;
		                        FG_auto_open_time = 0;
		                        if (FG_auto_manual_mode == 1)      //Manual_override_TIMER=13500;   //2Âà?0ÁßíËá™Âä®Êó†Êï?
		                            Manual_override_TIMER = 25850;//27200;//5 Minutes„ÄÇ24480; //4Âà?0ÁßíËá™Âä®Êó†Êï?
		                        if ((DATA_Packet_Control & 0x14) == 0x14)
		                        {
		                            if (TIMER1s == 0)
		                                TIMER1s = 3800 - 30;
		                        }
		                        else if(FLAG_testNo91==1) TIMER1s = 600;
		                        else TIMER1s = 1000;
		                    }
		                    TIMER300ms = 600;  //500
		                    //Receiver_LED_RX=1;
		                    FG_Receiver_LED_RX = 1;
		                }
			    }
				else if(Radio_Date_Type_bak==2)
				{
				   DATA_Packet_Control=0;
				   Struct_DATA_Packet_Contro_fno=Struct_DATA_Packet_Contro.Fno_Type.UN.fno;
				   if(Struct_DATA_Packet_Contro.Fno_Type.UN.type==1) DATA_Packet_Control=Struct_DATA_Packet_Contro.data[0].uc[0];
					if(((DATA_Packet_Control&0xA0)==0x20)||((DATA_Packet_Control&0xC0)==0x40))TIMER1s=500;
					else if(((DATA_Packet_Control&0xDF)>0x80)&&((DATA_Packet_Control&0x20)==0x00)){
						TIMER1s=1000;//(TIMER_Semi_open+1)*1000;
						if((DATA_Packet_Control&0xDF)<0xC0)TIMER_Semi_open=(DATA_Packet_Control&0x1F)+4;
						else TIMER_Semi_close=(DATA_Packet_Control&0x1F)+4;
					}
					else TIMER1s=1000;
					FLAG_APP_TX_once=1;
                    TIMER300ms = 100;
		            FG_Receiver_LED_RX = 1;
				}
            }


        }
    }
}

void Signal_DATA_Decode(UINT8 NUM_Type)
{
    UINT32 data_in;
    UINT16 data_out;
    UINT16 data_NRZ[7];
    UINT8 i, j,i_value;

	if((NUM_Type==0)||(NUM_Type==1))i_value=3;
	else if(NUM_Type==2)i_value=7;
    for (i = 0; i < i_value; i++)
    {
        if ((NUM_Type == 0)||(NUM_Type == 2))
            data_in = SPI_Receive_DataForC[i];
        else
            data_in = SPI_Receive_DataForC[i + 3];
        data_out = 0;
        data_in = data_in >> 1;
        for (j = 0; j < 16; j++)
        {
            data_out = data_out << 1;
            if (data_in & 0x00000001)
                data_out += 1;
            data_in = data_in >> 2;
        }
        data_NRZ[i] = data_out;
    }
	if((NUM_Type==0)||(NUM_Type==1))
	{
	    if (data_NRZ[2] == ((data_NRZ[0] + data_NRZ[1]) & 0xFFFF))
	    {
	        FLAG_Signal_DATA_OK = 1;
	        LCDUpdateIDFlag = 1;
	        DATA_Packet_ID = (data_NRZ[1] & 0x00FF) * 65536 + data_NRZ[0];
	        if (DATA_Packet_ID == 0)
	            FLAG_Signal_DATA_OK = 0;                          //2014.3.21ËøΩÂä†  ‰∏çÂÖÅËÆ∏‰ΩøÁî®ID=0
	        DATA_Packet_Contro_buf = (data_NRZ[1] & 0xFF00) >> 8; //2015.3.24‰øÆÊ≠£ ControlÁºìÂ≠òËµ?IDÂà§Êñ≠ÊòØÂê¶Â≠¶‰π†ËøáÂêéÊâçËÉΩ‰Ω?
	    }
	    else
	        FLAG_Signal_DATA_OK = 0;
	}
	else if(NUM_Type==2)
	{
	    if (data_NRZ[6] == ((data_NRZ[0] + data_NRZ[1] + data_NRZ[2] + data_NRZ[3] + data_NRZ[4] + data_NRZ[5]) & 0xFFFF))
	    {
	        FLAG_Signal_DATA_OK = 1;
	        LCDUpdateIDFlag = 1;
	        DATA_Packet_ID = (data_NRZ[1] & 0x00FF) * 65536 + data_NRZ[0];
	        if (DATA_Packet_ID == 0)
	            FLAG_Signal_DATA_OK = 0;                          //2014.3.21ËøΩÂä†  ‰∏çÂÖÅËÆ∏‰ΩøÁî®ID=0
	        Struct_DATA_Packet_Contro_buf.Fno_Type.byte = (data_NRZ[1] & 0xFF00) >> 8; //2015.3.24‰øÆÊ≠£ ControlÁºìÂ≠òËµ?IDÂà§Êñ≠ÊòØÂê¶Â≠¶‰π†ËøáÂêéÊâçËÉΩ‰Ω?
            for (i = 0; i < 4; i++)
				Struct_DATA_Packet_Contro_buf.data[i].ui=data_NRZ[i+2];
	    }
	    else
	        FLAG_Signal_DATA_OK = 0;
	}
}

void eeprom_IDcheck(void)
{
    UINT16 i;
	if(FLAG_testNo91==1)
		{
            FLAG_IDCheck_OK = 1;
            DATA_Packet_Control = DATA_Packet_Contro_buf;
		}
#ifndef DEF_test_MAX_32pcs
		if(Radio_Date_Type_bak==1)
		{
				i = 0;
                do
				{
                    //if (ID_Receiver_DATA[i] == DATA_Packet_ID)
                    if (ID_Receiver_DATA_READ(ID_Receiver_DATA[i]) == DATA_Packet_ID)
                    {
						INquiry = i;
						i = ID_DATA_PCS;
						FLAG_IDCheck_OK = 1;
						if(Radio_Date_Type_bak==1)DATA_Packet_Control = DATA_Packet_Contro_buf;
						else if(Radio_Date_Type_bak==2)Struct_DATA_Packet_Contro=Struct_DATA_Packet_Contro_buf;
					} //2015.3.24Å0ä1Å0Ü7Å0Ö3Å0ä3Å0Ö2Å0Ñ5 ControlÅ0ä4Å0Ü4°∞Å0ä2Å0Ö2Å0π0®®Å0Ö8?IDÅ0ä2Å0∑3°ËÅ0ä3®CÅ0Ö2Å0ä3Å0π0Å0Ö4Å0ä2Å0Ç6Å0Ñ7Å0ä2Å0Ö2Å0Ñ7Å0ä1Å0Ü1Å0Ñ2®®Å0Ü7Å6¶0Å0ä2Å0Ç6Å0ó5Å0ä3°ÎÅ0Ç3®®Å0ô6Å0Ü5Å0ä1Å0Ü5Å0Ü7Å0ä4°±°ß
                    else if (DATA_Packet_ID == ID_SCX1801_DATA)
                    {
                        i = ID_DATA_PCS;
                        FLAG_IDCheck_OK = 1;
                        DATA_Packet_Control = DATA_Packet_Contro_buf;
                    }
                    if ((FLAG_ID_Erase_Login == 1) && (FLAG_ID_Erase_Login_PCS == 1))
					{
						i = ID_DATA_PCS;
						FLAG_IDCheck_OK = 0;
						DATA_Packet_Control = DATA_Packet_Contro_buf;
					} //®®Å0Ü7Å0Ü5Å0ä2Å0î7Å0Ñ2Å0ä2°ËÅ0î8Å0ä3Å0Ö1Å0Ñ3IDÅ0ä4Å6æ4Å0Ü3Å0ä2Å0Ü5Å6¶1
                    i++;
                } while (i < ID_DATA_PCS);
        }
		else if((Radio_Date_Type_bak==2)&&(DATA_Packet_ID==ID_SCX1801_DATA))
		{
			FLAG_IDCheck_OK = 1;
			Struct_DATA_Packet_Contro=Struct_DATA_Packet_Contro_buf;
		}
#else
			for (i = 0; i < ID_DATA_PCS; i++)
			{
                //if (ID_Receiver_DATA[i] == DATA_Packet_ID)
                if (ID_Receiver_DATA_READ(ID_Receiver_DATA[i]) == DATA_Packet_ID)
                {
					INquiry = i;
					i = ID_DATA_PCS;
					FLAG_IDCheck_OK = 1;
					if(Radio_Date_Type_bak==1){DATA_Packet_Control = DATA_Packet_Contro_buf;ID_SCX1801_DATA=DATA_Packet_ID;}
					else if(Radio_Date_Type_bak==2){Struct_DATA_Packet_Contro=Struct_DATA_Packet_Contro_buf;ID_SCX1801_DATA=DATA_Packet_ID;}
				} //2015.3.24Å0ä1Å0Ü7Å0Ö3Å0ä3Å0Ö2Å0Ñ5 ControlÅ0ä4Å0Ü4°∞Å0ä2Å0Ö2Å0π0®®Å0Ö8?IDÅ0ä2Å0∑3°ËÅ0ä3®CÅ0Ö2Å0ä3Å0π0Å0Ö4Å0ä2Å0Ç6Å0Ñ7Å0ä2Å0Ö2Å0Ñ7Å0ä1Å0Ü1Å0Ñ2®®Å0Ü7Å6¶0Å0ä2Å0Ç6Å0ó5Å0ä3°ÎÅ0Ç3®®Å0ô6Å0Ü5Å0ä1Å0Ü5Å0Ü7Å0ä4°±°ß
				if ((FLAG_ID_Erase_Login == 1) && (FLAG_ID_Erase_Login_PCS == 1))
				{
					i = ID_DATA_PCS;
					FLAG_IDCheck_OK = 0;
					DATA_Packet_Control = DATA_Packet_Contro_buf;
				} //®®Å0Ü7Å0Ü5Å0ä2Å0î7Å0Ñ2Å0ä2°ËÅ0î8Å0ä3Å0Ö1Å0Ñ3IDÅ0ä4Å6æ4Å0Ü3Å0ä2Å0Ü5Å6¶1
			}

#endif
}

#if (SRX1332A == 0)
void TIM3_init(void)
{ // 2015.3.11????
    TIM3_CCMR1 = TIM3_CCMR1 | 0x70;
    TIM3_CCER1 = TIM3_CCER1 | 0x03;  //TIME3_CH1
    TIM3_ARRH = 0x03;                //0x07D0 -->PWM=2K   0x0880 -->PWM=1.83K   0x05be -->2.73k
    TIM3_ARRL = 0xe8;             //4kHz
                                     //TIM2_IER = 0x01;						// ??????????
    TIM3_CCR1H = 0x01;               //50%
    TIM3_CCR1L = 0xf4;
    TIM3_PSCR = 0x02;                // ?????=Fsystem/(2(PSC[2:0])????4MHz=16MHz/2/2
    //TIM3_EGR_bit.UG=1;
    //TIM2_CR1 = 0x01;					// ?????????????????
    TIM3_CR1 = TIM3_CR1 | 0x01;
    TIM3_BKR = 0x80;
}
#else
void TIM3_init(void)
{ // 2015.3.11????
    TIM3_CCMR1 = TIM3_CCMR1 | 0x70;
    TIM3_CCER1 = TIM3_CCER1 | 0x03;  //TIME3_CH1
    TIM3_ARRH = 0x08;                //0x07D0 -->PWM=2K        0x0880 -->PWM=1.83K
    TIM3_ARRL = 0x84;      //1.85kHz
                                     //TIM2_IER = 0x01;						// ??????????
    TIM3_CCR1H = 0x04;               //50%
    TIM3_CCR1L = 0x42;
    TIM3_PSCR = 0x02;                // ?????=Fsystem/(2(PSC[2:0])????4MHz=16MHz/2/2
    //TIM3_EGR_bit.UG=1;
    //TIM2_CR1 = 0x01;					// ?????????????????
    TIM3_CR1 = TIM3_CR1 | 0x01;
    TIM3_BKR = 0x80;
}
#endif

void Tone_OFF(void)
{                     // ???Tone   2015.3.11????
    TIM3_CR1_CEN = 0; // Timer 3 Disable
    TIM3_CCMR1 =  0x00;
    TIM3_CCER1 =  0x00; //????PWM?????????????????I/O
    PIN_BEEP = 0;
}

void BEEP_function(void)
{
    if(FG_beep_on)return;  //BEEP flag for login mode
    if (TIME_BEEP_on)
    {
        if (TIME_BEEP_on < 0xfff0) //??0xfff0?????
            --TIME_BEEP_on;
        if (FG_beep_on_Motor == 0)
        {
            FG_beep_on_Motor = 1;
            FG_beep_off_Motor = 0;
            //BEEP_CSR2_BEEPEN = 1;
            TIM3_init();
        }
    }
    else if (TIME_BEEP_off)
    {
        --TIME_BEEP_off;
        if (FG_beep_off_Motor == 0)
        {
            FG_beep_off_Motor = 1;
            FG_beep_on_Motor = 0;
            //BEEP_CSR2_BEEPEN = 0;
            Tone_OFF();
        }
    }
    else if (TIME_BEEP_freq)
    {
        if (TIME_BEEP_freq < 0xfff0) //??0xfff0???????
            --TIME_BEEP_freq;
        TIME_BEEP_on = BASE_TIME_BEEP_on;
        TIME_BEEP_off = BASE_TIME_BEEP_off;
        if (FG_beep_on_Motor == 0)
        {
            FG_beep_on_Motor = 1;
            FG_beep_off_Motor = 0;
            //BEEP_CSR2_BEEPEN = 1;
            TIM3_init();
        }
    }

    else if (TIME_BEEP_on2)
    {
        if (TIME_BEEP_on2 < 0xfff0) //??0xfff0?????
            --TIME_BEEP_on2;
        if (FG_beep_on_Motor2 == 0)
        {
            FG_beep_on_Motor2 = 1;
            FG_beep_off_Motor2 = 0;
            //BEEP_CSR2_BEEPEN = 1;
            TIM3_init();
        }
    }
    else if (TIME_BEEP_off2)
    {
        --TIME_BEEP_off2;
        if (FG_beep_off_Motor2 == 0)
        {
            FG_beep_off_Motor2 = 1;
            FG_beep_on_Motor2 = 0;
            //BEEP_CSR2_BEEPEN = 0;
            Tone_OFF();
        }
    }
    else if (TIME_BEEP_freq2)
    {
        if (TIME_BEEP_freq2 < 0xfff0) //??0xfff0???????
            --TIME_BEEP_freq2;
        TIME_BEEP_on = BASE_TIME_BEEP_on;
        TIME_BEEP_off = BASE_TIME_BEEP_off;
        TIME_BEEP_freq = BASE_TIME_BEEP_freq;
        TIME_BEEP_on2 = BASE_TIME_BEEP_on2;
        TIME_BEEP_off2 = BASE_TIME_BEEP_off2;
        if (FG_beep_on_Motor2 == 0)
        {
            FG_beep_on_Motor2 = 1;
            FG_beep_off_Motor2 = 0;
            //BEEP_CSR2_BEEPEN = 1;
            TIM3_init();
        }
    }
}

void _ReqBuzzer(UINT16 d_BEEP_on, UINT16 d_BEEP_off, UINT16 d_BEEP_freq)
{
    if ((d_BEEP_on < 10) && (d_BEEP_on))
        d_BEEP_on = 10;
    if (d_BEEP_on != 0xffff)
        BASE_TIME_BEEP_on = d_BEEP_on / 10;
    if ((d_BEEP_off < 10) && (d_BEEP_off))
        d_BEEP_off = 10;
    BASE_TIME_BEEP_off = d_BEEP_off / 10;
    TIME_BEEP_on = BASE_TIME_BEEP_on;
    TIME_BEEP_off = BASE_TIME_BEEP_off;
    if (d_BEEP_freq)
        BASE_TIME_BEEP_freq = d_BEEP_freq - 1;
    else
        BASE_TIME_BEEP_freq = 0;
    TIME_BEEP_freq = BASE_TIME_BEEP_freq;
}
void _ClearBuzzer(void)
{
    TIME_BEEP_on = 0;
    TIME_BEEP_off = 100; //Stop BEEP for 1000ms
    TIME_BEEP_freq = 0;
}
void _ReqBuzzer_2(UINT16 d_BEEP_on1, UINT16 d_BEEP_off1, UINT16 d_BEEP_freq1, UINT16 d_BEEP_on2, UINT16 d_BEEP_off2, UINT16 d_BEEP_freq2)
{
    if((d_BEEP_on1==0xffff)&&(d_BEEP_off1==0xffff)&&(d_BEEP_freq1==0xffff)&&(d_BEEP_on2==0xffff)&&(d_BEEP_off2==0xffff)&&(d_BEEP_freq2==0xffff))
        FLAG_BEEP_OFF=0;
    else
    {
        if((d_BEEP_on1==0)&&(d_BEEP_off1==0)&&(d_BEEP_freq1==0)&&(d_BEEP_on2==0)&&(d_BEEP_off2==0)&&(d_BEEP_freq2==0))
        {
            FLAG_BEEP_OFF=1;
            d_BEEP_on1=0;
            d_BEEP_off1=1;
            d_BEEP_freq1=0;
            d_BEEP_on2=0;
            d_BEEP_off2=0;
            d_BEEP_freq2=0;
        }

        if ((d_BEEP_on1 < 10)&&(d_BEEP_on1))
            d_BEEP_on1 = 10;
        if (d_BEEP_on1!=0xffff)
            BASE_TIME_BEEP_on = d_BEEP_on1 / 10;
        if ((d_BEEP_off1 < 10) && (d_BEEP_off1))
            d_BEEP_off1 = 10;
        BASE_TIME_BEEP_off = d_BEEP_off1/10;
        TIME_BEEP_on = BASE_TIME_BEEP_on;
        TIME_BEEP_off = BASE_TIME_BEEP_off;
        if (d_BEEP_freq1)
            BASE_TIME_BEEP_freq = d_BEEP_freq1 - 1;
        else
            BASE_TIME_BEEP_freq = 0;
        TIME_BEEP_freq = BASE_TIME_BEEP_freq;

        if ((d_BEEP_on2 < 10) && (d_BEEP_on2))
            d_BEEP_on2 = 10;
        if (d_BEEP_on2 != 0xffff)
            BASE_TIME_BEEP_on2 = d_BEEP_on2 / 10;
        if ((d_BEEP_off2 < 10) && (d_BEEP_off2))
            d_BEEP_off2 = 10;
        BASE_TIME_BEEP_off2 = d_BEEP_off2/10;
        TIME_BEEP_on2 = BASE_TIME_BEEP_on2;
        TIME_BEEP_off2 = BASE_TIME_BEEP_off2;
        if (d_BEEP_freq2)
            TIME_BEEP_freq2 = d_BEEP_freq2 - 1;
        else
            TIME_BEEP_freq2 = 0;
    }
}

/*
   time_beepON„ÄÅtime_beepOFFÂçï‰ΩçÊó∂Èó¥‰∏?0.4333333ms
*/
void BEEP_Module(UINT16 time_beepON, UINT16 time_beepOFF)
{
	UINT16 i;

	for (i = 0; i < time_beepON; i++)
	{
		//Receiver_Buzzer=!Receiver_Buzzer;   //ËúÇÈ∏£Âô®È¢ëÁé?.08KHZ
		if (FG_beep_on == 0)
		{
			FG_beep_on = 1;
			FG_beep_off = 0;
			//BEEP_CSR2_BEEPEN = 1;
            if(FLAG_BEEP_OFF==0)TIM3_init();
        }
		Delayus_With_UartACK(250); //80us
		Delayus_With_UartACK(250); //80us
		Delayus_With_UartACK(250); //80us
		ClearWDT();   // Service the WDT
	}
	for (i = 0; i < time_beepOFF; i++)
	{
		// Receiver_Buzzer=0;	//ËúÇÈ∏£Âô®È¢ëÁé?.08KHZ
		if (FG_beep_off == 0)
		{
			FG_beep_off = 1;
			FG_beep_on = 0;
			//BEEP_CSR2_BEEPEN = 0;
            Tone_OFF();
        }
		//Delayus_With_UartACK(240);
		Delayus_With_UartACK(250); //80us
		Delayus_With_UartACK(250); //80us
		Delayus_With_UartACK(250); //80us
		ClearWDT();   // Service the WDT
	}

}

void BEEP_and_LED(void)
{
    Receiver_LED_OUT = 1;
    Uart_TX_Data();
    BEEP_Module(7000,1);
    _ClearBuzzer();
    FG_beep_on = 0;
    TIME_Receiver_LED_OUT = 60; //185;
}

void Receiver_BEEP(void)
{
    /*
    UINT16 j;
    if (FLAG_Receiver_BEEP == 0)
    {
        FLAG_Receiver_BEEP = 1;
        for (j = 0; j < 3; j++)
          BEEP_Module(1050,1050);
    }
    */
}

void TEST_beep(void)
{
    /*
	if(FLAG_testBEEP==1)
		BEEP_Module(300,1);
	else if(FLAG_testBEEP==2)
		{
		BEEP_Module(300,900);
		BEEP_Module(300,1);
		}
	else if(FLAG_testBEEP==3) BEEP_CSR2_BEEPEN = 1;
	FLAG_testBEEP=0;
    */
}

void ID_Decode_OUT(void)
{
    u8 Control_i;

    Control_i = DATA_Packet_Control &  0xFF;
    if (TIMER1s)
    {
      if((FLAG_testNo91==1)||(FLAG_testNo91==2))
      {
			if((FLAG_testNo91SendUart==0)&&(FLAG_testNo91==1))
				{
				       switch (Control_i)
                       {
				        case 0x08: //open
//				            if(FLAG_testNo91_step==1)
//				            	{
					            Receiver_LED_OUT = 1;
								ACKBack[2]=0xA1;
								Send_Data(ACKBack, 3);
								FLAG_testNo91SendUart=1;
//				            	}
				            break;
				        case 0x04: //stop
//				            if(FLAG_testNo91_step==2)
//				            	{
					            Receiver_LED_OUT = 1;
								ACKBack[2]=0xA2;
								Send_Data(ACKBack, 3);
								FLAG_testNo91SendUart=1;
//				            	}
				            break;
				        case 0x02: //close
//				            if(FLAG_testNo91_step==3)
//				            	{
					            Receiver_LED_OUT = 1;
								ACKBack[2]=0xA4;
								Send_Data(ACKBack, 3);
								FLAG_testNo91SendUart=1;
//				            	}
				            break;
						default:
                            break;
                       }

				}
      }
	  else
	  {
        switch (Control_i)
        {
        case 0x14: //stop+login
            Receiver_LED_OUT = 1;
            TIMER250ms_STOP = 250;
            Receiver_OUT_VENT = FG_NOT_allow_out;
            Receiver_OUT_STOP = FG_allow_out;
            if (TIMER1s < 3550)
            {
                Receiver_OUT_OPEN = FG_allow_out;
                Receiver_OUT_CLOSE = FG_allow_out;
                Receiver_BEEP();
            }
            break;
        case 0x40: //Ëá™Âä®ÈÄÅ‰ø°
            if ((FG_auto_out == 0) && (Manual_override_TIMER == 0)&&(Radio_Date_Type_bak==1))
            {
                Receiver_LED_OUT = 1;
                TIMER250ms_STOP = 0;
                Receiver_OUT_VENT = FG_NOT_allow_out;
                Receiver_OUT_CLOSE = FG_NOT_allow_out;
                if (TIMER1s > 2000)
                {
                    Receiver_OUT_STOP = FG_allow_out;
                    Receiver_OUT_OPEN = FG_NOT_allow_out;
                } //1830
                else if (TIMER1s > 1000)
                {
                    Receiver_OUT_STOP = FG_NOT_allow_out;
                    Receiver_OUT_OPEN = FG_NOT_allow_out;
                } //810
                else
                {
                    FG_auto_open_time = 1;
                    Receiver_OUT_STOP = FG_NOT_allow_out;
                    Receiver_OUT_OPEN = FG_allow_out;
                }
            }
            break;
        case 0x01: //VENT
            Receiver_LED_OUT = 1;
            if (Receiver_vent == 0)
            { //Âèó‰ø°Êú∫Êç¢Ê∞îËÅîÂä®OFF
                Receiver_OUT_STOP = FG_NOT_allow_out;
                Receiver_OUT_VENT = FG_NOT_allow_out;
                Receiver_OUT_OPEN = FG_allow_out;
                Receiver_OUT_CLOSE = FG_allow_out;
            }
            else
            { //Âèó‰ø°Êú∫Êç¢Ê∞îËÅîÂä®ON
                Receiver_OUT_STOP = FG_NOT_allow_out;
                Receiver_OUT_OPEN = FG_NOT_allow_out;
                Receiver_OUT_CLOSE = FG_NOT_allow_out;
                Receiver_OUT_VENT = FG_allow_out;
            }
            break;
        case 0x02: //close
            Receiver_LED_OUT = 1;
            Receiver_OUT_OPEN = FG_NOT_allow_out;
            Receiver_OUT_STOP = FG_NOT_allow_out;
            Receiver_OUT_VENT = FG_NOT_allow_out;
            Receiver_OUT_CLOSE = FG_allow_out;
            break;
        case 0x04: //stop
            Receiver_LED_OUT = 1;
            Receiver_OUT_OPEN = FG_NOT_allow_out;
            Receiver_OUT_CLOSE = FG_NOT_allow_out;
            Receiver_OUT_VENT = FG_NOT_allow_out;
            Receiver_OUT_STOP = FG_allow_out;
            break;
        case 0x08: //open
            Receiver_LED_OUT = 1;
            Receiver_OUT_STOP = FG_NOT_allow_out;
            Receiver_OUT_CLOSE = FG_NOT_allow_out;
            Receiver_OUT_VENT = FG_NOT_allow_out;
            Receiver_OUT_OPEN = FG_allow_out;
            break;
        case 0x0C: //open+stop
            Receiver_LED_OUT = 1;
            TIMER250ms_STOP = 250;
            Receiver_OUT_CLOSE = FG_NOT_allow_out;
            Receiver_OUT_VENT = FG_NOT_allow_out;
            Receiver_OUT_STOP = FG_allow_out;
            if (FG_OUT_OPEN_CLOSE == 0)
            {
                FG_OUT_OPEN_CLOSE = 1;
                TIME_OUT_OPEN_CLOSE = 25;
            }
            if (TIME_OUT_OPEN_CLOSE == 0)
                Receiver_OUT_OPEN = FG_allow_out;
            break;
        case 0x06: //close+stop
            Receiver_LED_OUT = 1;
            TIMER250ms_STOP = 250;
            Receiver_OUT_OPEN = FG_NOT_allow_out;
            Receiver_OUT_VENT = FG_NOT_allow_out;
            Receiver_OUT_STOP = FG_allow_out;
            if (FG_OUT_OPEN_CLOSE == 0)
            {
                FG_OUT_OPEN_CLOSE = 1;
                TIME_OUT_OPEN_CLOSE = 25;
            }
            if (TIME_OUT_OPEN_CLOSE == 0)
                Receiver_OUT_CLOSE = FG_allow_out;
            break;
        case 0x0A: //close+OPEN
            Receiver_LED_OUT = 1;
            Receiver_OUT_STOP = FG_NOT_allow_out;
            Receiver_OUT_VENT = FG_NOT_allow_out;
            Receiver_OUT_OPEN = FG_allow_out;
            Receiver_OUT_CLOSE = FG_allow_out;
            break;
        case 0x09: //vent+OPEN
            Receiver_LED_OUT = 1;
            Receiver_OUT_STOP = FG_NOT_allow_out;
            Receiver_OUT_CLOSE = FG_NOT_allow_out;
            Receiver_OUT_OPEN = FG_allow_out;
            Receiver_OUT_VENT = FG_allow_out;
            break;
        case 0x03: //vent+close
            Receiver_LED_OUT = 1;
            Receiver_OUT_STOP = FG_NOT_allow_out;
            Receiver_OUT_OPEN = FG_NOT_allow_out;
            Receiver_OUT_CLOSE = FG_allow_out;
            Receiver_OUT_VENT = FG_allow_out;
            break;
        default:
            break;
        }
        if(Radio_Date_Type_bak==2)
		{             //429M   ÂçäÂºÄ‰ø°Âè∑/ÂçäÈó≠
                    if(((DATA_Packet_Control&0xDF)>0x80)&&((DATA_Packet_Control&0x20)==0x00))
						{
                                if((DATA_Packet_Control&0xDF)<0xC0){
                                    FLAG__Semi_open_T=1;
                                    FLAG__Semi_close_T=0;
                                    Receiver_LED_OUT=1;
                                    Receiver_OUT_STOP=FG_NOT_allow_out;
                                    Receiver_OUT_CLOSE=FG_NOT_allow_out;
                                    Receiver_OUT_OPEN=FG_allow_out;
                                    TIMER250ms_STOP=((TIMER_Semi_open+1)*1000/107)*100;
                                }
                                else {
                                    FLAG__Semi_open_T=0;
                                    FLAG__Semi_close_T=1;
                                    Receiver_LED_OUT=1;
                                    Receiver_OUT_STOP=FG_NOT_allow_out;
                                    Receiver_OUT_CLOSE=FG_allow_out;
                                    Receiver_OUT_OPEN=FG_NOT_allow_out;
                                    TIMER250ms_STOP=((TIMER_Semi_close+1)*1000/107)*100;
                                }
                      }
//					if((DATA_Packet_Control==0x7F)&&(Flag_ERROR_Read==0)&&(Flag_shutter_stopping==0))
//					{
//					   Flag_ERROR_Read=1;
//					   FLAG_APP_TX_fromUART_err_read=0;
//					  Send_Data(Send_err_com, 7);
//					  Flag_ERROR_Read_once_again=1;
//					  TIME_ERROR_Read_once_again=17;
//					  Time_error_read_timeout=100;
//					}
         }
		if((FLAG__Semi_open_T==1)||(FLAG__Semi_close_T==1)){
					 if((DATA_Packet_Control==0x02)||(DATA_Packet_Control==0x04)||(DATA_Packet_Control==0x08)||(DATA_Packet_Control==0x01)||(DATA_Packet_Control==0x20)||(DATA_Packet_Control==0x40)
					  ||(DATA_Packet_Control==0x9)||(DATA_Packet_Control==0x03)||(DATA_Packet_Control==0x0C)||(DATA_Packet_Control==0x06)||(DATA_Packet_Control==0x0A)){
						 //2015.12.29ËøΩÂä†ÔºåÂú®ÂçäÂºÄ„ÄÅÂçäÈó≠Âä®‰Ωú‰∏≠ÔºåÈ?Å‰ø°Êú∫ÔºàÂº?+Èó≠Ôºâ‰ø°Âè∑ÔºåËÆ©ÂÅúÊ≠¢ÁªßÁîµÂô®‰∏çÂä®‰Ωú
						 FLAG__Semi_open_T=0;FLAG__Semi_close_T=0;TIMER250ms_STOP=0;
					 }
		 }

         if(((DATA_Packet_Control==0x00)||(DATA_Packet_Control==0x02)||(DATA_Packet_Control==0x04)||(DATA_Packet_Control==0x08)||(DATA_Packet_Control==0x01)
               ||(DATA_Packet_Control==0x20)||(DATA_Packet_Control==0x40)||((FLAG__Semi_open_T==1)||(FLAG__Semi_close_T==1)))&&(FLAG_APP_TX_fromOUT==0)&&(Radio_Date_Type_bak==2)&&(FLAG_APP_TX==0)&&(FLAG_APP_TX_once==1))
         {
             FLAG_APP_TX_fromOUT=1;
			 if(DATA_Packet_Control==0x00)TIME_APP_TX_fromOUT=35;//15+DEF_APP_TX_freq*8;  //350ms
			 else TIME_APP_TX_fromOUT=35;//15+DEF_APP_TX_freq*8;  //350ms
         }


	 }

    }
    else
    {
        //           if(FLAG_APP_Reply==1){FLAG_APP_Reply=0;ID_data.IDL=DATA_Packet_ID;Control_code=HA_Status;FLAG_HA_START=1;}
        //           if(FLAG_426MHz_Reply==1){FLAG_426MHz_Reply=0;ID_data.IDL=DATA_Packet_ID;Control_code=HA_Status+4;FLAG_HA_START=1;}   //Âèó‰ø°Âô®Ëá™Âä®ÂèëÈÄÅHAÁä∂Ê?ÅÁ†Å‰∏∫ÂÆûÈôÖHAÁ†?4
        if ((FG_auto_out == 1) && (TIME_auto_out == 0))
        {
            FG_auto_out = 0;
            TIME_auto_close = 270;
            Receiver_LED_OUT = 1;
            TIME_auto_useful = 35100; //35100*10ms=(5min*60+90s)*90
        } //300
        if (TIME_auto_close)
        {
            if (TIME_auto_close > 180)
            {
                Receiver_OUT_STOP = FG_allow_out;
                Receiver_OUT_CLOSE = FG_NOT_allow_out;
            } //200
            else if (TIME_auto_close > 90)
            {
                Receiver_OUT_STOP = FG_NOT_allow_out;
                Receiver_OUT_CLOSE = FG_NOT_allow_out;
            } //100
            else
            {
                Receiver_OUT_STOP = FG_NOT_allow_out;
                Receiver_OUT_CLOSE = FG_allow_out;
                if (TIME_auto_close <= 1)
                    FG_auto_manual_mode = 0;
            }
        }
        else
        {
            Receiver_OUT_CLOSE = FG_NOT_allow_out;
        }
        FG_First_auto = 0;
        FLAG_Receiver_BEEP = 0;
		Receiver_OUT_OPEN = FG_NOT_allow_out;
		Receiver_OUT_VENT = FG_NOT_allow_out;
        if ((FLAG_ID_Erase_Login == 1) || (FLAG_ID_Login == 1) || (TIME_auto_close)||(FLAG_ID_SCX1801_Login==1))
            ;
        else if (TIME_Receiver_LED_OUT > 0)
            Receiver_LED_OUT = 1;
        else if(TIME_power_led==0)
            Receiver_LED_OUT = 0;

               if((FLAG__Semi_open_T==1)||(FLAG__Semi_close_T==1)){
//                   if(HA_Status==0x83)TIMER250ms_STOP=0;     //2015.12.29ËøΩÂä†ÔºåÂú®ÂçäÂºÄ„ÄÅÂçäÈó≠Âä®‰Ωú‰∏≠ÔºåÂèó‰ø°Êú∫ÁöÑÁä∂ÊÄÅÂèòÊàêÂºÇÂ∏?1ÁöÑÊó∂ÂÄôÔºåËÆ©ÂÅúÊ≠¢ÁªßÁîµÂô®‰∏çÂä®‰Ω?
                   if((TIMER250ms_STOP<1000)&&(TIMER250ms_STOP>0)){Receiver_OUT_STOP=FG_allow_out;Receiver_LED_OUT=1;}
                   else if(TIMER250ms_STOP==0){Receiver_OUT_STOP=FG_NOT_allow_out;FLAG__Semi_open_T=0;FLAG__Semi_close_T=0;}
               }
               else if((TIMER250ms_STOP==0)&&(TIME_auto_close==0)){Receiver_OUT_STOP=FG_NOT_allow_out;FG_OUT_OPEN_CLOSE=0;}    //2015.3.23‰øÆÊîπ

        if (FG_auto_open_time == 1)
        {
            FG_First_auto = 0;
            FG_auto_out = 1;
            FG_auto_open_time = 0;
        }
        if ((TIMER250ms_STOP == 0) && (TIME_auto_close == 0))
        {
            Receiver_OUT_STOP = FG_NOT_allow_out;
            FG_OUT_OPEN_CLOSE = 0;
        }
		FLAG_testNo91SendUart=0;
    }
    if (TIMER300ms == 0)
        FG_Receiver_LED_RX = 0; //Receiver_LED_RX=0;

}
