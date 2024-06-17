#include <SPI.h>
#include <SD.h>

#define CS_PIN_SD 4
#define ANALOG_PIN A1
#define NUM_READINGS 100 // Number of readings to store

volatile bool sampleReady = false;
int samples[NUM_READINGS];
int sampleIndex = 0;
File dataFile;

void setup() {
  Serial.begin(9600);
  pinMode(CS_PIN_SD, OUTPUT);
  pinMode(ANALOG_PIN, INPUT);

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

  // Configure Timer1 for 2kHz sampling rate (500 microseconds interval)
  noInterrupts(); // Disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 56999; // (16*10^6) / (2*10^3) - 1 (must be <65536)
  TCCR1B |= (1 << WGM12); // CTC mode
  TCCR1B |= (1 << CS10); // No prescaler
  TIMSK1 |= (1 << OCIE1A); // Enable timer compare interrupt
  interrupts(); // Enable all interrupts
}

ISR(TIMER1_COMPA_vect) {
  sampleReady = true;
}

void loop() {
  if (sampleReady) {
    sampleReady = false;
    if (sampleIndex < NUM_READINGS) {
      int sensorValue = analogRead(ANALOG_PIN);
      samples[sampleIndex++] = sensorValue;
    } else {
      // Calculate the average of the samples
      long sum = 0;
      for (int i = 0; i < NUM_READINGS; i++) {
        sum += samples[i];
      }
      float average = (float)sum / NUM_READINGS;

      // Write the average to SD card
      dataFile.println(average);
      Serial.println(average); // Optionally print to Serial Monitor

      dataFile.close();
      Serial.println("Data logging complete.");
      while (true) {} // Stop the loop
    }
  }
}