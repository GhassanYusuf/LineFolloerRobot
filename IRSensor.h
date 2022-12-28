
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
    
  };
