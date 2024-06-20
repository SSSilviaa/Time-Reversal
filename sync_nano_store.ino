const int syncPin = 2; // Synchronization pin (can be any available pin)

volatile bool syncReceived = false; // Flag to indicate synchronization signal received

void setup() {
  pinMode(syncPin, INPUT_PULLUP); // Set the synchronization pin as input with internal pull-up resistor
  
  // Attach an interrupt to the synchronization pin
  attachInterrupt(digitalPinToInterrupt(syncPin), syncInterrupt, FALLING);
  
  // Wait for the synchronization signal
  while (!syncReceived) {
    Serial.print("Not synched");
    Serial.println(" ");// Do nothing, wait for synchronization
  }

  Serial.print("Synched!");
  
  // Synchronization signal received, start sending or receiving
  // Add your sending or receiving code here
}

void loop() {
  // Main loop code
  // Add your main loop code here
}

// Interrupt service routine for the synchronization pin
void syncInterrupt() {
  syncReceived = true; // Set the flag to indicate synchronization signal received
}