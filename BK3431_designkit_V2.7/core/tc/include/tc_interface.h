#ifndef _PARTHUS_TC_INTERFACE_
#define _PARTHUS_TC_INTERFACE_

/*****************************************************************************
 *
 * MODULE NAME:     tc_interface.h   
 * PROJECT CODE:    BlueStream
 * DESCRIPTION:     test control interface header
 * MAINTAINER:      Daire McNamara <Daire.McNamara@sslinc.com>
 * CREATION DATE:   27th January 2000     
 *
 * SOURCE CONTROL: $Id: tc_interface.h,v 1.2 2012/05/14 23:14:21 garyf Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2004 Ceva Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *     27 Jan 2000    DMN   Initial Draft - support TCI_Read_Packet_Log
 *     01 Feb 2000    IG    Active IRQ, Build Params
 *     24 Feb 2000    DMN   Renamed and Split into two headers
 *
 ****************************************************************************/


#include "sys_config.h"
#include "sys_types.h"

void    TC_Write_Local_Hardware_Register(u_int8 reg_bank, u_int32 address_offset, u_int32 value, u_int32 mask);

void    TC_Read_Local_Default_Packet_Type(void);
void    TC_Write_Local_Default_Packet_Type(u_int8 packet_type);

void    TC_Activate_Remote_DUT(u_int16 acl_handle);
void    TC_Perform_Test_Control(t_p_pdu test_control);

void    TC_Increase_Remote_Power(u_int16 acl_handle);
void    TC_Decrease_Remote_Power(u_int16 acl_handle);

void    TC_Write_Local_Hop_Frequencies(u_int8*);
void    TC_Read_Local_Hop_Frequencies(void);

void    TC_Read_Local_Hardware_Version(void);

void    TC_Increase_Local_Volume(void);
void    TC_Decrease_Local_Volume(void);

void    TC_Write_Local_Hopping_Mode(u_int8*);
void    TC_Read_Local_Hopping_Mode(void);

void    TC_Write_Local_Whitening_Enable(u_int8*);
void    TC_Read_Local_Whitening_Enable(void);

void    TC_Write_Local_Link_Key_Type(u_int8*);
void    TC_Read_Local_Link_Key_Type(void);

void    TC_Read_Local_Extended_Features(void);
void    TC_Write_Local_Features(u_int8* features);
void    TC_Write_Local_Extended_Features(u_int8* features);

void    TC_Read_Local_Timing_Information(void);
void    TC_Write_Local_Timing_Information(u_int8* timing_info);

void    TC_Reset_Local_Pump_Monitors(t_p_pdu );
void    TC_Read_Local_Pump_Monitors(t_p_pdu );

void    TC_Write_Local_Encryption_Key_Length(u_int8* min_enc_key_len, u_int8* max_enc_key_len);
void    TC_Read_Local_Encryption_Key_Length(void);

void    TC_Set_Local_Failed_Attempts_Counter(u_int8*);
void    TC_Clear_Local_Failed_Attempts_Counter(u_int8*);

void    TC_Read_Local_Baseband_Monitors(void);

void    TC_Read_Local_Native_Clock(void);
void    TC_Write_Local_Native_Clock(t_p_pdu);

void    TC_Read_Local_Relative_Host_Controller_Mips(void);

void    TC_Set_Local_Next_Available_Am_Addr(t_am_addr);

void    TC_Set_Local_Bd_Addr(u_int8*);

void    TC_Write_Local_Radio_Power(u_int16 handle, u_int16 radio_power);
void    TC_Read_Local_Radio_Power(u_int16 handle);

void    TC_Write_Local_SyncWord(u_int8* p_sync);

void    TC_Update_Manufacturing_Information(u_int8* pdu);
void    TC_Local_Area_Download(u_int8* pdu);
void    TC_Write_Host_Specific_Parameters(u_int8 length, u_int8* pdu);
void    TC_Read_Local_Hardware_Area(u_int32 addr);
void    TC_Set_System_Hardware_Configuration(u_int32 parameter);

void    TC_Write_Local_Radio_Register(u_int8 reg_addr, u_int16 reg_val);
void    TC_Read_Local_Radio_Register(u_int8 reg_addr);

void    TC_Set_HCIT_UART_Baud_Rate(u_int32 baud_rate);

void    TC_Change_Radio_Modulation(u_int8* enb_or_not);
void    TC_Read_Radio_Modulation(void);

void    TC_Set_Disable_Low_Power_Mode(void);
void    TC_Set_Enable_Low_Power_Mode(void);
void    TC_Read_R2P_Min_Search_Window(void);
void    TC_Write_R2P_Min_Search_Window(u_int8 new_window);

void    TC_Write_Park_Parameters(u_int8* pdu);
void    TC_Set_Broadcast_Scan_Window(u_int8* p_pdu);

void    TC_Read_Unused_Stack_Space(void);

void    TC_Write_VCI_CLK_Override(u_int8 length, u_int8* p_pdu);

void    TC_Send_Encryption_Key_Size_Mask_Req(u_int16 acl_handle);

void    TC_Read_Raw_RSSI(u_int16 handle);
void    TC_Read_BER(void);
void    TC_Read_PER(void);
void    TC_Read_Raw_RSSI_PER_BER(void);

void    TC_Force_System_Halt(void);


void TC_Set_Disable_SCO_Via_HCI(void);
void TC_Set_Enable_SCO_Via_HCI(void);

void   TC_Write_eSCO_Retransmission_Mode(u_int8);
void   TC_Read_eSCO_Retransmission_Mode(void);

void    TC_Write_Security_Timeouts(u_int16 link_key_timeout, u_int16 pin_code_timeout,
            u_int8 pin_code_extend);

void    TC_Write_Features(u_int16 feature, boolean enable);

#if (HW_RADIO==HWradio_BU9469EKN)
/****************************************
* RF 32bit Register in Shoga_Radio_2006
*****************************************/
void TC_Write_Local_Radio_Register32(u_int8 reg_addr, u_int16 reg_val_high, u_int16 reg_val_low);
void TC_Read_Local_Radio_Register32(u_int8 reg_addr);
#endif
//add beken test control
void TC_Beken_Hardware_Test(u_int8* pdu);

#endif


