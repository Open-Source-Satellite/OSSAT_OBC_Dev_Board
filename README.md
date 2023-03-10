![](gui_assets/OSSAT-LOGO-BLUE.png)
# Open Source Satellite Development On Board Computer

## Contents
- [Background](#background)
- [Introduction to the OBC Development Board](#Introduction-to-the-OBC-Development-Board)
- [OBC Development Board Manual](#OBC-Development-Board-Manual)
- [Running the OSSAT OBC Application](#running-the-ossat-obc-application)

## Background
The key drivers for the Open Source Satellite (OSSAT) are to improve the affordability, accessibility and
sustainability of space; to address the market need for an improvement in the
price:performance point of small satellites; to stimulate the next wave of disruption
in the small satellite industry; and to enable the development of new missions,
applications and services.
<p style="text-align: center;">
<img src="https://github.com/Open-Source-Satellite/OSSAT_OBC_Dev_Board/blob/main/gui_assets/OSSAT_History.jpg?raw=true" alt="OSSAT History"/ width="300">
</p>
Small satellites initiated the initial market
disruption by leveraging COTS parts and
processes, but it appears that the unit cost
per spacecraft cannot be reduced much
further.  
CubeSats have been critical in recent years for
lowering the barriers to entry, however they
appear to be unable to provide sufficient
robustness and mission enabling capability to
support more demanding missions.  
The goal is to develop OSSAT to fill this gap and deliver microsatellite-class
performance for a CubeSat price, even when manufacturing in low volumes. The
aim is to increase the level of integration and robustness over subsequent missions
in order to develop a single platform that is affordable for use in vLEO (very Low
Earth Orbit), LEO (Low Earth Orbit), GEO (Geosynchronous Equatorial Orbit), Lunar and
interplanetary missions.  
OSSAT will be a performant and flexible
system that KISPE Space will be able to use to implement some of its own
commercial missions; however, simply using it for KISPE Space missions will not
result in the desired increase in accessibility to the design, nor stimulate the
market.  
Making the OSSAT design open source will maximise access and achieve the wider aims of the
Programme.  

## Introduction to the OBC Development Board
### Overview
The OSSAT OBC Development board provides a convenient hardware platform that collaborators can use, to develop satellite firmware.
The board is based upon the STM32 microcontroller and includes various interfaces for connection to other satellite sub-systems. 
The board includes an STLink programmer/debugger interface to facilitate firmware development and debugging activities.
The design shares several attributes with a sister board that is being developed for a space application.
Figure 1 shows a view of the board.

![image](https://user-images.githubusercontent.com/111048316/216616583-057bedc1-52d7-4567-b377-4d88b519655f.png)   
Figure 1:  View of the Board
<br/>
<br/>
### Features
- STM32 Microcontroller (STM32H753ZIT)
- SRAM 4Mx16bit (8Mx8bit) CMOS Static RAM
- STLink Programming and Debug interface (with UART)
- 2x CAN Bus channels (with local 120R termination), with D-Sub (9 way sockets)
- 2x UART interface
- 8x Thermistor interface (10K Negative Temperature Coefficient)
- 31x General purpose I/O lines
- Local 3V3 regulator, operating from USB supply, via Micro-USB connector
- MCU Reset Button
- 2x push-buttons for test use
- 3x status LEDs for test use
- 2x power status LEDs
- Real-time clock

### Technical Details
#### Board Overview
Figure 2 below shows a view of the fully assembled board. There are no components on the reverse side.

![image](https://user-images.githubusercontent.com/111048316/216617689-4c41ee5b-1571-40e6-a27d-4a0dc18d36d7.png)   
Figure 2:  View of Assembled Board
<br/>   
<br/> 
The block diagram for the Schematic is shown in Figure 3.

![image](https://user-images.githubusercontent.com/111048316/216617843-7995490b-6f9a-41d4-85e5-43f7f01a4bd4.png)   
Figure 3:  Schematic Block Diagram
<br/>   
<br/> 
#### STM32 Microcontroller (STM32H753ZIT)
The STM32 MCU includes a broad set of internal features, some of which are highlighted below:
- 32-bit Arm??-M7 core
- 2Mbyte onboard Flash
- 1Mbyte RAM
- Flexible Memory controller
- Power management
- Watchdog
- Multiple communication peripherals, including CAN, I2C, SPI, UART
- ADC
- Timers and RTC
- Large number of I/O ports
- STLink / JTAG compatibility

Full details of the MCU are available from the manufacturers data sheet and reference manual, available from the STM web site. Refer to:
- DS12117: STM32H753xl Datasheet (32-bit Arm?? Cortex??-M7 480MHz MCUs, 2MB Flash, 1MB RAM, 46 com. and analogue interfaces, crypto)
- RM0433: Reference manual, STM32H742, STM32H743/753 and STM32H750 Value line advanced Arm??-based 32-bit MCUs

### Related Documents
A list of documents related to this board is provided in the Manual and is listed here. This includes diagrams, design source files, manufacturing and & test information and test firmware.
- [KS-SCH-01466 (.PDF)](https://github.com/Open-Source-Satellite/OSSAT_OBC_Dev_Board/blob/main/OSSAT%20OBC%20Dev%20Board%20Drawings/KS-SCH-01466-01%20OSSAT%20OBC%20Dev%20Board%20Schematic.pdf) OSSAT OBC Development Board Schematic (Schematic diagram)
- [KS-SCH-01466 (.sch)](https://github.com/Open-Source-Satellite/OSSAT_OBC_Dev_Board/blob/main/OSSAT%20OBC%20Dev%20Board%20Drawings/CAD%20Source%20Files/KS-SCH-01466-01%20OSSAT%20OBC%20Dev%20Board%20Schematic.sch) OSSAT OBC Development Board Schematic (Fusion 360 schematic source file, in ???Eagle??? format)
- [KS-SCH-01466 (.lbr)](https://github.com/Open-Source-Satellite/OSSAT_OBC_Dev_Board/blob/main/OSSAT%20OBC%20Dev%20Board%20Drawings/CAD%20Source%20Files/KS-SCH-01466-01%20OSSAT_OBC_Dev_Board_Library.lbr) OSSAT OBC Dev board library (Fusion 360 Library file in ???Eagle??? format, for parts used in the design)
- [KS-SCH-01466 (.xlsx)](https://github.com/Open-Source-Satellite/OSSAT_OBC_Dev_Board/blob/main/OSSAT%20OBC%20Dev%20Board%20Drawings/KS-SCH-01466-01%20OSSAT%20OBC%20Dev%20Brd%20BOM%20with%20Mfr%20Data.xlsx) OSSAT OBC Dev Board BOM with manufacturer data (Bill of materials for the board assembly)
- [KS-BRD-01467 (.brd)](https://github.com/Open-Source-Satellite/OSSAT_OBC_Dev_Board/blob/main/OSSAT%20OBC%20Dev%20Board%20Drawings/CAD%20Source%20Files/KS-SCH-01467%20OSSAT%20OBC%20Dev%20Board%20PCB.brd) OSSAT OBC Development Board PCB (Fusion 360 PCB source file in ???Eagle??? format)
- [KS-BRD-01467 (.zip)](https://github.com/Open-Source-Satellite/OSSAT_OBC_Dev_Board/blob/main/OSSAT%20OBC%20Dev%20Board%20Drawings/Gerber%20Files/KS-BRD-01467-02%20OSSAT%20Sw%20Dev%20Board%20PCB_2022-12-08.zip) OSSAT OBC Dev Board PCB Manufacturing files	(Gerber file set and PnP file)
- [KS-BRD-01467 (.docx)](https://github.com/Open-Source-Satellite/OSSAT_OBC_Dev_Board/blob/main/OSSAT%20OBC%20Dev%20Board%20Drawings/KS-BRD-01467-02%20OSSAT%20OBC%20Dev%20board%20PCB%20Layer%20Stackup.docx) OSSAT OBC Dev Board Layer Stack-up (Defines the layer stack and pcb materials / thicknesses for the board)
- [KS-DOC-01526 (.docx)](https://github.com/Open-Source-Satellite/OSSAT_OBC_Dev_Board/blob/main/OSSAT%20OBC%20Dev%20Board%20Drawings/KS-DOC-01526-02%20OSSAT%20OBC%20Dev%20Board%20Verification%20Procedure.docx) OSSAT OBC Dev Board Verification Procedure	(Used to confirm board function after build)
- [KS-DOC-01529 (.docx)](https://github.com/Open-Source-Satellite/OSSAT_OBC_Dev_Board/blob/main/OSSAT%20OBC%20Dev%20Board%20Drawings/KS-DOC-01529-01%20OSSAT%20OBC%20Dev%20Board%20Build%20Document.docx) OSSAT OBC Dev Board Build Document (Build process control document)
- [KS-SOF-01541 (software)](https://github.com/Open-Source-Satellite/OSSAT_OBC_Dev_Board/releases/tag/KS-SOF-01541-01) Software to support h/w test of the OSSAT OBC dev board	(Test Software)

## OBC Development Board Manual
Document [KS-DOC-01543 is the manual](https://github.com/Open-Source-Satellite/OSSAT_OBC_Dev_Board/blob/main/KS-DOC%2001543-01%20OSSAT%20OBC%20Dev%20Board%20Manual.docx) for the board. It covers:
- Board features
- Technical details
- Building the board
- Testing the board
- Using the board

## Running the OSSAT OBC Application
Within one year of the first launch of the OSSAT, the OSSAT OBC Application software will become open source. In the interim time, in order to develop/build and run the OSSAT OBC Application, you'll need to become an OSSAT collaborator ([Click Here](https://www.opensourcesatellite.org/register/)).
If you are an OSSAT collaborator, that's great... Click [here](/OBC_CODE_README.md) to find out how to run the OSSAT OBC Application on this development board.

## Licenses
All information in this repository is licensed Open Source through the Creative Commons license:

![](gui_assets/CC-BY-SA.jpg)

## Get Involved
If you wish to contribute directly to the development of OSSAT, have a look at the existing Github issues and register [here](https://www.opensourcesatellite.org/register/).
