/************************* Værstasjon setup ***********************************/ 
#include <ESP8266WiFi.h>          //Internett modul/sensor bibliteket
#include "Adafruit_MQTT.h"        //MQTT protokoll bibliotek
#include "Adafruit_MQTT_Client.h" //MQTT klient bibliotek
/************************* WiFi Access Point **********************************/ 
#define WLAN_SSID       "[SSID]" 
#define WLAN_PASS       "[Wi-Fi password]" 
#define MQTT_SERVER     "[Host IP adress]" // static ip address
#define MQTT_PORT       1883                    
#define MQTT_USERNAME   "" 
#define MQTT_PASSWORD   "" 
/**************************** Global State ***********************************/ 
// Lager en ESP8266 WiFiClient klasse som kobles til MQTT serveren. 
WiFiClient client; 
// Etter opp MQTT client klassen ved å legge inn WiFi client, MQTT server og login detaljer. 
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD); 
/******************************* Feeds ***************************************/ 
// Definerer en feed kalt ute
// Legg merke til at MQTT veier for AIO følger formen: <bruker_navn>/feeds/<feed_navn> 
Adafruit_MQTT_Publish UTE = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/ute/pi");   
/**************************** Sketch Code ************************************/ 

//Henter inn DHT22 sensor bibliotek og definerer pin som sensoren er koblet til
#include "DHT.h"
#define DHTpin D2
DHT dht;
 
void setup() { 
 WiFi.mode (WIFI_STA); // skrur av AP (Access point) mode
 Serial.begin(115200); 
 delay(10); 
 Serial.println(F("RPi-ESP-MQTT")); 
 /********************* Kobler til WiFi access point. ***********************/
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
 //DHTsensor
 dht.setup(DHTpin);
} 
uint32_t x=0;
 
void loop() {
  float temp = dht.getTemperature(), humi = dht.getHumidity();
  Serial.print("Temperatur: ");
  Serial.println(temp);
  Serial.print("Fuktighet: ");
  Serial.println(humi); 
 // MQTT_connect() funskjonen kjører ved opp start og automatisk hvis den blir koblet av). 
 // Funksjonen ligger i vedlagt kodefil: tilkobling
  MQTT_connect();   
 
  UTE.publish(temp+500);
  UTE.publish(humi);
  delay(2500);
}
