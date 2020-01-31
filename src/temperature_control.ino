/*
 * Adafruit_Sensor ---> https://github.com/adafruit/Adafruit_Sensor
 * DHT ---> https://github.com/adafruit/DHT-sensor-library
 *
 * If Arduino IDE can't find Adafruit_Sensor.h, uncomment Adafruit_Sensor.h include line.
 */
//#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHT_PIN 2
#define FIRST_LOOP_DELAY 2000
#define STANDART_DELAY 1400
#define ACTION_DELAY 100

#define TEMP_SYMBOL "t"
#define TEMP_MAX_SYMBOL "m"
#define TEMP_MIN_SYMBOL "i"
#define HUMID_SYMBOL "h"
#define HUMID_MAX_SYMBOL "w"
#define HUMID_MIN_SYMBOL "q"

#define RESET_SYMBOL 'r'

DHT dht(DHT_PIN, DHT22);

float temperature;
float humidity;
float temperatureMax;
float temperatureMin;
float humidityMax;
float humidityMin;

char incomingOperation;

boolean initLoop = true;

void(* resetFunc) (void) = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  loopDelay();

  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  setupTrackingValues();

  sendData(TEMP_SYMBOL, temperature);
  sendData(TEMP_MAX_SYMBOL, temperatureMax);
  sendData(TEMP_MIN_SYMBOL, temperatureMin);
  
  sendData(HUMID_SYMBOL, humidity);
  sendData(HUMID_MAX_SYMBOL, humidityMax);
  sendData(HUMID_MIN_SYMBOL, humidityMin);

  checkResetCommand();
}

void loopDelay() {
  if (initLoop) {
    delay (FIRST_LOOP_DELAY);
  } else {
    delay(STANDART_DELAY);
  }
}

void setupTrackingValues() {
  if (initLoop) {
    temperatureMax = temperature;
    temperatureMin = temperature;
    humidityMax = humidity;
    humidityMin = humidity;

    initLoop = false;
  } else {
    if (temperature >= temperatureMax) {
      temperatureMax = temperature;
    } else if (temperature <= temperatureMin) {
      temperatureMin = temperature;
    }
    if (humidity >= humidityMax) {
      humidityMax = humidity;
    } else if (humidity <= humidityMin) {
      humidityMin = humidity;
    }
  }
}

void sendData(String symbol, float value) {
  Serial.println(symbol + (String) value);
  delay(ACTION_DELAY);
}

void checkResetCommand() {
  if (Serial.available() > 0) {
    incomingOperation = Serial.read();
    if (incomingOperation == RESET_SYMBOL) {
      resetFunc();
    }
  }
}
