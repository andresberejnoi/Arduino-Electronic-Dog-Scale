/**

   The code below is based on other peope's code and repurposed for this. There
   are some parts that were taken from different examples and compiled together.
   I will add sources as I find them.

**/
#include "HX711.h"
#include <TinyWireM.h> // I2C Master lib for ATTinys which use USI - comment this out to use with standard arduinos
#include <LiquidCrystal_attiny.h> // for LCD w/ GPIO MODIFIED for the ATtiny85

// Setting version number (Not strongly enforced but good to keep in mind
const char VERSION[8] = "0.5.1";

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 3; //    2;           digital pin 3 is physical pin 2
const int LOADCELL_SCK_PIN  = 4; //3;                digital pin 4 is physical pin 3

const int SPEAKER_OUT = 1;  //output pin for speaker. This is physical pin 6 on ATtiny85

const float KG_to_LBS_factor = 2.205;   //an approximation taken from the internet. Multiply kg by this factor to get pounds

float LAST_WEIGHT = 0;
int FINISHED_COUNT = 0;          // we will use it to keep track of whether we have reached a final weigh (to know if balance should keep updating)

HX711 scale;
LiquidCrystal_I2C lcd(0x27, 16, 2); // set address & 16 chars / 2 lines


void setup() {
  //Set up old PC speaker
  pinMode(SPEAKER_OUT, OUTPUT);    //this pin will output a signal

  //Set up LCD Display
  lcd.init();
  lcd.noBacklight();
  //delay(50);
  lcd.backlight();
  lcd.clear();

  beep();
  start_msg();

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  scale.read();
  scale.read_average(20);

  scale.get_value(5);
  scale.get_units(5);

  scale.set_scale(-22930.f);
  scale.tare();				        // reset the scale to 0

  scale.get_units(10);
  lcd.clear();
  beep();

  displayText("Get on...");
  delay(700);
  lcd.clear();
}

void loop() {
  float current_weight = scale.get_units(15);
  float weight_lbs = current_weight * KG_to_LBS_factor;

  print_kg(current_weight);
  print_lbs(weight_lbs);
  scale.power_down();              // put the ADC in sleep mode
  delay(1500);
  scale.power_up();
  LAST_WEIGHT = current_weight;
}


/*
 * Beep function to use with piezo speaker. Creates one beep.
 */
void beep() {
  for (int i = 0; i < 500; i++) {
    digitalWrite(SPEAKER_OUT, HIGH);
    delayMicroseconds(500);
    digitalWrite(SPEAKER_OUT, LOW);
    delayMicroseconds(500);
  }
}


/*
 * Prints str to the lcd screen. This function was taken from some online resource, but I can't find where.
 */
void displayText(String str) {
  char charBuf[16];
  str.toCharArray(charBuf, 16);
  for (int ind = 0; ind < str.length(); ind++) {
    lcd.print(charBuf[ind]);
  }
}


/*
 * Convenience function to print given weight with the string " kg" added at the end
 */
void print_kg(float kg){
  lcd.setCursor(0, 0);  //set cursor at column zero and row zero on display (top left for a 16x2 display)
  displayText(String(kg));
  lcd.setCursor(6, 0);
  displayText(" kg");
}


/*
 * Convenience function to print given weight with the string " lbs" added at the end
 */
void print_lbs(float lbs){
  lcd.setCursor(0, 1);  //set cursors at column zero and row 1 on display (bottom left for a 16x2 display)
  displayText(String(lbs));
  lcd.setCursor(6, 1);
  displayText(" lbs");
}


/*
 * Convenience function to print given value with "%" added at the end
 */
void print_percent_diff(float pct_diff){
  lcd.setCursor(0, 1);  //set cursors at column zero and row 1 on display (bottom left for a 16x2 display)
  displayText(String(pct_diff));
  lcd.setCursor(6, 1);
  displayText("%");
}


/*
 * Convenience function that displays version and startup messages
 */
void start_msg() {
  //Make simple text animation at startup just to have fun
  int delay_miliseconds = 350;

  /*First display current software version*/
  lcd.setCursor(0, 0);
  displayText("Ver ");
  displayText(VERSION);
  //lcd.print(VERSION);
  delay(1100);
  lcd.clear();

  /*Now go on to display the starting msg*/
  lcd.setCursor(0, 0);
  displayText("Turning on.");
  delay(delay_miliseconds);

  lcd.setCursor(11, 0);
  displayText(".");
  delay(delay_miliseconds);

  lcd.setCursor(12, 0);
  displayText(".");
  delay(delay_miliseconds);

  lcd.setCursor(11, 0);
  displayText("  ");
  delay(200);

  lcd.setCursor(11, 0);
  displayText(".");
  delay(delay_miliseconds);

  lcd.setCursor(12, 0);
  displayText(".");
  delay(delay_miliseconds);

}
