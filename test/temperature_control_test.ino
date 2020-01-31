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

float temperature;
float humidity;
float temperatureMax;
float temperatureMin;
float humidityMax;
float humidityMin;

float testTemperatureValues[] = { 23.4, 23.9, 24.8, 25.0, 24.1, 21.1, 27.9 };
float testHumidityValues[] = { 67.9, 78.3, 91.1, 50.1, 50.3, 51.7, 67.5 };

int testTemperatureValuesLength = 6;
int testHumidityValuesLength = 6;

int testValuesPositionIndex = 0;
int testHumidityValuesIndex = 0;

char incomingOperation;

boolean initLoop = true;

void setup() {
  Serial.begin(9600);
}

void(* resetFunc) (void) = 0;

void loop() {
  loopDelay();
  
  if (testValuesPositionIndex == testTemperatureValuesLength) {
    testValuesPositionIndex = 0;
  } else {
    testValuesPositionIndex++;
  }

  if (testHumidityValuesIndex == testHumidityValuesLength) {
    testHumidityValuesIndex = 0;
  } else {
    testHumidityValuesIndex++;
  }

  temperature = testTemperatureValues[testValuesPositionIndex];
  humidity = testHumidityValues[testHumidityValuesIndex];

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
    delay(FIRST_LOOP_DELAY);
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
