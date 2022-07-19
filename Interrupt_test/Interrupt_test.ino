/*
  Analog Input

  Demonstrates analog input by reading an analog sensor on analog pin 0 and
  turning on and off a light emitting diode(LED) connected to digital pin 13.
  The amount of time the LED will be on and off depends on the value obtained
  by analogRead().

  The circuit:
  - potentiometer
    center pin of the potentiometer to the analog input 0
    one side pin (either one) to ground
    the other side pin to +5V
  - LED
    anode (long leg) attached to digital output 13 through 220 ohm resistor
    cathode (short leg) attached to ground

  - Note: because most Arduinos have a built-in LED attached to pin 13 on the
    board, the LED is optional.

  created by David Cuartielles
  modified 30 Aug 2011
  By Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogInput
*/

int clockPin = 2;    // select the input pin for the potentiometer
unsigned int clockCount = 0;
unsigned int currMillis = 0;

int analogPin = A7;

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(clockPin, INPUT);
  Serial.begin(115200);
  Serial.println("Let's Rock");
  attachInterrupt(digitalPinToInterrupt(clockPin), countInterrupt, RISING);
  unsigned int currMillis = millis();
}

void loop() {
  // read the value from the sensor:

  //Serial.println(analogRead(analogPin));
  
  float BPM = 0;
  //delay(1);
  if(clockCount > 16){
    BPM = ((float)clockCount / ((float)(millis() - currMillis)/1000.0/60.0)/4.0);
    Serial.print("BPM = ");
    Serial.println(BPM);
    currMillis = millis();
    clockCount = 0;
  }
 // Serial.println(clockCount);
}

void countInterrupt(){
  clockCount++;
}
