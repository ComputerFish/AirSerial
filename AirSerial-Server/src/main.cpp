#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 17
#define CSN_PIN 5

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

// Setup radio settings
void setup() {
  Serial.begin(115200);

  radio.begin();
  radio.setRetries(5, 15);         // delay=5*250µs=1250µs, count=15
  radio.setPALevel(RF24_PA_LOW);   // Power amplifier level
  radio.setDataRate(RF24_1MBPS);
  radio.setCRCLength(RF24_CRC_16);
  radio.openWritingPipe(address);
  radio.stopListening();
}

// Basic flow:
// Read data from physical serial port
// and forward it to RF24 radio to transmit
void loop() {
  static char buffer[32];
  static uint8_t index = 0;

  while (Serial.available()) {
    char c = Serial.read();
    buffer[index++] = c;

    // If buffer full, send immediately
    if (index >= sizeof(buffer)) {
      radio.write(buffer, index);
      index = 0;
    }
  }

  // Flush partial buffer if some data is waiting and no new bytes arrive
  if (index > 0) {
    delay(2); // small gap to see if more bytes arrive
    if (!Serial.available()) {
      radio.write(buffer, index);
      index = 0;
    }
  }
}
