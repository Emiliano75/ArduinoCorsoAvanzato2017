/*
 *********************Arduino Source File Header**************************
__file_name__ = "Test_stepper.ino"
__description__="Codice quinta lezione corso Arduino Avanzato"
__author__ = "Emiliano Agostini"
__copyright__ = "Informazioni di Copyright"
__license__ = "GPL"
__email__ = "beviugo2@gmail.com"
__status__ = "Development[]";"Test[]";"Production[X]";
__History__: (repeat the following line as many times as applicable)
__version__ = "1.0"
***************************************************************************
             
             Collegamenti
  ******************************** 
  | Stepper_motor => Arduino UNO |
  ********************************
Stepper PCB       |      Arduino UNO
---------------------------------------
IN 1              |      Digital Pin 2
IN 2              |      Digital Pin 3
IN 3              |      Digital Pin 4
IN 4              |      Digital Pin 5
GND(-)            |      GND
VCC(+) (5-12V)    |      5 Volt
----------------------------------------

           *****************
           | Step sequence |
           *****************
          
           A+ A- B+ B- | Coil
           ------------------
           1  0  0  0  Step 1
           0  1  0  0  Step 2
           0  0  1  0  Step 3
           0  0  0  1  Step 4
*/

#define STEP_FOR_REVOLUTION 515
#define SPEED 3
#define WAIT 1000

byte Stepper_pin[]={2, 3, 4, 5};

void Set_StepperPin_Low(){
  for(byte i=0; i<=3; i++) {
    digitalWrite(Stepper_pin[i], LOW);
  }
}

void Step_Forward() {
    for(int i=0; i<=3; i++) {
    Set_StepperPin_Low();
    digitalWrite(Stepper_pin[i], HIGH);
    delay(SPEED);
  }
}

void Step_Backward() {
    for(int i=3; i>=0; i--) {
    Set_StepperPin_Low();
    digitalWrite(Stepper_pin[i], HIGH);
    delay(SPEED);
  }
}

void setup() {
  for(byte i=0; i<=3; i++) {
    pinMode(Stepper_pin[i], OUTPUT);
  }   
  Set_StepperPin_Low();
}

void loop() {
  for(int i=0; i<STEP_FOR_REVOLUTION; i++) {
    Step_Forward();
  }
  delay(WAIT);
  for(int i=STEP_FOR_REVOLUTION; i>0; i--) {
    Step_Backward();
  }
  delay(WAIT);
}
