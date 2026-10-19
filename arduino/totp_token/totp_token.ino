// Hardware TOTP token: Uno + button. Host PC supplies Unix time over serial
// (mock RTC), button press prints the current 6-digit TOTP code.
#include <TOTP.h>
#include "secret.h"   // defines hmacKey[]; see secret.example.h

#define BUTTON_PIN 2  // pull-down: LOW idle, HIGH pressed

TOTP totp = TOTP(hmacKey, sizeof(hmacKey));
long baseUnix = 0;
unsigned long baseMillis = 0;
bool synced = false;

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(9600);
  Serial.println(F("Arduino TOTP token ready. Waiting for time sync..."));
}

void loop() {
  if (Serial.available() > 0) {
    String line = Serial.readStringUntil('\n');
    line.trim();
    if (line.startsWith("T")) {
      baseUnix = line.substring(1).toInt();
      baseMillis = millis();
      synced = true;
    }
  }

  if (digitalRead(BUTTON_PIN) == HIGH) {
    if (!synced) {
      Serial.println(F("ERROR: No time sync yet from host."));
    } else {
      long now = baseUnix + (millis() - baseMillis) / 1000;
      Serial.print(F("TOTP Code: "));
      Serial.println(totp.getCode(now));
    }
    delay(300);
  }
}
