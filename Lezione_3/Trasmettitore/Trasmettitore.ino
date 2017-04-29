#include <Arduino.h>
#include <SPI.h>
#include "RF24.h"

#define cePin 9
#define csnPin 10
#define btnPin 7

RF24 radio(cePin, csnPin);
byte indirizzo[6] = "Nodo1";

void setup() {
  pinMode(btnPin, INPUT_PULLUP);
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(indirizzo);
  Serial.println("Radio accesa");
}

unsigned int stato = 0;

void loop() {
  stato = !digitalRead(btnPin);
  if (radio.write(&stato, sizeof(unsigned int))) {
    Serial.println("Invio OK");
  } else {
    Serial.println("Invio FALLITO");
  }
  delay(200);
}
