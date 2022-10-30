
  class LED {

    private:

      uint8_t _pin;
      unsigned long _prvTime = millis();

    public:

      LED(uint8_t user_pin) {
        _pin = user_pin;
        pinMode(_pin, OUTPUT);
      }

      bool state() {
        return digitalRead(_pin);
      }

      void on() {
        digitalWrite(_pin, HIGH);
      }

      void off() {
        digitalWrite(_pin, LOW);
      }

      void toggle() {
        digitalWrite(_pin, !State());
      }

      void blink(unsigned int interval) {
        if(millis() - _prvTime >= interval) {
          Toggle();
          _prvTime = millis();
        }
      }
    
  };
