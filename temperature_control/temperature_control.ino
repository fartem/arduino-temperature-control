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

#define RESET_SYMBOL 'e'

DHT dht(DHT_PIN, DHT22);

float temp, humid, tempMax, tempMin, humidMax, humidMin;

char incomingByte;

boolean firstLoop = true;

void(* resetFunc) (void) = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  checkFirstLoop();

  temp = dht.readTemperature();
  humid = dht.readHumidity();

  validateDataScopes();

  sendData(TEMP_SYMBOL, temp);
  sendData(TEMP_MAX_SYMBOL, tempMax);
  sendData(TEMP_MIN_SYMBOL, tempMin);
  
  sendData(HUMID_SYMBOL, humid);
  sendData(HUMID_MAX_SYMBOL, humidMax);
  sendData(HUMID_MIN_SYMBOL, humidMax);

  checkResetCommand();
}

void checkFirstLoop() {
  if (firstLoop) {
    delay (FIRST_LOOP_DELAY);
    firstLoop = false;
  } else {
    delay(STANDART_DELAY);
  }
}

void validateDataScopes() {
  if (firstLoop == 0) {
    tempMax = temp;
    tempMin = temp;
    humidMax = humid;
    humidMin = humid;

    firstLoop = 1;
  } else {
    if(temp >= tempMax) {
      tempMax = temp;
    } else if(temp <= tempMin) {
      tempMin = temp;
    }
    if(humid >= humidMax) {
      humidMax = humid;
    } else if(temp <= humidMin) {
      humidMin = humid;
    }
  }
}

void sendData(String symbol, float value) {
  Serial.println(symbol + (String) value);
  delay(ACTION_DELAY);
}

void checkResetCommand() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if(incomingByte == RESET_SYMBOL) {
      resetFunc();
    }
  }
}
