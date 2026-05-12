#include <SoftwareSerial.h>
SoftwareSerial picPort(D5, D6);

void setup() {
  Serial.begin(115200);
  picPort.begin(9600);
}
void loop() {
  picPort.write(0xBB);
  delay(300);            // let PIC read 0xBB + update LCD before next byte
  picPort.write(0x10);
  delay(300);            // let PIC read 0x10 + update LCD before next byte
  picPort.write(0xAA);
  Serial.println("TX: BB 10 AA");
  delay(10000);          // 10 s gap before next handshake
}