// Slave Teensy 4.1 Code

#include <ADC.h>
#include <IntervalTimer.h>
#include <SD.h>
#include <SPI.h>

#define SAMPLE_SIZE 1024
#define SAMPLE_RATE 1000 // 1000 Hz sampling rate

const int syncPin = 7;
const int adcPin = A0;  // ADC pin for Slave
const int chipSelect = BUILTIN_SDCARD;

ADC *adc = new ADC();
IntervalTimer timer;

volatile int samples[SAMPLE_SIZE];
volatile int sampleIndex = 0;
volatile bool isSampling = false;
volatile bool isOutputting = false;

unsigned long startTime;
const unsigned long duration = 60000; // 60 seconds

File myFile;

void setup() {
  pinMode(syncPin, INPUT);
  
  Serial.begin(9600);
  while (!Serial) ; // Wait for Serial
  Serial.println("Slave Teensy initializing...");
  
  adc->adc0->setAveraging(1);
  adc->adc0->setResolution(12);
  adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_HIGH_SPEED);
  adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED);
  
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  myFile = SD.open("adc_slave.txt", FILE_WRITE);
  
  Serial.println("Waiting for sync signal from Master...");
  while (digitalRead(syncPin) == LOW) ;
  while (digitalRead(syncPin) == HIGH) ;
  
  startTime = millis();
  isSampling = true;
  
  timer.begin(sampleAndOutput, 1000000 / SAMPLE_RATE);
}

void loop() {
  if (millis() - startTime >= duration) {
    myFile.close();
    Serial.println("File closed after one minute.");
    timer.end();
    while (true); // Halt further execution
  }
}

void sampleAndOutput() {
  if (isSampling) {
    if (sampleIndex < SAMPLE_SIZE) {
      int signal = adc->adc0->analogRead(adcPin);
      samples[sampleIndex] = signal;
      sampleIndex++;
    } else {
      isSampling = false;
      isOutputting = true;
      sampleIndex = 0;
    }
  } else if (isOutputting) {
    if (sampleIndex < SAMPLE_SIZE) {
      if (myFile) {
        myFile.println(samples[sampleIndex]);
      } else {
        Serial.println("error opening file");
      }
      Serial.println(samples[sampleIndex]);
      sampleIndex++;
    } else {
      isOutputting = false;
      isSampling = true;
      sampleIndex = 0;
    }
  }
}
