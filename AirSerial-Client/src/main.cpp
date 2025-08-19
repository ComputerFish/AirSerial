#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 17
#define CSN_PIN 5

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";
 
#define CLIENT_BAUDRATE 115200

// Setup radio settings
void setup() {
  Serial.begin(CLIENT_BAUDRATE);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setRetries(0, 0);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setCRCLength(RF24_CRC_16);
  radio.startListening();
}

// Basic Flow:
// 1. Check if data is available from the RF24 radio
// 2. If available, read the data into a buffer
// 3. Forward the data to the client Serial
void loop() {
  if (radio.available()) {
    char buffer[32];
    uint8_t len = radio.getDynamicPayloadSize(); // get actual size
    if (len == 0 || len > 32) len = 32;          // safety clamp
    radio.read(buffer, len);

    // Forward to client
    for (uint8_t i = 0; i < len; i++) {
      Serial.write(buffer[i]);
    }

    // Debug
    // Serial.print("Forwarded ");
    // Serial.print(len);
    // Serial.println(" bytes to client");
  }
}
