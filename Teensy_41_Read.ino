#include <SPI.h>
#include <SD.h>
#include <MCP492X.h> // Include the library for the MCP492X DAC

//const int CS_PIN_SD = 9;  // Define the chip select pin for SD card
const int CS_PIN_SD = BUILTIN_SDCARD;
#define CS_PIN_DAC 4 // Define the chip select pin for DAC

MCP492X myDac(CS_PIN_DAC);    

File dataFile;

void setup() {
  Serial.begin(9600);

  // Initialize SD card
  if (!SD.begin(CS_PIN_SD)) {
    Serial.println("SD Card initialization failed!");
    return;
  }
  Serial.println("SD Card initialized.");

  // Open file for reading
  dataFile = SD.open("adc.txt", FILE_READ);
  if (!dataFile) {
    Serial.println("Failed to open file for reading.");
    return;
  }

  // Initialize DAC
  Serial.println("DAC initializing.");
  myDac.begin();
  Serial.println("DAC initialized.");
}

//uint16_t output[300];

void loop() {
  if (dataFile.available()) {
    int sensorValue = dataFile.parseInt();
    myDac.analogWrite(sensorValue);
    delayMicroseconds(5);
  }
  else {
    // Close the file when done reading
    dataFile.close();
    Serial.println("Data playback complete.");
    while (true) {} // Stop the loop
  }
}
