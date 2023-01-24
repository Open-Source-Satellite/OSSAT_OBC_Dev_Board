/***********************************************************************************
 *  @file STM32CAN_ContollerAreaNetwork.h
 ***********************************************************************************
 *   _  _____ ____  ____  _____ 
 *  | |/ /_ _/ ___||  _ \| ____|
 *  | ' / | |\___ \| |_) |  _|  
 *  | . \ | | ___) |  __/| |___ 
 *  |_|\_\___|____/|_|   |_____|
 *
 ***********************************************************************************
 *  Copyright (c) 2022 KISPE Space Systems Ltd.
 *  
 *  www.kispe.co.uk/projectlicenses/RA2001001001
 ***********************************************************************************
 *  Created on: 04-May-2022 11:40:19                      
 *  Implementation of the Class STM32CAN_ContollerAreaNetwork       
 *  @author: Paul Madle                     
 ***********************************************************************************/

#ifndef __STM32CAN_CONTOLLERAREANETWORK_H__
#define __STM32CAN_CONTOLLERAREANETWORK_H__

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This assert routine is written as a Macro because the __FILE__ and __LINE__
 * macros work correctly when replaced by the macro (the line numbers are
 * correct)
 * Also, it results in a tidy, simple routine to check for an error before logging
 * it.
 * The do{}while(0) loop is there simply to enforce an ending
 * semi colon in the code, without the semi colon, there is a compilation error.
 */
#define EH_ASSERT(CONDITION, EXCEPTION, TEXT) do{if(CONDITION) { (void)0; }else{EH_Exception(__FILE__, __LINE__, TEXT);goto EXCEPTION;}}while(0)

/**
 * This throw macro is exactly the same as the EH_ASSERT macro except that
 * the control flow does not move to exception handling code and instead carries
 * on from where the macro finishes. It is to be used under conditions where the
 * error does not warrant a change to the control flow.
 */ // TODO: Add this to EA.
#define EH_THROW(CONDITION, TEXT) ((CONDITION) ? (void)0 : EH_Exception(__FILE__, __LINE__, TEXT))

extern UART_HandleTypeDef huart3;

typedef unsigned char tUINT8;
typedef unsigned int tUINT32;
typedef unsigned int tBOOL;
typedef enum teFUNC_STATUS
{
	BT_FAIL = 0,
	BT_SUCCESS = 1
}teFUNC_STATUS;

#define TRUE (1)
#define FALSE (0)

typedef teFUNC_STATUS (*tFUNC_PTR)();

//#include "BT_BaseTypes.h"
//#include "EH_ExceptionHandler.h"
//#include "SM_StringManipulation.h"

/**
 * This object drives the STM32 CAN module. It does not support CAN-FD or CAN-TT
 * features in the CAN hardware.
 * The driver has the capability to filter received messages using an acceptance
 * filter and mask on the ID or a range of input IDs.
 * The driver assumes that 1 RX FIFIOs and 1 TX FIFO is used to hold the message
 * data. These FIFOs are held in a memory that is shared between the 2 CAN
 * interfaces available on the STM32.
 * Interrupts are enabled on CAN reception (following filters), CAN transmission
 * and on errors.
 */

#define STM32CAN_FDCAN1_MESSAGE_RAM_ADDRESS (0x4000AC00UL) // TODO: Add to EA
#define STM32CAN_FDCAN2_MESSAGE_RAM_ADDRESS (0x4000FC12UL) // TODO: Add to EA
/**
 * The maximum length of a CAN 2.0b frame's data field for the STM32.
 */
#define STM32CAN_FRAME_DATA_MAX_LENGTH (8UL)
/**
 * This is an enumeration that specifies the data rate on the CAN bus.
 */
typedef enum teSTM32CAN_BAUD_RATE
{
	CAN_BAUD_1MBPS,
	CAN_BAUD_500KBPS,
	CAN_BAUD_250KBPS,
	CAN_BAUD_125KBPS
} teSTM32CAN_BAUD_RATE;

/**
 * This is used to distinguish between the 2 CAN peripherals. The numbers
 * represent the base address of the registers.
 */
typedef enum teSTM32CAN_PERIPHERAL
{
	CAN_PERIPHERAL_FDCAN1 = 0x4000A000,
	CAN_PERIPHERAL_FDCAN2 = 0x4000A400
} teSTM32CAN_PERIPHERAL;

/**
 * This is a structure holding all of the data relevant to a particular CAN frame.
 * 
 * All frames handled by this driver are Data (not Remote) frames.
 * All frames are CAN 2.0b (not CAN-FD), therefore the CAN frame has max 8 data
 * bytes.
 */
typedef struct tsSTM32CAN_Frame
{
	tUINT32 iIdentifier;
	tUINT32 iDataLength;
	tUINT8 aData[STM32CAN_FRAME_DATA_MAX_LENGTH];
}  tsSTM32CAN_Frame;


/**
 * A struct for an RX FIFO element.
 */
typedef struct tsSTM32CAN_RXFifoElement
{
	tUINT32 iID:29;
	tUINT32 iRTR:1;
	tUINT32 iXTD:1;
	tUINT32 iESI:1;
	tUINT32 iRXTS:16;
	tUINT32 iDLC:4;
	tUINT32 iBRS:1;
	tUINT32 iFDF:1;
	tUINT32 reserved:2;
	tUINT32 iFIDX:7;
	tUINT32 iANMF:1;
	tUINT8 aDataBytes[STM32CAN_FRAME_DATA_MAX_LENGTH];
}  tsSTM32CAN_RXFifoElement;

void EH_Exception(char* sFile, char* sLine, char* sErrorString);
teFUNC_STATUS STM32CAN_Initialise(teSTM32CAN_PERIPHERAL tePeripheral, tUINT32* pBufferMemory, teSTM32CAN_BAUD_RATE teBaudRate, tUINT32 iInputClockSpeed, tUINT32 iNumberOf29bitFilters, tUINT32 iNumberOfReceiveFramesInFIFO, tUINT32 iNumberOfTransmitFramesInFIFO);
teFUNC_STATUS STM32CAN_Transmit(teSTM32CAN_PERIPHERAL tePeripheral, tsSTM32CAN_Frame atsCanFrame[], tUINT32 iNumberOfFrames);
teFUNC_STATUS STM32CAN_Receive(teSTM32CAN_PERIPHERAL tePeripheral, tUINT32* pStartAddressOfRxFIFO, tUINT32* piIndexOfFirst, tUINT32* piNumberOFElementsToRead, tUINT32* piMaxNumberOFElementsInFIFO);
teFUNC_STATUS STM32CAN_Flush(teSTM32CAN_PERIPHERAL tePeripheral);
teFUNC_STATUS STM32CAN_HandleErrors(teSTM32CAN_PERIPHERAL tePeripheral, tBOOL *pbIsBusOff);
teFUNC_STATUS STM32CAN_ISRTransmissionComplete(teSTM32CAN_PERIPHERAL tePeripheral);
teFUNC_STATUS STM32CAN_ISRErrorTransition(teSTM32CAN_PERIPHERAL tePeripheral);
teFUNC_STATUS STM32CAN_AddFilterRange(teSTM32CAN_PERIPHERAL tePeripheral, tUINT32 iFilterMin, tUINT32 iFilterMax);
teFUNC_STATUS STM32CAN_AddFilterAcceptMask(teSTM32CAN_PERIPHERAL tePeripheral, tUINT32 iAcceptanceFilter, tUINT32 iAcceptanceMask);
teFUNC_STATUS STM32CAN_Connect(teSTM32CAN_PERIPHERAL tePeripheral);
teFUNC_STATUS STM32CAN_RegisterCallbacks(teSTM32CAN_PERIPHERAL tePeripheral, tFUNC_PTR pReceiveCallback, tFUNC_PTR pTransmitCallback);
teFUNC_STATUS STM32CAN_CancelTransmission(teSTM32CAN_PERIPHERAL tePeripheral);
teFUNC_STATUS STM32CAN_Disconnect(teSTM32CAN_PERIPHERAL tePeripheral);


#ifdef __cplusplus
}
#endif
#endif /* __STM32CAN_CONTOLLERAREANETWORK_H__ */

