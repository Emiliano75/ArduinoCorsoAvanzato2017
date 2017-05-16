/*
 *********************Arduino Source File Header**************************
__file_name__ = "Joystick_stepper.ino"
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
       ***************************
       | Joystick => Arduino UNO |
       ***************************
Joystick PCB      |      Arduino UNO
---------------------------------------
GND(-)            |      GND
VCC(+5v)          |      5 Volt
VRx               |      Analog IN A0
VRy               |      Not Used
SW (Switch)       |      Not Used
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

#define PIN_X_AXES A0
#define FORWARD_DIR 570
#define BACKWARD_DIR 480

int Speed = 0;

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
    delay(Speed);
  }
}

void Step_Backward() {
    for(int i=3; i>=0; i--) {
    Set_StepperPin_Low();
    digitalWrite(Stepper_pin[i], HIGH);
    delay(Speed);
  }
}

void setup() {
  for(byte i=0; i<=3; i++) {
    pinMode(Stepper_pin[i], OUTPUT);
  }   
  Set_StepperPin_Low();   
}

void loop() {
  int X_Axes_Reading = analogRead(PIN_X_AXES);
  if(X_Axes_Reading > FORWARD_DIR) {
    Speed = map(X_Axes_Reading, 570, 1023, 30, 3);
    Step_Forward();
  } else if(X_Axes_Reading < BACKWARD_DIR) {
      Speed = map(X_Axes_Reading, 480, 0, 30, 3);
      Step_Backward();
  }
}
