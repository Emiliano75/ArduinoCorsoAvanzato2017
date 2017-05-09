/*
BME280 I2C Test.ino
This code shows how to record data from the BME280 environmental sensor
using I2C interface. This file is an example file, part of the Arduino
BME280 library.
Copyright (C) 2016  Tyler Glenn

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Written: Dec 30 2015.
Last Updated: Sep 19 2016.

Connecting the BME280 Sensor:
Sensor              ->  Board
-----------------------------
Vin (Voltage In)    ->  3.3V
Gnd (Ground)        ->  Gnd
SDA (Serial Data)   ->  A4 on Uno/Pro-Mini, 20 on Mega2560/Due, 2 Leonardo/Pro-Micro
SCK (Serial Clock)  ->  A5 on Uno/Pro-Mini, 21 on Mega2560/Due, 3 Leonardo/Pro-Micro

 */

/* ==== Includes ==== */
#include <BME280I2C.h>
#include <Wire.h>             // Needed for legacy versions of Arduino.
#include <LiquidCrystal_I2C.h> // Libreria per LCD con interfaccia I2C
/* ====  END Includes ==== */

/* ==== Defines ==== */
#define SERIAL_BAUD 115200
/* ==== END Defines ==== */

/* ==== Global Variables ==== */
BME280I2C bme;                   // Default : forced mode, standby time = 1000 ms
                              // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,
                              //definisco oggetto lcd del tipo LiquidCrystal_I2C
LiquidCrystal_I2C lcd(0x3F, 16, 2);

bool metric = true;
float temp(0), hum(0), pres(0),dewPoint(0),SSI(0);
uint8_t pressureUnit = 0x4;
/* ==== END Global Variables ==== */


float C2F(float tempC){

  return (9.0/5.0)*tempC +32;
}

float F2C(float tempC){

  return (tempC-32)*5.0/9.0;
}

float Calc_SSI(float tempC, float rh){

float SSI=0;
float tempF=0;

SSI=tempC;

  if(tempC>=22){
    tempF=C2F(tempC);
    //SSI = 1.98 (Ta - (0.55 - 0.0055 UR) (Ta - 58)) - 56.83
    SSI=1.98*(tempF- (0.55-0.0055*rh)*(tempF-58))-56.83;
    SSI=F2C(SSI);
  }
return SSI;
}


/* ==== Setup ==== */
void setup() {

  lcd.begin(); 
  delay(1000);
  Serial.begin(SERIAL_BAUD);
  while(!Serial) {} // Wait
  while(!bme.begin()){
    Serial.println("Could not find BME280 sensor!");
  }

  lcd.backlight();
  lcd.setCursor(0, 0); // Sposto il cursore nella prima colonna, prima riga
  lcd.print("  Gulli BME280 ");   // Stampo il messaggio
  lcd.setCursor(0,1);
  lcd.print(" Meteo Station  ");
  delay(3000); 
  lcd.clear();
}
/* ==== END Setup ==== */

/* ==== Loop ==== */
void loop() {

   // unit: B000 = Pa, B001 = hPa, B010 = Hg, B011 = atm, B100 = bar, B101 = torr, B110 = N/m^2, B111 = psi
   // Parameters: (float& pressure, float& temp, float& humidity, bool celsius = false, uint8_t pressureUnit = 0x0)
   bme.read(pres, temp, hum, metric, pressureUnit);      
   dewPoint = bme.dew(metric);
   SSI=Calc_SSI(temp, hum);
              
  Serial.print("Temp=");
  Serial.print(temp,1);
  Serial.print("|"); 
  Serial.print("Hum=");
  Serial.print(int(hum));
  Serial.print("|"); 
  Serial.print("Press=");
  Serial.print(int(pres*1000));
  Serial.print("|");  
  Serial.print("DEW=");
  Serial.print(dewPoint,1);
  Serial.print("|");  
  Serial.print("SSI=");
  Serial.println(SSI,1);
  
  //stampo su LCD
  lcd.setCursor(0, 0); // Sposto il cursore nella prima colonna, prima riga
  lcd.print("T=");   // Stampo il messaggio
  lcd.print(temp,1);   // Stampo il messaggio
  lcd.print((char)223);   // Stampo il carattete °

  lcd.setCursor(7, 0); // Sposto il cursore nella nona colonna, prima riga
  lcd.print("SSI=");   // Stampo il messaggio
  lcd.print(SSI,1);   // Stampo il messaggio
  lcd.print((char)223);   // Stampo il carattete °

  lcd.setCursor(0, 1); // Sposto il cursore nella nona colonna, prima riga
  lcd.print("RH=");   // Stampo il messaggio
  lcd.print(int(hum));   // Stampo il messaggio
  lcd.print("%");   

  lcd.setCursor(6, 1); // Sposto il cursore nella nona colonna, prima riga
  lcd.print("P=");   // Stampo il messaggio
  lcd.print(int(pres*1000));   // Stampo il messaggio
  lcd.print("mbar"); 


  
   delay(1000);
}
