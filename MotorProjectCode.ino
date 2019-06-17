
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "NagRaj"
#define WLAN_PASS       "7060196036"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "NagRaj0308"
#define AIO_KEY         "f6949289e31843668555900441fb4396"

/************ Global State (you don't need to change this!) ******************/

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

Adafruit_MQTT_Subscribe l = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/left");

Adafruit_MQTT_Subscribe r = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/right");

Adafruit_MQTT_Subscribe f = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/forward");

Adafruit_MQTT_Subscribe b = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/backward");

Adafruit_MQTT_Subscribe s = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/stop");

/*************************** Sketch Code ************************************/


void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(D0,OUTPUT);
  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT);
  pinMode(D3,OUTPUT);
  pinMode(D5,OUTPUT);
  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&l);
  mqtt.subscribe(&r);
  mqtt.subscribe(&s);
  mqtt.subscribe(&f);
  mqtt.subscribe(&b);
}

uint32_t x=0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &l) {

      Serial.println((char *)l.lastread);
      uint16_t num1 = atoi((char *)l.lastread);
      if ( num1 == 1) {
      digitalWrite(D0,HIGH);
      digitalWrite(D1,LOW);
      digitalWrite(D2,LOW);
      digitalWrite(D3,LOW);
      digitalWrite(D5,LOW);
      delay(500);
      digitalWrite(D0,HIGH);
      digitalWrite(D1,LOW);
      digitalWrite(D2,HIGH);
      digitalWrite(D3,LOW);
    }}

    if (subscription == &r) {

      Serial.println((char *)r.lastread);
      uint16_t num1 = atoi((char *)r.lastread);
      if ( num1 == 1) {
      digitalWrite(D5,LOW);
      digitalWrite(D0,LOW);
      digitalWrite(D1,LOW);
      digitalWrite(D2,HIGH);
      digitalWrite(D3,LOW);
      delay(500);
      digitalWrite(D0,HIGH);
      digitalWrite(D1,LOW);
      digitalWrite(D2,HIGH);
      digitalWrite(D3,LOW);
      
    }}

    if (subscription == &f) {

      Serial.println((char *)f.lastread);
      uint16_t num1 = atoi((char *)f.lastread);
      if ( num1 == 1) {
      digitalWrite(D0,HIGH);
      digitalWrite(D1,LOW);
      digitalWrite(D2,HIGH);
      digitalWrite(D3,LOW);
      digitalWrite(D5,LOW);
    }}

    if (subscription == &b) {

      Serial.println((char *)b.lastread);
      uint16_t num1 = atoi((char *)b.lastread);
      if ( num1 == 1) {
      digitalWrite(D0,LOW);
      digitalWrite(D1,HIGH);
      digitalWrite(D2,LOW);
      digitalWrite(D3,HIGH);
      digitalWrite(D5,LOW);
    }}

    if (subscription == &s) {

      Serial.println((char *)s.lastread);
      uint16_t num1 = atoi((char *)s.lastread);
      if ( num1 == 1) {
     digitalWrite(D0,LOW);
     digitalWrite(D1,LOW);
     digitalWrite(D2,LOW);
     digitalWrite(D3,LOW);
     digitalWrite(D5,HIGH);
      }
    }
  }
}
// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
