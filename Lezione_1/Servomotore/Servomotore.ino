/*
 *********************Arduino Source File Header**************************
__file_name__ = "Servomotore.ino"
__description__="Codice prima lezione corso Arduino Avanzato"
__author__ = "Emiliano Agostini"
__copyright__ = "Informazioni di Copyright"
__license__ = "GPL"
__email__ = "beviugo2@gmail.com"
__status__ = "Development[]";"Test[]";"Production[X]";
__History__: (repeat the following line as many times as applicable)
__version__ = "1.0"
***************************************************************************
*/
/*
################################################
############ Collegamento LCD I2C ##############
################################################
#    Display LCD   |   Arduino                 #
#                                              #   
#        VCC      >>>    5v                    #
#        GND      >>>    GND                   #
#        SCL      >>>    Arduino Pin A5        #
#        SDA      >>>    Arduino Pin A4        #
#                                              #
################################################   
*/

#define LCD
#define PinPotenziometro A0
#include <Servo.h>

#ifdef LCD
#include <LiquidCrystal_I2C.h>
#endif

byte PosizioneServo = 0;

Servo myservo;

#ifdef LCD
LiquidCrystal_I2C lcd(0x3F, 16, 2);  
#endif

void Pulisci_Riga(){
  lcd.setCursor(2, 1);
  lcd.print(" ");
}

void setup() {
  Serial.begin(9600);
  myservo.attach(6);
  #ifdef LCD
  lcd.begin();
  #endif
}

void loop() {
  int Lettura_Potenziometro_RAW = analogRead(PinPotenziometro);
  PosizioneServo = map(Lettura_Potenziometro_RAW, 0, 1023, 0, 180);
  myservo.write(PosizioneServo);
  Serial.println(PosizioneServo);
  #ifdef LCD
  lcd.setCursor(0, 0);
  lcd.print("Posizione Servo");
  lcd.setCursor(0, 1);
  lcd.print(PosizioneServo);
  if(PosizioneServo < 100)Pulisci_Riga();
  #endif
}
