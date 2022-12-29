
  class IRSensor {

    private:

      // Pin Settings
      uint8_t   _pin        = 0;
      bool      _invert     = false;
      int       _min        = 0;
      int       _max        = 0;
      int       _threshold  = 500;

    public:

      // Constructor
      IRSensor(uint8_t input) {
        _pin = input;
      }

      // Inverts The Reading Result
      void invert() {
        _invert = !_invert;
      }

      // Setting The Threshold
      void threshold(int input) {
        _threshold = input;
      }

      // Get Threshold
      int getThreshold() {
        return _threshold;
      }

      // Function For Decision Making
      uint8_t read() {

        // Do The Readings
        int x = raw();
        int y = _threshold;
        int d = 0;

        // Do The Deside Value
        if( x < y ) {
          d = 0;
        } else if( x >= y) {
          d = 1;
        }

        // Serial.println("Reading:" + String(x) + ", TH:" + String(y) + ", D:" + String(d));

        // Return The Value
        return d;

      }

      // Raw Reading
      unsigned int raw() {
        return analogRead(_pin);
      }

      // Read Avarage Value
      int readAVG(uint8_t times) {

        int result = 0;

        for(int i=0; i<times; i++) {
          result += read();
          delayMicroseconds(10);
        }

        result = result/times;

        return result;

      }

      // Calibrate Sensor
      int calibrate(Stream &serial) {

        // Preparing Variables
        int     x       = 0;
        int     min     = 1203;
        int     max     = 0;
        String  buffer  = "";

        // Display Message For User
        serial.println("Place The Sensor On The Darkest And The Brightest Side For Some Time, Then Send 'close' in serial terminal To end the calibration");

        // Infinit Loop
        while(1) {

          // Reading Raw Value
          x = raw();

          // Finding Max Value
          if(x > max) {
            max = x;
          }

          // Finding Min Vlaue
          if(x < min) {
            min = x;
          }

          // Calculate Treshold Value
          int th = ((max - min)/2) + min;

          // Display Reading
          serial.println("MIN = " + String(min) + ", MAX = " + String(max) + ", TH VALUE = " + String(th));

          // Wait For Serial Command To Exit Calibration
          if(serial.available()) {
            buffer = serial.readStringUntil("\n");
            while(serial.available()) { serial.read(); }
            buffer.trim();
            if(buffer == "exit") {
              _threshold = th;
              return th;
            }
          }

        }

      }
    
  };
