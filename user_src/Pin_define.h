#ifndef __PIN_DEFINE_H__
#define __PIN_DEFINE_H__
/***********************************************************************/
/*  File Name   :Pin_Define.h                                          */
/*  DATE        :Mar, 2014                                             */
/*  Programmer	:xiang 'R                                              */
/*  CPU TYPE    :STM8L151G6     Crystal: 16M HSI                       */
/*  DESCRIPTION :                                                      */
/*  Mark        :ver 1.0                                               */
/***********************************************************************/
#include "Timer.h"
//以下是IO定义
/********************LED寄存器*****************************************/
extern u8 LED_Cache;
#define LED_ON 1
#define LED_OFF 0
#define SRX1332A 1  //1用于SRX1332A-DC，0用于CY2201

#define Receiver_LED_TX PB_ODR_ODR2       //PC_ODR_ODR1       // Output   受信机送信指示  高电平有效
#define Receiver_LED_TX_direc PB_DDR_DDR2 // Output   受信机送信指示  高电平有效
#define Receiver_LED_TX_CR1 PB_CR1_C12    // Output   受信机送信指示  高电平有效
#define Receiver_LED_TX_CR2 PB_CR2_C22    //输出频率

#define Receiver_LED_RX PB_ODR_ODR3       //PC_ODR_ODR0       // Output   受信机受信指示  高电平有效
#define Receiver_LED_RX_direc PB_DDR_DDR3 // Output   受信机受信指示  高电平有效
#define Receiver_LED_RX_CR1 PB_CR1_C13    // Output   受信机受信指示  高电平有效
#define Receiver_LED_RX_CR2 PB_CR2_C23    //输出频率

#define Receiver_LED_OUT_IDR PA_IDR_IDR3
#define Receiver_LED_OUT PA_ODR_ODR3        //PB_ODR_ODR1              // Output   受信机继电器动作输出  高电平有效
#define Receiver_LED_OUT_direc PA_DDR_DDR3 // Output   受信机继电器动作输出  高电平有效
#define Receiver_LED_OUT_CR1 PA_CR1_C13    // Output   受信机继电器动作输出  高电平有效
#define Receiver_LED_OUT_CR2 PA_CR2_C23    //输出频率

/******************以下是ADF7030-1寄存器************************************/
/* ADF7030-1 register interface */
#define ADF7030_REST PD_ODR_ODR0//ADF7030_REST_Cache
#define ADF7030_REST_DDR PD_DDR_DDR0//ADF7030_REST_Cache
#define ADF7030_REST_CR1 PD_CR1_C10//ADF7030_REST_Cache
#define ADF7030_REST_CR2 PD_CR2_C20//ADF7030_REST_Cache

#define ADF7030_GPIO2 PC_IDR_IDR5
#define ADF7030_GPIO2_DDR PC_DDR_DDR5
#define ADF7030_GPIO2_CR1 PC_CR1_C15
#define ADF7030_GPIO2_CR2 PC_CR2_C25

#define ADF7030_GPIO3 PD_IDR_IDR4
#define ADF7030_GPIO3_DDR PD_DDR_DDR4
#define ADF7030_GPIO3_CR1 PD_CR1_C14
#define ADF7030_GPIO3_CR2 PD_CR2_C24
//
#define ADF7030_GPIO4 PC_IDR_IDR4
#define ADF7030_GPIO4_DDR PC_DDR_DDR4
#define ADF7030_GPIO4_CR1 PC_CR1_C14
#define ADF7030_GPIO4_CR2 PC_CR2_C24

#define ADF7030_GPIO5 PC_IDR_IDR6
#define ADF7030_GPIO5_DDR PC_DDR_DDR6
#define ADF7030_GPIO5_CR1 PC_CR1_C16
#define ADF7030_GPIO5_CR2 PC_CR2_C26

#define ADF7030CLK ADF7030_GPIO4
#define ADF7030DATA ADF7030_GPIO5
/********************天线切换 CG2214M6控制引脚寄存器*****************************************/
#define CG2214M6_VC1 PA_ODR_ODR5//PC_ODR_ODR1     //VC1
#define CG2214M6_VC1_DDR PA_DDR_DDR5//PC_DDR_DDR1 //方向
#define CG2214M6_VC1_CR1 PA_CR1_C15//PC_CR1_C11  //模式MODE
#define CG2214M6_VC1_CR2 PA_CR2_C25//PC_CR2_C21  //输出频率或者中断控制

#define CG2214M6_VC2 PB_ODR_ODR0//PC_ODR_ODR0     //VC2
#define CG2214M6_VC2_DDR PB_DDR_DDR0//PC_DDR_DDR0 //方向
#define CG2214M6_VC2_CR1 PB_CR1_C10//PC_CR1_C10  //模式MODE
#define CG2214M6_VC2_CR2 PB_CR2_C20//PC_CR2_C20  //输出频率或者中断控制

#if (SRX1332A == 1)
#define CG2214M6_RF1_USE  \
    {                     \
        CG2214M6_VC1 = 1; \
        CG2214M6_VC2 = 0; \
    }
#define CG2214M6_RF2_USE  \
    {                     \
        CG2214M6_VC1 = 0; \
        CG2214M6_VC2 = 1; \
    }
#else
#define CG2214M6_RF1_USE  \
    {                     \
        CG2214M6_VC1 = 0; \
        CG2214M6_VC2 = 1; \
    }
#define CG2214M6_RF2_USE  \
    {                     \
        CG2214M6_VC1 = 1; \
        CG2214M6_VC2 = 0; \
    }
#endif
#define CG2214M6_USE_T CG2214M6_RF1_USE
#define CG2214M6_USE_R CG2214M6_RF2_USE
/******************以下是KEY寄存器*******输入*****************************/
//#define KEY_SW2 PA_IDR_IDR4
//#define KEY_SW2_DDR PA_DDR_DDR4
//#define KEY_SW2_CR1 PA_CR1_C14
//#define KEY_SW2_CR2 PA_CR2_C24
//
//#define KEY_SW3 PA_IDR_IDR5
//#define KEY_SW3_DDR PA_DDR_DDR5
//#define KEY_SW3_CR1 PA_CR1_C15
//#define KEY_SW3_CR2 PA_CR2_C25
//
//#define KEY_SW4 PA_IDR_IDR2
//#define KEY_SW4_DDR PA_DDR_DDR2
//#define KEY_SW4_CR1 PA_CR1_C12
//#define KEY_SW4_CR2 PA_CR2_C22

// #define Receiver_Login PC_IDR_IDR6       // Input   受信机登录键   低电平有效
// #define Receiver_Login_direc PC_DDR_DDR6 // Input   受信机登录键   低电平有效
// #define Receiver_Login_CR1 PC_CR1_C16    // Input   受信机登录键   低电平有效

#define Receiver_Login PA_IDR_IDR2       // Input   受信机登录键   低电平有效
#define Receiver_Login_direc PA_DDR_DDR2 // Input   受信机登录键   低电平有效
#define Receiver_Login_CR1 PA_CR1_C12    // Input   受信机登录键   低电平有效
#define Receiver_Login_CR2 PA_CR2_C22    // Input   受信机登录键   低电平有效

#define KEY_Empty 0
#define KEY_SW2_Down 1
#define KEY_SW3_Down 2
#define KEY_SW4_Down 3

/**受信机使用的IO HA 异常信号 寄存器 */
// #define HA_L_signal PA_IDR_IDR4       // Input   HA 下限信号   低电平有效
// #define HA_L_signal_direc PA_DDR_DDR4 // Input   HA 下限信号   低电平有效
// #define HA_L_signal_CR1 PA_CR1_C14    // Input   HA 下限信号   低电平有效
// #define HA_L_signal_CR2 PA_CR2_C24

// #define HA_ERR_signal PA_IDR_IDR5       // Input   HA 异常信号  低电平有效
// #define HA_ERR_signal_direc PA_DDR_DDR5 // Input   HA 异常信号  低电平有效
// #define HA_ERR_signal_CR1 PA_CR1_C15    // Input   HA 异常信号  低电平有效
// #define HA_ERR_signal_CR2 PA_CR2_C25

// #define HA_Sensor_signal PA_IDR_IDR3       // Input   HA 传感器信号  低电平有效
// #define HA_Sensor_signal_direc PA_DDR_DDR3 // Input   HA 传感器信号  低电平有效
// #define HA_Sensor_signal_CR1 PA_CR1_C13    // Input   HA 传感器信号  低电平有效
// #define HA_Sensor_signal_CR2 PA_CR2_C23

/**WORK/TEST切换引脚寄存器*****/
#define WORK_TEST PC_IDR_IDR0//PB_IDR_IDR0     // Input 受信机测试脚  高电平有效
#define WORK_TEST_DDR PC_DDR_DDR0//PB_DDR_DDR0 // Input 受信机测试脚  高电平有效
#define WORK_TEST_CR1 PC_CR1_C10//PB_CR1_C10  // Input 受信机测试脚  高电平有效
#define WORK_TEST_CR2 PC_CR2_C20//PB_CR2_C20  // Input 受信机测试脚  高电平有效

#define Receiver_test PC_IDR_IDR0//PB_IDR_IDR0       // Input 受信机测试脚  高电平有效
#define Receiver_test_direc PC_DDR_DDR0//PB_DDR_DDR0 // Input 受信机测试脚  高电平有效
#define Receiver_test_CR1 PC_CR1_C10 //PB_CR1_C10    // Input 受信机测试脚  高电平有效

#define TP3 PA_IDR_IDR4
#define TP3_DDR PA_DDR_DDR4
#define TP3_CR1 PA_CR1_C14
#define TP3_CR2 PA_CR2_C24

#define TP4 PC_IDR_IDR1//PA_IDR_IDR5
#define TP4_DDR PC_DDR_DDR1//PA_DDR_DDR5
#define TP4_CR1 PC_CR1_C11//PA_CR1_C15
#define TP4_CR2 PC_CR2_C21//PA_CR2_C25

/******************以下是data寄存器************************************/
#define Receiver_vent   1 //Receiver_vent_Cache       // Input   受信机换气联动ON/OFF
#define Receiver_vent_direc Receiver_vent_Cache // Input   受信机换气联动ON/OFF
#define Receiver_vent_CR1 Receiver_vent_Cache   // Input   受信机换气联动ON/OFF

#define PIN_BEEP PB_ODR_ODR1       //PA_ODR_ODR0       // Output   蜂鸣器
#define PIN_BEEP_direc PB_DDR_DDR1 // Output   蜂鸣器
#define PIN_BEEP_CR1 PB_CR1_C11    // Output   蜂鸣器

#define Receiver_OUT_OPEN_IDR PD_IDR_IDR3
#define Receiver_OUT_OPEN PD_ODR_ODR3       // Output   受信机继电器OPEN  高电平有效
#define Receiver_OUT_OPEN_direc PD_DDR_DDR3 // Output   受信机继电器OPEN  高电平有效
#define Receiver_OUT_OPEN_CR1 PD_CR1_C13    // Output   受信机继电器OPEN  高电平有效

#define Receiver_OUT_CLOSE_IDR PD_IDR_IDR2
#define Receiver_OUT_CLOSE PD_ODR_ODR2       // Output   受信机继电器close  高电平有效
#define Receiver_OUT_CLOSE_direc PD_DDR_DDR2 // Output   受信机继电器close  高电平有效
#define Receiver_OUT_CLOSE_CR1 PD_CR1_C12    // Output   受信机继电器close  高电平有效

#define Receiver_OUT_STOP_IDR PD_IDR_IDR1
#define Receiver_OUT_STOP PD_ODR_ODR1       // Output   受信机继电器stop  高电平有效
#define Receiver_OUT_STOP_direc PD_DDR_DDR1 // Output   受信机继电器stop  高电平有效
#define Receiver_OUT_STOP_CR1 PD_CR1_C11    // Output   受信机继电器stop  高电平有效

#define Receiver_OUT_VENT_IDR PA_IDR_IDR4
#define Receiver_OUT_VENT PA_ODR_ODR4       // Output   受信机继电器VENT  高电平有效
#define Receiver_OUT_VENT_direc PA_DDR_DDR4 // Output 受信机继电器VENT  高电平有效
#define Receiver_OUT_VENT_CR1 PA_CR1_C14    // Output 受信机继电器VENT  高电平有效


// #define Inverters_OUT PA_IDR_IDR2       // 输入   继电器输出信号反向   低电平有效
// #define Inverters_OUT_direc PA_DDR_DDR2 // 输入   继电器输出信号反向   低电平有效
// #define Inverters_OUT_CR1 PA_CR1_C12    // 输入   继电器输出信号反向   低电平有效

/*********************************************************************************/

//----------------------------------ML7345D------------------------------------------------
#define Key_Login_Val   PB_IDR_IDR0 //登录按键
#define Key_Login_DDR   PB_DDR_DDR0
#define Key_Login_CR1   PB_CR1_C10
#define Key_Login_CR2   PB_CR2_C20

#define RX_LED          PB_ODR_ODR3 //受信指示
#define RX_LED_DDR      PB_DDR_DDR3
#define RX_LED_CR1      PB_CR1_C13
#define RX_LED_CR2      PB_CR2_C23

#define TX_LED          PB_ODR_ODR2 //送信指示
#define TX_LED_DDR      PB_DDR_DDR2
#define TX_LED_CR1      PB_CR1_C12
#define TX_LED_CR2      PB_CR2_C22
/*
#define RX_LED_OUT      PA_ODR_ODR3 //动作指示
#define RX_LED_OUT_DDR  PA_DDR_DDR3
#define RX_LED_OUT_CR1  PA_CR1_C13
#define RX_LED_OUT_CR2  PA_CR2_C23
*/
#define KEY_TX_OPEN     PD_IDR_IDR3 /* 工作模式下按键发送OPEN,低电平有效 */
#define KEY_TX_OPEN_DDR PD_DDR_DDR3
#define KEY_TX_OPEN_CR1 PD_CR1_C13
#define KEY_TX_OPEN_CR2 PD_CR2_C23
#define Key_Tx_Carrier  KEY_TX_OPEN /* 测试模式下按键发载波 */

#define KEY_TX_STOP     PD_IDR_IDR1 /* 工作模式下按键发送STOP,低电平有效 */
#define KEY_TX_STOP_DDR PD_DDR_DDR1
#define KEY_TX_STOP_CR1 PD_CR1_C11
#define KEY_TX_STOP_CR2 PD_CR2_C21
#define Key_Rx_Data     KEY_TX_STOP /* 测试模式下按键接收数据 */

#define KEY_TX_CLOSE     PD_IDR_IDR2 /* 工作模式下按键发送CLOSE,低电平有效 */
#define KEY_TX_CLOSE_DDR PD_DDR_DDR2
#define KEY_TX_CLOSE_CR1 PD_CR1_C12
#define KEY_TX_CLOSE_CR2 PD_CR2_C22
#define Key_Tx_Data      KEY_TX_CLOSE   /* 测试模式下按键发数据 */

#define TP7_TEST_MODE       PC_IDR_IDR0  /* 0:测试模式；1:工作模式 */
#define TP7_TEST_MODE_DDR   PC_DDR_DDR0
#define TP7_TEST_MODE_CR1   PC_CR1_C10
#define TP7_TEST_MODE_CR2   PC_CR2_C20

#define TP8_TXRX_MODE       PC_IDR_IDR1  /* 预留 */
#define TP8_TXRX_MODE_DDR   PC_DDR_DDR1
#define TP8_TXRX_MODE_CR1   PC_CR1_C11
#define TP8_TXRX_MODE_CR2   PC_CR2_C21
/*
#define RF_WORKE_MODE          PD_ODR_ODR0  //0:RF处于工作模式;1:RF处于深度睡眠模式
#define RF_WORKE_MODE_DDR      PD_DDR_DDR0
#define RF_WORKE_MODE_CR1      PD_CR1_C10
#define RF_WORKE_MODE_CR2      PD_CR2_C20
*/
#define ML7345_RESETN      PD_ODR_ODR0 /* L 硬件复位; H 工作 */
#define ML7345_RESETN_DDR  PD_DDR_DDR0
#define ML7345_RESETN_CR1  PD_CR1_C10
#define ML7345_RESETN_CR2  PD_CR2_C20

#define ML7345_INT_GPIO0        PD_IDR_IDR4 // 低电平有效
#define ML7345_INT_GPIO0_DDR    PD_DDR_DDR4
#define ML7345_INT_GPIO0_CR1    PD_CR1_C14
#define ML7345_INT_GPIO0_CR2    PD_CR2_C24

#define ML7345_INT_GPIO1        PC_IDR_IDR6 /* 低电平有效 */
#define ML7345_INT_GPIO1_DDR    PC_DDR_DDR6
#define ML7345_INT_GPIO1_CR1    PC_CR1_C16
#define ML7345_INT_GPIO1_CR2    PC_CR2_C26

#define ML7345_INT_GPIO2        PC_IDR_IDR5 /* 低电平有效 */
#define ML7345_INT_GPIO2_DDR    PC_DDR_DDR5
#define ML7345_INT_GPIO2_CR1    PC_CR1_C15
#define ML7345_INT_GPIO2_CR2    PC_CR2_C25

#define ML7345_INT_GPIO3        PC_IDR_IDR4 /* 低电平有效 */
#define ML7345_INT_GPIO3_DDR    PC_DDR_DDR4
#define ML7345_INT_GPIO3_CR1    PC_CR1_C14
#define ML7345_INT_GPIO3_CR2    PC_CR2_C24

#define KEY_POWER_ON      PA_ODR_ODR4 /* 按键使能,高电平有效 */
#define KEY_POWER_ON_DDR  PA_DDR_DDR4
#define KEY_POWER_ON_CR1  PA_CR1_C14
#define KEY_POWER_ON_CR2  PA_CR2_C24

#define RF_POWER_ON         PA_ODR_ODR5 /* RF使能,高电平有效 */
#define RF_POWER_ON_DDR     PA_DDR_DDR5
#define RF_POWER_ON_CR1     PA_CR1_C15
#define RF_POWER_ON_CR2     PA_CR2_C25


#define ML7345_CLK      PC_IDR_IDR6
#define ML7345_CLK_DDR  PC_DDR_DDR6
#define ML7345_CLK_CR1  PC_CR1_C16
#define ML7345_CLK_CR2  PC_CR2_C26

#define ML7345_DATA     PC_IDR_IDR4
#define ML7345_DATA_DDR PC_DDR_DDR4
#define ML7345_DATA_CR1 PC_CR1_C14
#define ML7345_DATA_CR2 PC_CR2_C24


#endif
