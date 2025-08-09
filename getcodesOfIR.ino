#include <IRremote.h>

const int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();  // Start the receiver
  Serial.println("IR Code Scanner - Press buttons to see their codes!");
}

void loop() {
  if (irrecv.decode()) {
    Serial.print("IR Code: 0x");
    Serial.println(irrecv.decodedIRData.decodedRawData, HEX);

    irrecv.resume();  // Receive the next value
  }
}
