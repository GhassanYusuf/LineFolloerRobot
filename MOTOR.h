
  class MOTOR {

    private:

      // This Is The Control, Enable Pins
      uint8_t _ena_pin    = 0;
      uint8_t _in1_pin    = 0;
      uint8_t _in2_pin    = 0;

      // Set Speed Data
      uint8_t _speed      = 0;
      uint8_t _min_speed  = 0;
      uint8_t _max_speed  = 255;

      // Invert The Direction
      bool    _invert     = false;

    public:

      // Constructor With Parameter
      MOTOR(uint8_t enable, uint8_t in1, uint8_t in2) {
        _ena_pin  = enable;
        _in1_pin  = in1;
        _in2_pin  = in2;
      }

      // Set The Starting Parameters
      void begin() {
        pinMode(_ena_pin, OUTPUT);
        pinMode(_in1_pin, OUTPUT);
        pinMode(_in2_pin, OUTPUT);
        digitalWrite(_ena_pin, LOW);
        digitalWrite(_in1_pin, LOW);
        digitalWrite(_in2_pin, LOW);
        _speed  = 255;
      }

      // Set Minimum Speed
      void setMin(uint8_t value) {
        _min_speed = value;
      }

      // Set Maximum Speed
      void setMax(uint8_t value) {
        _max_speed = value;
      }

      // Invert Motor Direction
      void invert() {
        _invert = !_invert;
      }

      // To know what direction the motors are mooving
      bool isInvert() {

        return _invert;

      }

      // Move Forword
      void forward() {
        if(_invert == false) {
          digitalWrite(_in1_pin, LOW);
          digitalWrite(_in2_pin, HIGH);
        } else {
          digitalWrite(_in1_pin, HIGH);
          digitalWrite(_in2_pin, LOW);
        }
      }

      // Move Backword
      void backward() {
        if(_invert == false) {
          digitalWrite(_in1_pin, HIGH);
          digitalWrite(_in2_pin, LOW);
        } else {
          digitalWrite(_in1_pin, LOW);
          digitalWrite(_in2_pin, HIGH);
        }
      }

      // Motor Speed
      void goMin() {
        analogWrite(_ena_pin, _min_speed);
      }

      // Motor Speed
      void goMax() {
        analogWrite(_ena_pin, _max_speed);
      }

      // Motor Speed
      void goSpeed(uint8_t speed) {
        // Make Sure You Are Between The Limits
        speed = constrain(speed, _min_speed, _max_speed);
        // Analog 
        analogWrite(_ena_pin, speed);
      }

      // Press Break
      void stop() {
        digitalWrite(_ena_pin, LOW);
      }

      // Toggle Break
      void toggleMove() {
        digitalWrite(_ena_pin, !digitalRead(_ena_pin));
      }

      // Command By Serial
      void command(Stream &serial) {

        // Print A Message
        serial.print("Please Enter Your Position Value : ");

        // Wait For User Input
        while(!serial.available()) {}

        // Read The Value Convert To Number
        if(serial.available()) {
          String x = serial.readStringUntil('\n');
          while(serial.available()) {
            serial.read();
          }
          serial.println();
          x.trim();
          serial.println(x);
          goSpeed(x.toInt());
        }
        
      }
    
  };
