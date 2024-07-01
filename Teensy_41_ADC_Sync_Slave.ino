// Teensy Slave
#include <ADC.h>
#include <IntervalTimer.h>
#include <SD.h>
#include <SPI.h>

#define SAMPLE_SIZE 1024  // Define the size of the sample buffer

const int syncPin = 7; // Pin to send synchronization signal
const int adcPin = A2;  // Analog input pin
const int chipSelect = BUILTIN_SDCARD;

ADC *adc = new ADC();
IntervalTimer timer;

int samples[SAMPLE_SIZE];  // Buffer to store samples
int sampleIndex = 0;
bool isSampling = false;
unsigned long startTime;
const unsigned long duration = 60000; // 60 seconds for demo

File myFile;

void setup() {
  pinMode(syncPin, INPUT);
  pinMode(adcPin, INPUT);
  Serial.begin(9600);
  
  Serial.println("INPUT - Receiving synchronization signal...");
  
  // wait until mater signal
  while (digitalRead(syncPin) != HIGH) {};
  Serial.println("Received master signal...");

  // Initialize ADC
  adc->adc0->setAveraging(1);
  adc->adc0->setResolution(12);
  adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_HIGH_SPEED);
  adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED);
  adc->adc0->startContinuous(adcPin);

  // Initialize SD card
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  myFile = SD.open("adc_master.txt", FILE_WRITE);
  
  isSampling = true;
  startTime = millis(); // Record the start time

  timer.begin(outputDAC, 1); // 1 microsecond timer
}


void loop() {
  if (isSampling) {
    if (sampleIndex < SAMPLE_SIZE) {
      int signal = adc->adc0->analogReadContinuous();
      // Serial.println(signal);
      samples[sampleIndex] = signal;
      sampleIndex++;
      delayMicroseconds(10);
    } else {
      isSampling = false;
      sampleIndex = 0;
    }
  }
  if (millis() - startTime >= duration) {
      myFile.close(); // Close the file after one minute
      Serial.println("File closed after one minute.");
      // Optionally, stop writing to the file
      while (true); // Halt further execution
  } 
}

void outputDAC() {
  if(!isSampling && (millis() - startTime < duration)) {
    //analogWrite(dacPin, samples[sampleIndex]);
    if (myFile) {
      myFile.println(samples[sampleIndex]);
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening test.txt");
    }
    Serial.println(samples[sampleIndex]);
    sampleIndex++;
    if(sampleIndex >= SAMPLE_SIZE) {
      sampleIndex = 0;
      isSampling = true;
    }
  }
}

