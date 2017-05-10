/*
 *********************Arduino Source File Header**************************
__file_name__ = bme280_Alti.ino
__description__="sketch per lettura pressione ed altitudine  barometrica del sensore BME280. Calcolo del dislivello a partire 
da un valore di riferimento settabile tramite pulsante. L'altimetri barometrico può esssere calibrato via linea seriale
inserendo il valore di altitudine noto.
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

// definisco oggetto BME280I2C con settaggi adatti per una misura si precisione della pressione
BME280I2C bme(2, 1, 5, 3, 0, 4); // Indoor Navigation : normal mode, standby time = 0.5ms
                                // pressure ×16, temperature ×2, humidity ×1, filter = x16
                                // Current Consumption = 633 μA
                                // RMS Noise = 0.2 Pa/1.7 cm
                                // Data Output Rate = 25Hz
                                // Filter Bandwidth = 0.53 Hz
                                // Response Time (75%) = 0.9 s

//definisco oggetto lcd del tipo LiquidCrystal_I2C su bus I2C all'indirizzo 0X3F
LiquidCrystal_I2C lcd(0x3F, 16, 2);


// flag che determina l'uso di unità di misura metriche o imperiali
bool metric = true;

// altitudine di riferimento
float refAltitudine=0;

// altitudine corrente
float altitudine=0.0;

// pressione a livello del mare
float seaLevelPressure=1013.25; //millibar

// pressione corrente
float pressure=0.0;

//dislivello calcolato rispetto ad altitudine di riferimento
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
  // Sposto il cursore nella prima colonna, prima riga
  lcd.setCursor(0, 0); 
  lcd.print("  BME280 Gulli ");  
  // Sposto il cursore nella prima colonna, seconda riga 
  lcd.setCursor(0, 1); 
  lcd.print("    Altimetro   ");  
  delay(3000);
  lcd.clear();

// imposto valore iniziale per altitudine di riferimento
  refAltitudine=altitudine=bme.alt(metric,seaLevelPressure*100);
}

void loop() {

// controllo se ci sono comandi da seriale per calibrare l'altimetro
  if (Serial.available()){

    switch(Serial.peek()){
      case 'a':
      Serial.read();
      Serial.println("Inserisci l'altitudine in mt.");
      while(!Serial.available());

      // leggo altitudine nota
      altitudine=Serial.parseFloat();
      //calcolo pressione SLM che corrisponde alla altitudine inserita
      seaLevelPressure=bme.sealevel(altitudine);
      break;
      case 'p':
      Serial.read();
      Serial.println("Inserisci la pressione SLM in mbar.");
      while(!Serial.available());
      seaLevelPressure=Serial.parseFloat();
      break;
      default:
      Serial.read();
      Serial.println("Comando non valido");
      break;
      }
  } 


// leggo l'altitudine dal sensore BME280  in base al valore della pressione corrente e della pressione SLM
  altitudine=bme.alt(metric,seaLevelPressure*100);
// calcolo il dislivello
  dislivello=bme.alt(metric,seaLevelPressure*100)-refAltitudine;

 // se il bottone è premuto salvo l'attuale altezza come riferimento per il calcolo del dislivello
  if (!digitalRead(RESET_BUTTON)){
  refAltitudine=bme.alt(metric,seaLevelPressure*100);
  }



  //visualizzo su seriale
  Serial.print("Altitudine");
  Serial.print(altitudine);
  Serial.println(" mt.");
  Serial.print("Dislivello");
  Serial.println(dislivello);
  Serial.println("-------------------");
  
  //visualizzo su modulo LCD
  // Sposto il cursore nella prima colonna, prima riga
  lcd.setCursor(0, 0); 
  lcd.print("Alt.=");   
  lcd.print(altitudine); 
  lcd.setCursor(14, 0); 
  lcd.print("mt");
  
 // Sposto il cursore nella prima colonna, seconda riga
  lcd.setCursor(0, 1); 
  lcd.print("DSLV=");   
  lcd.print(dislivello); 
  lcd.setCursor(14, 1); 
  lcd.print("mt");
  delay(500);
}


