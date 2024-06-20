// master
const int syncPin = 7; // Pin to send synchronization signal

void setup() {
  pinMode(syncPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println("Sending synchronization signal...");
  digitalWrite(syncPin, HIGH);
  delay(100); // Pulse duration
  digitalWrite(syncPin, LOW);
  delay(1000); // Wait for 1 second before sending the next signal
}

// slave
// const int syncPin = 7; // Pin to receive synchronization signal

// void setup() {
//   pinMode(syncPin, INPUT);
//   Serial.begin(9600);
// }

// void loop() {
//   if (digitalRead(syncPin) == HIGH) {
//     Serial.println("Synchronization signal received!");
//     // Add code to be executed upon receiving the synchronization signal
//     while (digitalRead(syncPin) == HIGH); // Wait for the signal to go LOW again
//   }
// }
