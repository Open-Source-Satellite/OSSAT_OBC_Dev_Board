/***********************************************************************************
 *  @file STM32CAN_ContollerAreaNetwork.c
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
 *  Created on: 04-May-2022 11:40:20                      
 *  Implementation of the Class STM32CAN_ContollerAreaNetwork       
 *  @author: Paul Madle                     
 ***********************************************************************************/

#include "STM32CAN_ContollerAreaNetwork.h"

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
/* PRIVATE FUNCTIONS */

#ifndef UNIT_TEST


#endif /* UNIT_TEST */



/* PRIVATE ATTRIBUTES */

/**
 * A struct for an extended ID filter element.
 */
typedef struct tsSTM32CAN_ExtendedIDFilterElement
{
	tUINT32 iEFID1:29;
	tUINT32 iEFEC:3;
	tUINT32 iEFID2:29;
	// cppcheck-suppress unusedStructMember
	tUINT32 reserved:1;
	tUINT32 iEFTI:2;
}  tsSTM32CAN_ExtendedIDFilterElement;


/**
 * A struct for a TX FIFO element.
 */
typedef struct tsSTM32CAN_TXFifoElement
{
	tUINT32 iID:29;
	tUINT32 iRTR:1;
	tUINT32 iXTD:1;
	tUINT32 iESI:1;
	// cppcheck-suppress unusedStructMember
	tUINT32 reserved1:16;
	tUINT32 iDLC:4;
	tUINT32 iBPS:1;
	tUINT32 iFDF:1;
	// cppcheck-suppress unusedStructMember
	tUINT32 reserved2:1;
	tUINT32 iEFC:1;
	tUINT32 iMM:8;
	tUINT8 aDataBytes[STM32CAN_FRAME_DATA_MAX_LENGTH];
}  tsSTM32CAN_TXFifoElement;


/**
 * A struct that holds the Rx and Tx complete callback function pointers for a CAN
 * controller.
 */
typedef struct tsSTM32CAN_Callbacks
{
	tFUNC_PTR pTxCallback;
	tFUNC_PTR pRxCallback;
}  tsSTM32CAN_Callbacks;


/**
 * A struct holding the registers for a CAN Controller.
 */
typedef struct tsSTM32CAN_Registers
{
	tUINT32 FDCAN_CREL;
	tUINT32 FDCAN_ENDN;
	tUINT32 reserved1;
	tUINT32 FDCAN_DBTP;
	tUINT32 FDCAN_TEST;
	tUINT32 FDCAN_RWD;
	tUINT32 FDCAN_CCCR;
	tUINT32 FDCAN_NBTP;
	tUINT32 FDCAN_TSCC;
	tUINT32 FDCAN_TSCV;
	tUINT32 FDCAN_TOCC;
	tUINT32 FDCAN_TOCV;
	tUINT32 unused2[4];
	tUINT32 FDCAN_ECR;
	tUINT32 FDCAN_PSR;
	tUINT32 FDCAN_TDCR;
	tUINT32 unused3;
	tUINT32 FDCAN_IR;
	tUINT32 FDCAN_IE;
	tUINT32 FDCAN_ILS;
	tUINT32 FDCAN_ILE;
	tUINT32 unused4[8];
	tUINT32 FDCAN_GFC;
	tUINT32 FDCAN_SIDFC;
	tUINT32 FDCAN_XIDFC;
	tUINT32 unused5;
	tUINT32 FDCAN_XIDAM;
	tUINT32 FDCAN_HPMS;
	tUINT32 FDCAN_NDAT1;
	tUINT32 FDCAN_NDAT2;
	tUINT32 FDCAN_RXF0C;
	tUINT32 FDCAN_RXF0S;
	tUINT32 FDCAN_RXF0A;
	tUINT32 FDCAN_RXBC;
	tUINT32 FDCAN_RXF1C;
	tUINT32 FDCAN_RXF1S;
	tUINT32 FDCAN_RXF1A;
	tUINT32 FDCAN_RXESC;
	tUINT32 FDCAN_TXBC;
	tUINT32 FDCAN_TXFQS;
	tUINT32 FDCAN_TXESC;
	tUINT32 FDCAN_TXBRP;
	tUINT32 FDCAN_TXBAR;
	tUINT32 FDCAN_TXBCR;
	tUINT32 FDCAN_TXBTO;
	tUINT32 FDCAN_TXBCF;
	tUINT32 FDCAN_TXBTIE;
	tUINT32 FDCAN_TXBCIE;
	tUINT32 unused6[2];
	tUINT32 FDCAN_TXEFC;
	tUINT32 FDCAN_TXEFS;
	tUINT32 FDCAN_TXEFA;
	tUINT32 unused7;
	tUINT32 FDCAN_TTTMC;
	tUINT32 FDCAN_TTRMC;
	tUINT32 FDCAN_TTOCF;
	tUINT32 FDCAN_TTMLM;
	tUINT32 FDCAN_TURCF;
	tUINT32 FDCAN_TTOCN;
	tUINT32 FDCAN_TTGTP;
	tUINT32 FDCAN_TTTMK;
	tUINT32 FDCAN_TTIR;
	tUINT32 FDCAN_TTIE;
	tUINT32 FDCAN_TTILS;
	tUINT32 FDCAN_TTOST;
	tUINT32 FDCAN_TURNA;
	tUINT32 FDCAN_TTLGT;
	tUINT32 FDCAN_TTCTC;
	tUINT32 FDCAN_TTCPT;
	tUINT32 FDCAN_TTCSM;
	tUINT32 unused8[112];
	tUINT32 FDCAN_TTTS;
}  tsSTM32CAN_Registers;

/**
 * An array of booleans used to mark the transition to the error active state for
 * both CAN controllers, this is set in the ISR which ensures minimal time in the
 * interrupt routine.
 */
tBOOL abSTM32CAN_TransitionToErrorActive[2];
/**
 * An array of booleans used to mark the transition to the error active passive
 * for both CAN controllers, this is set in the ISR which ensures minimal time in
 * the interrupt routine.
 */
tBOOL abSTM32CAN_TransitionToErrorPassive[2];
/**
 * An array of booleans used to mark the transition to the bus off state for both
 * CAN controllers, this is set in the ISR which ensures minimal time in the
 * interrupt routine.
 */
tBOOL abSTM32CAN_TransitionToBusOff[2];
/**
 * The maximum number of extended filters per CAN controller.
 */
#define STM32CAN_MAX_EXTENDED_FILTERS (64UL)
/**
 * The size of a 29bit extended filter in bytes.
 */
#define STM32CAN_EXTENDED_FILTER_SIZE_BYTES (8UL)
/**
 * The maximum value of an extended filter ID.
 */
#define STM32CAN_EXTENDED_FILTER_ID_MAXIMUM_VALUE (0x1FFFFFFFUL)
/**
 * The maximum number of elements that can be stored in the RX FIFO.
 */
#define STM32CAN_MAX_FRAMES_IN_RX_FIFO (64UL)
/**
 * The size of an RX FIFO element in bytes.
 */
#define STM32CAN_RX_FIFO_ELEMENT_SIZE_BYTES (16UL)
/**
 * The maximum number of elements that can be stored in the TX FIFO.
 */
#define STM32CAN_MAX_FRAMES_IN_TX_FIFO (32UL)
/**
 * The size of a TX FIFO element in bytes.
 */
#define STM32CAN_TX_FIFO_ELEMENT_SIZE_BYTES (16UL)
/**
 * The maximum size of used space in the message RAM, in bytes, per CAN Controller. The RAM is split in
 * half so that each controller gets the same amount.
 */
#define STM32CAN_MESSAGE_RAM_MAX_SIZE_BYTES (5120UL)
/**
 * The start address of the dedicated CAN message RAM.
 */
#define STM32CAN_MESSAGE_RAM_ADDRESS (0x4000AC00UL)
/**
 * A global array of structs containing the Rx and Tx callbacks for each CAN Controller.
 */
tsSTM32CAN_Callbacks atsSTM32CAN_Callbacks[2] = {0};
/**
 * A global array of the last read element of the RX FIFO buffer for each CAN Controller.
 */
tUINT32 aiSTM32CAN_LastElementReadIndex[2] = {0};



/**
 * This initialise routine initialises the specified CAN controller into CAN 2.0B mode. It does not
 * start CAN-TT or CAN-FD features. It allocates memory for the receive FIFO, transmit FIFO and the
 * acceptance filters and leaves the devices in a mode where it is not yet connected to the bus. This
 * allows for the AddFilter routines to configure the acceptance filters ahead of connecting the CAN
 * bus via the Connect routine.
 * @returns SUCCESS unless there is an exception.
 * @throws STM32CAN_BAD_ARGS if any arguments are invalid.
 * @throws STM32CAN_CONNECTED if the CAN node is still connected to the bus.
 * @param tePeripheral: teSTM32CAN_PERIPHERAL: The physical CAN bus to initialise
 * @param pBufferMemory: tUINT32*: A pointer to the buffer memory used by the STM32 CAN controller
 * @param teBaudRate: teSTM32CAN_BAUD_RATE: The Baud rate that CAN bus will be running on.
 * @param iInputClockSpeed: tUINT32: The speed of the clock driving the CAN Hardware. Used to work out
 * the baudrate.
 * @param iNumberOf29bitFilters: tUINT32: The number of 29 bit filters defined in the message memory.
 * @param iNumberOfReceiveFramesInFIFO: tUINT32: The number of received 8 byte frames in the receive
 * FIFO
 * @param iNumberOfTransmitFramesInFIFO: tUINT32: The number of received 8 byte frames in the transmit
 * FIFO
 * @re-entrant: 
 */
teFUNC_STATUS STM32CAN_Initialise(teSTM32CAN_PERIPHERAL tePeripheral, tUINT32* pBufferMemory, teSTM32CAN_BAUD_RATE teBaudRate, tUINT32 iInputClockSpeed, tUINT32 iNumberOf29bitFilters, tUINT32 iNumberOfReceiveFramesInFIFO, tUINT32 iNumberOfTransmitFramesInFIFO)
{
	tsSTM32CAN_Registers* ptsCANRegisters = (tsSTM32CAN_Registers*)tePeripheral;
	tUINT32* pInitialBufferMemory = pBufferMemory;

	// Throw an exception if the requested peripheral isn't valid.
	EH_ASSERT(tePeripheral == CAN_PERIPHERAL_FDCAN1 || tePeripheral == CAN_PERIPHERAL_FDCAN2, STM32CAN_BAD_ARGS, "CAN Init Fail, Invalid CAN peripheral selected.");
	// Throw an exception if the requested number of 29 bit filters exceeds the maximum.
	EH_ASSERT(iNumberOf29bitFilters <= STM32CAN_MAX_EXTENDED_FILTERS, STM32CAN_BAD_ARGS, "CAN Init Fail, requested filters exceeds maximum of 64.");
	// Throw an exception if the requested number of receive frames in the RX FIFO exceeds the maximum.
	EH_ASSERT(iNumberOfReceiveFramesInFIFO <= STM32CAN_MAX_FRAMES_IN_RX_FIFO, STM32CAN_BAD_ARGS, "CAN Init Fail, requested RX FIFO elements exceeds maximum of 64");
	// Throw an exception if the requested number of transmit frames in the TX FIFO exceeds the maximum.
	EH_ASSERT(iNumberOfTransmitFramesInFIFO <= STM32CAN_MAX_FRAMES_IN_TX_FIFO, STM32CAN_BAD_ARGS, "CAN Init Fail, requested TX FIFO elements exceeds maximum of 32.");

	// Work out the size of the TX FIFO section by using the max number of TX FIFO elements requested.
	pBufferMemory += (iNumberOf29bitFilters * STM32CAN_EXTENDED_FILTER_SIZE_BYTES) +
			(iNumberOfReceiveFramesInFIFO * STM32CAN_RX_FIFO_ELEMENT_SIZE_BYTES) +
			(iNumberOfTransmitFramesInFIFO * STM32CAN_TX_FIFO_ELEMENT_SIZE_BYTES);
	// If we've exceeded the max space in message RAM, throw an exception.
	EH_ASSERT((tUINT32)(pBufferMemory - pInitialBufferMemory) <= (tUINT32)STM32CAN_MESSAGE_RAM_MAX_SIZE_BYTES, STM32CAN_BAD_ARGS, "CAN Init Fail, Exceeded Max Space In Message RAM");

	// Throw an exception if the CAN is already connected to the bus. Make a call to STM32CAN_Disconnect() first.
	EH_ASSERT((ptsCANRegisters->FDCAN_CCCR & 0x1UL) == 0x1UL, STM32CAN_CONNECTED, "CAN Init Fail, Still Connected To The Bus.");

	// Set the CCE bit in FDCAN_CCCR register to enable configuration.
	// Setting this bit resets certain registers (refer to page 2488 of the datasheet).
	ptsCANRegisters->FDCAN_CCCR |= 0x2UL;

	// Set the operating mode to normal event-driven.
	ptsCANRegisters->FDCAN_TTOCF &= ~0x3UL;
	// Set the Global Filter Configuration to reject frames that don't match a registered filter.
	ptsCANRegisters->FDCAN_GFC = 0x3FUL;

	// Set the nominal bit timing.
	// input clock is 50MHz
	// prescalar down to 1 tq (/5) time quanta is at 10MHz
	// TSEG1 (7) + TSEG2 (2) + 1 = num Tq in bit time... 1 bit time is at 1Mbps.

	ptsCANRegisters->FDCAN_NBTP = 0;
	ptsCANRegisters->FDCAN_NBTP |= (1 - 1) << 25; // SJW
	ptsCANRegisters->FDCAN_NBTP |= (5 - 1) << 16; // Prescaler
	ptsCANRegisters->FDCAN_NBTP |= (7 - 1) << 8; // TSEG1
	ptsCANRegisters->FDCAN_NBTP |= (2 - 1); // TSEG2

	/* Set up Message RAM
	 * The structure of the Message RAM for this implementation of CAN is like so:
	 * EXTENDED FILTERS
	 * RX FIFO
	 * TX EVENT FIFO
	 */

	// Set the pointer back to the start of the message buffer.
	pBufferMemory = pInitialBufferMemory;

	// Set start address and clear number of extended filters.
	ptsCANRegisters->FDCAN_XIDFC = ((tUINT32)pBufferMemory - STM32CAN_MESSAGE_RAM_ADDRESS) & 0xFFFCUL;
	// Work out the start of the next section (RX FIFO) using the number of filters requested.
	pBufferMemory += (iNumberOf29bitFilters * STM32CAN_EXTENDED_FILTER_SIZE_BYTES);
	// Set start address and number of RX FIFO elements.
	ptsCANRegisters->FDCAN_RXF0C = (((((tUINT32)pBufferMemory - STM32CAN_MESSAGE_RAM_ADDRESS) | (iNumberOfReceiveFramesInFIFO << 16UL))) & 0x7FFFFCUL);
	// Work out the start of the next section (TX FIFO) using the max number of RX FIFO elements requested.
	pBufferMemory += (iNumberOfReceiveFramesInFIFO * STM32CAN_RX_FIFO_ELEMENT_SIZE_BYTES);
	// Set start address and number of TX FIFO elements.
	ptsCANRegisters->FDCAN_TXBC = (((((tUINT32)pBufferMemory - STM32CAN_MESSAGE_RAM_ADDRESS) | (iNumberOfTransmitFramesInFIFO << 24UL))) & 0x3F00FFFCUL);

	// Enable interrupt line 1.
	ptsCANRegisters->FDCAN_ILE = 0x2UL;
	// Attach interrupts for changing error passive and bus off states to interrupt line 1.
	ptsCANRegisters->FDCAN_ILS = 0x2800000UL;
	// Enable interrupts for changing error passive and bus off states.
	ptsCANRegisters->FDCAN_IE = 0x2800000UL;

	// Clear INIT bit to finish initialisation and connect to the bus. This is performed by calling STM32CAN_Connect()

	return BT_SUCCESS;

STM32CAN_BAD_ARGS:
	return BT_FAIL;
STM32CAN_CONNECTED:
	return BT_FAIL;
}

/**
 * This routine queues an amount of messages for transmission by placing them in the TX FIFO. Only
 * data frames can be transmitted (Remote frames are not supported).
 * @throws STM32CAN_TX_FIFO_FULL if there is no more room in the transmit FIFO
 * @returns SUCCESS unless there is an exception.
 * @param tePeripheral: teSTM32CAN_PERIPHERAL: The transmission will go from this physical CAN bus.
 * @param atsCanFrame[]: tsSTM32CAN_Frame: The CAN frames for transmission.
 * @param iNumberOfFrames: tUINT32: The number of CAN frames to transmit.
 * @re-entrant: 
 */
teFUNC_STATUS STM32CAN_Transmit(teSTM32CAN_PERIPHERAL tePeripheral, tsSTM32CAN_Frame atsCanFrame[], tUINT32 iNumberOfFrames)
{
	tsSTM32CAN_Registers* ptsCANRegisters = (tsSTM32CAN_Registers*)tePeripheral;
	tsSTM32CAN_TXFifoElement* ptsNewTxElement = {0};
	tUINT32 iNewElementAddressOffset = 0;
	tUINT32 iNewElementNumber = 0;
	tUINT32 iFrameCounter = 0;
    tUINT32 iDataByteCounter = 0;
    tUINT32 iLowerFourBytes = 0;
    tUINT32 iUpperFourBytes = 0;

	// Throw an exception if the requested peripheral isn't valid.
	EH_ASSERT(tePeripheral == CAN_PERIPHERAL_FDCAN1 || tePeripheral == CAN_PERIPHERAL_FDCAN2, STM32CAN_BAD_ARGS, "CAN Transmit Fail, Invalid CAN peripheral selected.");

	for (iFrameCounter = 0; iFrameCounter < iNumberOfFrames; iFrameCounter++)
	{
		// Throw an exception if the TX FIFO is full.
		EH_ASSERT((ptsCANRegisters->FDCAN_TXFQS & 0x200000UL) != 0x200000UL, STM32CAN_TX_FIFO_FULL, "CAN Transmit Fail, TX FIFO is Full.");

		// Get the start address of the TX FIFO.
		iNewElementAddressOffset = STM32CAN_MESSAGE_RAM_ADDRESS + (ptsCANRegisters->FDCAN_TXBC & 0xFFFCUL);
		// Get the put index of the next available TX FIFO buffer.
		iNewElementNumber = ((ptsCANRegisters->FDCAN_TXFQS >> 16) & 0x1FUL);
		// Work out the address of the next available TX FIFO buffer.
		ptsNewTxElement = (tsSTM32CAN_TXFifoElement*)(iNewElementAddressOffset + (iNewElementNumber * STM32CAN_TX_FIFO_ELEMENT_SIZE_BYTES));
		// Store the new entry in the FIFO buffer.
	    ptsNewTxElement->iESI = 0;
	    ptsNewTxElement->iXTD = 1; // Configure identifier to extended 29 bit format.
	    ptsNewTxElement->iRTR = 0; // Set to transmit a data frame.
	    ptsNewTxElement->iID = atsCanFrame[iFrameCounter].iIdentifier & 0x1FFFFFFFUL; // Set the identifier.
	    ptsNewTxElement->iMM = 0;
	    ptsNewTxElement->iEFC = 0; // Do not store TX events.
	    ptsNewTxElement->iFDF = 0; // Configure for classic CAN format.
	    ptsNewTxElement->iBPS = 0; // No bitrate switching.
	    ptsNewTxElement->iDLC = atsCanFrame[iFrameCounter].iDataLength & 0xFUL; // Set the data length code.


	    // Set the first 4 bytes in the data field.
	    for (iDataByteCounter = 0; iDataByteCounter < atsCanFrame[iFrameCounter].iDataLength || iDataByteCounter < 4; iDataByteCounter++)
	    {
	    	iLowerFourBytes |= ((tUINT32)atsCanFrame[iFrameCounter].aData[iDataByteCounter]) << (8 * iDataByteCounter);
	    }

        *(tUINT32*)ptsNewTxElement->aDataBytes = iLowerFourBytes;

        // Set the last 4 bytes in the data field.
	    for (iDataByteCounter = 4; iDataByteCounter < atsCanFrame[iFrameCounter].iDataLength || iDataByteCounter < 8; iDataByteCounter++)
	    {
	    	iUpperFourBytes |= ((tUINT32)atsCanFrame[iFrameCounter].aData[iDataByteCounter]) << (8 * (iDataByteCounter - 4));
	    }

        *(tUINT32*)(ptsNewTxElement->aDataBytes + 4) = iUpperFourBytes;

		// Request transmission by setting the corresponding bit in TXBAR register.
		ptsCANRegisters->FDCAN_TXBAR |= (0x1UL << iNewElementNumber);
	}

	return BT_SUCCESS;

STM32CAN_BAD_ARGS:
	return BT_FAIL;
STM32CAN_TX_FIFO_FULL:
	return BT_FAIL;
}

/**
 * This routine identifies if there are messages in the receive FIFO and presents these messages to
 * the calling routine.
 * @returns SUCCESS always, no exceptions are applicable.
 * @param tePeripheral: teSTM32CAN_PERIPHERAL: The physical CAN bus to receive data from.
 * @param pStartAddressOfRxFIFO: tUINT32*: A pointer to set to the address of the Rx FIFO and then
 * return to the calling function.
 * @param piIndexOfFirst: tUINT32*: The value of the get index. Which will be the first element to
 * read out. Returned to the calling function.
 * @param piNumberOFElementsToRead: tUINT32*: The value of the fifo fill level. The number of elements
 * from the get index that can be read. Returned to the calling function.
 * @param piMaxNumberOFElementsInFIFO: tUINT32*: The maximum number of elements in the buffer.
 * Returned to the calling function.
 * @re-entrant: True
 */
teFUNC_STATUS STM32CAN_Receive(teSTM32CAN_PERIPHERAL tePeripheral, tUINT32* pStartAddressOfRxFIFO, tUINT32* piIndexOfFirst, tUINT32* piNumberOFElementsToRead, tUINT32* piMaxNumberOFElementsInFIFO)
{
	tsSTM32CAN_Registers* ptsCANRegisters = (tsSTM32CAN_Registers*)tePeripheral;
	tUINT32 iPutIndex = 0;
	tUINT32 iPeripheralIndex = 0;

	// Throw an exception if the requested peripheral isn't valid.
	EH_ASSERT(tePeripheral == CAN_PERIPHERAL_FDCAN1 || tePeripheral == CAN_PERIPHERAL_FDCAN2, STM32CAN_BAD_ARGS, "CAN Init Fail, Invalid CAN peripheral selected.");

	// Set the peripheral index so that arrays can be accessed for the correct CAN peripheral.
	if (tePeripheral == CAN_PERIPHERAL_FDCAN2)
	{
		iPeripheralIndex = 1;
	}
	else
	{
		iPeripheralIndex = 0;
	}

	// Get the value of the get index.
	*piIndexOfFirst = (ptsCANRegisters->FDCAN_RXF0S >> 8) & 0x3FUL;
	// Get the start address of the RX FIFO.
	*pStartAddressOfRxFIFO = STM32CAN_MESSAGE_RAM_ADDRESS + (ptsCANRegisters->FDCAN_RXF0C & 0xFFFCUL);
	// Get the value of the put index.
	iPutIndex = (ptsCANRegisters->FDCAN_RXF0S >> 16) & 0x3FUL;
	// Get the number of elements to read.
	*piNumberOFElementsToRead = ptsCANRegisters->FDCAN_RXF0S & 0x7FUL;
	// Get the max number of elements in the FIFO.
	*piMaxNumberOFElementsInFIFO = (ptsCANRegisters->FDCAN_RXF0C >> 16) & 0x7FUL;

	// Set the index of the last element read.
	if (iPutIndex == 0)
	{
		// Set the last read element to the last element in the FIFO.
		aiSTM32CAN_LastElementReadIndex[iPeripheralIndex] = *piMaxNumberOFElementsInFIFO - 1;
	}
	else
	{
		// Set the last read element to the element just before the put index.
		aiSTM32CAN_LastElementReadIndex[iPeripheralIndex] = iPutIndex - 1;
	}

	return BT_SUCCESS;

STM32CAN_BAD_ARGS:
	return BT_FAIL;
}

/**
 * A function that flushes the RX FIFO by acknowledging a read performed by STM32CAN_Receive().
 * Will set the get index to the next element following the last element read, so that the buffers
 * that have been read can now be overwritten with new incoming frames.
 * @return BT_SUCCESS always.
 * @param tePeripheral: teSTM32CAN_PERIPHERAL: The physical CAN bus to flush.
 * @re-entrant: True
 */
teFUNC_STATUS STM32CAN_Flush(teSTM32CAN_PERIPHERAL tePeripheral)
{
	tsSTM32CAN_Registers* ptsCANRegisters = (tsSTM32CAN_Registers*)tePeripheral;
	tUINT32 iPeripheralIndex = 0;

	// Throw an exception if the requested peripheral isn't valid.
	EH_ASSERT(tePeripheral == CAN_PERIPHERAL_FDCAN1 || tePeripheral == CAN_PERIPHERAL_FDCAN2, STM32CAN_BAD_ARGS, "CAN Init Fail, Invalid CAN peripheral selected.");

	// Set the peripheral index so that the arrays of booleans can be accessed for the correct CAN peripheral.
	if (tePeripheral == CAN_PERIPHERAL_FDCAN2)
	{
		iPeripheralIndex = 1;
	}
	else
	{
		iPeripheralIndex = 0;
	}

	// Acknowledge the read by setting the bit in the acknowledge register representing the last element read.
	ptsCANRegisters->FDCAN_RXF0A = aiSTM32CAN_LastElementReadIndex[iPeripheralIndex];

	return BT_SUCCESS;

STM32CAN_BAD_ARGS:
	return BT_FAIL;
}

/**
 * In the event that the CAN controller observes errors (including Bus Off, Error Active & Error
 * Passive). In the event of an Error Passive/Error Active state entry, this event will simply be
 * logged (no recovery is necessary).
 * In the event of a bus off error, the bus off indication is reported to the calling function so that
 * the calling function can decide what to do to recover.
 * This function reads some module scope variables set from the ISR that signifies the transitions
 * between error modes.
 * @throws STM32CAN_BAD_ARGS if the peripheral selected was invalid.
 * @returns SUCCESS unless there is an exception.
 * @param tePeripheral: teSTM32CAN_PERIPHERAL: The physical CAN bus who's errors need handling.
 * @param *pbIsBusOff: tBOOL: If true, the CAN controller is in bus off.
 * @re-entrant: 
 */
teFUNC_STATUS STM32CAN_HandleErrors(teSTM32CAN_PERIPHERAL tePeripheral, tBOOL *pbIsBusOff)
{
	tUINT32 iPeripheralIndex = 0;

	// Throw an exception if the requested peripheral isn't valid.
	EH_ASSERT(tePeripheral == CAN_PERIPHERAL_FDCAN1 || tePeripheral == CAN_PERIPHERAL_FDCAN2, STM32CAN_BAD_ARGS, "CAN Init Fail, Invalid CAN peripheral selected.");

	// Set the peripheral index so that the arrays of booleans can be accessed for the correct CAN peripheral.
	if (tePeripheral == CAN_PERIPHERAL_FDCAN2)
	{
		iPeripheralIndex = 1;
	}
	else
	{
		iPeripheralIndex = 0;
	}

	// Throw an exception if a transition to error active was detected.
	EH_THROW(abSTM32CAN_TransitionToErrorActive[iPeripheralIndex] == FALSE, "CAN Entered Error Active.");
	// Throw an exception if a transition to error passive was detected.
	EH_THROW(abSTM32CAN_TransitionToErrorPassive[iPeripheralIndex] == FALSE, "CAN Entered Error Passive.");

	// If we have transitioned to Bus Off.
	if (abSTM32CAN_TransitionToBusOff[iPeripheralIndex] == TRUE)
	{
		// Throw an exception if a transition to bus off was detected.
		EH_THROW(FALSE, "CAN Entered Bus Off.");
		*pbIsBusOff = TRUE;
	}

	abSTM32CAN_TransitionToErrorActive[iPeripheralIndex] = FALSE;
	abSTM32CAN_TransitionToErrorPassive[iPeripheralIndex] = FALSE;
	abSTM32CAN_TransitionToBusOff[iPeripheralIndex] = FALSE;

	return BT_SUCCESS;

STM32CAN_BAD_ARGS:
	return BT_FAIL;
}

/**
 * This routine is called upon a transmission or reception event such that the service/application
 * layer code can be scheduled in response to CAN frame receptions. This interrupt will only be
 * enabled if a callback is registered for either transmission or reception.
 * @returns BT_SUCCESS unless there is an exception.
 * @param tePeripheral: teSTM32CAN_PERIPHERAL: The physical CAN bus that has kicked off an interrupt.
 * @re-entrant: 
 */
teFUNC_STATUS STM32CAN_ISRTransmissionComplete(teSTM32CAN_PERIPHERAL tePeripheral)
{
	tsSTM32CAN_Registers* ptsCANRegisters = (tsSTM32CAN_Registers*)tePeripheral;
	tUINT32 iPeripheralIndex = 0;

	// Throw an exception if the requested peripheral isn't valid.
	EH_ASSERT(tePeripheral == CAN_PERIPHERAL_FDCAN1 || tePeripheral == CAN_PERIPHERAL_FDCAN2, STM32CAN_BAD_ARGS, "CAN Init Fail, Invalid CAN peripheral selected.");

	// Set the peripheral index so that the arrays of booleans can be accessed for the correct CAN peripheral.
	if (tePeripheral == CAN_PERIPHERAL_FDCAN2)
	{
		iPeripheralIndex = 1;
	}
	else
	{
		iPeripheralIndex = 0;
	}

	// If Transmission Completed.
	if ((ptsCANRegisters->FDCAN_IR & 0x200UL) == 0x200UL)
	{
		// Call the transmit complete callback for CAN1.
		atsSTM32CAN_Callbacks[iPeripheralIndex].pTxCallback();
		// Clear the interrupt.
		ptsCANRegisters->FDCAN_IR |= 0x200UL;
	}
	// If New Message in RX FIFO.
	if ((ptsCANRegisters->FDCAN_IR & 0x1UL) == 0x1UL)
	{
		// Call the reception complete callback for CAN1.
		atsSTM32CAN_Callbacks[iPeripheralIndex].pRxCallback();
		// Clear the interrupt.
		ptsCANRegisters->FDCAN_IR |= 0x1UL;
	}

	return BT_SUCCESS;

STM32CAN_BAD_ARGS:
	return BT_FAIL;
}

/**
 * This routine is called upon CAN errors transitions to and from Error Passive, Error Active and Bus
 * Off.
 * This function sets module scope variables that signify the transition between the CAN error states.
 * 
 * @returns BT_SUCCESS unless there is an exception.
 * @param tePeripheral: teSTM32CAN_PERIPHERAL: The physical CAN bus that has kicked off an interrupt.
 * @re-entrant: 
 */
teFUNC_STATUS STM32CAN_ISRErrorTransition(teSTM32CAN_PERIPHERAL tePeripheral)
{
	tsSTM32CAN_Registers* ptsCANRegisters = (tsSTM32CAN_Registers*)tePeripheral;
	tUINT32 iPeripheralIndex = 0;

	// Throw an exception if the requested peripheral isn't valid.
	EH_ASSERT(tePeripheral == CAN_PERIPHERAL_FDCAN1 || tePeripheral == CAN_PERIPHERAL_FDCAN2, STM32CAN_BAD_ARGS, "CAN Init Fail, Invalid CAN peripheral selected.");

	// Set the peripheral index so that the arrays of booleans can be accessed for the correct CAN peripheral.
	if (tePeripheral == CAN_PERIPHERAL_FDCAN2)
	{
		iPeripheralIndex = 1;
	}
	else
	{
		iPeripheralIndex = 0;
	}

	// Check for which state change has triggered the interrupt.
	if (((ptsCANRegisters->FDCAN_IR & 0x2000000UL) == 0x2000000UL))
	{
		// If state is Bus Off.
		if ((ptsCANRegisters->FDCAN_PSR & 0x80UL) == 0x80UL)
		{
			abSTM32CAN_TransitionToBusOff[iPeripheralIndex] = TRUE;
		}
		// Must have transitioned to Error Active from Initialisation.
		else
		{
			abSTM32CAN_TransitionToErrorActive[iPeripheralIndex] = TRUE;
		}
		// Clear interrupt flags.
		ptsCANRegisters->FDCAN_IR |= 0x2000000UL;
	}
	if (((ptsCANRegisters->FDCAN_IR & 0x800000UL) == 0x800000UL))
	{
		// If state is Error Passive.
		if ((ptsCANRegisters->FDCAN_PSR & 0x20UL) == 0x20UL)
		{
			abSTM32CAN_TransitionToErrorPassive[iPeripheralIndex] = TRUE;
		}
		// Must have transitioned to Error Active.
		else
		{
			abSTM32CAN_TransitionToErrorActive[iPeripheralIndex] = TRUE;
		}
		// Clear interrupt flags.
		ptsCANRegisters->FDCAN_IR |= 0x800000UL;
	}

	return BT_SUCCESS;

STM32CAN_BAD_ARGS:
	return BT_FAIL;
}

/**
 * The STM32 CAN controller can be configured to filter on a number of ranges of CAN IDs (29 bit),
 * this routine will add to the list of filters with a min/max range.
 * @throws STM32CAN_NO_MORE_AVAILABLE_FILTER_SLOTS if there are no more available filter slots for the
 * device.
 * @throws STM32CAN_BAD_ARGS if the filter values are bigger than a 29 bit value, if max is less than
 * min or if requested peripheral is not valid.
 * @returns SUCCESS unless there is an exception.
 * @param tePeripheral: teSTM32CAN_PERIPHERAL: The physical CAN bus to associated this filter to.
 * @param iFilterMin: tUINT32: The minimum filter ID value (inclusive)
 * @param iFilterMax: tUINT32: The maximum filter ID value (inclusive)
 * @re-entrant: 
 */
teFUNC_STATUS STM32CAN_AddFilterRange(teSTM32CAN_PERIPHERAL tePeripheral, tUINT32 iFilterMin, tUINT32 iFilterMax)
{
	tsSTM32CAN_Registers* ptsCANRegisters = (tsSTM32CAN_Registers*)tePeripheral;
	tUINT32 iMaxNumberOfFiltersConfigured = 0;
	tUINT32 iNumberOfFilters = 0;
	tUINT32 iExtendedFiltersStartAddress = 0;
	tsSTM32CAN_ExtendedIDFilterElement* pNewFilterEntryAddress = 0;

	// Throw an exception if the requested peripheral isn't valid.
	EH_ASSERT(tePeripheral == CAN_PERIPHERAL_FDCAN1 || tePeripheral == CAN_PERIPHERAL_FDCAN2, STM32CAN_BAD_ARGS, "CAN Add Filter Accept Mask Fail, Invalid CAN peripheral selected.");
	// Throw an exception if the range filter min is bigger than a 29 bit value.
	EH_ASSERT(iFilterMin <= STM32CAN_EXTENDED_FILTER_ID_MAXIMUM_VALUE , STM32CAN_BAD_ARGS, "CAN Add Range Filter Fail, Requested Min Value is Larger than a 29 bit value.");
	// Throw an exception if the range filter max value is bigger than a 29 bit value.
	EH_ASSERT(iFilterMax <= STM32CAN_EXTENDED_FILTER_ID_MAXIMUM_VALUE, STM32CAN_BAD_ARGS, "CAN Add Range Filter Fail, Requested Max Value is Larger than a 29 bit value.");
	// Throw an exception if the range filter max value is smaller than the min value.
	EH_ASSERT(iFilterMin <= iFilterMax, STM32CAN_BAD_ARGS, "CAN Add Range Filter Fail, Requested Max Value is Smaller than the Min value.");

	// Work out the maximum filters configured at initialisation.
		iMaxNumberOfFiltersConfigured = (ptsCANRegisters->FDCAN_RXF0C & 0xFFFCUL) / STM32CAN_EXTENDED_FILTER_SIZE_BYTES;
	// Find the number of current filters already configured.
	iNumberOfFilters = ptsCANRegisters->FDCAN_XIDFC >> 16UL;
	// Throw an exception if there are no more available filter slots.
	EH_ASSERT(iNumberOfFilters < iMaxNumberOfFiltersConfigured && iNumberOfFilters <= STM32CAN_MAX_EXTENDED_FILTERS, STM32CAN_NO_MORE_AVAILABLE_FILTER_SLOTS, "CAN Add Filter Accept Mask Fail, No More Filter Slots");
	// Find the start address of the extended filters section
	iExtendedFiltersStartAddress = (ptsCANRegisters->FDCAN_XIDFC & 0xFFFCUL);
	// Find the address of the new filter entry.
	pNewFilterEntryAddress = (tsSTM32CAN_ExtendedIDFilterElement*)(STM32CAN_MESSAGE_RAM_ADDRESS + iExtendedFiltersStartAddress + (iNumberOfFilters * STM32CAN_EXTENDED_FILTER_SIZE_BYTES));

	// Add the new filter into the message RAM.
	pNewFilterEntryAddress->iEFEC = 1UL; // Configure filter to store message in FIFO if matches.
	pNewFilterEntryAddress->iEFID1 = iFilterMin & 0x1FFFFFFFUL; // Configure the range filter minimum ID.
	pNewFilterEntryAddress->iEFTI = 2UL; // Configure filter as classic acceptance mask.
	pNewFilterEntryAddress->iEFID2 = iFilterMax & 0x1FFFFFFFUL; // Configure the range filter maximum ID.

	// Increment the number of filters by 1 and add to the XIDFC register.
	iNumberOfFilters++;
	ptsCANRegisters->FDCAN_XIDFC |= ((iNumberOfFilters << 16UL) & 0xFFFFFCUL);

	return BT_SUCCESS;

STM32CAN_BAD_ARGS:
	return BT_FAIL;
STM32CAN_NO_MORE_AVAILABLE_FILTER_SLOTS:
	return BT_FAIL;
}

/**
 * The STM32 can be configured to filter incoming CAN frames by ID through a "don't care" mask and an
 * acceptance filter (29 bit). This routine adds an acceptance mask/filter.
 * @throws STM32CAN_NO_MORE_AVAILABLE_FILTER_SLOTS if there are no more available filter slots for the
 * device.
 * @throws STM32CAN_BAD_ARGS if the filter values are bigger than a 29 bit value or if requested
 * peripheral is not valid.
 * @returns SUCCESS unless there is an exception.
 * @param tePeripheral: teSTM32CAN_PERIPHERAL: The physical CAN bus to associate the filter to.
 * @param iAcceptanceFilter: tUINT32: The acceptance filter value
 * @param iAcceptanceMask: tUINT32: The mask defining which bits will be ignored during the
 * application of the acceptance filter.
 * @re-entrant: 
 */
teFUNC_STATUS STM32CAN_AddFilterAcceptMask(teSTM32CAN_PERIPHERAL tePeripheral, tUINT32 iAcceptanceFilter, tUINT32 iAcceptanceMask)
{
	tsSTM32CAN_Registers* ptsCANRegisters = (tsSTM32CAN_Registers*)tePeripheral;
	tUINT32 iMaxNumberOfFiltersConfigured = 0;
	tUINT32 iNumberOfFilters = 0;
	tUINT32 iExtendedFiltersStartAddress = 0;
	tsSTM32CAN_ExtendedIDFilterElement* pNewFilterEntryAddress = 0;

	// Throw an exception if the requested peripheral isn't valid.
	EH_ASSERT(tePeripheral == CAN_PERIPHERAL_FDCAN1 || tePeripheral == CAN_PERIPHERAL_FDCAN2, STM32CAN_BAD_ARGS, "CAN Add Filter Accept Mask Fail, Invalid CAN peripheral selected.");
	// Throw an exception if the acceptance filter value is bigger than a 29 bit value.
	EH_ASSERT(iAcceptanceFilter <= STM32CAN_EXTENDED_FILTER_ID_MAXIMUM_VALUE, STM32CAN_BAD_ARGS, "CAN Add Accept Mask Filter Fail, Requested Filter Value is Larger than a 29 bit value.");
	// Throw an exception if the acceptance mask value is bigger than a 29 bit value.
	EH_ASSERT(iAcceptanceMask <= STM32CAN_EXTENDED_FILTER_ID_MAXIMUM_VALUE, STM32CAN_BAD_ARGS, "CAN Add Accept Mask Filter Fail, Requested Mask Value is Larger than a 29 bit value.");

	// Work out the maximum filters configured at initialisation.
	iMaxNumberOfFiltersConfigured = (ptsCANRegisters->FDCAN_RXF0C & 0xFFFCUL) / STM32CAN_EXTENDED_FILTER_SIZE_BYTES;
	// Find the number of current filters already configured.
	iNumberOfFilters = ptsCANRegisters->FDCAN_XIDFC >> 16UL;
	// Throw an exception if there are no more available filter slots.
	EH_ASSERT(iNumberOfFilters < iMaxNumberOfFiltersConfigured && iNumberOfFilters <= STM32CAN_MAX_EXTENDED_FILTERS, STM32CAN_NO_MORE_AVAILABLE_FILTER_SLOTS, "CAN Add Filter Accept Mask Fail, No More Filter Slots");
	// Find the start address of the extended filters section
	iExtendedFiltersStartAddress = (ptsCANRegisters->FDCAN_XIDFC & 0xFFFCUL);
	// Find the address of the new filter entry.
	pNewFilterEntryAddress = (tsSTM32CAN_ExtendedIDFilterElement*)(STM32CAN_MESSAGE_RAM_ADDRESS + iExtendedFiltersStartAddress + (iNumberOfFilters * STM32CAN_EXTENDED_FILTER_SIZE_BYTES));

	// Add the new filter into the message RAM.
	pNewFilterEntryAddress->iEFEC = 1UL; // Configure filter to store message in FIFO if matches.
	pNewFilterEntryAddress->iEFID1 = iAcceptanceFilter & 0x1FFFFFFFUL; // Configure the acceptance filter.
	pNewFilterEntryAddress->iEFTI = 2UL; // Configure filter as classic acceptance mask.
	pNewFilterEntryAddress->iEFID2 = iAcceptanceMask & 0x1FFFFFFFUL; // Configure the acceptance mask.

	// Increment the number of filters by 1 and add to the XIDFC register.
	iNumberOfFilters++;
	ptsCANRegisters->FDCAN_XIDFC |= ((iNumberOfFilters << 16UL) & 0xFFFFFCUL);

	return BT_SUCCESS;

STM32CAN_BAD_ARGS:
	return BT_FAIL;
STM32CAN_NO_MORE_AVAILABLE_FILTER_SLOTS:
	return BT_FAIL;
}

/**
 * Following the call to initialise and the call to add filters for frame reception, this call
 * connects the CAN controller to the CAN bus through the CAN transceiver.
 * @returns SUCCESS unless there is an exception.
 * @throws STM32CAN_BAD_ARGS if any arguments are invalid.
 * @param tePeripheral: teSTM32CAN_PERIPHERAL: The physical CAN bus to connect.
 * @re-entrant: 
 */
teFUNC_STATUS STM32CAN_Connect(teSTM32CAN_PERIPHERAL tePeripheral)
{
	// Throw an exception if the requested peripheral isn't valid.
	EH_ASSERT(tePeripheral == CAN_PERIPHERAL_FDCAN1 || tePeripheral == CAN_PERIPHERAL_FDCAN2, STM32CAN_BAD_ARGS, "CAN Init Fail, Invalid CAN peripheral selected.");

	tsSTM32CAN_Registers* ptsCANRegisters = (tsSTM32CAN_Registers*)tePeripheral;

	// Throw an exception if the CAN is already connected to the bus.
	EH_ASSERT((ptsCANRegisters->FDCAN_CCCR & 0x1UL) == 0x1UL, STM32CAN_CONNECTED, "CAN Connect Fail, Already Connected To The Bus.");

	// Connect to the bus by clearing the INIT bit in FDCAN_CCCR register.
	ptsCANRegisters->FDCAN_CCCR &= ~0x1UL;
	// The node will now be live on the bus. CCE bit is automatically cleared if set.

	return BT_SUCCESS;

STM32CAN_BAD_ARGS:
	return BT_FAIL;
STM32CAN_CONNECTED:
	return BT_FAIL;
}

/**
 * This function registers callbacks that will be triggered if a message is either received or
 * transmitted.
 * @param tePeripheral: teSTM32CAN_PERIPHERAL: The physical CAN bus to connect the callbacks to.
 * @param pReceiveCallback(): tFUNC_PTR: A pointer to the function that is called in the event of a
 * frame reception.
 * @param pTransmitCallback(): tFUNC_PTR: A pointer to the function that is called in the event of a
 * frame transmission.
 * @re-entrant: 
 */
teFUNC_STATUS STM32CAN_RegisterCallbacks(teSTM32CAN_PERIPHERAL tePeripheral, tFUNC_PTR pReceiveCallback, tFUNC_PTR pTransmitCallback)
{
	tsSTM32CAN_Registers* ptsCANRegisters = (tsSTM32CAN_Registers*)tePeripheral;
	tUINT32 iPeripheralIndex = 0;

	// Throw an exception if the requested peripheral isn't valid.
	EH_ASSERT(tePeripheral == CAN_PERIPHERAL_FDCAN1 || tePeripheral == CAN_PERIPHERAL_FDCAN2, STM32CAN_BAD_ARGS, "CAN Init Fail, Invalid CAN peripheral selected.");

	// Set the peripheral index so that the arrays of booleans can be accessed for the correct CAN peripheral.
	if (tePeripheral == CAN_PERIPHERAL_FDCAN2)
	{
		iPeripheralIndex = 1;
	}
	else
	{
		iPeripheralIndex = 0;
	}

	// If a receive callback is defined.
	if(pReceiveCallback)
	{
		// Register the receive callback for FDCAN1.
		atsSTM32CAN_Callbacks[iPeripheralIndex].pRxCallback = pReceiveCallback;
		// Enable the RX FIFO new message interrupt.
		ptsCANRegisters->FDCAN_IE |= 0x1UL;
		// Attach the interrupt to interrupt line 0.
		ptsCANRegisters->FDCAN_ILS &= ~0x1UL;
		// Enable interrupt line 0.
		ptsCANRegisters->FDCAN_ILE |= 0x1UL;
	}
	// If a transmit callback is defined.
	if(pTransmitCallback)
	{
		// Register the transmit callback for FDCAN1.
		atsSTM32CAN_Callbacks[iPeripheralIndex].pTxCallback = pTransmitCallback;
		// Enable the transmission complete interrupt.
		ptsCANRegisters->FDCAN_IE |= 0x200UL;
		// Attach the interrupt to interrupt line 0.
		ptsCANRegisters->FDCAN_ILS &= ~0x200UL;
		// Enable interrupt line 0.
		ptsCANRegisters->FDCAN_ILE |= 0x1UL;
	}

	return BT_SUCCESS;

STM32CAN_BAD_ARGS:
	return BT_FAIL;
}

/**
 * This function stops transmission on the selected CAN peripheral. This is needed in the case where
 * the CAN transmission goes on for a pre-longed period of time. Under CAN 2.0b protocol, the
 * transmitter will transmit the same frame repeatedly until it receives an acknowledgement in the ACK
 * bit slot from the receiver.
 * In the case that the transmitter ends up transmitting data at a different baud rate, this can
 * prevent further transmissions on the CAN bus.
 * 
 * @returns SUCCESS always, no exceptions need to be handled. 
 * @param tePeripheral: teSTM32CAN_PERIPHERAL: The physical CAN bus for the transmission is to be
 * halted.
 * @re-entrant: 
 */
teFUNC_STATUS STM32CAN_CancelTransmission(teSTM32CAN_PERIPHERAL tePeripheral)
{
	tsSTM32CAN_Registers* ptsCANRegisters = (tsSTM32CAN_Registers*)tePeripheral;
	tUINT32 iCancelElementNumber = 0;

	// Throw an exception if the requested peripheral isn't valid.
	EH_ASSERT(tePeripheral == CAN_PERIPHERAL_FDCAN1 || tePeripheral == CAN_PERIPHERAL_FDCAN2, STM32CAN_BAD_ARGS, "CAN Init Fail, Invalid CAN peripheral selected.");

	// Get the current get index of the TX FIFO.
	iCancelElementNumber = ((ptsCANRegisters->FDCAN_TXFQS >> 8) & 0x1FUL);

	// Cancel the current TX FIFO buffer pointed to by the get index.
	ptsCANRegisters->FDCAN_TXBCR |= 0x1UL << iCancelElementNumber;

	return BT_SUCCESS;

STM32CAN_BAD_ARGS:
	return BT_FAIL;
}

/**
 * This call disconnects the CAN controller from the CAN bus. This puts the CAN controller in
 * initialisation mode, configure the CAN by calling the initialise function.
 * @returns SUCCESS unless there is an exception.
 * @throws STM32CAN_BAD_ARGS if any arguments are invalid.
 * @param tePeripheral: teSTM32CAN_PERIPHERAL: The physical CAN bus to disconnect.
 * @re-entrant: 
 */
teFUNC_STATUS STM32CAN_Disconnect(teSTM32CAN_PERIPHERAL tePeripheral)
{
	// Throw an exception if the requested peripheral isn't valid.
	EH_ASSERT(tePeripheral == CAN_PERIPHERAL_FDCAN1 || tePeripheral == CAN_PERIPHERAL_FDCAN2, STM32CAN_BAD_ARGS, "CAN Init Fail, Invalid CAN peripheral selected.");

	tsSTM32CAN_Registers* ptsCANRegisters = (tsSTM32CAN_Registers*)tePeripheral;

	// Throw an exception if the CAN is already disconnected from the bus.
	EH_ASSERT((ptsCANRegisters->FDCAN_CCCR & 0x1UL) == 0x0UL, STM32CAN_DISCONNECTED, "CAN Disconnect Fail, Already Disconnected.");

	// Connect to the bus by setting the INIT bit in FDCAN_CCCR register.
	ptsCANRegisters->FDCAN_CCCR |= 0x1UL;
	// The node will now be disconnected from the bus.

	return BT_SUCCESS;

STM32CAN_BAD_ARGS:
	return BT_FAIL;
STM32CAN_DISCONNECTED:
	return BT_FAIL;
}

void EH_Exception(char* sFile, char* sLine, char* sErrorString)
{
	HAL_UART_Transmit(&huart3, sErrorString, 120, 1000);
}
