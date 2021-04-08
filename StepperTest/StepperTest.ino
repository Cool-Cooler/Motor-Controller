#include <AccelStepper.h>
#include <AccelStepperWithDistance.h>
#include <HTTPClient.h>
#include <WiFiManager.h>

// Motor pin definitions
#define motorPin1  27         // IN1 & IN4 on the ULN2003 drivers
#define motorPin2  14         // IN2 & IN3 on the ULN2003 drivers  
#define motorPin3  12         // IN3 & IN2 on the ULN2003 drivers
#define motorPin4  13         // IN4 & IN1 on the ULN2003 drivers
// Relay attached on pin 25
#define relayPin 25
 
// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper(8, motorPin1, motorPin3, motorPin2, motorPin4);

int current_pos = 0, tuning_dist= 10000, CURR_POS = 0;
int positions[3] = {0, -15000, -30000};
bool position_packet_sent = true, on_state = false;


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
  pinMode(25, OUTPUT);
  digitalWrite(relayPin, LOW);
  
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
  
  // State machine is turned on with a switch
  if(on_state){
    
    // If you've reached the last destination, then procede
    if(stepper.distanceToGo() == 0){
      Serial.print("Current yolk ");
      Serial.println(CURR_POS);
            
      // Take a photo, when you first reach a new position
      digitalWrite(relayPin, LOW);
      HTTPClient http;
      http.begin("http://192.168.0.45/capture");
      int httpCode = http.GET();
      
      //Check for the returning code
      if (httpCode > 0) { //Check for the returning code
          String payload = http.getString();
          Serial.println(httpCode);
          Serial.println(payload);
        }
      else {
        Serial.println("Error on HTTP request");
      }
      //Free the resources
      http.end(); 
      // TODO: communicate current position to camera

      if(CURR_POS < 3){
        digitalWrite(relayPin, HIGH);
        CURR_POS++;
        Serial.print("new pos ");
        Serial.println(CURR_POS);
      }
      else{
        Serial.println("Waiting and restarting");
        CURR_POS = 0;
        on_state = false;
      }
      
      // Move the stepper motor to the position
      if(CURR_POS < 3){
        Serial.println("Moving time");
        stepper.moveTo(positions[CURR_POS]);
      }
//      stepper.moveTo(30000);
//      on_state = false;
    }
  }

  if(stepper.distanceToGo() != 0){
    digitalWrite(relayPin, HIGH);
  }
  else{
    digitalWrite(relayPin, LOW);    
  }

  if(digitalRead(26) == HIGH || digitalRead(33) == HIGH || digitalRead(32) == HIGH){
    on_state = true;
  }
}
