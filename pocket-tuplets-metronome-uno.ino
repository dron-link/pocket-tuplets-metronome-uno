#include <math.h>

const byte STRONG_BEAT_PIN = A5;
const byte BUZZER_PIN = A0;
const double STARTUP_BASE_PERIOD = 1000; // milliseconds. Equals One second

const int BUZZER_HIGH_DURATION = 7; // milliseconds. The longer, the louder

// used for: config mode Set As Nth Subdivision
const byte SET_AS_NTH_SUBDIVISION = A3;

// used for: config mode Manually Set Period
const byte MANUALLY_SET_PERIOD = A2; // begin config
const byte PERIOD_START_PIN = A4;
const byte PERIOD_END_PIN = A5; // this shares pin with the strong beat, on purpose,
                                // just to avoid automatically jumping to the nth subdivision.
                                // example: if you used pin 8 as PERIOD_END_PIN, you would hear 8ths
                                // as soon as you clicked that pin. for some reason.

void setup() {
  pinMode(STRONG_BEAT_PIN, INPUT_PULLUP);  // hear measure's strong beat

  pinMode(2, INPUT_PULLUP);  // subdivision of 2
  pinMode(3, INPUT_PULLUP);  // subdivision of 3
  pinMode(4, INPUT_PULLUP);  // subdivision of 4
  pinMode(5, INPUT_PULLUP);  // subdivision of 5
  pinMode(6, INPUT_PULLUP);  // subdivision of 6
  pinMode(7, INPUT_PULLUP);  // and so on
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  // do not use pin 13 for input pullup

  pinMode(SET_AS_NTH_SUBDIVISION, INPUT_PULLUP);
  pinMode(MANUALLY_SET_PERIOD, INPUT_PULLUP);

  pinMode(PERIOD_START_PIN, INPUT_PULLUP);
  pinMode(PERIOD_END_PIN, INPUT_PULLUP);

  pinMode(BUZZER_PIN, OUTPUT);  // BUZZER turns on when we write HIGH to this pin
  digitalWrite(BUZZER_PIN, LOW);

  // save battery life by turning off L led
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

byte getSubdivisionInput() {
  if (!digitalRead(STRONG_BEAT_PIN)) { 
    return 1; // subdivision 1 is the same as sounding only the downbeat (strong beat)
  }
  for (byte inputPin = 2; inputPin < 13; inputPin++) {
    if (!digitalRead(inputPin)) {
      return inputPin; // pin corresponds to the chosen subdivision
    }
  }
  // else
  return 0;  // no pin
}

byte getConfigModeInput() {
  if (!digitalRead(SET_AS_NTH_SUBDIVISION)) {
    return SET_AS_NTH_SUBDIVISION;
  }
  if (!digitalRead(MANUALLY_SET_PERIOD)) {
    return MANUALLY_SET_PERIOD;
  }
  // else
  return 0;  // no pin
}

void loop() {
  static double basePeriod = STARTUP_BASE_PERIOD;
  static double instancePeriod = basePeriod;
  static double currentTimeRemainder = 0;
  static byte subdivision = 1;  // initial subdivision, should be 1, to match with
                                // the fact that the initial value of
                                // instancePeriod is the same as basePeriod
  static byte configMode = 0;   // initial config mode state
  static bool lastInputWasASubdivisionPin = true;

  // save variables' state from last call +++++++++++++++++++++++++++++++++++++++++++++++++++

  double lastTimeRemainder = currentTimeRemainder;

  // update variables +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  byte subdivisionRead = getSubdivisionInput();
  byte configModeRead = getConfigModeInput();

  // pins 2 through 12
  if (subdivisionRead) {
    // if CONFIG MODE. Keep the music going, but listen to see what subdivision the user wants the current sound to be.
    if ((!lastInputWasASubdivisionPin) && (configMode == SET_AS_NTH_SUBDIVISION)) {
      subdivision = subdivisionRead;
      basePeriod = instancePeriod * subdivision;
    }

    // normal mode. change subdivision only if subdivision is different to the current one
    else if (subdivision != subdivisionRead) {  
      subdivision = subdivisionRead;
      instancePeriod = basePeriod / subdivision;

      lastTimeRemainder = 0;  // do not make a sound until next loop with new period ends
    }

    lastInputWasASubdivisionPin = true;
    configMode = 0;  // turn off config mode
  }

  // input is NOT subdivision or NO input at all
  else if (configModeRead) {
    lastInputWasASubdivisionPin = false;

    if (!configMode) { // only enter config mode if we haven't entered configMode just before, evidenced by it being ON
      configMode = configModeRead;

      if (configMode == MANUALLY_SET_PERIOD) {
        // option: Listen to Speed, in order to manually set period
        while (digitalRead(PERIOD_START_PIN)) {
          // wait
          delay(1);
        }
        unsigned long periodStartTimestamp = millis();
        while (digitalRead(PERIOD_END_PIN)) {
          // wait
          delay(1);
        }
        basePeriod = (double)(millis() - periodStartTimestamp); // duration = end time - beginning time

        subdivision = 1;
        instancePeriod = basePeriod;
        lastInputWasASubdivisionPin = true;

        lastTimeRemainder = 0;  // do not make a sound until next loop with new period ends

        configMode = 0; // new period was set, so, turn off config mode now
      }
    }
  }


  // this increases as time advances, but loops around to zero once instancePeriod passes
  currentTimeRemainder = fmod((double)millis(), instancePeriod);

  if (currentTimeRemainder < lastTimeRemainder) { // only happens when currentTimeRemainder loops back to zero
    // beep
    digitalWrite(BUZZER_PIN, HIGH);
    delay(BUZZER_HIGH_DURATION);
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(10);
}

