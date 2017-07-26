# attiny-gps-watch
Schematics, code, and notes for a GPS watch based on ATMEL AVR ATTiny85 Micro-controller 

# Status - WIP
Finalizing code for upload

Current features:
- Software Serial interface UART (used for RX of GPS signal)
- USI and Timer used for I2C interface with SSD1306 Display 
- Software Serial output interface UART for sending debug text out to Arduino

ToDo:
- Processing NMEA messages 
- Code profiling 
- Final interface integration and design 

# About this project
Nowadays, we are spoiled with the availability of fast processors with ample memory, storage, and peripheral features. The purpose of this project is to design a workable hand-portable GPS solution using a minimal microcontroller that consumes a very low amount of power. Like many people, my first experience with microcontrollers was with the very friendly Arduino platform. I can't remember when, but I eventually stumbled upon the ATTiny series, which are propular amongst hobbyists for their low price and relative simplicity. The ATTiny85 comes in a 8-pin DIP package, and 3 pins are taken up by VCC, GND, and RESET, 2 for external clock source, leaving 3 pins to control and receive signals. I like this device because it challenges you to implement your requirements using a bare minimal amount of features present on a microcontroller today. Most basic projects don't necessarily need the power of chips present in most Arduinos, let alone an ARM processor. There aren't even any dedicated UART or SPI peripherals on the ATTiny85, only a "Universal Serial Interface" or USI. A lot of thinking for beginning microcontorller programmers and software workarounds will be necessary to interface with multiple sensors and outputs. 

The entire project is compiled in C++ (but uses very few C++ features in order to maintain simplicity) with some in-line assembly. ATMEL's Windows based ATMEL Studio was used as the IDE. Programming was performed via Sparkfun's basic ATTiny USB Programmer (search the web for tutorials on how to set it up with ATMEL Studio). Because we are using such a basic programmer, debugging was done via LED flashes in the beginning and then serial output through an Arduino once a basic software serial was coded. 

This project was a learning process for me to help test my skills in lower level programming in C while working with hardware and integrating a complete project. 

I've provided documentation below as a tutorial for others and as documentatoin. 

# Design Process Step 1 - Getting familiar with ATTiny85 and Initial Theory 
