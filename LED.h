
  class LED {

    private:

      uint8_t _pin;
      unsigned long _prvTime = millis();

    public:

      LED(uint8_t user_pin) {
        _pin = user_pin;
        pinMode(_pin, OUTPUT);
      }

      bool State() {
        return digitalRead(_pin);
      }

      void On() {
        digitalWrite(_pin, HIGH);
      }

      void Off() {
        digitalWrite(_pin, LOW);
      }

      void Toggle() {
        digitalWrite(_pin, !State());
      }

      void Blink(unsigned int interval) {
        if(millis() - _prvTime >= interval) {
          Toggle();
          _prvTime = millis();
        }
      }
    
  };
