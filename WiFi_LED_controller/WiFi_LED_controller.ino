#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FastLED.h>

#include "index.h" //Include the index page

/* Variables */

/** SETTINGS **/
#define WIFI_NAME "LED Strip"
#define WIFI_PASSWORD "12345"

//Sets which pins control the LED strips
#define PIN_1 1
#define PIN_2 15
//Total number of LED strips
const int numLEDstrips = 2;
//Max number of LEDs per strip
const int LEDcount = 300;

//How smoothly the LEDs update in rainbowsolid mode, higher value = smoother
const int smoothness = 5;
//Default time (in milliseconds) between each LED colour update
int Delay = 50;
/** SETTINGS **/

//LED hardware vars
CRGB leds[numLEDstrips][LEDcount];
const int redLED = 16;
const int blueLED = 2;

//LED software vars
int current[numLEDstrips];
int currentLEDstrip;
int counter[numLEDstrips];
int solidCounter[numLEDstrips];
CRGB col[numLEDstrips];

//Server variables
AsyncWebServer server(80); //Create an AsyncWebServer on port 80
unsigned long lastVisit = 0;


/* Functions */

int hexToStr(String s, int i){
  return strtoul(s.substring(i, i+2).c_str(), nullptr, 16);
}

//Replaces placeholder with actual values
String processor(const String& var){
  return String();
}


void setup() {
  //Create access point
  WiFi.softAP(WIFI_NAME, WIFI_PASSWORD);
  WiFi.softAPConfig(IPAddress(1,2,3,4), IPAddress(1,1,1,1), IPAddress(255,255,255,0));
  
  //Page request handler
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(blueLED, LOW);
    lastVisit = millis();
    request->send_P(200, "text/html", index_html, processor);
  });
  
  //Mode settings
  server.on("/rainbow", HTTP_GET, [](AsyncWebServerRequest *request){
    currentLEDstrip = request->getParam("LEDstrip")->value().toInt();
    current[currentLEDstrip] = 2;
    Delay = request->getParam("delay")->value().toInt();
    request->redirect("/");
  });
  server.on("/rainbowsolid", HTTP_GET, [](AsyncWebServerRequest *request){
    currentLEDstrip = request->getParam("LEDstrip")->value().toInt();
    current[currentLEDstrip] = 1;
    Delay = request->getParam("delay")->value().toInt();
    request->redirect("/");
  });
  server.on("/solid", HTTP_GET, [](AsyncWebServerRequest *request){
    currentLEDstrip = request->getParam("LEDstrip")->value().toInt();
    current[currentLEDstrip] = 0;
    if (request->hasParam("col")) {
      String s = request->getParam("col")->value();
      col[currentLEDstrip] = CRGB(hexToStr(s, 1), hexToStr(s, 3), hexToStr(s, 5));
    }
    Delay = 100;
    request->redirect("/");
  });
  
  //Start server
  server.begin();

  //LED setup
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  for (int i=0; i<numLEDstrips; i++) {
    current[i] = 1;
    col[i] = CRGB(255, 0, 0);
    counter[i] = 0;
    solidCounter[i] = 0;
  }
  delay(1000);
  FastLED.addLeds<WS2812B, PIN_1, GRB>(leds[0], LEDcount);
  FastLED.addLeds<WS2812B, PIN_2, GRB>(leds[1], LEDcount);
}

void loop() {
  if (millis()-lastVisit > 1000) {
    digitalWrite(blueLED, HIGH);
  }

  for (int i=0; i<numLEDstrips; i++) {
    counter[i]++;
    counter[i] %= 255;
  }
  
  for (int i=0; i<2; i++) {
    if (current[i] == 0) { //solid
      fill_solid(leds[i], LEDcount, col[i]);
    } else if (current[i] == 1) { //rainbowsolid
      for (int j=0; j<LEDcount; j++) {
        if (j%smoothness == solidCounter[i]) {
          leds[i][j].setHue(counter[i]);
        }
      }
      solidCounter[i]++;
      solidCounter[i] %= smoothness;
      if (solidCounter[i] != 0){
        counter[i]--;
      }
    } else if (current[i] == 2) { //rainbow
      fill_rainbow(leds[i], LEDcount, counter[i]);
    }
  }
  
  FastLED.show();
  delay(Delay);
}
