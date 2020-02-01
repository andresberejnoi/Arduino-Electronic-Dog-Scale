#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <SoftwareSerial.h>
#include "HX711.h"
#define calibration_factor 1100 //This value is obtained using the SparkFun_HX711_Calibration sketch
#define zero_factor 146500 //8421804 This large value is obtained using the SparkFun_HX711_Calibration sketch

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

// interrupt INT0  2 // INT0 is pin 7
# define sleep_pin 1 // sleep if master is shutdown or disconnected
HX711 scale;

SoftwareSerial mySerial(5,0); // RX, TX (RX not used in program)
float result = 0.0;
static unsigned long TimeOn = 0;
int currState = HIGH;
int prevState = HIGH;

void setup() {
ADCSRA &= ~_BV(ADEN);// Switch Analog to Digital converter OFF
  // Open serial communications and wait for port to open:
  mySerial.begin(9600);
 OSCCAL  = 150;
 pinMode(sleep_pin,INPUT_PULLUP); // go to sleep pin
 pinMode(2,INPUT_PULLUP); // INT0, wake from sleep pin
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.set_offset(zero_factor); //Zero out the scale using a previously known zero_factor
  MCUCR &= ~(_BV(ISC01) | _BV(ISC00));      //INT0 on low level
  GIMSK |= _BV(INT0);                       //enable INT0
  sei();                        //enable interrupts
}

void loop() {
  
result = scale.get_units();
mySerial.println(result,1);
if(millis()-TimeOn > 50000){
system_sleep(); 
} 
int currState = digitalRead(sleep_pin);
if(currState != prevState){
   delay(300);
   system_sleep(); 
}
prevState = currState;
}


void system_sleep()
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set sleep mode, max power savings
  scale.power_down();
  sleep_mode(); // ATtiny and A/D sleeps here. Manualy turn off Master station after all data received for BLE module power down.
 // Upon waking up, sketch continues from this point. PIO4 High is pin 7 low(INT0), then put PIO4 Low again.
    sleep_disable();
    scale.power_up();
    TimeOn=millis();
    return;
    }
ISR(INT0_vect)
{
}
