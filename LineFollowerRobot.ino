  // Including Header File
  #include "Header.h"

  // Main uC Setup
  void setup() {

    // Serial Ports Start
    Serial.begin(9600);

    // while(1) {
    //   Serial.println(String(LS.raw()) + " " + String(RS.raw()));
    // }

    // Starting The Servo
    Rotate.begin();

    // Setting Your Limits
    Rotate.setMin(0);
    Rotate.setMax(96);
    Rotate.setInit(45);

    // Go To Initial Position
    Rotate.goInit();

    // Move To 50% Between Your Limits
    Rotate.percent(50);

    // Start LEFT Motors
    LM.begin();
    LM.invert();
    LM.forward();
    LM.setMin(80);
    LM.setMax(150);
    LM.stop();    

    // Start Right Motors
    RM.begin();
    RM.invert();
    RM.forward();
    RM.setMin(80);
    RM.setMax(150);
    RM.stop();

    // Inverting Sensors
    // LS.invert();
    // RS.invert();
    
  }

  // Main uC Program Loop
  void loop() {
    
    movment();
    
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
        LM.goMin();
        RM.goMin();
        LM.forward();
        RM.forward();
      }
    } else if(lx == true && rx == false) {
      if(State != RIGHT) {
        State = RIGHT;
        Serial.println("Move Right");
        LM.goMax();
        RM.goMax();
        LM.backward();
        RM.forward();
      }
    } else if(lx == false && rx == true) {
      if(State != LEFT) {
        State = LEFT;
        Serial.println("Move Left");
        LM.goMax();
        RM.goMax();
        LM.forward();
        RM.backward();
      }
    } else {
      if(State != STOP) {
        State = STOP;
        Serial.println("Stop");
        LM.stop();
        RM.stop();
      }
    }
    
  }
