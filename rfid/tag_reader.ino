/*

 * --------------------------------------------------------------------------------------------------------------------

 * Example sketch/program showing how to read new NUID from a PICC to serial.

 * --------------------------------------------------------------------------------------------------------------------

 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid

 * 

 * Example sketch/program showing how to the read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID

 * Reader on the Arduino SPI interface.

 * 

 * When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE

 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When

 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output

 * will show the type, and the NUID if a new card has been detected. Note: you may see "Timeout in communication" messages

 * when removing the PICC from reading distance too early.

 * 

 * @license Released into the public domain.

 * 

 * Typical pin layout used:

 * -----------------------------------------------------------------------------------------

 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino

 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro

 * Signal      Pin          Pin           Pin       Pin        Pin              Pin

 * -----------------------------------------------------------------------------------------

 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST

 * SPI SS      SDA(SS)      10            53        D10        10               10

 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16

 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14

 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15

 */



#include <SPI.h>

#include <MFRC522.h>



#define SS_PIN 10

#define RST_PIN 9

#define WLsize 4

unsigned long whiteList[WLsize]={3320721658L,52796406L,0,0};  // RFID TAGS abilitati [unsigned long] 
unsigned long currentID=0L;
bool ACK=false;

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

void printHex(byte *buffer, byte bufferSize);
void printDec(byte *buffer, byte bufferSize);



void setup() { 

  Serial.begin(115200);

  SPI.begin(); // Init SPI bus

  rfid.PCD_Init(); // Init MFRC522 


  Serial.println(F("MIFARE Classic RFID TAG Scanner."));
  Serial.println(F("Avvicinare un TAG RFID allo scanner"));
}

 

void loop() {

  // Look for new cards

  if ( ! rfid.PICC_IsNewCardPresent())return;

  // Verify if the NUID has been read

  if ( ! rfid.PICC_ReadCardSerial()) return;


// leggo il TAG ID in una singola variabile unsigned long
  memcpy(&currentID,rfid.uid.uidByte, 4);

  // stampo il TAG ID in HEX,BCD e DEC
  Serial.print(F("TAG ID = "));
  printHex(rfid.uid.uidByte, rfid.uid.size);
  Serial.print("(HEX Bytes) -");
  printDec(rfid.uid.uidByte, rfid.uid.size);
  Serial.print("(DEC Bytes) - ");
  Serial.print(currentID,DEC );
  Serial.println("(LONG)");

  //controllo se il TAG ID è tra quelli abilitati
    for (int j=0;j<WLsize;j++) {
      if(currentID==whiteList[j]){
        ACK=true;
        break;
      }
    }
    
    if (ACK){
      // TODO abilitazione e attivazione di qualcosa
      Serial.println(F("Accesso abilitato !"));
      ACK=false;
    } 
    else {
      Serial.println(F("Carta non abilitata. Accesso negato !"));
    }
    
   Serial.println();
   Serial.println(F("Avvicinare un TAG RFID allo scanner"));
    delay(500);
 
}





/**

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
