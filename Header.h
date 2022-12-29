  // Libraries Including
  #include  "LED.h"
  #include  "MOTOR.h"
  #include  "Sonar.h"
  #include  "IRSensor.h"
  #include  "NewServo.h"
  #include  <ArduinoJson.h>
  #include  <SoftwareSerial.h>

  // ROBO STATE
  enum RoboState {
    STOP,
    LEFT,
    RIGHT,
    FORWARD,
    BACKWARD
  };

  // Object Data
  struct movData {
    uint8_t b   = 0;      // Bed Number
    uint8_t d   = 0;      // Direction -> (0) Center (), (1) Left, (2) Right
    uint8_t c1  = 0;      // Pill Container 1 => Dump Count
    uint8_t c2  = 0;      // Pill Container 2 => Dump Count
    bool    New = false;  // If Received New Command Flip Value To True, Once Its Used Then Make It False To Avoid Redudndant Movment
  };

  // MOVMENT 
  #define   CENTER                  0
  #define   LEFT                    1
  #define   RIGHT                   2

  // Assign Analog Pin For Sensor
  #define   FRONT_LEFT_SENSOR_PIN   A0
  #define   FRONT_RIGHT_SENSOR_PIN  A1
  #define   BACK_LEFT_SENSOR_PIN    A5
  #define   BACK_RIGHT_SENSOR_PIN   A4

  // Assign Pins For LEFT Motors
  #define   LEFT_MOTOR_ENA_PIN      6
  #define   LEFT_MOTOR_IN1_PIN      8
  #define   LEFT_MOTOR_IN2_PIN      9
  
  // Assign Pins For RIGHT Motors
  #define   RIGHT_MOTOR_ENA_PIN     5
  #define   RIGHT_MOTOR_IN1_PIN     10
  #define   RIGHT_MOTOR_IN2_PIN     11

  // ULTRA SONIC SENSOR
  #define   FRONT_USENSOR_TRIG      3
  #define   FRONT_USENSOR_ECHO      2
  #define   BACK_USENSOR_TRIG       A5
  #define   BACK_USENSOR_ECHO       A4
  Sonar     FrontBumper(FRONT_USENSOR_TRIG, FRONT_USENSOR_ECHO);
  Sonar     BackBumper(BACK_USENSOR_TRIG, BACK_USENSOR_ECHO);
  unsigned long prvTime             = millis();

  // STOP COUNT
  #define   STOP_MAX                3
  uint8_t   CURRENT_STOP            = 0;

  // ROBO STATE
  RoboState State = STOP;

  // Left Side Objects
  IRSensor  FLS(FRONT_LEFT_SENSOR_PIN);
  IRSensor  BLS(BACK_LEFT_SENSOR_PIN);
  MOTOR     LM(LEFT_MOTOR_ENA_PIN, LEFT_MOTOR_IN1_PIN, LEFT_MOTOR_IN2_PIN);

  // Right Side Objects
  IRSensor  FRS(FRONT_RIGHT_SENSOR_PIN);
  IRSensor  BRS(BACK_RIGHT_SENSOR_PIN);
  MOTOR     RM(RIGHT_MOTOR_ENA_PIN, RIGHT_MOTOR_IN1_PIN, RIGHT_MOTOR_IN2_PIN);

  // This Is The Rotate
  NewServo  Rotate(4);
  NewServo  dsp1(7);
  NewServo  dsp2(12);
  #define   PILL_DUMP_DELAY         600

  // Global Data Of Data Type
  movData   postMan;

  // Created Global JSON Document
  DynamicJsonDocument               doc(200);

  // Serial Ports
  #define   terminal                Serial
  SoftwareSerial                    bluetooth(A3, A2); // RX, TX

  // Fix Defined Terms
  #define   NULL                    ""

  // Commands
  #define   JSON_CMD_CL             "{\"COMMAND\":\"RESET\"}"
  #define   JSON_CMD_RO             "{\"COMMAND\":\"ROTATE\"}"

  // Messages
  #define   JSON_MSG_OK             "{\"STATUS\":\"OK\"}"
  #define   JSON_MSG_ER             "{\"STATUS\":\"ERROR\"}"
  #define   JSON_MSG_EA             "{\"STATUS\":\"ANGLE ERROR ( 0 = Center, 1 = LEFT, 2 = RIGHT )\"}"
  #define   JSON_MSG_NA             "{\"STATUS\":\"EXCEED LIMITS\"}"
  #define   JSON_MSG_OB             "{\"STATUS\":\"OBJECT BLOCKING\"}"
  #define   JSON_MSG_DF             "{\"STATUS\":\"DRIVING FORWARD\"}"
  #define   JSON_MSG_DB             "{\"STATUS\":\"DRIVING BACKWARDS\"}"