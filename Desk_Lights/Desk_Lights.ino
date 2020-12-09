/*
   Desk Lights
   Created by Gary Tou
	 https://github.com/garytou2/Desk-Lights
*/
#include <Adafruit_NeoPixel.h>
#include <FirebaseESP8266.h>
#include "ESP8266WiFi.h"


// LED Strip constants
int pin = 14; //check Adafruit Feather HUZZAH with ESP8266 pin out for more pin options
int numLeds = 60;
int brightness = 255;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLeds, pin, NEO_GRBW + NEO_KHZ800);

// WIFI
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "get off my LAN :)"

// Firebase
#define FIREBASE_HOST "YOUR_DATABASE_URL" //get from Firebase
#define FIREBASE_AUTH "YOUR_DATABASE_AUTH" //get from Firebase, under Service accounts

FirebaseData fb_rgbw;

// RGBW colors
uint8_t rgbw_r = 0;
uint8_t rgbw_g = 0;
uint8_t rgbw_b = 0;
uint8_t rgbw_w = 0;

/* ----------------------------------- ARDUINO METHODS ---------------------------------*/
void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println("Serial ready");

  ledSetup();
  wifiSetup();
  firebaseSetup();
}

void loop() {}

/* ----------------------------------- LED STRIP ---------------------------------*/
void ledSetup() {
  strip.begin();
  strip.show();
  strip.setBrightness(brightness);
}

/* ----------------------------------- WIFI ---------------------------------*/
void wifiSetup() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Waiting to connect...");
  delay(500);
  while (WiFi.status() != WL_CONNECTED)
  { //Wait for connection
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
}

/* ----------------------------------- FIREBASE ---------------------------------*/
String parentPath = "/rgbw";
String childPath[4] = {"/r", "/g", "/b", "/w"}; //child path to individual RGBW value on firebase
size_t childPathSize = 4; //size of the array above

void firebaseSetup() {
  // connects to firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  // optional settings
  fb_rgbw.setBSSLBufferSize(1024, 1024);
  fb_rgbw.setResponseSize(1024);

  // begin multi path stream (this way we are updated of any changes to the RBGW values)
  if (!Firebase.beginMultiPathStream(fb_rgbw, parentPath, childPath, childPathSize))  {
    Serial.println("COULD NOT BEGIN STREAM");
    Serial.println("REASON: " + fb_rgbw.errorReason());
    Serial.println();
  }

  Firebase.setMultiPathStreamCallback(fb_rgbw, streamCallback, streamTimeoutCallback);
}

void streamCallback(MultiPathStreamData stream) {
  //this method is called whenever there is a chage to the RGBW values on Firebase
  Serial.println();
  Serial.println("Stream available...");

  size_t numChild = sizeof(childPath) / sizeof(childPath[0]);

  for (size_t i = 0; i < numChild; i++) {
    if (stream.get(childPath[i])) {
      // update their respective varible
      if (stream.dataPath == "/r") {
        rgbw_r = stream.value.toInt();
      } else if (stream.dataPath == "/g") {
        rgbw_g = stream.value.toInt();
      } else if (stream.dataPath == "/b") {
        rgbw_b = stream.value.toInt();
      } else if (stream.dataPath == "/w") {
        rgbw_w = stream.value.toInt();
      } else {
        Serial.println("Unknown stream data path for RGBW");
      }
      Serial.println("path: " + stream.dataPath + ", type: " + stream.type + ", value: " + stream.value);
    }
  }

  // update the LED colors
  strip.fill(strip.Color(rgbw_r, rgbw_g, rgbw_b, rgbw_w));
  strip.show();

  Serial.println();
}

void streamTimeoutCallback(bool timeout) {
  if (timeout) {
    Serial.println();
    Serial.println("Stream timeout, resume streaming...");
    Serial.println();
  }
}
