/* 
 * Project newHue
 * Description: Example code of IoT Classroom Hue and Wemo
 * Author: Brian Rashap
 * Date: 24-OCT-2023
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "IoTClassroom_CNM.h"

SYSTEM_MODE(MANUAL); //control logging into classroom router
//SYSTEM_THREAD(ENABLED); Caution - will allow code to execute before fully connected

const int BULB=1; 
int color;

void setup() {
  Serial.begin(9600);
  waitFor(Serial.isConnected,15000);

  WiFi.on();
  WiFi.clearCredentials();
  WiFi.setCredentials("IoTNetwork");
  
  WiFi.connect();
  while(WiFi.connecting()) {
    Serial.printf(".");
  }
  Serial.printf("\n\n");
}

void loop() {
  Serial.printf("Setting color of bulb %i to color %06i\n",BULB,HueRainbow[color%7]);
  setHue(BULB,true,HueRainbow[color%7],random(32,255),255);
  color++;
  delay(5000);
}