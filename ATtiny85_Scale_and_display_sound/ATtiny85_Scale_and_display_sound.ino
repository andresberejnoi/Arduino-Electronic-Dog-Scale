/**
 *
 * The code below is based on other peope's code and repurposed for this. There
 * are many parts that were taken from different examples and compiled together.
 * I will add sources as I find them.
 *
**/
#include "HX711.h"
#include <TinyWireM.h> // I2C Master lib for ATTinys which use USI - comment this out to use with standard arduinos
#include <LiquidCrystal_attiny.h> // for LCD w/ GPIO MODIFIED for the ATtiny85

// Setting version number (Not strongly enforced but good to keep in mind
const char VERSION[16] = "0.1.0";

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 3; //    2;           digital pin 3 is physical pin 2
const int LOADCELL_SCK_PIN  = 4; //3;                digital pin 4 is physical pin 3

const int SPEAKER_OUT = 1;  //output pin for speaker. This is physical pin 6 on ATtiny85

const float KG_to_LBS_factor = 2.205;   //an approximation taken from the internet. Multiply kg by this factor to get pounds

HX711 scale;
//LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3,POSITIVE);      //got all the LCD stuff from a post online. I wish I saved the link
LiquidCrystal_I2C lcd(0x27, 16, 2); // set address & 16 chars / 2 lines

void setup() {
  //Set up old PC speaker
  pinMode(SPEAKER_OUT,OUTPUT);     //this pin will output a signal

  //Set up LCD Display
  //lcd.begin(16,2);   //starting a 16x2 display (16 columns and 2 rows)
  lcd.init();
  lcd.noBacklight();
  delay(50);
  lcd.backlight();
  lcd.clear();

  //Serial.begin(9600);
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
  //char str_buf[20];     //this will contain the text to print to console
  //sprintf(str_buf, "%f kg",current_weight);
 // Serial.println(str_buf);

  //Now in pounds
  //sprintf(str_buf,"%f lb",weight_lbs);
  //Serial.println(str_buf);

  //Serial.println("");     //extra space for easier reading
  /*
   * End of debugging portion
   ****************************************************************************************/



  lcd.setCursor(0,0);   //set cursor at column zero and row zero on display (top left for a 16x2 display)
  lcd.print(current_weight);
  lcd.setCursor(6,0);
  displayText(" kg");

  lcd.setCursor(0,1);   //set cursors at column zero and row 1 on display (bottom left for a 16x2 display)
  lcd.print(weight_lbs);
  lcd.setCursor(6,1);
  displayText(" lbs");

  scale.power_down();			        // put the ADC in sleep mode
  delay(2000);
  scale.power_up();

}


void beep(){
  for (int i=0;i<500;i++){
    digitalWrite(SPEAKER_OUT, HIGH);
    delayMicroseconds(500);
    digitalWrite(SPEAKER_OUT, LOW);
    delayMicroseconds(500);
  }
}

void displayText(String str) {
  char charBuf[16];
  str.toCharArray(charBuf, 16);
  for (int ind = 0; ind < str.length(); ind++) {
    lcd.print(charBuf[ind]);
  }
}

void start_msg(){
  //Make simple text animation at startup just to have fun
  int delay_miliseconds = 400;

  /*First display current software version*/
  lcd.setCursor(0,0);
  displayText("Ver ");
  lcd.print(VERSION);
  delay(1200);
  lcd.clear();

  /*Now go on to display the starting msg*/
  lcd.setCursor(0,0);
  displayText("Turning on.");
  delay(delay_miliseconds);

  lcd.setCursor(11,0);
  displayText(".");
  delay(delay_miliseconds);

  lcd.setCursor(12,0);
  displayText(".");
  delay(delay_miliseconds);

  lcd.setCursor(11,0);
  displayText("  ");
  delay(200);

  lcd.setCursor(11,0);
  displayText(".");
  delay(delay_miliseconds);

  lcd.setCursor(12,0);
  displayText(".");
  delay(delay_miliseconds);

}
