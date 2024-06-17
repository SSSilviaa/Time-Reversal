#include <SPI.h>
#include <SD.h>

#define CS_PIN_SD 10
#define ANALOG_PIN A0
#define NUM_READINGS 1000 // Number of readings to store

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

  // Delete the existing file if it exists
  if (SD.exists("data.txt")) {
    SD.remove("data.txt");
  }

  // Open file for writing
  dataFile = SD.open("data.txt", FILE_WRITE);
  if (!dataFile) {
    Serial.println("Failed to open file for writing.");
    return;
  }

  // Configure ADC prescaler to 16
  ADCSRA &= ~(1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0); // Clear prescaler bits
  ADCSRA |= (1 << ADPS2); // Set prescaler to 16 (ADC clock = 1MHz)

  Serial.println("ADC configured for maximum speed.");

  // Read analog values and store in SD card
  for (int i = 0; i < NUM_READINGS; i++) {
    int sensorValue = i; // fastAnalogRead(ANALOG_PIN); //analogRead(ANALOG_PIN);
    dataFile.println(sensorValue);
    Serial.println(sensorValue);
    delayMicroseconds(1); // Adjust delay as needed
  }

  // Close the file
  dataFile.close();
  Serial.println("Data logging complete.");
}

void loop() {
  // Nothing to do here
}

int fastAnalogRead(uint8_t pin) {
  // Configure ADC mux to select the analog pin
  ADMUX = (1 << REFS0) | (pin & 0x07);

  // Start the conversion
  ADCSRA |= (1 << ADSC);

  // Wait for the conversion to complete
  while (ADCSRA & (1 << ADSC));

  // Return the ADC value
  return ADC;
}
