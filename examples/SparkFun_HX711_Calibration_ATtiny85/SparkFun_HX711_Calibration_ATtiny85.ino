/*
 Example using the SparkFun HX711 breakout board with a scale
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 19th, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 This is the calibration sketch. Use it to determine the calibration_factor that the main example uses. It also
 outputs the zero_factor useful for projects that have a permanent mass on the scale in between power cycles.
 
 Setup your scale and start the sketch WITHOUT a weight on the scale
 Once readings are displayed place the weight on the scale
 Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight
 Use this calibration_factor on the example sketch
 
 This example assumes pounds (lbs). If you prefer kilograms, change the Serial.print(" lbs"); line to kg. The
 calibration factor will be significantly different but it will be linearly related to lbs (1 lbs = 0.453592 kg).
 
 Your calibration factor may be very positive or very negative. It all depends on the setup of your scale system
 and the direction the sensors deflect from zero state

 This example code uses bogde's excellent library: https://github.com/bogde/HX711
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE

 Arduino pin 2 -> HX711 CLK
 3 -> DOUT
 5V -> VCC
 GND -> GND
 
 Most any pin on the Arduino Uno will be compatible with DOUT/CLK.
 
 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.
 
*/

#include "HX711.h" //This library can be obtained here http://librarymanager/All#Avia_HX711
#include <TinyWireM.h> // I2C Master lib for ATTinys which use USI - comment this out to use with standard arduinos
#include <LiquidCrystal_attiny.h> // for LCD w/ GPIO MODIFIED for the ATtiny85

// HX711 circuit wiring
#define LOADCELL_DOUT_PIN 3 //    2;           digital pin 3 is physical pin 2
#define LOADCELL_SCK_PIN  4 //3;                digital pin 4 is physical pin 3

HX711 scale;
LiquidCrystal_I2C lcd(0x27, 16, 2); // set address & 16 chars / 2 lines

float calibration_factor = -2000; //-7050 worked for my 440lb max scale setup

void setup() {

  lcd.init();
  lcd.noBacklight();
  //delay(50);
  lcd.backlight();
  lcd.clear();

  
  //Serial.begin(9600);
  //Serial.println("HX711 calibration sketch");
  //Serial.println("Remove all weight from scale");
  //Serial.println("After readings begin, place known weight on scale");
  //Serial.println("Press + or a to increase calibration factor");
  //Serial.println("Press - or z to decrease calibration factor");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();
  scale.tare();	//Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading
  displayText("Zero factor: ");
  lcd.print(zero_factor);
  //Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  //Serial.println(zero_factor);
  delay(1500);
}

void loop() {

  scale.set_scale(calibration_factor); //Adjust to this calibration factor

  lcd.clear();
  lcd.setCursor(0,0);
  displayText("Reading: ");
  lcd.print(scale.get_units()); //, 2);
  displayText(" kg");
  
  lcd.setCursor(0,1);
  displayText("cal factor: ");
  lcd.print(calibration_factor);
  
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  
  float init_calibration_factor = calibration_factor;
  for (int i=0;i<50;i++){
      displayText("iter: ");
      lcd.print(i);
      lcd.setCursor(0,1);
      calibration_factor += 10;
      //      /print_reading("factor: ",calibration_factor);
      displayText("F: ");
      lcd.setCursor(3,1);
      //lcd.print(String(calibration_factor));
      displayText(String(calibration_factor));
      delay(1200);
      
      lcd.clear();
      lcd.setCursor(0,0);
      //print_reading("Weight: ",scale.get_units());
      displayText("W: ");
      lcd.setCursor(3,0);
      lcd.print(scale.get_units());
      displayText(" kg");
      
      scale.power_down();
      delay(1500);
      scale.power_up();
      lcd.clear();
      lcd.setCursor(0,0);
  }

  calibration_factor = init_calibration_factor;
  for (int i=0;i<50;i++){
      displayText("iter: ");
      lcd.print(i);
      lcd.setCursor(0,1);
      calibration_factor -= 10;
//      //print_reading("factor: ",calibration_factor);
      displayText("F: ");
      lcd.setCursor(3,1);
      //lcd.print(String(calibration_factor));
      displayText(String(calibration_factor));
      delay(1200);
      
      lcd.clear();
      lcd.setCursor(0,0);
      //print_reading("Weight: ",scale.get_units());
      displayText("W: ");
      lcd.setCursor(3,0);
      lcd.print(scale.get_units());
      displayText(" kg");
      
      scale.power_down();
      delay(1500);
      scale.power_up();
      lcd.clear();
      lcd.setCursor(0,0);
  }

  //scale.power_down();              // put the ADC in sleep mode
  //delay(200);
  //scale.power_up();
}

void print_reading(String str, float reading){
  displayText(str);
  displayText(String(reading));
}

void displayText(String str) {
  char charBuf[16];
  str.toCharArray(charBuf, 16);
  for (int ind = 0; ind < str.length(); ind++) {
    lcd.print(charBuf[ind]);
  }
}
