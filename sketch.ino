#include <Keyboard.h>

// User Settings ------------
const char key = KEY_F23;
// --------------------------
const int connectedPin = 9;
const int buttonPin = 10;
const int ledPin = 17;
// --------------------------

boolean connected = false;

void setup()
{
    pinMode(ledPin, OUTPUT);
    TXLED0;  // Disable TX LED

    // Safety check. Ground pin #2 to stop program.
    pinMode(2, INPUT_PULLUP);

    if (!digitalRead(2)) {
        failsafe();
    }

    pinMode(connectedPin, INPUT_PULLUP);
    pinMode(buttonPin, INPUT_PULLUP);

    // Safety check #2. Hold button down on startup to stop the program.
    // (Will also trigger if the jack isn't properly connected)
    if (switchConnected() && buttonPressed()) {
        failsafe();
    }

    Keyboard.begin();
}

void loop()
{
    if (switchConnected()) {
        if (!connected) {
            delay(50); // Quick debounce
        }

        connected = true;
        readButton();
        delay(2);
    } else {
        connected = false;
        Keyboard.press(KEY_F21);
        delay(50);
        Keyboard.release(KEY_F21);
        delay(500);
    }
}

void readButton()
{
    static boolean lastState = false;

    boolean buttonState = buttonPressed();

    if (lastState == buttonState) {
        return; // Nothing to see here, folks
    }

    buttonState ? Keyboard.press(key) : Keyboard.release(key);
    lastState = buttonState;
}

boolean switchConnected()
{
    return digitalRead(connectedPin);
}

boolean buttonPressed()
{
    return digitalRead(buttonPin);
}

void failsafe()
{
    const uint16_t blinkTime = 500;

    for (;;) {
        digitalWrite(ledPin, HIGH);
        delay(blinkTime);
        digitalWrite(ledPin, LOW);
        delay(blinkTime);
    }
}
