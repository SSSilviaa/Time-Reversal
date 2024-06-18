#include <ADC.h>
#include <IntervalTimer.h>
#include <SD.h>
#include <SPI.h>

#define SAMPLE_SIZE 1024  // Define the size of the sample buffer
#define AVERAGE_SAMPLES 16 // Number of samples to average


File myFile;

// Create an ADC object
ADC *adc = new ADC();
IntervalTimer timer;

const int adcPin = A2;  // Analog input pin
const int dacPin = A0;  // DAC output pin on Teensy 4.0 (A21 is pin 14)
const int chipSelect = BUILTIN_SDCARD;

int samples[SAMPLE_SIZE];  // Buffer to store samples
int sampleIndex = 0;
bool isSampling = true;

unsigned long startTime;
const unsigned long duration = 10000; // 1 minute in milliseconds

void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 baud rate
  Serial.println("Teensy 4.1 ADC to DAC example");

  pinMode(adcPin, INPUT);
  pinMode(dacPin, OUTPUT);

  adc->adc0->setAveraging(1); // Average 1 samples
  adc->adc0->setResolution(10); // 10-bit resolution
  adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_HIGH_SPEED); // High speed conversion
  adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED);
  adc->adc0->startContinuous(adcPin);
  analogWriteResolution(10);

  timer.begin(outputDAC,1);    

  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
  // open the file. 
  myFile = SD.open("adc.txt", FILE_WRITE);
  startTime = millis(); // Record the start time
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