/**
 * @file main.cpp
 * @author Theodoros Temourtzidis (thodoristem@gmail.com)
 * @brief An Arduino firmware for a DIY Button box 20 buttons (4 X 5) and 4 encoders
 * @version 1.0
 * @date 2024-01-30
 */
#include <joystick.h>
#include <Keypad.h>
#include <AnalogInput.h>
#include <Encoder.h>

#define ENCODER_NUM 4
#define ENCODER_DELAY 100

#define ROWS 4
#define COLS 5

char keys[ROWS][COLS];

struct EncoderStruct
{
    // Counter Clockwise Key
    short ccwKey;
    // Clockwise Key
    short cwKey;
    // Encoder
    Encoder encoder;
};

EncoderStruct encoders[ENCODER_NUM] = {
    {20, 21, Encoder(0, 4)},
    {22, 23, Encoder(1, 5)},
    {24, 25, Encoder(2, 6)},
    {26, 27, Encoder(3, 8)}};

byte colPins[COLS] = {17, 16, 15, 14, 13};

// Left
byte rowPins[ROWS] = {9, 10, 11, 12};

// Right
// byte rowPins[ROWS] = {12, 11, 10, 9};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Joystick configuration
Joystick_ joystick(
    JOYSTICK_DEFAULT_REPORT_ID, // USB ID
    JOYSTICK_TYPE_JOYSTICK,     // TYPE
    28,                         // Button count
    0,                          // Hat Switch
    false,                      // X
    false,                      // Y
    false,                      // Z
    false,                      // RotationX
    false,                      // RotationY
    false,                      // RotationZ
    false,                      // Rudder
    false,                      // Throttle
    false,                      // Accelarator
    false,                      // Brake
    false                       // Steering
);

// Update encoder on interrupt
void updateEncoders()
{
    for (short i = 0; i < ENCODER_NUM; i++)
    {
        encoders[i].encoder.update();
    }
}

void setup()
{
    // Serial.begin(115200);

    byte keyValue = 0;

    for (short i = 0; i < ROWS; i++)
    {
        for (short j = 0; j < COLS; j++)
        {
            keys[i][j] = keyValue++;
        }
    }

    for (short i = 0; i < ENCODER_NUM; i++)
    {
        encoders[i].encoder.begin();
        attachInterrupt(
            digitalPinToInterrupt(encoders[i].encoder.getIntPin()),
            updateEncoders,
            CHANGE);
    }

    joystick.begin();
}

// Rotary Encoders Input
void encoderLoop()
{
    for (short i = 0; i < ENCODER_NUM; i++)
    {
        int encValue = encoders[i].encoder.getValue();

        // if (encValue != 0)
        // {
        //   Serial.print("Encoder ");
        //   Serial.print(i);
        //   Serial.print(" -> ");
        //   Serial.print("Value: ");
        //   Serial.println(encValue);
        // }

        switch (encValue)
        {
        case -1:
            joystick.pressButton(encoders[i].ccwKey);
            delay(ENCODER_DELAY);
            break;
        case 0:
            joystick.releaseButton(encoders[i].ccwKey);
            joystick.releaseButton(encoders[i].cwKey);
            break;
        case 1:
            joystick.pressButton(encoders[i].cwKey);
            delay(ENCODER_DELAY);
            break;
        }
    }
}

// Buttons input
void buttonLoop()
{
    if (keypad.getKeys())
    {
        for (int i = 0; i < LIST_MAX; i++)
        {
            Key k = keypad.key[i];
            if (k.stateChanged)
            {
                switch (k.kstate)
                {
                case PRESSED:
                    joystick.pressButton(k.kchar);
                    break;
                case HOLD:
                    joystick.setButton(k.kchar, 1);
                    break;
                case RELEASED:
                    joystick.releaseButton(k.kchar);
                    break;
                case IDLE:
                    joystick.setButton(k.kchar, 0);
                    break;
                }
            }
        }
    }
}

void loop()
{
    encoderLoop();
    buttonLoop();
}