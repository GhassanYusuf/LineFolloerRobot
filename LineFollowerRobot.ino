  // Including Header File
  #include "Header.h"

  // Main uC Setup
  void setup() {

    // Serial Ports Start
    terminal.begin(9600);
    bluetooth.begin(9600);
    analogReference(INTERNAL);

    // Setting Rotating Platform Limits
    Rotate.begin();
    Rotate.setMin(0);
    Rotate.setMax(180);
    Rotate.setInit(96);
    Rotate.goInit();

    // Setting Dispenser 1 Limits
    dsp1.begin();
    dsp1.setMin(25);
    dsp1.setMax(140);
    dsp1.setInit(140);
    dsp1.goInit();

    // Setting Dispenser 2 Limits
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

    // FRONT Sensors Calibration
    FLS.threshold(595);
    FRS.threshold(603);

    // BACK Sensors Calibration
    BLS.threshold(575);
    BRS.threshold(575);

    // Testing
    FrontBumper.begin();
    BackBumper.begin();

  }

  // Main uC Program Loop
  void loop() {

    // Test Read The Line Sensors
    // terminal.println(
    //   String(FLS.read()) + ", " +
    //   String(FRS.read()) + ", " +
    //   String(BLS.read()) + ", " +
    //   String(BRS.read())
    // );

    // Delay
    // delay(25);

    // Movement
    operation();

    // terminal.println(String(FrontBumper.read()) + ", " + String(BackBumper.read()));
    // terminal.println(BackBumper.read());    
    // delay(200);
    
  }