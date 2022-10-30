
  class IRSensor {

    private:

      // Pin Settings
      uint8_t   _pin        = 0;
      bool      _invert     = false;
      int       _threshold  = 50;

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

      // Function For Decision Making
      bool read() {

        // Reading The Input And Maping It
        int reading = map(analogRead(_pin), 0, 1023, 0, 100);

        // Checking For Invert Signal
        if(_invert == false) {

          // Return Not Inverted Decision
          if(reading > _threshold) {
            return true;
          } else {
            return false;
          }
          
        } else {
          
          // Return Inverted Decision
          if(reading > _threshold) {
            return false;
          } else {
            return true;
          }
        }
        
      }
    
  };
