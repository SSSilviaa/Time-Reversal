// DAC Read and Output Slave
#include <SPI.h>
#include <SD.h>
#include <MCP492X.h> // Include the library for the MCP492X DAC

//const int CS_PIN_SD = 9;  // Define the chip select pin for SD card
const int CS_PIN_SD = BUILTIN_SDCARD;
#define CS_PIN_DAC 4 // Define the chip select pin for DAC
const int syncPin = 7; // Pin to send synchronization signal

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
  dataFile = SD.open("output_9_T2.txt", FILE_READ);
  if (!dataFile) {
    Serial.println("Failed to open file for reading.");
    return;
  }

  // Initialize DAC
  Serial.println("DAC initializing.");
  myDac.begin();
  Serial.println("DAC initialized.");

  pinMode(syncPin, INPUT);
  // wait until mater signal
  Serial.println("INPUT - Receiving synchronization signal...");
  while (digitalRead(syncPin) != HIGH) {};
  Serial.println("Received master signal...");
}

//uint16_t output[300];

void loop() {
  if (dataFile.available()) {
    int sensorValue = dataFile.parseInt();
    sensorValue = (sensorValue - 2000) * 9;
    //Serial.println(sensorValue);
    myDac.analogWrite(sensorValue);    
    delayMicroseconds(4);
  }
  else {
    // Close the file when done reading
    dataFile.close();
    Serial.println("Data playback complete.");
    dataFile = SD.open("output_9_T2.txt", FILE_READ);
    if (!dataFile) 
    {
      Serial.println("Delaying...");
      delayMicroseconds(10000);
    //while (true) {} // Stop the loop
    }
  }
}
