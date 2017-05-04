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
  Serial.println("Canale in scrittura attivato");
}

bool stato = false;

void loop() {
  stato = !digitalRead(btnPin);
  if (radio.write(&stato, sizeof(bool))) {
    Serial.println("Invio OK");
    Serial.print("Dato inviato: ");
    Serial.println(stato);
  } else {
    Serial.println("Invio FALLITO");
  }
  delay(100);
}

  }
  delay(100);
}