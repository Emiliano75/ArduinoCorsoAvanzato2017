/*
 *********************Arduino Source File Header**************************
__file_name__ = webserver.ino
__description__="Implementa un server web su Arduino+Scheda Ethernet basata su chip W5100"
__author__ = "Stefano Baldacci"
__copyright__ = 
__license__ = 
__email__ = 
__STATUS__ = "Development[x]";"Test[]";"Production]";
__branch__= Master (SHA1) 
__History__: (repeat the following line as many times as applicable)
__version__ = "1.0 start development" 25 Maggio 2017
***************************************************************************
*/
#include <Arduino.h>
#include <Ethernet.h>
#include <SPI.h>
//libreria webduino
#include <WebServer.h>      

//#define FILENAME "datalog.txt"
#define LED 5
#define AI 0 // analog input
#define URL_COMMAND_ON "ON"
#define URL_COMMAND_OFF "OFF"
#define LINK_TEXT_ON "ACCENDI LED"
#define LINK_TEXT_OFF "SPEGNI LED"


bool LED_Status=false;
float Vin=0.0f;
unsigned long  time =0;
String s1="";

// Indirizzo MAC della scheda. Deve essere unico tra tutte le schede collegate nella stessa rete.
// Selezionare un indirizzo diverso dagli altri

static byte mac_Add[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x00 };
//static byte mac_Add[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 };
//static byte mac_Add[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x02 };
//static byte mac_Add[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x03 };
//static byte mac_Add[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x04 };



//Indirizzo IP della scheda
//ogni dispositivo collegato alla rete locale deve avere un ip address diverso
// Selezionare un indirizzo diverso dagli altri

static byte ip_Add[] = { 192, 168, 0, 190 };
//static byte ip_Add[] = { 192, 168, 0, 191 };
//static byte ip_Add[] = { 192, 168, 0, 192 };
//static byte ip_Add[] = { 192, 168, 0, 193 };
//static byte ip_Add[] = { 192, 168, 0, 194 };


//Funzione di servizio per creare un link da usare in una pagina html
//byte* ip_Add array che contiene i 4 byte dell'indirizzo ip
//char* command -> una stringa di comando da spedire nella URL
//char* msg -> il testo del link html che viene visualizzato nel browser
//Ritorna una stringa formattata html con un link clikkabile del tipo 
//"<a href=\"http://ip_address/?command\">msg</a>"
String MakeLinkURL(byte* ip_Add, char* command,  char* msg){

  String linkURL="<a href=\"http://";
  
  // scandisco l'array con l'indirizzo ip e costruisco la stringa
  for(byte i=0;i<4;i++){
    linkURL+=ip_Add[i];
    if(i<3)linkURL+=".";
  }
  
  // completo la stringa con comando e testo link
  linkURL = linkURL + "/?" + command + "\">" + msg + "</a>";
  
  return linkURL;
  }

  
  
//creazione oggetto Webduino. Webserver in ascolto sulla porta 80
//WebServer(const char *urlPrefix = "", uint16_t port = 80);
WebServer webserver("", 80);




//funzione Start che viene chiamata per gestire una richeista quando un client
//si collega al WebServer
void Start(WebServer &server, WebServer::ConnectionType type, char *url_param, bool param_complete) {
   

  
//restituisce al browser l'intestazione http 200 OK
server.httpSuccess();

//"<h1>Arduino Web Server</h1>"
P(strMsg1) = "<h1 style=\"color: #5e9ca0; text-align: center;\"><span style=\"color: #0000ff;\">Arduino WEB Server </span></h1>";
P(strMsg2) = "<h1 style=\"color: #5e9ca0; text-align: center;\"><span style=\"color: #0000ff;\">Gulli Corso Avanzato Lezione 6</span></h1>";


//Gestisco la richiesta di tipo GET fatta dal browser
if (type == WebServer::GET) {

//se il parametro e' disponibile nella url del browser
  if (param_complete == true){
    //copio i parametri in una stringa
    String parametri = url_param;
    //ricerco nella stringa dei parametri  i valori che mi aspetto
    int LedON = parametri.indexOf(URL_COMMAND_ON);
    int LedOFF = parametri.indexOf(URL_COMMAND_OFF);

    if (LedON >= 0 ) // ho clickato sul link per accendere il LED
    {
  // assegno lo stato del LED
    LED_Status=true;
    digitalWrite(LED, LED_Status);
    }
    if (LedOFF >= 0) // ho clickato sul link per spegnere il LED
      {
      // assegno lo stato del LED
    LED_Status=false;
    digitalWrite(LED, LED_Status);
      }
    }

	
  
  // Genero la pagina html che verrà visualizzata dal Browser che ha fatto la richiesta. 
  // L'html va nella Stringa s1 che poi viene inviata al browser
  
  //stampo Intestazione
  server.printP(strMsg1);
  server.printP(strMsg2);

    //stampo lo stato del LED
  s1=URL_COMMAND_OFF;
  if(LED_Status)s1=URL_COMMAND_ON;
  s1="<p>|LED = " + s1 + "| |";
  
  // stampo i link html per accendere e spegnere il LED
  s1+=MakeLinkURL(ip_Add,URL_COMMAND_ON, LINK_TEXT_ON);
  s1+="| |";
  s1+=MakeLinkURL(ip_Add,URL_COMMAND_OFF, LINK_TEXT_OFF);
  s1+="|<\p>";
  
  // invio al browser
  server.print(s1);
  
  // Stampo una riga con la lettura del Canale Analogico VIN
  Vin=analogRead(AI)*5/1024.0;
  char  vinstring[6];
  dtostrf(Vin,4,2,vinstring); // trasformo un float in una stringa
  s1= "<p>Vin = " + String(vinstring) + "<\p>";
  server.print(s1);

  }
}


//################################## setup() #################################
void setup()
{
// inizializzo la porta seriale per eventuale debug
Serial.begin(9600);

  
//inizializzo l'ethernet shield con il MAC e IP address
Ethernet.begin(mac_Add, ip_Add);

//definisci l'azione di default che verra' eseguita quando l'utente
//naviga nella root del sito
webserver.setDefaultCommand(&Start);

//aggiungi una pagina con la sua funzione di servizio. Se richiedo al WEB Server la 
//pagina index.html (Default) allora viene eseguita la funzione di servizio Start
webserver.addCommand("index.htm", &Start);

//avvia il web server
webserver.begin();

//imposto il pin di uscita per il LED
pinMode(LED, OUTPUT);
// spengo il LED
digitalWrite(LED, LED_Status);
}
//################################## setup() #################################




//################################## loop() ##################################
void loop()
{

//elabora costantemente tutte le richieste provenienti da un client connesso in rete locale
webserver.processConnection();

}
//################################## loop() ##################################
