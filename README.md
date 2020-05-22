# Dog Scale
This is a scale meant for weighting our dogs and maybe suitcases by using 4 3-wire load cells connected to an amplifier HX711 and displaying the code to a 16x2 LCD screen with an included I2C backpack to control it.

The micro-controller was initially an Arduino Uno / Diavolino model but since there were many pins not being used, and I did not want to use my entire Arduino for this, I decided to miniaturize this project by using an ATtiny85 chip. It has just the right amount of pins for this and the size is very conservative.

## ArduinoUno_Scale_Final
This sketch contains the code I used for the Arduino Uno.There is a chance it is behind the ATtiny85 version since I switched over to that chip to complete things. However, the code is super short, so differences will be minuscule. 

## ATtiny85_Scale_Final
This sketch is to be used with the ATtiny85 version of the scale.

## Two Versions
I will try to maintain both versions (Arduino and ATtiny85) working and synched to each other but the final objective is that the ATtiny85 will be the chip of choice.
I used one of the examples of the HX711 library as started code and modified it to suit my needs. There are some functions that I also used from online forums and the like. If I find the links again, I will add them to the coode.

## Components
I will do my best keep this updated with all components used for the main part of the project but also for extra related activities.

Edit: I'll go ahead and add some Amazon affiliate links so you can find the components more easily. If you end up buying something, I will get a comission but it will not come at an extra cost to you. It is a way to support more projects like this. Thanks.

Main Project Stuff:
- [HX711 amplifier](https://amzn.to/2TlRH4c) for load cell sensors
- [4 50kg 3-wire load cells](https://amzn.to/2Zskk3q) (one on each corner of a square) connected together
- One old PC beeper or [piezo speaker](https://amzn.to/2AFxFeh) (for simple sounds when turning on)
- [Arduino Uno](https://amzn.to/2ZjUChB) / Diavolino / [ATtiny85](https://amzn.to/2LJxjpj)
- [16x2 LCD screen with an included I2C backpack](https://amzn.to/3dUkh4u) (with 0x27 address but that's just my particular component)
- Power source (a portable usb battery in my case)
- USB cable for power
  - In my case, my Diavolino needs an FTDI breakout board to connect through a usb cable.

Other obvious things to have are:
- [Jumper wires](https://amzn.to/2ZwV1gT)
- [Soldering kit](https://amzn.to/2z6pLdM)
- A base where to set the sensors
- Housing for the components

Other Components:
- [10 uF capacitor](https://amzn.to/2LL1BYK) (for when using Arduino Uno as ISP to program ATtiny85 or burn its bootloader)
