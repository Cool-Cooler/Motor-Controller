#include <AccelStepper.h>
#include <AccelStepperWithDistance.h>
#include <HTTPClient.h>
#include <WiFiManager.h>

 
// Motor pin definitions
#define motorPin1  27         // IN1 on the ULN2003 driver 1
#define motorPin2  14         // IN2 on the ULN2003 driver 1
#define motorPin3  12         // IN3 on the ULN2003 driver 1
#define motorPin4  13        // IN4 on the ULN2003 driver 1
 
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

  // Bluetooth Setup
  Serial.begin(115200);
  /*
//  SerialBT.begin("ESP32test", true); 
//  Serial.println("Bluetooth Master started");
//  //Has master connected to camera?
//  connected = SerialBT.connect(BLE_name);
//  if(connected) {
//    Serial.println("Connected Succesfully!");
//  } else {
//    while(!SerialBT.connected(10000)) {
//      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app."); 
//    }
//  }
//  if (SerialBT.disconnect()) {
//    Serial.println("Disconnected Succesfully!");
//  }
//  // this would reconnect to the name(will use address, if resolved) or address used with connect(name/address).
//  SerialBT.connect();
//  
//  Serial.println("Setup completed!");
//  http.begin("http://192.168.0.57/capture");
*/
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  wm.setAPCallback(configModeCallback);
  if (!wm.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(1000);
  }

/*
//  WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(1000);
//    Serial.println("Connecting to WiFi..");
//  }
*/
  Serial.println("Connected to the WiFi network");
}
 
void loop() {
  // Set steppers going
  stepper.run();

  // if you've reached the last destination, then procede
  if(stepper.distanceToGo() == 0){
    // If the camera hasn't taken a picture, then tell it to
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
    
//      Serial.print("Packet sent : ");
//      Serial.println(CURR_POS);
      
      position_packet_sent = true;
    }
    // If any position trigger have gone off, then move there.
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
  
//  if (SerialBT.available()) {
//    Serial.write(SerialBT.read());
//  }
  
}




            
