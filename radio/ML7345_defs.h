#ifndef __ML7345_DEFS_H__
#define __ML7345_DEFS_H__


#define BANK0_SEL           0x11    /* set bank0 */
#define BANK1_SEL           0x22    /* set bank1 */
#define BANK2_SEL           0x44    /* set bank2 */
#define BANK3_SEL           0x88    /* set bank3 */

#define AUTO_TX_EN          0x10    /* auto tx Enable,move to IDLE state(TRX_OFF) after packet transmission completion */
#define AUTO_TXDONE_GORX    (AUTO_TX_EN | 0x02)         /* move to RX state after packet transmission completion,move to IDLE state(TRX_OFF) after packet reception completion */
#define AUTO_TXDONE_CONTINUE_RX  (AUTO_TX_EN | 0x0A)    /* move to RX state after packet transmission completion,continue RX state after packet reception completion */

#define TX_FIFO_POINTER     0x81    /* Clear write pointer of FIFO */
#define RX_FIFO_POINTER     0x82    /* Clear read pointer of FIFO */
#define PHY_STATE           0x84    /* Clear PHY state */
#define TX_DONE_FLAG        0x01    /* 发送完成中断 */
#define RX_DONE_FLAG        0x00    /* 接收完成中断 */

#define GPIO_INTOUTPUT_ENABLE  0x07
#define GPIO_INTOUTPUT_DISABLE 0x00

/* bank 0 */
/* Register name            Address */
#define ADDR_BANK_SEL        0x00    /* BANK selection */
#define ADDR_RST_SET         0x01    /* Reset control */
#define ADDR_CLK_SET1        0x02    /* Clock configuration 1 */
#define ADDR_CLK_SET2        0x03    /* Clock configuration 2 */
#define ADDR_PKT_CTRL1       0x04    /* Packet configuration 1 */
#define ADDR_PKT_CTRL2       0x05    /* Packet configuration 2 */
#define ADDR_ADDR_DRATE_SET  0x06    /* Data rate setting */
#define ADDR_DATA_SET1       0x07    /* TX/RX data configuration 1 */
#define ADDR_DATA_SET2       0x08    /* TX/RX data configuration 2 */
#define ADDR_CH_SET          0x09    /* RF channel setting */
#define ADDR_RF_STATUS_CTRL  0x0A    /* RF status transition control */
#define ADDR_RF_STATUS       0x0B    /* RF status setting */
#define ADDR_DIO_SET         0x0C    /* DIO mode configuration */
#define ADDR_INT_SOURCE_GRP1 0x0D    /* Interrupt status for INT0 to INT7 */
#define ADDR_INT_SOURCE_GRP2 0x0E    /* Interrupt status for INT8 to INT15 (RX) */
#define ADDR_INT_SOURCE_GRP3 0x0F    /* Interrupt status for INT16 to INT23 (TX) */
#define ADDR_INT_EN_GRP1     0x10    /* Interrupt enable 1 */
#define ADDR_INT_EN_GRP2     0x11    /* Interrupt enable 2 */
#define ADDR_INT_EN_GRP3     0x12    /* Interrupt enable 3 */
#define ADDR_CRC_ERR_H       0x13    /* CRC error status (high byte) */
#define ADDR_CRC_ERR_M       0x14    /* CRC error status (middle byte) */
#define ADDR_CRC_ERR_L       0x15    /* CRC error status (low byte) */
#define ADDR_STATE_CLR       0x16    /* State clear control */
#define ADDR_TXFIFO_THRH     0x17    /* TX FIFO-Full level setting */
#define ADDR_TXFIFO_THRL     0x18    /* TX FIFO-Empty threshold, FAST_TX enable threshold */
#define ADDR_RXFIFO_THRH     0x19    /* RX FIFO-Full threshold */
#define ADDR_RXFIFO_THRL     0x1A    /* RX FIFO-Empty threshold */
#define ADDR_C_CHECK_CTRL    0x1B    /* Control field (C-field) detection setting */
#define ADDR_M_CHECK_CTRL    0x1C    /* Manufacturer ID field (M-field) detection setting */
#define ADDR_A_CHECK_CTRL    0x1D    /* Address field (A-field) detection setting */
#define ADDR_C_FIELD_CODE1   0x1E    /* C-field setting code #1 */
#define ADDR_C_FIELD_CODE2   0x1F    /* C-field setting code #2 */
#define ADDR_C_FIELD_CODE3   0x20    /* C-field setting code #3 */
#define ADDR_C_FIELD_CODE4   0x21    /* C-field setting code #4 */
#define ADDR_C_FIELD_CODE5   0x22    /* C-field setting code #5 */
#define ADDR_M_FIELD_CODE1   0x23    /* M-field 1st byte setting code 1 */
#define ADDR_M_FIELD_CODE2   0x24    /* M-field 1st byte setting code 2 */
#define ADDR_M_FIELD_CODE3   0x25    /* M-field 2nd byte setting code 1 */
#define ADDR_M_FIELD_CODE4   0x26    /* M-field 2nd byte setting code 2 */
#define ADDR_A_FIELD_CODE1   0x27    /* A-field 1st byte setting */
#define ADDR_A_FIELD_CODE2   0x28    /* A-field 2nd byte setting */
#define ADDR_A_FIELD_CODE3   0x29    /* A-field 3rd byte setting */
#define ADDR_A_FIELD_CODE4   0x2A    /* A-field 4th byte setting */
#define ADDR_A_FIELD_CODE5   0x2B    /* A-field 5th byte setting */
#define ADDR_A_FIELD_CODE6   0x2C    /* A-field 6th byte setting */
#define ADDR_SLEEP_WU_SET    0x2D    /* SLEEP execution and Wake-up operation setting */
#define ADDR_WUT_CLK_SET     0x2E    /* Wake-up timer clock division setting */
#define ADDR_WUT_INTERVAL_H  0x2F    /* Wake-up timer interval setting (high byte) */
#define ADDR_WUT_INTERVAL_L  0x30    /* Wake-up timer interval setting (low byte) */
#define ADDR_WU_DURATION     0x31    /* Continue operation timer (after Wake-up) setting */
#define ADDR_GT_SET          0x32    /* General purpose timer configuration */
#define ADDR_GT_CLK_SET      0x33    /* General purpose timer clock division setting */
#define ADDR_GT1_TIMER       0x34    /* General purpose timer #1 setting */
#define ADDR_GT2_TIMER       0x35    /* General purpose timer #2 setting */
#define ADDR_CCA_IGNORE_LVL  0x36    /* ED threshold level setting for excluding CCA judgment */
#define ADDR_CCA_LVL         0x37    /* CCA threshold level setting */
#define ADDR_CCA_ABORT       0x38    /* Timing setting for forced termination of CCA operation */
#define ADDR_CCA_CTRL        0x39    /* CCA control setting and result indication */
#define ADDR_ED_RSLT         0x3A    /* ED value indication */
#define ADDR_IDLE_WAIT_H     0x3B    /* IDLE detection period setting during CCA (high byte) */
#define ADDR_IDLE_WAIT_L     0x3C    /* IDLE detection period setting during CCA (low byte) */
#define ADDR_CCA_PROG_H      0x3D    /* IDLE detection elapsed time display during CCA (highbyte) */
#define ADDR_CCA_PROG_L      0x3E    /* IDLE detection elapsed time display during CCA (low byte) */
#define ADDR_PREAMBLE_SET    0x3F    /* Preamble pattern setting */
#define ADDR_VCO_VTRSLT      0x40    /* VCO adjustment voltage result display */
#define ADDR_ED_CTRL         0x41    /* ED detection control setting */
#define ADDR_TXPR_LEN_H      0x42    /* TX preamble length setting (high byte) */
#define ADDR_TXPR_LEN_L      0x43    /* TX preamble length setting (low byte) */
#define ADDR_POSTAMBLE_SET   0x44    /* Postamble setting */
#define ADDR_SYNC_CONDITION1 0x45    /* RX preamble setting and ED control setting */
#define ADDR_SYNC_CONDITION2 0x46    /* ED threshold setting during synchronization */
#define ADDR_SYNC_CONDITION3 0x47    /* Error tolerance setting */
#define ADDR_CTRL_2DIV       0x48    /* Antenna diversity setting */
#define ADDR_RSLT_2DIV       0x49    /* Antenna diversity result */
#define ADDR_ANT1_ED         0x4A    /* Acquired ED value by antenna 1 display */
#define ADDR_ANT2_ED         0x4B    /* Acquired ED value by antenna 2 display */
#define ADDR_ANT_CTRL        0x4C    /* TX/RX antenna control setting */
#define ADDR_MON_CTRL        0x4D    /* Monitor function setting */
#define ADDR_GPIO0_CTRL      0x4E    /* GPIO0 pin (pin#16) configuration setting */
#define ADDR_GPIO1_CTRL      0x4F    /* GPIO1 pin (pin#17) configuration setting */
#define ADDR_GPIO2_CTRL      0x50    /* GPIO2 pin (pin#18) configuration setting */
#define ADDR_GPIO3_CTRL      0x51    /* GPIO3 pin (pin#19) configuration setting */
#define ADDR_EXTCLK_CTRL     0x52    /* EXT_CLK pin (pin #10) control setting */
#define ADDR_SPI_EXT_PA_CTRL 0x53    /* SPI interface IO configuration /external PA control setting */
#define ADDR_CHFIL_BW        0x54    /* Channel filter bandwidth setting */
#define ADDR_DC_I_ADJ_H      0x55    /* I phase DC offset adjustment setting(high 6bits) */
#define ADDR_DC_I_ADJ_L      0x56    /* I phase DC offset adjustment setting(low byte) */
#define ADDR_DC_Q_ADJ_H      0x57    /* Q phase DC offset adjustment setting(high 6bits) */
#define ADDR_DC_Q_ADJ_L      0x58    /* Q phase DC offset adjustment setting(low byte) */
#define ADDR_DC_FIL_ADJ      0x59    /* DC offset adjustment filter setting */
#define ADDR_IQ_MAG_ADJ_H    0x5A    /* IF IQ amplitude balance adjustment (high 4bits) */
#define ADDR_IQ_MAG_ADJ_L    0x5B    /* IF IQ amplitude balance adjustment (low byte) */
#define ADDR_IQ_PHASE_ADJ_H  0x5C    /* IF IQ phase balance adjustment (high 4bits) */
#define ADDR_IQ_PHASE_ADJ_L  0x5D    /* IF IQ phase balance adjustment (low byte) */
#define ADDR_IQ_ADJ_WAIT     0x5E    /* IF IQ automatic adjustment RSSI acquisition wait time */
#define ADDR_IQ_ADJ_TARGET   0x5F    /* IF IQ automatic adjustment RSSI judgment threshold */
#define ADDR_DEC_GAIN        0x60    /* Decimation gain setting */
#define ADDR_IF_FREQ         0x61    /* IF frequency selection */
#define ADDR_OSC_ADJ1        0x62    /* Coarse adjustment of load capacitance for oscillation circuits */
#define ADDR_OSC_ADJ2        0x63    /* Fine adjustment of load capacitance for oscillation circuits */
#define ADDR_Reserved        0x64    /* Reserved */
#define ADDR_OSC_ADJ4        0x65    /* Oscillation circuits bias adjustment (high speed start-up) */
#define ADDR_RSSI_ADJ        0x66    /* RSSI value adjustment */
#define ADDR_PA_MODE         0x67    /* PA mode setting/PA regulator coarse adjustment */
#define ADDR_PA_REG_FINE_ADJ 0x68    /* PA regulator fine adjustment */
#define ADDR_PA_ADJ          0x69    /* PA gain adjustment */
#define ADDR_VCO_CAL         0x6E    /* VCO calibration setting or status indication */
#define ADDR_VCO_CAL_START   0x6F    /* VCO calibration execution */
#define ADDR_CLK_CAL_SET     0x70    /* Clock calibration setting */
#define ADDR_CLK_CAL_TIME    0x71    /* Clock calibration time setting */
#define ADDR_CLK_CAL_H       0x72    /* Clock calibration value readout (high byte) */
#define ADDR_CLK_CAL_L       0x73    /* Clock calibration value readout (low byte) */
#define ADDR_SLEEP_INT_CLR   0x75    /* Interrupt clear setting during SLEEP */
#define ADDR_RF_TEST_MODE    0x76    /* TX test pattern setting */
#define ADDR_STM_STATE       0x77    /* State machine status and synchronization status indication */
#define ADDR_FIFO_SET        0x78    /* FIFO readout setting */
#define ADDR_RX_FIFO_LAST    0x79    /* RX FIFO data usage status indication */
#define ADDR_TX_PKT_LEN_H    0x7A    /* TX packet length setting */
#define ADDR_TX_PKT_LEN_L    0x7B    /* TX packet length setting */
#define ADDR_WR_TX_FIFO      0x7C    /* TX FIFO */
#define ADDR_RX_PKT_LEN_H    0x7D    /* RX packet length indication */
#define ADDR_RX_PKT_LEN_L    0x7E    /* RX packet length indication */
#define ADDR_RD_FIFO         0x7F    /* FIFO read */



/* bank 1 */
#define ADDR_TX_RATE_H  0x02    /* TX data rate conversion setting (high 4bits) */
#define ADDR_TX_RATE_L  0x03    /* TX data rate conversion setting (low byte) */

#define ADDR_TXFREQ_I   0x1B    /* TX frequency setting (I counter) */
#define ADDR_TXFREQ_FH  0x1C    /* TX frequency setting (F counter high 4bit) */
#define ADDR_TXFREQ_FM  0x1D    /* TX frequency setting (F counter middle byte) */
#define ADDR_TXFREQ_FL  0x1E    /* TX frequency setting (F counter low byte) */
#define ADDR_RXFREQ_I   0x1F    /* RX frequency setting (I counter) */
#define ADDR_RXFREQ_FH  0x20    /* RX frequency setting (F counter high 4bit) */
#define ADDR_RXFREQ_FM  0x21    /* RX frequency setting (F counter middle byte) */
#define ADDR_RXFREQ_FL  0x22    /* RX frequency setting (F counter low byte) */

#define ADDR_VCO_CAL_MIN_I  0x4D /* VCO calibration low limit frequency setting (I counter) */
#define ADDR_VCO_CAL_MIN_FH 0x4E /* VCO calibration low limit frequency setting (F counter high 4bits) */
#define ADDR_VCO_CAL_MIN_FM 0x4F /* VCO calibration low limit frequency setting (F counter middle byte) */
#define ADDR_VCO_CAL_MIN_FL 0x50 /* VCO calibration low limit frequency setting (F counter low byte) */
#define ADDR_VCO_CAL_MAX_N  0x51 /* VCO calibration upper limit frequency setting */
#define ADDR_VCAL_MIN       0x52 /* VCO calibration low limit value indication and setting */
#define ADDR_VCAL_MAX       0x53 /* VCO calibration upper limit value indication and setting */


#endif
