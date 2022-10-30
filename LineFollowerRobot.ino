  // Including Header File
  #include "Header.h"

  // Main uC Setup
  void setup() {

    // Serial Ports Start
    Serial.begin(9600);

    // Start Motors Libraries
    LM.begin();
    RM.begin();

    // Setting The Motors Speed
    LM.setSpeed(255);    
    RM.setSpeed(255);
    
  }

  // Main uC Program Loop
  void loop() {
    
    movment();
    delay(100);
    
  }

  // Movment Algo
  void movment() {
    
    // Reading From Sensor
    bool lx = LS.read();
    bool rx = RS.read();
  
    // Movment Decision Making
    if(lx == false && rx == false) {
      if(State != FORWARD) {
        State = FORWARD;
        Serial.println("Move Forward");
        LM.setSpeed(255);
        RM.setSpeed(255);
        LM.forward();
        RM.forward();
      }
    } else if(lx == true && rx == false) {
      if(State != RIGHT) {
        State = RIGHT;
        Serial.println("Move Right");
        LM.setSpeed(100);
        RM.setSpeed(255);
        LM.forward();
        RM.forward();
      }
    } else if(lx == false && rx == true) {
      if(State != LEFT) {
        State = LEFT;
        Serial.println("Move Left");
        LM.setSpeed(255);
        RM.setSpeed(100);
        LM.forward();
        RM.forward();
      }
    } else {
      if(State != BREAK) {
        State = BREAK;
        Serial.println("Stop");
        LM.breakOn();
        RM.breakOn();
      }
    }
    
  }
