  // Our New Servo Class
  class Sonar {

    private:

      // Private Variables
      uint8_t _echo_pin         = 0;      // Echo Pin Number
      uint8_t _trig_pin         = 0;      // Trigger Pin Number
      uint8_t _min_dst          = 0;      // Stop Position
      uint8_t _max_dst          = 0;      // Start Position

    public:

      // Initialize With No Pin
      Sonar() {}

      // Initialize With Pin Definition
      Sonar(uint8_t trig, uint8_t echo) {
        setPins(trig, echo);
      }

      // Begin Operation
      void begin() {
        pinMode(_trig_pin, OUTPUT);
        digitalWrite(_trig_pin, LOW);
        pinMode(_echo_pin, INPUT);
      }

      // Setting The Pin
      void setPins(uint8_t trig, uint8_t echo) {
        _trig_pin = trig;
        _echo_pin = echo;
      }

      // Set Min Position Limit
      void setMin(uint8_t value) {
        _min_dst = value;
      }

      // Set Max Position Limit
      void setMax(uint8_t value) {
        _max_dst = value;
      }

      // Set Initial Position
      void setMid(uint8_t value) {
        _max_dst = value;
      }

      // Go To Stop Position
      float read() {
        digitalWrite(_trig_pin, LOW);
        delayMicroseconds(2);
        digitalWrite(_trig_pin, HIGH);
        delayMicroseconds(10);
        digitalWrite(_trig_pin, LOW);
        unsigned long time = pulseIn(_echo_pin, HIGH);
        return (time * 0.000001 * (34300/2));
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
          x.trim();
          serial.println(x);
          // Put The Code Here
        }
        
      }
  
  };
