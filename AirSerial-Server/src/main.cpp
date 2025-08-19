#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 17
#define CSN_PIN 5

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

void setup() {
  Serial.begin(115200);

  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.setCRCLength(RF24_CRC_16);
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop() {
  static char line[32];  // NRF24 max payload = 32
  static uint8_t idx = 0;

  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n') {
      // End of message -> null terminate + send
      line[idx] = '\0';
      radio.write(line, idx + 1);   // include terminator
      idx = 0;                      // reset buffer
    } else if (idx < sizeof(line) - 1) {
      line[idx++] = c;              // accumulate chars
    } else {
      // overflow safety
      idx = 0;
    }
  }
}
