#include <SPI.h>
#include <SD.h>

#define CS_PIN_SD 10
#define ANALOG_PIN A0
#define NUM_READINGS 100 // Number of readings to store

File dataFile;

void setup() {
  Serial.begin(9600);
  pinMode(CS_PIN_SD, OUTPUT);

  // Initialize SD card
  if (!SD.begin(CS_PIN_SD)) {
    Serial.println("SD Card initialization failed!");
    return;
  }
  Serial.println("SD Card initialized.");

  // Open file for writing
  dataFile = SD.open("data.txt", FILE_WRITE);
  if (!dataFile) {
    Serial.println("Failed to open file for writing.");
    return;
  }

  // Read analog values and store in SD card
  for (int i = 0; i < NUM_READINGS; i++) {
    int sensorValue = i; //analogRead(ANALOG_PIN);
    dataFile.println(sensorValue);
    Serial.println(sensorValue);
    delay(100); // Adjust delay as needed
  }

  // Close the file
  dataFile.close();
  Serial.println("Data logging complete.");
}

void loop() {
  // Nothing to do here
}
