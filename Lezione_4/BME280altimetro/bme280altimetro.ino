

/* ==== Includes ==== */
#include <BME280I2C.h>
#include <Wire.h>             // Needed for legacy versions of Arduino.
#include <LiquidCrystal_I2C.h> // Libreria per LCD con interfaccia I2C
/* ====  END Includes ==== */

/* ==== Defines ==== */
#define SERIAL_BAUD 115200
#define RESET_BUTTON 3
/* ==== END Defines ==== */

/*
Connessioni del sensore BME280
Sensore             ->  Arduino
-----------------------------
Vin (Voltage In)    ->  3.3V
Gnd (Ground)        ->  Gnd
SDA (Serial Data)   ->  A4 on Uno/Pro-Mini, 20 on Mega2560/Due, 2 Leonardo/Pro-Micro
SCK (Serial Clock)  ->  A5 on Uno/Pro-Mini, 21 on Mega2560/Due, 3 Leonardo/Pro-Micro
 */



/* Global Variables */
BME280I2C bme(2, 1, 5, 3, 0, 4); // Indoor Navigation : normal mode, standby time = 0.5ms
                                // pressure ×16, temperature ×2, humidity ×1, filter = x16
                                // Current Consumption = 633 μA
                                // RMS Noise = 0.2 Pa/1.7 cm
                                // Data Output Rate = 25Hz
                                // Filter Bandwidth = 0.53 Hz
                                // Response Time (75%) = 0.9 s


//definisco oggetto lcd del tipo LiquidCrystal_I2C su bus I2C all'indirizzo 0X3F
LiquidCrystal_I2C lcd(0x3F, 16, 2);


bool metric = true;
float refAltitudine=0;
float altitudine=0.0;
float seaLevelPressure=1013.25; //millibar
float pressure=0.0;
float dislivello=0.0;
/* ==== END Global Variables ==== */



void setup() {

  pinMode(RESET_BUTTON,INPUT_PULLUP);
  Serial.begin(SERIAL_BAUD);
  while(!Serial) {} // Wait
  while(!bme.begin()){
    Serial.println("Could not find BME280I2C sensor!");
    delay(1000);
  }

  // set up LCD here
  lcd.begin(); 
  lcd.backlight();
  lcd.setCursor(0, 0); // Sposto il cursore nella prima colonna, prima riga
  lcd.print("  BME280 Gulli ");   // Stampo il messaggio
   delay(3000);
  
  
  
}

void loop() {




// controllo se ci sono comandi da serialie per rimettere l'orologio
  if (Serial.available()){

   
    switch(Serial.peek()){
      case 'a':
      Serial.read();
      Serial.println("Inserisci l'altitudine in mt.");
      while(!Serial.available());
      altitudine=Serial.parseFloat();
      seaLevelPressure=bme.sealevel(altitudine);
      break;
      case 'p':
      Serial.read();
      Serial.println("Inserisci la pressione SLM in mbar.");
      while(!Serial.available());
      pressure=Serial.parseFloat();
      break;
      default:
      break;
      }

  } 

  altitudine=bme.alt(metric,seaLevelPressure*100);
 
  if (!digitalRead(RESET_BUTTON)){
  refAltitudine=bme.alt(metric,seaLevelPressure*100);
  }

  dislivello=bme.alt(metric,seaLevelPressure*100)-refAltitudine;
  Serial.print("Altitudine");
  Serial.println(altitudine);
  lcd.setCursor(0, 0); // Sposto il cursore nella prima colonna, prima riga
  lcd.print("Altitudine=");   // Stampo il messaggio
  lcd.print(altitudine); 
  lcd.print("mt");
  
  Serial.print("Dislivello");
  Serial.println(dislivello);
  lcd.setCursor(0, 1); // Sposto il cursore nella prima colonna, prima riga
  lcd.print("Dislivello");   // Stampo il messaggio
  lcd.print(dislivello); 
  lcd.print("mt");
  delay(500);
}


