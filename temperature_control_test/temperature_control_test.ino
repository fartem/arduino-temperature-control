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

float temp, humid, tempMax, tempMin, humidMax, humidMin;
float testTemperatureValues[] = { 23.4, 23.9, 24.8, 25.0, 24.1, 21.1, 27.9 };
float testHumidityValues[] = { 67.9, 78.3, 91.1, 50.1, 50.3, 51.7, 67.5 };

int testTemperatureValuesLength = 6;
int testHumidityValuesLength = 6;

int testValuesPositionIndex = 0;
int testHumidityValuesIndex = 0;

char incomingByte;

boolean firstLoop = true;

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

  temp = testTemperatureValues[testValuesPositionIndex];
  humid = testHumidityValues[testHumidityValuesIndex];

  validateDataScopes();

  sendData(TEMP_SYMBOL, temp);
  sendData(TEMP_MAX_SYMBOL, tempMax);
  sendData(TEMP_MIN_SYMBOL, tempMin);
  
  sendData(HUMID_SYMBOL, humid);
  sendData(HUMID_MAX_SYMBOL, humidMax);
  sendData(HUMID_MIN_SYMBOL, humidMax);

  checkResetCommand();
}

void loopDelay() {
  if (firstLoop) {
    delay(FIRST_LOOP_DELAY);
  } else {
    delay(STANDART_DELAY);
  }
}

void validateDataScopes() {
  if (firstLoop) {
    tempMax = temp;
    tempMin = temp;
    humidMax = humid;
    humidMin = humid;

    firstLoop = false;
  } else {
    if(temp >= tempMax) {
      tempMax = temp;
    } else if(temp <= tempMin) {
      tempMin = temp;
    }
    if(humid >= humidMax) {
      humidMax = humid;
    } else if(humid <= humidMin) {
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
