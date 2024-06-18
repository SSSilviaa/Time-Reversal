#include <SPI.h>
#include <SD.h>
#include <MCP492X.h> // Include the library for the MCP492X DAC

const int CS_PIN_SD = BUILTIN_SDCARD; // Define the chip select pin for SD card
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

void loop() {
  if (dataFile.available()) {
    // Read value from SD card
    int sensorValue = dataFile.parseInt();
    Serial.println(sensorValue);

    // Output value to DAC
    myDac.analogWrite(sensorValue);

    delayMicroseconds(10); // Adjust delay as needed
  } else {
    // Close the file when done reading
    dataFile.close();
    Serial.println("Data playback complete.");
    while (true) {} // Stop the loop
  }
}

