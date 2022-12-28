//======================================================
//  MOTOR MACRO FUNCTIONS  
//======================================================

  void stateFORWARD() {
    if(State != FORWARD) {
      State = FORWARD;
      // Serial.println("Move Forward");
      LM.goMin();
      RM.goMin();
      LM.forward();
      RM.forward();
    }
  }

//------------------------------------------------------

  void stateBACKWARD() {
    if(State != BACKWARD) {
      State = BACKWARD;
      // Serial.println("Move Forward");
      LM.goMin();
      RM.goMin();
      LM.backward();
      RM.backward();
    }
  }

//------------------------------------------------------

  void stateLEFT() {
    if(State != LEFT) {
      State = LEFT;
      // Serial.println("Move Left");
      LM.goMax();
      RM.goMax();
      LM.forward();
      RM.backward();
    }
  }

//------------------------------------------------------

  void stateRIGHT() {
    if(State != RIGHT) {
      State = RIGHT;
      // Serial.println("Move Right");
      LM.goMax();
      RM.goMax();
      LM.backward();
      RM.forward();
    }
  }

//------------------------------------------------------

  void stateSTOP() {
    if(State != STOP) {
      State = STOP;
      // Serial.println("Move Left");
      LM.stop();
      RM.stop();
    }
  }

//------------------------------------------------------
  
  void chargeRUN(int _delay) {
    LM.goMax();
    RM.goMax();
    LM.forward();
    RM.forward();
    delay(_delay);
    stateFORWARD();
  }

//======================================================
//  TERMINATOR DETECTOR - LINE FOLLOWER
//======================================================

  bool lineFollower() {

    // Reading From Sensor
    int lx    = readAVG(LS, 30);      // Left IR Sensor
    int rx    = readAVG(RS, 30);      // Right IR Sensor

    // Movment Decision Making
    if(lx == 0 && rx == 0) {
      stateFORWARD();
      return false;
    } 
    
    // Move Right
    else if(lx == 1 && rx == 0) {
      stateRIGHT();
      return false;
    } 
    
    // Move Left
    else if(lx == 0 && rx == 1) {
      stateLEFT();
      return false;
    }

    // Terminator Detected
    else {
      // STOP THE MOTORS
      stateSTOP();
      // Draft Delay
      delay(250);
      // Terminator Detected
      CURRENT_STOP += 1;
      // Return Success Flag
      return true;
    }

  }

//======================================================
//  Movment Algo  
//======================================================

  void movment(Stream &_serial) {

    // Intercept Serial Signal
    if(intercep(_serial)) {

      // Print Message To User
      _serial.println("Signal Recived Successfully");
      _serial.println("New Command   -> Bed Number : " + String(command.b) + ", Direction : " + String(command.d) + ", Pill (Type 1) : " + String(command.c1) + ", Pill (Type 2) : " + String(command.c2));
      _serial.println("Cur Position  -> " + String(CURRENT_STOP));
      _serial.println("Nxt Position  -> " + String(command.b - CURRENT_STOP) + " Bed");

      // Check If the Message Was New
      if(command.New == true) {

        // This Is Invalid ( No Bed After Final Position & No Bed Below 1)
        if(command.b > STOP_MAX || command.b < 1) {
          
          // Print ERROR Message
          _serial.println(JSON_MSG_NA);

          // Stop Motors
          stateSTOP();

          // Do Nothing And Return
          return;

        }
        
        // Continue Moving Forward & Line Follower
        else if(command.b > CURRENT_STOP) {

          // The Loop Will Continue Till The command.b == CURRENT_STOP
          while(command.b > CURRENT_STOP) {
            if(lineFollower()) {
              chargeRUN(120);
            }
          }

          // Stop Motors
          stateSTOP();

          // Do The Dumping Here
          execute();

        } 
        
        // To Move The Opposit Direction & Line Follower
        else if(command.b < CURRENT_STOP) {
          
          // Here The Robot Should Turn Around
          _serial.println(JSON_MSG_TR);

          // Stop Motors
          stateSTOP();

          // Do The Dumping Here
          execute();

        } 
        
        // We Are At The Same Stop So Dont Move Line Follower
        else if(command.b == CURRENT_STOP) {

          // Stop Motors
          stateSTOP();

          // Do The Dumping Here
          execute();

        }

      }

      // Make The Message Old
      command.New = false;

      // Send Back Confirmation Message
      _serial.println(JSON_MSG_OK);

    }

  }

//======================================================
//  New Movment Algo  
//======================================================

  void newMovment() {
    
    // Reading From Sensor
    int lx = readAVG(LS, 30);
    int rx = readAVG(RS, 30);

    // Movment Decision Making
    if(lx == 0 && rx == 0) {
      stateFORWARD();
    } 
    
    // Move Right
    else if(lx == 1 && rx == 0) {
      stateRIGHT();
    } 
    
    // Move Left
    else if(lx == 0 && rx == 1) {
      stateLEFT();
    } 
    
    // STOP THE MOTORS
    else if(lx == 1 && rx == 1) {

      if(State != STOP) {       

        // Serial.println("Stop");
        LM.stop();
        RM.stop();

        // This is Where It Continues Moving
        CURRENT_STOP += 1;

        // Deside Where To Dump
        switch(CURRENT_STOP) {
          case 1:
            dumpPills(0, 1, 1);
            // delay(300);
            break;
          case 2:
            dumpPills(180, 1, 2);
            // delay(300);
            break;
          case 3:
            dumpPills(0, 0, 1);
            // delay(300);
            break;
        }

        // This is Where It Stops Permanantly
        if(CURRENT_STOP == 3) {
          State = STOP;
          LM.stop();
          RM.stop();
          while(1) {}
        }

        // Continue Moving Forward
        chargeRUN(120);

      }
      
    }
    
  }

//======================================================
//  Calibrate IR Sensor  
//======================================================

  void calibrate(IRSensor &_sensor) {

    int x = 0;
    int min = 1203;
    int max = 0;

    while(1) {

      x = _sensor.raw();

      if(x > max) {
        max = x;
      }

      if(x < min) {
        min = x;
      }

      int th = ((max - min)/2) + min;

      // Display Reading
      Serial.println(String(min) + " " + String(max) + " " + String(th));

    }

  }

//======================================================
//  Dump Pills Functions
//======================================================

  void dumpPills(int angle, int num1, int num2) {

    // Make Sure That Dumping Is Not 0
    if(num1 == 0 && num2 == 0) {
      return;
    }

    // Rotate To Position
    Rotate.move(angle);
    delay(100);

    // Make Sure Num1 Not Equals 0 Then Dump
    if(num1 != 0) {
      for(int i=0; i<num1; i++) {
        dsp2.goMin();
        delay(PILL_DUMP_DELAY);
        dsp2.goMax();
        delay(PILL_DUMP_DELAY);
        dsp2.goMin();
        delay(PILL_DUMP_DELAY);
      }
    }

    // Make Sure Num2 Not Equals 0 Then Dump
    if(num2 != 0) {
      for(int i=0; i<num2; i++) {
        dsp1.goMin();
        delay(PILL_DUMP_DELAY);
        dsp1.goMax();
        delay(PILL_DUMP_DELAY);
        dsp1.goMin();
        delay(PILL_DUMP_DELAY);
      }
    }

    // Move Back To Initial Position
    Rotate.goInit();
    // delay(PILL_DUMP_DELAY);

  }

  void execute() {

    // Decide Direction
    switch(command.d) {
      case CENTER:  // Center = 0
        dumpPills(90, command.c1, command.c2);
        break;
      case LEFT:    // Left = 1
        dumpPills(180, command.c1, command.c2);
        break;
      case RIGHT:   // Right = 2
        dumpPills(0, command.c1, command.c2);
        break;
      default:      // Center = 0
        dumpPills(0, command.c1, command.c2);
        break;
    }

  }

//======================================================
//  Read Avarage Of IR Sensors  
//======================================================

  int readAVG(IRSensor &sensor, uint8_t times) {

    int result = 0;

    for(int i=0; i<times; i++) {
      result += sensor.read();
      delayMicroseconds(5);
    }

    result = result/times;

    return result;

  }  

//======================================================
//  Intercept Signal With JSON Content
//======================================================

  bool intercep(Stream &serial) {

    // Preparing String Buffer
    String buffer = "";

    // Reading Serial String
    if(serial.available()) {
      buffer = serial.readStringUntil("\n");
      while(serial.available()) { serial.read(); }
      buffer.trim();
    } else {
      return false;
    }

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, buffer);

    // Test if parsing succeeds.
    if (error) {
      doc.clear();
      Serial.println(JSON_MSG_ER);
      return false;
    }

    // Getting The Readings
    uint8_t b   = doc["b"];
    uint8_t d   = doc["d"];
    uint8_t c1  = doc["c1"];
    uint8_t c2  = doc["c2"];

    // Move Data To Global Variable
    command.b   = b;
    command.d   = d;
    command.c1  = c1;
    command.c2  = c2;
    command.New = true;

    // Clearing Memory
    doc.clear();

    // Return True Indicator
    return true;

  }

//======================================================
//  Intercept Signal With JSON Content
//======================================================

  String readFromSerial(Stream &serial) {

    // Preparing String Buffer
    String buffer = "";

    // Reading Serial String
    if(serial.available()) {
      buffer = serial.readStringUntil("\n");
      while(serial.available()) { serial.read(); }
      buffer.trim();
      return buffer;
    } else {
      return NULL;
    }

  }

  void clearSerial(Stream &_serial) {
    while(_serial.available()) {
      _serial.read();
    }
  }