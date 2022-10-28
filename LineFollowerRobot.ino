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
    LM.SetSpeed(255);    
    RM.SetSpeed(255);
    
  }

  // Main uC Program Loop
  void loop() {
    
    movment();
    delay(100);
    
  }

  // Movment Algo
  void movment() {
    
    // Reading From Sensor
    bool lx = LS.Read();
    bool rx = RS.Read();
  
    // Movment Decision Making
    if(lx == false && rx == false) {
      if(State != FORWARD) {
        State = FORWARD;
        Serial.println("Move Forward");
        LM.SetSpeed(255);
        RM.SetSpeed(255);
        LM.Forward();
        RM.Forward();
      }
    } else if(lx == true && rx == false) {
      if(State != RIGHT) {
        State = RIGHT;
        Serial.println("Move Right");
        LM.SetSpeed(100);
        RM.SetSpeed(255);
        LM.Forward();
        RM.Forward();
      }
    } else if(lx == false && rx == true) {
      if(State != LEFT) {
        State = LEFT;
        Serial.println("Move Left");
        LM.SetSpeed(255);
        RM.SetSpeed(100);
        LM.Forward();
        RM.Forward();
      }
    } else {
      if(State != BREAK) {
        State = BREAK;
        Serial.println("Stop");
        LM.Break();
        RM.Break();
      }
    }
    
  }
