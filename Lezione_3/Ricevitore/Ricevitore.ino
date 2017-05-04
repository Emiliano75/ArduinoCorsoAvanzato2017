#include <Arduino.h>
#include <SPI.h>
#include "RF24.h"

#define cePin 9
#define csnPin 10
#define ledPin 7

RF24 radio(cePin, csnPin);
byte indirizzo[6] = "Nodo1";

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(1, indirizzo);
  radio.startListening();
  Serial.println("Canale in lettura attivato");
}

bool stato;

void loop() {
  while (radio.available()) {
    radio.read(&stato, sizeof(bool));
    Serial.print("Dato ricevuto: ");
    Serial.println(stato);
    if (stato) {
      digitalWrite(7, HIGH);
    } else {
      digitalWrite(7, LOW);
    }
  }
  delay(50);
}