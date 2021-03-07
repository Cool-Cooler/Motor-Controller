#include <AccelStepper.h>
#include <AccelStepperWithDistance.h>
#include <HTTPClient.h>
#include <WiFiManager.h>

// Motor pin definitions
#define motorPin1  27         // IN1 & IN4 on the ULN2003 drivers
#define motorPin2  14         // IN2 & IN3 on the ULN2003 drivers  
#define motorPin3  12         // IN3 & IN2 on the ULN2003 drivers
#define motorPin4  13         // IN4 & IN1 on the ULN2003 drivers
 
// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper(8, motorPin1, motorPin3, motorPin2, motorPin4);

int currentPos = 0, moveDist = 10000, CURR_POS = 0;
int positions[3] = {0, -15000, -30000};
bool position_packet_sent = true;


//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}
 
void setup() {
  // Motor Setup
  stepper.setMaxSpeed(2500.0);
  stepper.setAcceleration(1000.0);
  stepper.setSpeed(2500);
  stepper.moveTo(0);
  pinMode(32, INPUT);
  pinMode(33, INPUT);
  pinMode(26, INPUT);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  wm.setAPCallback(configModeCallback);
  if (!wm.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(1000);
  }
  Serial.println("Connected to the WiFi network");
}
 
void loop() {
  // Set steppers going
  stepper.run();

  // if you've reached the last destination, then procede
  if(stepper.distanceToGo() == 0){
    // Take a photo, when you first reach a new position
    if(!position_packet_sent){
      Serial.println("1");
      HTTPClient http;
      Serial.println("2");
      http.begin("http://192.168.0.57/capture");
      Serial.println("http://192.168.0.57/capture");
      int httpCode = http.GET();
      if (httpCode > 0) { //Check for the returning code
          String payload = http.getString();
          Serial.println(httpCode);
          Serial.println(payload);
        }
      else {
        Serial.println("Error on HTTP request");
      }
      http.end(); //Free the resources
//      TODO: communicate current position to camera
//      Serial.print("Packet sent : ");
//      Serial.println(CURR_POS);
      
      position_packet_sent = true;
    }
    // If any position trigger have gone off, then move to that position.
    if(digitalRead(32) == HIGH){    
      CURR_POS = 0;
      position_packet_sent = false;
      stepper.moveTo(positions[CURR_POS]);
    }
    else if(digitalRead(33) == HIGH){  
      CURR_POS = 1;
      position_packet_sent = false;
      stepper.moveTo(positions[CURR_POS]);
    }
    else if(digitalRead(26) == HIGH){
      CURR_POS = 2;
      position_packet_sent = false;
      stepper.moveTo(positions[CURR_POS]);
    }
  }
}




            
