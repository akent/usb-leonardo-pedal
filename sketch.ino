#include <Keyboard.h>

const int connectedPin = 9;
const int buttonPin = 10;
const int ledPin = 17;

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
            delay(2); // Quick debounce
        }

        connected = true;
        readButton();
        delay(2);
    } else {
        connected = false;
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

    if (buttonState) {
        Keyboard.press(KEY_ESC);
        Keyboard.release(KEY_ESC);
        Keyboard.press('a');
        Keyboard.release('a');
    } else {
        Keyboard.press(KEY_ESC);
        Keyboard.release(KEY_ESC);
    }
    lastState = buttonState;
}

boolean switchConnected()
{
    return !digitalRead(connectedPin);
}

boolean buttonPressed()
{
    return !digitalRead(buttonPin);
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
