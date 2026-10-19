# Hardware TOTP Token

A hardware access token built on an Arduino Uno. Press a button and it prints a
6-digit TOTP code that matches Google Authenticator.

The Uno has no real-time clock, so a C++ program on the PC acts as a **mock RTC**,
sending Unix time over serial every second.

## How it works
```
PC (time_sync.exe) --T<unix>--> Arduino (TOTP.h) --code--> serial output
                                                        compare with Google Authenticator
```
TOTP = HMAC-SHA1(secret, unix_time / 30), truncated to 6 digits (RFC 6238).

## Hardware
Uno R3, breadboard, push button, 10k resistor, jumper wires, USB-B cable.

- 5V -> one side of the button
- Other side of the button -> pin 2 **and** 10k resistor -> GND (pull-down)

## Setup
1. Generate a secret: `python3 -c "import base64,os;print(base64.b32encode(os.urandom(10)).decode())"`
2. Add it to Google Authenticator (**Enter a setup key**, time based).
3. Copy `arduino/totp_token/secret.example.h` to `secret.h` and fill in the secret as bytes
   (command is in the file).
4. Arduino IDE: install the **TOTP** library (Luca Dentella), upload `totp_token.ino`, close Serial Monitor.
5. Build and run the time sync:
   ```
   g++ -std=c++17 host/time_sync.cpp -o time_sync.exe
   .\time_sync.exe COM5
   ```
6. Press the button and compare the code to Google Authenticator.

## Security note
The shared secret is the only thing that must stay private. `secret.h` is git-ignored.
