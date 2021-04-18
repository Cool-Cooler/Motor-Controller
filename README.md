# Motor-Controller

## Purpose
* Initialise internet connection.
* Wait for starting signal (manual begin command, fridge door detected closing).
* Initialise client, connected to [Web-Controlled-ESP-EYE](https://github.com/Cool-Cooler/Web-Controlled-ESP-EYE).
  1. When in position 1 - command ESP-EYE to take photo 1. Wait until action is complete. 
  2. Command Stepper Motors to move to position 2.
  3. When in position 2 - command ESP-EYE to take photo 2. Wait until action is complete. 
  4. Command Stepper Motors to move to position 3.
  5. When in position 3 - command ESP-EYE to take photo 3. Wait until action is complete. 
  6. Command Stepper Motors to return to home position.
* Disconnect from server.
* Put motors into powered-off state until new starting signal is detected.
 
  *  *  *  *  *

## Wiring Diagram
![Hardware(1)](https://user-images.githubusercontent.com/30498489/115150341-d1600400-a05f-11eb-9d02-9134287702a5.png)


## CAD vs Real design implementation
![Combined hardware](https://user-images.githubusercontent.com/30498489/115150463-6cf17480-a060-11eb-936d-65fd7c3568d3.PNG)


## Working Demo of System
https://user-images.githubusercontent.com/30498489/115150762-babaac80-a061-11eb-92b8-7f006a7cb054.mp4
