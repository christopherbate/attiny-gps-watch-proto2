# attiny-gps-watch
Schematics, code, and notes for a GPS watch based on ATMEL AVR ATTiny85 Micro-controller 

# Status - WIP
Finalizing code for upload

Current features:
- Software Serial interface UART (used for RX of GPS signal)
- USI and Timer used for I2C interface 
- SSD1306 Driver 
- Software serial output for debugging purposes
- Simple finite-state machine for procesing NMEA messages
- Basic clock utilizing timer to keep track of time to millisecond

ToDo:
- Finish NMEA processing for lat/long/direction information 
- Code profiling 
- Syncing watch clock with GPS time 

# About this project
Nowadays, we are spoiled with the availability of fast processors with ample memory, storage, and peripheral features. The purpose of this project is to design a workable hand-portable GPS solution using a minimal microcontroller that consumes a very low amount of power. Like many people, my first experience with microcontrollers was with the very friendly Arduino platform. I can't remember when, but I eventually stumbled upon the ATTiny series, which are propular amongst hobbyists for their low price and relative simplicity. The ATTiny85 comes in a 8-pin DIP package, and 3 pins are taken up by VCC, GND, and RESET, 2 for external clock source, leaving 3 pins to control and receive signals. I like this device because it challenges you to implement your requirements using a bare minimal amount of features present on a microcontroller today. Most basic projects don't necessarily need the power of chips present in most Arduinos, let alone an ARM processor. There aren't even any dedicated UART or SPI peripherals on the ATTiny85, only a "Universal Serial Interface" or USI. A lot of thinking for beginning microcontorller programmers and software workarounds will be necessary to interface with multiple sensors and outputs. Most projects one sees online with the ATTiny85 use it for one purpose (i.e. one sensor or driving one output, with no external clock.)

The entire project is compiled in C++ (but uses very few C++ features in order to maintain simplicity) with some in-line assembly. ATMEL's Windows based ATMEL Studio was used as the IDE. Programming was performed via Sparkfun's basic ATTiny USB Programmer (search the web for tutorials on how to set it up with ATMEL Studio). Because we are using such a basic programmer, debugging was done via LED flashes in the beginning and then serial output through an Arduino once a basic software serial was coded. 

This project was a learning process for me to help test my skills in lower level programming in C while working with hardware and integrating a complete project. I have plenty of experience programming in C/C++, but mostly in the WIN32 environment. 

I've provided documentation below in a step-by-step manner below to serve as a basic tutorial. 

# Designing the System - Getting familiar with the ATTiny85 and creating a project binder

The first thing I did when I began to work with the ATTiny85 in earnest was to create a binder where I would put all the necessary documentation to help visualize the information flow. Then read through the chapters in the documentation so that you begin to understand the AVR core and the memories available. You don't need to print out the entire PDF of the datasheet, but I would print out the following right away:

1) The pinout sheet, so you don't have to keep opening the PDF when you forget which pin is which
2) The entire sections on whichever peripherals you will be using (in this case, Counter0, Counter1, and USI)
3) The sections detailing how to set the system clock frequency and what the fuze settings should be 
4) The section detailing the interrupt vectors' names 

As you skim through the documentation (its not necessary to read the entire thing, just get the gist of the capabilities of the processor), you should ask yourself the following questions:
1) What kinds of memory (SRAM, EEPROM, fFash, etc) are available to me, and what are their limitations / sizes?
2) What peripherals are available and what are their limitations?

# Designing the System - What components are necessary and how will they interact

We are designing a GPS watch, so we will need the following items at minium -
1- A microcontroller
2- A display meant for low power, portable applications. Low res, non-color will do. 
3- A GPS module, ideally with low peak current draw 
4- A battery

As we read the documentation for the above items, we will need to ask ourselves similar questions as we asked for the microcontroller. The basic idea is "how is information getting from point A to point B". For the display, I selected an OLED SSD1306 display, because these are commonly available (Ebay, etc), cheap displays that come with a controller already bundled with the display. The SSD1306 is that controller, and consists of the integrated circuit (IC) actually reading data from a RAM buffer and manipulating the screen's electronic inputs. We allow the SSD1306 and our microcontroller(MCCU) to communicate by attaching wires between the two and then using an agreed upon protocol to dictate how information will be transmitted.

The protocols most often encountered are SPI, I2C, U(S)ART, generally requireing 2,3, and 1 wires respectfully.  

The first thing you should note is that the ATTiny85 has neither a I2C or SPI hardware interface. Rather, it has a "USI" which is essentially a general interface that can be used in 2 or 3 wire interfaces. I2C is great because it allows multiple "slave" devices to be connected to one MCU master for multi-sensor applications. Unfortunately, most GPS units operate via UART. 

I selected a cheap, low power, small GPS with an integrated antennae. 

With that we have 

1- Microcontroller - ATTiny85 DIP package
2- Display - SSD1306 OLED 128x32 or 128x64
3- GPS - GP-20U7 sourced from Sparkfun. Serial communications @9600 baud
4- Battery - Any 3.3v liPo will do. buy an usb charging and monitoring board to make things easier, but they are not directly integrated into this project. 






