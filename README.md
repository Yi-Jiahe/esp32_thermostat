Environmental control system running on an ESP32 written with the Arduino Framework

Create a `Secrets.h` file in the root directory with the following contents.
```
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";
```

External libraries are installed using git submodules in the src/ directory.
Run `git submodule update --init --recursive` to install required dependencies.