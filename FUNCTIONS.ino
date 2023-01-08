//======================================================
//  MOTOR MACRO FUNCTIONS  
//======================================================

  void stateFORWARD() {
    if(State != FORWARD) {
      State = FORWARD;
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
      LM.stop();
      RM.stop();
    }
  }

//------------------------------------------------------
  
  void chargeRUN(int _delay, int _dir) {

    if(_dir == MOVE_FORWARD) {
      LM.goMax();
      RM.goMax();
      LM.forward();
      RM.forward();
      delay(_delay);
      stateFORWARD();
    } else {
      LM.goMax();
      RM.goMax();
      LM.backward();
      RM.backward();
      delay(_delay);
      stateBACKWARD();
    }
    
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

          // Hot Start
          chargeRUN(250, MOVE_FORWARD);

          // The Loop Will Continue Till The postMan.b == CURRENT_STOP
          while(postMan.b > CURRENT_STOP) {
            if(move(MOVE_FORWARD)) {
              writeDualComm("{\"POSITION\":\"" + String(CURRENT_STOP) + "\"}");
              chargeRUN(250, MOVE_FORWARD);
            }
          }

          // Stop Motors
          stateSTOP();

        } 
        
        // Backward
        else if(postMan.b < CURRENT_STOP) {

          // Hot Start
          chargeRUN(250, MOVE_BACKWARD);

          // The Loop Will Continue Till The postMan.b == CURRENT_STOP
          while(postMan.b < CURRENT_STOP) {
            if(move(MOVE_BACKWARD)) {
              writeDualComm("{\"POSITION\":\"" + String(CURRENT_STOP) + "\"}");
              chargeRUN(250, MOVE_BACKWARD);
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
    int fls = 0;
    int frs = 0;
    int bls = 0;
    int brs = 0;
    int dst = 0;
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Forward Moving Direction
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    if(dir == MOVE_FORWARD) {

      // Reading From Sensor
      fls = FLS.readAVG(30);      // Left IR Sensor
      frs = FRS.readAVG(30);      // Right IR Sensor

      // Movment Decision Making
      if(fls == 0 && frs == 0) {
        // Move Forward
        stateFORWARD();
        // The Terminator Line Not Meet
        return false;
      } 
      
      // Move Right
      else if(fls == 1 && frs == 0) {
        // Steer Right
        stateRIGHT();
        // The Terminator Line Not Meet
        return false;
      } 
      
      // Move Left
      else if(fls == 0 && frs == 1) {
        // Steer Left
        stateLEFT();
        // The Terminator Line Not Meet
        return false;
      }

      // Terminator Detected
      else if(fls == 1 && frs == 1) {
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

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Backward Moving Direction
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    else {

      // Reading From Sensors For Line Following
      bls = BLS.readAVG(30);  // Back LEFT IR Sensor
      brs = BRS.readAVG(30);  // Back Right IR Sensor

      // Reading From Sensor FOR TERMINATOR Detection
      fls = FLS.readAVG(30);  // Left IR Sensor
      frs = FRS.readAVG(30);  // Right IR Sensor

      // Movment Decision Making
      if(bls == 0 && brs == 0) {
        // Move Forward
        stateBACKWARD();
        // The Terminator Line Not Meet
        return false;
      } 
      
      // Move Right
      else if(bls == 1 && brs == 0) {
        // Steer Right
        stateRIGHT();
        // The Terminator Line Not Meet
        return false;
      } 
      
      // Move Left
      else if(bls == 0 && brs == 1) {
        // Steer Left
        stateLEFT();
        // The Terminator Line Not Meet
        return false;
      }

      // Terminator Detected
      else if(bls == 1 && brs == 1) {
        // STOP THE MOTORS
        stateSTOP();
        // Draft Delay
        delay(250);
        // Terminator Detected
        CURRENT_STOP -= 1;
        // Return Success Flag
        return true;
      }
      
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
