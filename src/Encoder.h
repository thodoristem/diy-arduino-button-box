#include <Arduino.h>

class Encoder
{

private:
    uint8_t pinA;
    uint8_t pinB;

    int stateA;
    int stateB;
    int lastStateA = LOW;

    int value = 0;

public:
    Encoder(uint8_t pinA, uint8_t pinB)
    {
        this->pinA = pinA;
        this->pinB = pinB;
    }

    void begin()
    {
        delayMicroseconds(2000);

        pinMode(pinA, INPUT_PULLUP);
        pinMode(pinB, INPUT_PULLUP);

        update();
    }

    int getValue()
    {
        this->value = 0;

        if (this->lastStateA == LOW && this->stateA == HIGH)
        {
            this->value = (this->stateB != LOW) ? -1 : 1;
        }

        this->lastStateA = this->stateA;

        return this->value;
    }

    uint8_t getIntPin()
    {
        return pinA;
    }

    void update()
    {
        this->stateA = digitalRead(this->pinA);
        this->stateB = digitalRead(this->pinB);
    }
};