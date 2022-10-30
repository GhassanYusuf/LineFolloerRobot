  // Libraries Including
  #include  "MOTOR.h"
  #include  "IRSensor.h"

  // ROBO STATE
  enum RoboState {
    BREAK,
    LEFT,
    RIGHT,
    FORWARD,
    BACKWARD
  };

  // Assign Analog Pin For Sensor
  #define   LEFT_SENSOR_PIN         A0
  #define   RIGHT_SENSOR_PIN        A1

  // Assign Pins For LEFT Motors
  #define   LEFT_MOTOR_ENA_PIN      6
  #define   LEFT_MOTOR_IN1_PIN      8
  #define   LEFT_MOTOR_IN2_PIN      9
  
  // Assign Pins For RIGHT Motors
  #define   RIGHT_MOTOR_ENA_PIN     5
  #define   RIGHT_MOTOR_IN1_PIN     10
  #define   RIGHT_MOTOR_IN2_PIN     11

  // ROBO STATE
  RoboState State = BREAK;

  // Left Side Objects
  IRSensor  LS(LEFT_SENSOR_PIN);  
  MOTOR     LM(LEFT_MOTOR_ENA_PIN, LEFT_MOTOR_IN1_PIN, LEFT_MOTOR_IN2_PIN);

  // Right Side Objects
  IRSensor  RS(RIGHT_SENSOR_PIN);
  MOTOR     RM(RIGHT_MOTOR_ENA_PIN, RIGHT_MOTOR_IN1_PIN, RIGHT_MOTOR_IN2_PIN);
