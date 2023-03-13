#ifndef __EEPROM_H__
#define __EEPROM_H__
/************************************************/
/*  FILE        :eeprom.h                       */
/*  DATE        :June, 2011                     */
/*  DESCRIPTION :routine for VHF2011            */
/*  CPU TYPE    :STM8S207C8                     */
/*  Programmer	:Gong Dong Sheng                */
/*  Mark        :0x4000~0x47FF                  */
/************************************************/
void OTA_bootloader_enable(void);
void InitialFlashReg(void);
void UnlockFlash(unsigned char Type); // ½âËøflash
void LockFlash(unsigned char Type);   // Ëø¶¨´æ´¢Æ÷

unsigned char ReadByteEEPROM(unsigned long Addr);             // ¶ÁÈ¡1×Ö½Ú
void WriteByteToFLASH(unsigned long Addr, unsigned char Dat); // Ð´ÈëÒ»×Ö½Ú
void EraseByteFLASH(unsigned int Addr);                       // ²Á³ýeeprom

void eeprom_save(void);
void ID_Receiver_DATA_WRITE(u8 *address, u32 Data);
u32 ID_Receiver_DATA_READ(u8 *address);
void eeprom_sys_load(void);

void ALL_ID_EEPROM_Erase(void);
void ID_EEPROM_write(void);
void ID_SCX1801_EEPROM_write(u32 id);
void Delete_GeneralID_EEPROM(u32 id);
void ID_EEPROM_write_0x00(void);
void ID_Login_EXIT_Initial(void);
void ID_learn(void);
void eeprom_write_byte(u16 addr,u8 data);
void EEPROM_write_For_UART(void);
/************************************************/

#define UNLOCK_FLASH_TYPE ((unsigned char)0x00)
#define UNLOCK_EEPROM_TYPE ((unsigned char)0x01)

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%                  EEPROM MAP                  %
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define addr_eeprom_sys 0x1000 // SYS=0x4000~0x400F
#define Addr_rf_offset 0x3F9

#endif
