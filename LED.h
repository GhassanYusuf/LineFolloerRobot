
  class LED {

    private:

      uint8_t       _pin      = 13;
      bool          _invert   = false;
      unsigned long _prvTime  = millis();

    public:

      LED(uint8_t user_pin) {
        _pin = user_pin;
        pinMode(_pin, OUTPUT);
      }

      void invert() {
        _invert = !_invert;
      }

      bool state() {
        return digitalRead(_pin);
      }

      void on() {
        if(_invert == false) {
          digitalWrite(_pin, HIGH);
        } else {
          digitalWrite(_pin, !HIGH);
        }
      }

      void off() {
        if(_invert == false) {
          digitalWrite(_pin, LOW);
        } else {
          digitalWrite(_pin, !LOW);
        }
      }

      void toggle() {
        digitalWrite(_pin, !state());
      }

      void blink(unsigned int interval) {
        if(millis() - _prvTime >= interval) {
          toggle();
          _prvTime = millis();
        }
      }
    
  };
