/*
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
float temp(0), hum(0), pres(0);
float altitude;
float dewPoint;
uint8_t pressureUnit = 0x4;
/* ==== END Global Variables ==== */




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
  lcd.print("BME280 Gulli");   // Stampo il messaggio
  delay(1000); 
  lcd.clear();
}
/* ==== END Setup ==== */

/* ==== Loop ==== */
void loop() {

   // unit: B000 = Pa, B001 = hPa, B010 = Hg, B011 = atm, B100 = bar, B101 = torr, B110 = N/m^2, B111 = psi
   // Parameters: (float& pressure, float& temp, float& humidity, bool celsius = false, uint8_t pressureUnit = 0x0)
   bme.read(pres, temp, hum, metric, pressureUnit);                   
  Serial.print("Temp= ");
  Serial.print(temp);
  Serial.print("|"); 
  Serial.print("Hum=");
  Serial.print(hum);
  Serial.print("|"); 
  Serial.print("Press=");
  Serial.print(pres);
  Serial.print("|"); 
  altitude = bme.alt(metric);
  dewPoint = bme.dew(metric);
  Serial.print("Alt");
  Serial.print(altitude);
  Serial.print("|"); 
  Serial.print("DEW");
  Serial.println(dewPoint);
  lcd.setCursor(0, 1); // Sposto il cursore nella prima colonna, prima riga
  lcd.print("T= ");   // Stampo il messaggio
  lcd.print(temp);   // Stampo il messaggio
  lcd.print((char)223);   // Stampo il carattete °
 
   delay(500);
}
