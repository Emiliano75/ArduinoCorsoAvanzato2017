/*
 *********************Arduino Source File Header**************************
__file_name__ = bme280stazionemeteo.ino
__description__="sketch per acquisizione dati del sensore BME280 (temperatura, umidità e pressione e DewPoint )
                e visualizzazione su display LCD"
__author__ = "Stefano Baldacci"
__copyright__ = 
__license__ = 
__email__ = 
__STATUS__ = "Development[x]";"Test[]";"Production]";
__branch__= Master (SHA1) 
__History__: (repeat the following line as many times as applicable)
__version__ = "1.0 start development" 11 Maggio 2017
***************************************************************************
*/

/*
Tabella Connessioni
BME280              ->  Arduino
-------------------------------
Vin (Voltage In)    ->  3.3V
Gnd (Ground)        ->  GND
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

// definisco oggetto BME280I2C con settaggi di default adatti ad un uso tipo Meteo Station
// Default : forced mode, standby time = 1000 ms
// Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,


BME280I2C bme;                   
                                 
 //definisco oggetto lcd del tipo LiquidCrystal_I2C all'indirizzo 0x3F
LiquidCrystal_I2C lcd(0x3F, 16, 2);

// flag che determina l'uso di unità di misura metriche o imperiali
bool metric = true;

// variabili per temperatura, pressione, umidità, DewPoint, Indice di Disagio Termico
float temp(0), hum(0), pres(0),dewPoint(0),SSI(0);

// pressione espressa in mbar. 1 mbar = 100 hPa
// unit: B000 = Pa, B001 = hPa, B010 = Hg, B011 = atm, B100 = bar, B101 = torr, B110 = N/m^2, B111 = psi

uint8_t pressureUnit = 0x4;

/* ==== END Global Variables ==== */


// funzione per la conevrsione da gradi Centigradi a gradi Fahrenheit
float C2F(float tempC){
  return (9.0/5.0)*tempC +32;
}


// funzione per la conevrsione da gradi Fahrenheit a gradi Centigradi  
float F2C(float tempC){
  return (tempC-32)*5.0/9.0;
}


// funzione per il calcolo del disagio termico espresso secondo il Summer Simmer Index.
// parametri: float tempC -> temperatura in °C
//            float rh umidità relativa in %
// Return:    SSI in °C
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

  //inizializzo porta seriale
  Serial.begin(SERIAL_BAUD);
  
  //inizializzo sensore BME280
  while(!bme.begin()){
    Serial.println("Could not find BME280 sensor!");
  }

  // inizializzo modulo LCD I2C
  lcd.begin(); 
  lcd.backlight();
  
  // Sposto il cursore nella prima colonna, prima riga
  lcd.setCursor(0, 0); 
  lcd.print("  Gulli BME280 ");   
  // Sposto il cursore nella prima colonna, seconda riga
  lcd.setCursor(0,1); 
  lcd.print(" Meteo Station  ");
  delay(3000); 
  lcd.clear();
}
/* ==== END Setup ==== */

/* ==== Loop ==== */
void loop() {

   //leggo dal sensoreBME280 
  
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
  
  //visualizzo su LCD

  // Sposto il cursore nella prima colonna, prima riga
  lcd.setCursor(0, 0); 
  lcd.print("T=");   
  lcd.print(temp,1);   
  lcd.print((char)223);   

// Sposto il cursore nella ottava colonna, prima riga
  lcd.setCursor(7, 0); 
  lcd.print("SSI=");   
  lcd.print(SSI,1);   
  lcd.print((char)223);   

// Sposto il cursore nella prima colonna, seconda riga
  lcd.setCursor(0, 1); 
  lcd.print("RH=");   
  lcd.print(int(hum)); 
  lcd.print("%");   

// Sposto il cursore nella settima colonna, seconda riga
  lcd.setCursor(6, 1); 
  lcd.print("P=");   
  lcd.print(int(pres*1000));   
  lcd.print("mbar"); 

  delay(1000);
}
