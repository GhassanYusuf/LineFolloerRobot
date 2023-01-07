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

  void operation() {

    // Reading The Message
    String Message = readDualComm();

    // If Message Empty Return
    if(Message == NULL) {
      return;
    }
    
    // If Message Not Empty Check If It Was A postMan Command Or Not
    else {

      // IF IT WAS POSITION RESET REQUEST
      if(Message == JSON_CMD_CL) {
        CURRENT_STOP = 0;
        writeDualComm("{\"STATUS\":\"POSITION = " + String(CURRENT_STOP) + "\"}");
        writeDualComm(JSON_MSG_OK);
        return;
      }
      
      // IF IT WAS ROTATE REQUEST
      else if(Message == JSON_CMD_RO) {
        writeDualComm(JSON_MSG_OK);
        return;
      }

    }

    // Intercept Serial Signal
    if(decodeJSON(Message)) {

      // Print Message To User
      writeDualComm(JSON_MSG_RC);
      writeDualComm("{\"REQUEST\":\"BED " + String(postMan.b) + ", DIR " + String(postMan.d) + ", Pill (Type 1) : " + String(postMan.c1) + ", Pill (Type 2) : " + String(postMan.c2) + "\"}");
      writeDualComm("{\"REQUEST\":\"CUR " + String(CURRENT_STOP) + ", NXT " + String(postMan.b - CURRENT_STOP) + "\"}");

      // Check If the Message Was New
      if(postMan.New == true) {

        // This Is Invalid ( No Bed After Final Position & No Bed Below 1)
        if(postMan.b > STOP_MAX || postMan.b < 1) {
          
          // Print ERROR Message
          writeDualComm(JSON_MSG_NA);

          // Stop Motors
          stateSTOP();

          // Do Nothing And Return
          return;

        }
        
        // Forward
        else if(postMan.b > CURRENT_STOP) {

          // The Loop Will Continue Till The postMan.b == CURRENT_STOP
          while(postMan.b > CURRENT_STOP) {
            if(move(MOVE_FORWARD)) {
              writeDualComm("{\"POSITION\":\"" + String(CURRENT_STOP) + "\"}");
              chargeRUN(120);
            }
          }

          // Stop Motors
          stateSTOP();

        } 
        
        // Backward
        else if(postMan.b < CURRENT_STOP) {

          // The Loop Will Continue Till The postMan.b == CURRENT_STOP
          while(postMan.b < CURRENT_STOP) {
            if(move(MOVE_BACKWARD)) {
              writeDualComm("{\"POSITION\":\"" + String(CURRENT_STOP) + "\"}");
              chargeRUN(120);
            }
          }

          // Stop Motors
          stateSTOP();

        } 
        
        // We Are At The Same Stop So Dont Move Line Follower
        else if(postMan.b == CURRENT_STOP) {

          // Stop Motors
          stateSTOP();

        }

        // Do The Dumping
        dumpPills(postMan.d, postMan.c1, postMan.c1);

      }

      // Make The Message Old
      postMan.New = false;

      // Send Back Confirmation Message
      writeDualComm(JSON_MSG_OK);

    }

  }

//------------------------------------------------------

  bool move(bool dir) {

    // Initiate Variables
    int lx = 0;
    int rx = 0;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Forward Moving Direction
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    if(dir == FORWARD) {
      // Reading From Sensor
      lx = FLS.readAVG(30);      // Left IR Sensor
      rx = FRS.readAVG(30);      // Right IR Sensor
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Backward Moving Direction
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    else {
      // Reading From Sensor
      lx = BLS.readAVG(30);      // Left IR Sensor
      rx = BRS.readAVG(30);      // Right IR Sensor
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Movment Decision Making
    if(lx == 0 && rx == 0) {
      // Move Forward
      stateFORWARD();
      // The Terminator Line Not Meet
      return false;
    } 
    
    // Move Right
    else if(lx == 1 && rx == 0) {
      // Steer Right
      stateRIGHT();
      // The Terminator Line Not Meet
      return false;
    } 
    
    // Move Left
    else if(lx == 0 && rx == 1) {
      // Steer Left
      stateLEFT();
      // The Terminator Line Not Meet
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
//  Dispenser Functions
//======================================================

  void dispense(NewServo &container, int count) {

    // Make Sure Count Not Equals 0 Then Dump
    if(count >= 0) {
      for(int i=0; i<count; i++) {
        container.goMin();
        delay(PILL_DUMP_DELAY);
        container.goMax();
        delay(PILL_DUMP_DELAY);
        container.goMin();
        delay(PILL_DUMP_DELAY);
      }
    }

  }

//------------------------------------------------------

  void dumpPills(int angle, int pill1, int pill2) {

    // Make Sure That Dumping Is Not 0
    if(pill1 == 0 && pill2 == 0) {
      return;
    }

    // Rotate Platform To Set Position
    switch(angle) {
      case 0:   // Move To Initial Position : Center
        Rotate.goInit();
        break;
      case 1:   // Move To Left Side
        Rotate.goMax();
        break;
      case 2:   // Move To Right Side
        Rotate.goMin();
        break;
      default:  // If Wrong Message Sent
        writeDualComm(JSON_MSG_EA);
        return;
    }
    
    // Wait For Some Time
    delay(200);

    // Dispense From Container 2 Pills = pill1
    dispense(dsp2, pill1);

    // Dispense From Container 1 Pills = pill2
    dispense(dsp1, pill2);

    // Wait For Some Time
    delay(600);

    // Move Back To Initial Position
    Rotate.goInit();

    // Wait For Some Time
    delay(400);

  }

//------------------------------------------------------

  void disconnectServos() {
    dsp1.end();
    dsp2.end();
    Rotate.end();
  }

//------------------------------------------------------

  void connectServos() {
    dsp1.begin();
    dsp2.begin();
    Rotate.begin();
  }

//======================================================
//  Communications Routines
//======================================================

  void writeDualComm(String Message) {
    
    // Disable the Servos
    disconnectServos();

    // Using Hardware Origional Serial Port
    terminal.println(Message);

    // Using Software Serial
    bluetooth.println(Message);
    
    // Enable The Servos
    connectServos();

  }

//------------------------------------------------------

  String readDualComm() {

    // Disconnect Servos
    disconnectServos();

    // Preparing String Buffer
    String buffer = "";

    // Reading Serial String
    if(terminal.available()) {

      buffer = terminal.readStringUntil("\n");
      clearComm(terminal);
      buffer.trim();

      // Reconnect Servos
      connectServos();

      // Return The Message Value
      return buffer;

    } 
    
    // Read From Bluetooth
    else if(bluetooth.available()) {

      buffer = bluetooth.readStringUntil("\n");
      clearComm(bluetooth);
      buffer.trim();

      // Reconnect Servos
      connectServos();

      // Return The Message Value
      return buffer;
      
    } 
    
    // Return Null If There Is Nothing
    else {

      return NULL;

    }

  }

//------------------------------------------------------

  void clearComm(Stream &_serial) {
    while(_serial.available()) {
      _serial.read();
    }
  }

//======================================================
//  Decode JSON Signal With JSON Content
//======================================================

  bool decodeJSON(String buffer) {

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
    postMan.b   = b;
    postMan.d   = d;
    postMan.c1  = c1;
    postMan.c2  = c2;
    postMan.New = true;

    // Clearing Memory
    doc.clear();

    // Return True Indicator
    return true;

  }
