/**
  ******************************************************************************
  * @file    Project/STM8L15x_StdPeriph_Template/main.c
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    13-May-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include <iostm8l151g4.h> // CPU型号
#include "Pin_define.h"   // 管脚定义
#include "initial.h"      // 初始�?  预定�?
#include "ram.h"          // RAM定义
#include "ML7345.h"    // 初始化ADF7021
#include "Timer.h"        // 定时�?
#include "ID_Decode.h"    // ID_Decode处理
#include "eeprom.h"       // eeprom
#include "uart.h"         // uart
/** @addtogroup STM8L15x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

void main(void)
{
    _DI();             // 关全�?中断
    RAM_clean();       // 清除RAM
    //OTA_bootloader_enable();  //IAP OTA
    WDT_init();        //看门�?
    VHF_GPIO_INIT();   //IO初始�?
    SysClock_Init();   //系统时钟初始�?
    InitialFlashReg(); //flash EEPROM
    eeprom_sys_load(); //ID载入
    TIM4_Init();       // 定时�?
    ClearWDT();        // Service the WDT
    SPI_Config_Init();
    PROFILE_CH_FREQ_32bit_200002EC = 426075000;
    RF_ML7345_Init(Fre_426_075,0x55,12);
    UART1_INIT();      // UART1 for PC Software
    _EI();             // 允许中断
    TIME_power_led=500;
    ClearWDT();        // Service the WDT
    ML7345D_RF_test_mode();
    FLAG_APP_RX = 1;
    FG_Receiver_LED_RX = 0;
    TIME_EMC = 10;
    FLAG_testNo91=0;
	FLAG_testBEEP=0;
    FLAG_Uart_WirelessLogin = 1;
    Flag_SendUart_Receiver_LED_OUT = 1;
    ML7345_SetAndGet_State(RX_ON);
    CG2214M6_USE_R;

    while (1)
    {
        ClearWDT(); // Service the WDT
        if (FLAG_testBEEP != 0)
            TEST_beep();

        if (time_Login_exit_256 == 0)
            ID_Decode_OUT();
        ID_learn();
        if((ID_SCX1801_DATA!=0)&&(Receiver_429MHz_mode==1)) APP_TX_PACKET();
        else
        {
            FLAG_APP_RX = 1;
            Flag_TxDone = 0;
            Receiver_LED_TX = 0;
        }
        if(FLAG_APP_RX==1)
        {
            ML7345D_Freq_Scanning();
            SCAN_RECEIVE_PACKET();
        }

        TranmissionACK();  //note:Don't move
        Uart_TX_Data();
        EEPROM_write_For_UART();

        if (FG_Receiver_LED_RX == 1)
            Receiver_LED_RX = 1;
        else if (FG_Receiver_LED_RX == 0)
            Receiver_LED_RX = 0;
    }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
