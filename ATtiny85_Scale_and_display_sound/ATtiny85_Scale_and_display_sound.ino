/**
 *
 * HX711 library for Arduino - example file
 * https://github.com/bogde/HX711
 *
 * MIT License
 * (c) 2018 Bogdan Necula
 *
**/
#include "HX711.h"
#include <Wire.h>    //for display 
#include <LiquidCrystal_I2C.h>     //for display




// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

const int SPEAKER_OUT = 9;  //output pin for speaker

float current_weight;
float weight_kg;
float weight_lbs;
float KG_to_LBS_factor = 2.205;   //an approximation taken from the internet. Multiply kg by this factor to get pounds

HX711 scale;
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3,POSITIVE);      //got all the LCD stuff from a post online. I wish I saved the link

void setup() {
  //Set up old PC speaker
  pinMode(SPEAKER_OUT,OUTPUT);     //this pin will output a signal

  //Set up LCD Display
  lcd.begin(16,2);   //starting a 16x2 display (16 columns and 2 rows)
  lcd.clear();
  
  Serial.begin(9600);
  //lcd.print("HX711 Demo");

  //lcd.setCursor(0,0);
  //lcd.print("Starting Scale");
  //delay(2000);
  //lcd.clear();
  beep();
  start_msg();
  
  // Initialize library with data output pin, clock input pin and gain factor.
  // Channel selection is made by passing the appropriate gain:
  // - With a gain factor of 64 or 128, channel A is selected
  // - With a gain factor of 32, channel B is selected
  // By omitting the gain factor parameter, the library
  // default "128" (Channel A) is used here.
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  //lcd.print(scale.read());			// print a raw reading from the ADC
  scale.read();

  //lcd.print(scale.read_average(20));  	// print the average of 20 readings from the ADC
  scale.read_average(20);
  
  //lcd.print("get value: \t\t");setting up the scale
  //lcd.print(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight (not set yet)
  scale.get_value(5);
  //lcd.print(scale.get_units(5), 1);	// print the average of 5 readings from the ADC minus tare weight (not set) divided
						// by the SCALE parameter (not set yet)
  scale.get_units(5);
  
  scale.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();				        // reset the scale to 0

  //lcd.print(scale.read());                 // print a raw reading from the ADC

  //lcd.print(scale.read_average(20));       // print the average of 20 readings from the ADC
  
  //lcd.print(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight, set with tare()

  //lcd.print(scale.get_units(10));        // print the average of 5 readings from the ADC minus tare weight, divided
						// by the SCALE parameter set with set_scale
  scale.get_units(10);
  lcd.clear();
  beep();
}

void loop() {
  float current_weight = (scale.get_units(10) / 10) * -1;   //result is in kg. Divide by 10 because it is one order of magnitude higher. Multiply by -1 because results are giving negative for some reason
  float weight_lbs = current_weight * KG_to_LBS_factor;

  /*****************************************************************************************
   * 
   * This little section below is for debugging purposes to print to the console.
   */
  
  //First print weight in kg
  char str_buf[20];     //this will contain the text to print to console
  sprintf(str_buf, "%f kg",current_weight);
  Serial.println(str_buf);

  //Now in pounds 
  sprintf(str_buf,"%f lb",weight_lbs);
  Serial.println(str_buf);

  Serial.println("");     //extra space for easier reading
  /*  
   * End of debugging portion
   ****************************************************************************************/

  
  
  lcd.setCursor(0,0);   //set cursor at column zero and row zero on display (top left for a 16x2 display)
  lcd.print(current_weight);
  lcd.setCursor(6,0);
  lcd.print(" kg");

  lcd.setCursor(0,1);   //set cursors at column zero and row 1 on display (bottom left for a 16x2 display)
  lcd.print(weight_lbs);
  lcd.setCursor(6,1);
  lcd.print(" lbs");

  scale.power_down();			        // put the ADC in sleep mode
  delay(2000);
  scale.power_up();
  //lcd.clear();
  //beep();
}


void beep(){
  for (int i=0;i<500;i++){
    digitalWrite(SPEAKER_OUT, HIGH);
    delayMicroseconds(500);
    digitalWrite(SPEAKER_OUT, LOW);
    delayMicroseconds(500);
  }
}
void start_msg(){
  //Make simple text animation at startup just to have fun
  int delay_miliseconds = 500;
  lcd.setCursor(0,0);
  lcd.print("Turning on.");
  delay(delay_miliseconds);
  
  lcd.setCursor(11,0);
  lcd.print(".");
  delay(delay_miliseconds);

  lcd.setCursor(12,0);
  lcd.print(".");
  delay(delay_miliseconds);

  lcd.setCursor(11,0);
  lcd.print("  ");
  delay(200);

  lcd.setCursor(11,0);
  lcd.print(".");
  delay(delay_miliseconds);

  lcd.setCursor(12,0);
  lcd.print(".");
  delay(delay_miliseconds);

  //lcd.clear();
}
