
  class MOTOR {

    private:

      // This Is The Control, Enable Pins
      uint8_t _ena_pin  = 0;
      uint8_t _in1_pin  = 0;
      uint8_t _in2_pin  = 0;

      // Set Speed Data
      uint8_t _speed    = 0;

      // Invert The Direction
      bool    _invert   = false;

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

      // Invert Motor Direction
      void Invert() {
        _invert = !_invert;
      }

      // Move Forword
      void Forward() {
        if(_invert == false) {
          digitalWrite(_in1_pin, LOW);
          digitalWrite(_in2_pin, HIGH);
        } else {
          digitalWrite(_in1_pin, HIGH);
          digitalWrite(_in2_pin, LOW);
        }
      }

      // Move Backword
      void Backward() {
        if(_invert == false) {
          digitalWrite(_in1_pin, HIGH);
          digitalWrite(_in2_pin, LOW);
        } else {
          digitalWrite(_in1_pin, LOW);
          digitalWrite(_in2_pin, HIGH);
        }
      }

      // Motor Speed
      void SetSpeed(uint8_t speed) {
        analogWrite(_ena_pin, constrain(speed, 0, 255));
      }

      // Press Break
      void Break() {
        digitalWrite(_ena_pin, LOW);
      }

      // Toggle Break
      void ToggleBreak() {
        digitalWrite(_ena_pin, !digitalRead(_ena_pin));
      }
    
  };
