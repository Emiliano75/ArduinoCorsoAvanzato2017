/*
 *********************Arduino Source File Header**************************
__file_name__ = rfid_readUID
__description__="sketch per la lettura di tag RFID MIFARE Classic  basato sullo scanner RFID-RC522"
__author__ = "Stefano Baldacci"
__copyright__ = "Informazioni di Copyright"
__license__ = "GPL"
__email__ = "stefano.baldacci@gmail.com"
__STATUS__ = "Development[x]";"Test[]";"Production]";
__branch__= Master (SHA1) 
__History__: (repeat the following line as many times as applicable)
__version__ = "1.0 start development"
***************************************************************************
*/

/*
 * Collegamenti Arduino<->RFID Tag Reader
 * --------------------------------------
 *             MFRC522      Arduino
 *             Reader/PCD   Uno R3
 * Signal      Pin          Pin
 * --------------------------------------
 * RST/Reset   RST          9    
 * SPI SS      SDA(SS)      10       
 * SPI MOSI    MOSI         11 
 * SPI MISO    MISO         12  
 * SPI SCK     SCK          13 
 * --------------------------------------
 */

#define DEBUG

//libreria per collegamento su BUS SPI
#include <SPI.h>
//libreria per collegamento con Scanner RFID
#include <MFRC522.h>

// Pin SDA(SS)
#define SS_PIN 10
// Pin RST
#define RST_PIN 9

#define BLU_LED 8 // linea I/O da attivare quando un tag è riconosciuto
#define RED_LED 7 // linea I/O da attivare quando un tag non è riconosciuto

// dimensione del vettore che contiene i codici delle TAG abilitate
#define WLsize 4

//vettore  che contiene i codici delle TAG abilitate
unsigned long whiteList[WLsize]={3320721658L,52796406L,0,0};  // RFID TAGS abilitati [unsigned long] 
//valore del TAG letto dallo scanner
unsigned long currentID=0L; 
// Flag di abilitazione (TAG riconosciuto tra quelli abilitati)
bool ACK=false;

// Inizializzo un oggetto di tipo MRFC522
MFRC522 rfid(SS_PIN, RST_PIN); 

//prototipi funzione helper per stampa valori ID dei TAG
void printHex(byte *buffer, byte bufferSize);
void printDec(byte *buffer, byte bufferSize);



void setup() { 

  // Inizializzo porta seriale
  Serial.begin(115200);
  // inizilizzo BUS SPI
  SPI.begin(); // Init SPI bus
  // inizializzo Scanner RFID
  rfid.PCD_Init(); 
  pinMode(BLU_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  digitalWrite(BLU_LED,LOW);
  digitalWrite(RED_LED,LOW);

  Serial.println(F("MIFARE Classic RFID TAG Scanner."));
  Serial.println(F("Avvicinare un TAG RFID allo scanner"));
}

 

void loop() {

  // Controllo se un TAG è stata rilevata
  if ( ! rfid.PICC_IsNewCardPresent()) return; // se no salto tutto il loop e torno all'inizio

  // Controllo se i codici identificativi del TAG sono stati letti
  if ( ! rfid.PICC_ReadCardSerial()) return; // se no salto tutto il loop e torno all'inizio 

// IL TAG ID è costituito da 4 Bytes
// copio il TAG ID in una singola variabile unsigned long
  memcpy(&currentID,rfid.uid.uidByte, 4);

  #ifdef DEBUG
  // stampo il TAG ID in HEX,DEC e LONG
  Serial.print(F("TAG ID = "));
  printHex(rfid.uid.uidByte, rfid.uid.size);
  Serial.print("(HEX Bytes) -");
  printDec(rfid.uid.uidByte, rfid.uid.size);
  Serial.print("(DEC Bytes) - ");
  Serial.print(currentID,DEC );
  Serial.println("(LONG)");
  #endif
  
  //controllo se il TAG ID è tra quelli abilitati
    for (int j=0;j<WLsize;j++) {
      if(currentID==whiteList[j]){
        ACK=true;
        break; // esco dal ciclo appena trovo un codice valido
      }
    }
  
    
    if (ACK){ // TAG abilitato
      // TODO abilitazione e attivazione di qualcosa
      digitalWrite(BLU_LED,HIGH);
      delay(500);
      digitalWrite(BLU_LED,LOW);
      ACK=false;
      #ifdef DEBUG
      Serial.println(F("Accesso abilitato !"));
      #endif
      
    } 
    else {
      digitalWrite(RED_LED,HIGH);
      delay(500);
      digitalWrite(RED_LED,LOW);
       #ifdef DEBUG
      Serial.println(F("TAG non riconosciuto. Accesso negato !"));
      #endif
    }
    
  #ifdef DEBUG
   Serial.println();
   Serial.println(F("Avvicinare un TAG RFID allo scanner"));
  #endif
   delay(500);
 
}


/*
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {

  for (byte i = 0; i < bufferSize; i++) {

    Serial.print(buffer[i] < 0x10 ? " 0" : " ");

    Serial.print(buffer[i], HEX);

  }

}



/**
 * Helper routine to dump a byte array as dec values to Serial.
 */

void printDec(byte *buffer, byte bufferSize) {

  for (byte i = 0; i < bufferSize; i++) {

    Serial.print(buffer[i] < 0x10 ? " 0" : " ");

    Serial.print(buffer[i], DEC);

  }

}
