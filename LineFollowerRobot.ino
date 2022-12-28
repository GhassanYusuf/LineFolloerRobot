  // Including Header File
  #include "Header.h"

  // Main uC Setup
  void setup() {

    // Serial Ports Start
    terminal.begin(9600);
    bluetooth.begin(9600);
    analogReference(INTERNAL);

    // Starting The Servo
    Rotate.begin();
    Rotate.setMin(0);
    Rotate.setMax(180);
    Rotate.setInit(96);
    Rotate.goInit();

    // Setting Your Limits
    dsp1.begin();
    dsp1.setMin(25);
    dsp1.setMax(140);
    dsp1.setInit(140);
    dsp1.goInit();

    // Setting Your Limits
    dsp2.begin();
    dsp2.setMin(13);
    dsp2.setMax(143);
    dsp2.setInit(13);
    dsp2.goInit();

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

    // Sensors Calibration
    LS.threshold(589);
    RS.threshold(603);

  }

  // Main uC Program Loop
  void loop() {

    // Movement
    // movment(bluetooth);

    // BLUETOOTH INTERCEPTION
    if(bluetooth.available()) {

      String x = bluetooth.readStringUntil('\n');
      
      clearSerial(bluetooth);

      if(x == JSON_CMD_CL) {
        
        CURRENT_STOP = 0;
        bluetooth.println(JSON_MSG_OK);

      } 
      
      else if(x == JSON_CMD_CL) {
        
        RM.backward();
        LM.backward();
        RM.goMax();
        LM.goMax();
        
        delay(100);

        RM.backward();
        LM.forward();
        RM.goMax();
        LM.goMax();

        while(readAVG(RS, 30) == 0) {
          
        }

        delay(50);

        while(readAVG(RS, 30) == 1) {
          
        }

        RM.stop();
        LM.stop();
        
      }

    }
    
  }