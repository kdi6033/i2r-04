#include <Wire.h>
// Output pin numbers
const int outputPins[8] = {26, 27, 32, 33, 21, 22, 23, 25};
// Input pin numbers
const int inputPins[8] = {12, 13, 14, 15, 18, 19, 4, 5};

void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);

  // Set each output pin as an output
  for (int i = 0; i <8; i++) {
    pinMode(outputPins[i], OUTPUT);
  }

  // Set each input pin as an input
  for (int i = 0; i < 8; i++) {
    pinMode(inputPins[i], INPUT);
  }
}

void loop() {

  // Cycle through each output pin
  for (int i = 0; i < 8; i++) {
    // Turn the current output pin on
    digitalWrite(outputPins[i], HIGH);
    // Wait for a second
    delay(1000);
    // Turn the current output pin off
    digitalWrite(outputPins[i], LOW);
  }

  // Cycle through each input pin
  for (int i = 0; i < 8; i++) {
    // Read the state of the current input pin
    int pinState = digitalRead(inputPins[i]);
    // Print the state of the current input pin
    Serial.print("Pin ");
    Serial.print(inputPins[i]);
    Serial.print(": ");
    Serial.println(pinState);
  }

  //밭데리 입력 전압 19V=4095 15V=2927 12V=2299
  Serial.print("밭데리 전압 :");
  Serial.println(analogRead(35));

  //1V=464 3V=1712  3.4V=1968
  Serial.print("ADC 전압 :");
  Serial.println(analogRead(34));
 
  delay(2000); // Wait for 2 seconds before reading again
}



