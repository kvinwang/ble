/***********************************************************************
 *
 * MODULE NAME:    sys_hal_config.c
 * PROJECT CODE:    BlueStream
 * DESCRIPTION:    Customer specific configuration
 * MAINTAINER:     John Nelson
 * DATE:           15 June 2001
 *
 * SOURCE CONTROL: $Id: sys_hal_config.c,v 1.8 2011/01/19 17:35:32 garyf Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2004 Ceva Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    15 June 2001 -   JN       - first version extracted from sys_config
 *
 *
 * This module is a customer specific module per hal that allows the 
 * customer to configure the stack through the sys_config interface.
 *
 * The customer can:
 * 1. Initialise the BD_ADDR
 * 2. Initialise the device class
 * 2. Override the default features supported
 * 3. Override the ACL/SCO buffer allocations
 * 4. Override default major hardware register initialisations
 * 5. Override the default unit key
 * 6. Override the default random number seed.
 ***********************************************************************/
#include <string.h>
#include "sys_config.h"
#include "sys_hal_features.h"
#include "BKAPI.h" //Kevin Add @ 20171220
extern t_SYS_Config g_sys_config;

#define TEN_MICRO_SECONDS (0x30d40)
void EEPROM_Wait_For(u_int32 len);
//Kevin Add@ 20171218
#ifdef PWMLIGHT
	u_int8 devNameStr[16] = "M_light";
#endif
/*****************************************************************************
 * SYShal_config_Initialise
 *
 * Setup/override the major system configuration structure g_sys_config.
 *
 * Customer modifiable.
 *
 *****************************************************************************/
void SYShal_config_Initialise(void)
{
#if (EEPROM_STORAGE == 1)
    /*
     * A. Initialise the chimera N2W peripheral
     */
    SYShalconfig_Initialise_EEPROM();

    /*
     * 1. Initialise the BD_ADDR
     */
    SYShalconfig_Get_Stored_BD_ADDR();

    /*
     * 3. Override the default features supported
     */

    /*
     * 4. Override the ACL/SCO buffer allocations
     */

    /*
     * 5. Override default major hardware register initialisations
     */

    /*
     * 6. Override default SYSrand seed using 16 bits of local address as upper
     *    16 bits of seed.
     */
    g_sys_config.rand_seed = (g_sys_config.local_device_address.bytes[0] << 24) + 
                             (g_sys_config.local_device_address.bytes[1] << 16);

#else
	#if (STORE_BTADDR_FLASH == 1)
		/* //u_int8 bd_addr[8] = {0x00, 0xff, 0x00, 0x22, 0x2d, 0xae}; */
		//　/*Uu_intt8 bd_addr_bak[8] = {0x33, 0xff, 0x00, 0x22, 0x2d, 0xae};/* Com4 */
		//u_int8 bd_addr_bak[8] = {0x66, 0xff, 0x00, 0x22, 0x2d, 0xae};
		/* //u_int8 bd_addr[8] = {0x09, 0xff, 0x00, 0x22, 0x2d, 0xae};/\* Com9 *\/ */
		u_int8 i = 0;
		u_int8 macAddr[6];
		u_int32 macAddrOrg = 0;    	  //设备编号起始mac地址对应的数值
		u_int32 devNameSuffix = 0;    //设备名称后缀编号
		u_int32 devNameSuffixOrg = 0; //设备名称后缀起始编号
		FlashStorageInit();
		
		for(i=0; i<6; i++)
		{
			g_sys_config.local_device_address.bytes[i] = g_sys_vars->bt_addr.bytes[i];//bd_addr_bak[i];
			//memcpy(g_sys_config.local_device_address.bytes, bd_addr_bak, BDADDR_LENGTH);
			macAddr[i] = g_sys_vars->bt_addr.bytes[i]; //Kevin Add @ 20171220
		}
		#ifdef PWMLIGHT
			//将当前mac地址的后四个字节转化为对应数值
			devNameSuffix = (macAddr[3]<<24) | (macAddr[2]<<16) | (macAddr[1]<<8) | macAddr[0]; 
			devNameSuffix = devNameSuffix - macAddrOrg + devNameSuffixOrg; //得到当前设备对应的名称后缀编号
			#if 1 //Method one
				for(i = 0; i < 16; i++)
				{
					if((devNameStr[i] == 0) && (i < 16))
					{
						devNameStr[i++] = (devNameSuffix/10000)%10 + 0x30;
						devNameStr[i++] = (devNameSuffix/1000)%10 + 0x30;
						devNameStr[i++] = (devNameSuffix/100)%10 + 0x30;
						devNameStr[i++] = (devNameSuffix/10)%10 + 0x30;
						devNameStr[i++] = (devNameSuffix)%10 + 0x30;
						devNameStr[i] = '\0';
						break;
					}
				}
			#else //Method two
				devNameSuffix = devNameSuffix % 100000;
				for(i = 0; i < 16; i++)
				{
					if((devNameStr[i] == 0) && (i < 16))
					{
						if(devNameSuffix < 10)
						{
							devNameStr[i++] = (devNameSuffix)%10 + 0x30;
							devNameStr[i] = 0;
							break;
						}
						else if(devNameSuffix < 100)
						{
							devNameStr[i++] = ((devNameSuffix / 10) % 10) + 0x30;
							devNameStr[i++] = (devNameSuffix % 10) + 0x30;
							devNameStr[i] = 0;
							break;
						}
						else if(devNameSuffix < 1000)
						{
							devNameStr[i++] = ((devNameSuffix / 100) % 10) + 0x30;
							devNameStr[i++] = ((devNameSuffix / 10) % 10) + 0x30;
							devNameStr[i++] = (devNameSuffix)%10 + 0x30;
							devNameStr[i] = 0;
							break;
						}
						else if(devNameSuffix < 10000)
						{
							devNameStr[i++] = ((devNameSuffix / 1000) % 10) + 0x30;
							devNameStr[i++] = ((devNameSuffix / 100) % 10) + 0x30;
							devNameStr[i++] = ((devNameSuffix / 10) % 10) + 0x30;
							devNameStr[i++] = (devNameSuffix)%10 + 0x30;
							devNameStr[i] = 0;
							break;
						}
						else
						{
							devNameStr[i++] = ((devNameSuffix / 10000) % 10) + 0x30;
							devNameStr[i++] = ((devNameSuffix / 1000) % 10) + 0x30;
							devNameStr[i++] = ((devNameSuffix / 100) % 10) + 0x30;
							devNameStr[i++] = ((devNameSuffix / 10) % 10) + 0x30;
							devNameStr[i++] = (devNameSuffix)%10 + 0x30;
							devNameStr[i] = 0;
							break;
						}
					}
				}
			#endif
			memcpy(g_sys_vars->bt_name, devNameStr, 16);
		#endif
		
	#else
		/*#ifdef FIX_ROM_ADDRESS
				memcpy(g_sys_config.local_device_address.bytes, bd_addr_bak, BDADDR_LENGTH);
		#else

			u_int32 bd_addr[2];
			read_nvr(0, bd_addr, 2);
			if ((bd_addr[0] == 0xffffffff) && (bd_addr[1] == 0xffffffff))
				memcpy(g_sys_config.local_device_address.bytes, bd_addr_bak, BDADDR_LENGTH);
			else
				memcpy(g_sys_config.local_device_address.bytes, bd_addr, BDADDR_LENGTH);
		#endif*/
	#endif
#endif
}


void SYShalconfig_BD_ADDR_Was_Set(void)
{
#if (EEPROM_STORAGE == 1)
    int i = 0;

    for(i = 0; i < 6; i++)
    {
        EEPROM_Write_Byte(0, BD_ADDR_LOCATION+i, g_sys_config.local_device_address.bytes[i]);
    }
#endif
}

void SYShalconfig_Get_Stored_BD_ADDR(void)
{
#if (EEPROM_STORAGE == 1)
    int i = 0;

    for(i = 0; i < 6 ; i++)
    {
        g_sys_config.local_device_address.bytes[i] = EEPROM_Read_Byte(0, (BD_ADDR_LOCATION+i));
    }
#endif
}


void SYShalconfig_SYS_Rand_Seed_Was_Set(void)
{
    /*
     * Override default SYS Rand Seed.
     */
}

void SYShalconfig_Initialise_EEPROM(void)
{
#if (EEPROM_STORAGE == 1)
    /*
     * Do Soft Reset of N2W
     */
    EEPROM_Soft_Reset();

    /*
     * Initialise N2W Clock Registers
     */
    EEPROM_Initialise_Clock();

    /*
     * For some reason, soft reset does not
     * necessarily clean out the RX FIFO.
     * Clean out RX Fifo
     */
    EEPROM_Initialise_Rx_Fifo();

    /*
     * Run a small self-check on the EEPROM
     */
    /* EEPROM_Self_Check(); */
#endif
}


void SYShalconfig_Set_System_Hardware_Configuration(u_int32 info)
{
/*  
 *             *) hardware configuration information
 *                  bit31..bit16: reserved
 *                  bit15..bit12: transmitter class type
 *                                 0: external power amplifier unavailable (class2/3 tx)
 *                                 1: external power amplifier available (class1 tx)
 *                                 others: reserved
 *                  bit11..bit8 : voice codec via voice control interface
 *                                 0: Motorola MC145483
 *                                 1: ST Micro STLC7550
 *                                 others: reserved
 *                  bit7..bit4  : system clock scheme for low power mode operation
 *                                 0: radio clocks cpu
 *                                 1: an external oscillator clocks cpu
 *                                 others: reserved
 *                  bit3..bit0  : low power oscillator type
 *                                 0: low power oscillator unavailable
 *                                 1: 32.768KHz low power oscillator available
 *                                 2: 32.000KHz low power oscillator available
 *                                 others: reserved
 */
}





void EEPROM_Soft_Reset(void)
{
#if (EEPROM_STORAGE == 1)
    u_int16 status;
    N2W_CONTROL_REG = (u_int16)N2W_SOFT_RESET;
    EEPROM_Wait_For(0x1ff);
    status = N2W_STATUS_REG;
#endif
}

void EEPROM_Initialise_Clock(void)
{
#if (EEPROM_STORAGE == 1)
    u_int16 status;
    N2W_CLOCK_REG = (u_int16)N2W_CLOCK_RATE;
    EEPROM_Wait_For(0x1ff);
    status = N2W_STATUS_REG;
#endif
}

void EEPROM_Initialise_Rx_Fifo(void)
{
#if (EEPROM_STORAGE == 1)
    u_int16 foo;
    u_int16 status_reg;

    while(!((status_reg = N2W_STATUS_REG) & N2W_RX_FIFO_EMPTY_BIT))
    {
        foo = N2W_RX_FIFO;
    }
#endif
}

void EEPROM_Write_Byte(u_int8 addr_block, u_int16 address, u_int8 byte)
{
#if (EEPROM_STORAGE == 1)
    /*
     * Start Write Condition
     */
    N2W_TX_FIFO = (u_int16)(N2W_START_BIT | EEPROM_ADDR | (addr_block << 1) | WRITE_CYCLE);
    /* EEPROM_Wait_For(0x1ff); */

    /*
     * Provide Address to write to
     */
    N2W_TX_FIFO = (u_int16)address;
    /* EEPROM_Wait_For(0x1ff); */

    /*
     * Write byte and
     * end write Condition
     */
    N2W_TX_FIFO = (u_int16)(N2W_STOP_BIT | byte);

    EEPROM_Wait_For(0x30d40);
    EEPROM_Wait_For(0x30d40);

#endif
}


u_int8 EEPROM_Read_Byte(u_int8 addr_block, u_int16 address)
{
#if (EEPROM_STORAGE == 1)
    u_int16 status_reg;
    u_int8 result;

    SYShalconfig_Initialise_EEPROM();
    
    /*
     * Start read condition by "writing" byte's address
     */
    N2W_TX_FIFO = (u_int16)(N2W_START_BIT | EEPROM_ADDR | (addr_block << 1) | WRITE_CYCLE);
    EEPROM_Wait_For(0x1ff);

    /*
     *  Set address within block to read from
     */
    N2W_RX_FIFO = address;

    /*
     * Ask for a read value
     */
    N2W_RX_FIFO = (u_int16)(N2W_START_BIT | EEPROM_ADDR | (addr_block << 1) | READ_CYCLE);

    /*
     * Set address within block to read from
     */ 
    N2W_RX_FIFO = (u_int16)address;
    
    /*
     * send stop bit
     */
    N2W_RX_FIFO = (u_int16)N2W_STOP_BIT;

    EEPROM_Wait_For(0x30d40);

    status_reg = N2W_STATUS_REG;

    
    result = (N2W_RX_FIFO & 0xFF);

    EEPROM_Initialise_Rx_Fifo();

    return result;
#else
    return 0;
#endif
}

void EEPROM_Self_Check(void)
{
    /*
     * TODO: Detect if EEPROM present or not
     */
}

void EEPROM_Wait_For(u_int32 len)
{
#if (EEPROM_STORAGE == 1)
    int i = 0;

    for(i = 0; i < len; i++)
        ;
#endif
}

