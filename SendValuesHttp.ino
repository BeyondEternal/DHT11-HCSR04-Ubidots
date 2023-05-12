// This example sends data to multiple variables to
// Ubidots through HTTP protocol.

/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"


#include "DHT.h"

#define DHTPIN 0     

const int trigPin = 12;
const int echoPin = 14;

#define SOUND_VELOCITY 0.034
#define DHTTYPE DHT11   

long duration;
float distanceCm;
float distanceInch;
DHT dht(DHTPIN, DHTTYPE);


const char* UBIDOTS_TOKEN = "...";  // Put here your Ubidots TOKEN
const char* WIFI_SSID = "...";      // Put here your Wi-Fi SSID
const char* WIFI_PASS = "...";      // Put here your Wi-Fi password
Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);

/****************************************
 * Auxiliar Functions
 ****************************************/

// Put here your auxiliar functions

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  Serial.begin(115200);
  ubidots.wifiConnect(WIFI_SSID, WIFI_PASS);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  dht.begin();}

void loop() {
   float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  
  distanceCm = duration * SOUND_VELOCITY/2;
  ;
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
  
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);


  ubidots.add("Humedad", h);
  ubidots.add("Temperatura", f);
  ubidots.add("Distancia", distanceCm);
  bool bufferSent = false;
  bufferSent = ubidots.send();  // Will send data to a device label that matches the device Id

  if (bufferSent) {
    // Do something if values were sent properly
    Serial.println("Values sent by the device");
  }

  delay(5000);
}
