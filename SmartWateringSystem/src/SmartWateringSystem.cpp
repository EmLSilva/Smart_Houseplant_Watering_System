/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include <Adafruit_MQTT.h>
#include "credentials.h"
#include <Adafruit_MQTT/Adafruit_MQTT_SPARK.h>
#include <Adafruit_MQTT/Adafruit_MQTT.h>

const int soilMoisturePin = A2;
const int pumpPin = D13;
//const int publishTime = 30000; 


int buttonValue;  
int moistureValue;
int pumpLastTime;
int currentTime;
int activatePump;
int deactivatePump;

/************ Global State (you don't need to change this!) ***   ***************/ 
TCPClient TheClient; 

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details. 
Adafruit_MQTT_SPARK mqtt(&TheClient,AIO_SERVER,AIO_SERVERPORT,AIO_USERNAME,AIO_KEY); 

/****************************** Feeds ***************************************/ 
// Setup Feeds to publish or subscribe 
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname> 
Adafruit_MQTT_Subscribe buttonFeed = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/buttononoff"); 
Adafruit_MQTT_Publish soilFeed=Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Moisture");

/************Declare Variables*************/
unsigned int last, lastTime;
float subValue,pubValue;

/************Declare Functions*************/
void MQTT_connect();
bool MQTT_ping();
float buttononoff;
float randomnumber;

SYSTEM_MODE(SEMI_AUTOMATIC);


void setup() {
  WiFi.on();
   WiFi.connect();
   Serial.begin(9600);
   pinMode(soilMoisturePin, INPUT);
   pinMode(pumpPin,OUTPUT);
   Time.zone(-7);
   Particle.syncTime();
   Wire.begin();
   mqtt.subscribe(&buttononoff);
   
}

void loop() {
  //currentTime=millis();
  moistureValue=analogRead(soilMoisturePin); 
  //if((currentTime-pumpLastTime>=10000)){
     if(moistureValue>=2500){
      digitalWrite(pumpPin, HIGH);
      delay(500);
      digitalWrite(pumpPin, LOW);
    }
  if(moistureValue<2500){
       digitalWrite(pumpPin, LOW);
    }
     // pumpLastTime=millis();
   
  }
      
Adafruit_MQTT_Subscribe *subscription;
  while ((subscription=mqtt.readSubscription(4000))) {
    if (subscription==&buttononoff) {
        buttonValue=atoi((char *)buttonFeed.lastread);
  Serial.printf("buttonValue%i\n",buttonValue);
    }
  }
    if (buttonValue==HIGH) {
      digitalWrite(pumpPin, HIGH);
      Serial.printf("Manual Water Button ON \n");
    }
    else {
      digitalWrite(pumpPin, LOW);
      Serial.printf("Manual Water Button OFF \n");
    }

    // if((millis()-lastTime>30000)){
    // if(mqtt.Update()){
    //      SoilFeed.publish(moistureValue);
    //     Serial.printf("Publishing %i \n",moistureValue); 
    //     lastTime=millis();
    //   } 
  //}
 // Serial.printf("MQTT Connected!\n");

bool MQTT_ping(){
  static unsigned int last;
  bool pingStatus;

  if ((millis()-last)>120000) {
      Serial.printf("Pinging MQTT \n");
      pingStatus = mqtt.ping();
      if(!pingStatus) {
        Serial.printf("Disconnecting \n");
        mqtt.disconnect();
      }
      last = millis();
  }
  return pingStatus;
}