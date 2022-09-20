Environmental control system running on an ESP32 written with the Arduino Framework

Create a `Secrets.h` file in the root directory with the following contents.
```
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";
```

External libraries are installed by placing them in the Arduino Sketch Libriaries.
`C:\Users\User\Documents\Arduino\libraries`

Dependencies not in Arduino Library Repository

- [AsyncTCP](https://github.com/me-no-dev/AsyncTCP)
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)