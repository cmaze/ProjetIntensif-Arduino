
// Load Wifi library
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
const char* ssid = "DOME_FORMATION";
const char* password = "dome_formation";
const char* serverRoot = "http://192.168.200.12:8080/demo/ecociteTeam/newEvent";

// Load Led library
#include <Adafruit_NeoPixel.h>
#define PIXEL_COUNT 8
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, D2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripB = Adafruit_NeoPixel(PIXEL_COUNT, D3, NEO_GRB + NEO_KHZ800);
//FAIRE PRESENTOIRE TABLETTE
// Pin declarations
int buttonState = 0;
int buttonStateB = 0;
boolean flag = false;
boolean flagB = false;
const int buttonPin = D0;  
const int buttonPinB = D1;


boolean flagStop = true;
unsigned long durationLightOnOne = 30000;
unsigned long timeOne;
boolean flagStopB = true;
unsigned long durationLightOnB = 10000000;
unsigned long timeB;

void setup() {
  Serial.begin(115200);
  
  strip.begin();
  strip.show();
  
  stripB.begin();
  stripB.show();
  
  pinMode(buttonPin, INPUT);
  pinMode(buttonPinB, INPUT);

  Serial.begin(115200); // Serial connection
  WiFi.begin(ssid, password); // WiFi connection

  while (WiFi.status() != WL_CONNECTED) {  // Wait for the WiFI connection completion
    delay(500);
    Serial.println("Waiting for connection");
  }
  httpPostDataToServer();
  
}

void loop() {
  onButton();
}
//ip:8080/ecociteTeam/newEvent -> {"Values" : { "Value1" : "1", "Value2" : "2", "Value3" : "3" } }
//ssid : dome_fablab
//mdp : dome_fablab

void httpPostDataToServer(){
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status

   HTTPClient http;    //Declare object of class HTTPClient

   String json = "{\"values\" : {\"value1\" : \"";
   
   if (flag) {
    json.concat(100);
    json.concat("\", \"value2\" : \"");
    json.concat(50);
   } else {
    json.concat(0);
    json.concat("\", \"value2\" : \"");
    json.concat(0);
   }

   if (flagB) {
    json.concat("\" , \"value3\" : \"");
    json.concat(300);
   } else {
    json.concat("\" , \"value3\" : \"");
    json.concat(0);
   }

   json.concat("\"}}");
   
   http.begin(serverRoot);      //Specify request destination
   http.addHeader("Content-Type", "application/json");  //Specify content-type header

   int httpCode = http.POST(json);   //Send the request
   String payload = http.getString();                  //Get the response payload

   Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payload

   http.end();  //Close connection

 } else {
    Serial.println("Error in WiFi connection");   
 }
}


void onButton(){
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW ) {
    flag = !flag;
    do {
        buttonState = digitalRead(buttonPin);
        delay(50);
    } while(buttonState == LOW);
    httpPostDataToServer();
  }
  if (flag){
    strip.setPixelColor(4, strip.Color(255, 255, 255));
    strip.setPixelColor(7, strip.Color(255, 255, 255));
    strip.show();
    flagStop = true;
  } else {
    strip.setPixelColor(4, strip.Color(0, 0, 0));
    strip.setPixelColor(7, strip.Color(0, 0, 0));
    strip.show();
    if(flagStop){
      timeOne = millis();
      flagStop = false;
    } else{
      if ((millis()- timeOne)>durationLightOnOne) {
        flag = true;
        flagStop = true;
        httpPostDataToServer();
      }
    } 
  }
  
  buttonStateB = digitalRead(buttonPinB);
  if (buttonStateB == LOW ) {
    flagB = !flagB;
    do {
        buttonStateB = digitalRead(buttonPinB);
        delay(50);
    } while(buttonStateB == LOW);
    httpPostDataToServer();
  }
  if (flagB){
    stripB.setPixelColor(2, strip.Color(255, 255, 255));
    stripB.show(); 
    flagStopB=true;
  } else {
    stripB.setPixelColor(2, strip.Color(0, 0, 0));
    stripB.show(); 
    
    if(flagStopB){
      timeB = millis();
      flagStopB = false;
    } else{
      if ((millis()- timeB)>durationLightOnB) {
        flagB = true;
        flagStopB = true;
        httpPostDataToServer();
      }
    }
  }
}
