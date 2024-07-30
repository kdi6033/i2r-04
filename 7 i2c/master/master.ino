#include <Wire.h>

#define I2C_SLAVE_ADDR 0x01
#define SCL_PIN 17
#define SDA_PIN 16

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.begin(115200);
  Serial.println("i2c start");
}

void loop() {
  Wire.beginTransmission(I2C_SLAVE_ADDR);
  Wire.write("Hello, Slave!");
  Wire.endTransmission();
  delay(1000);

  Wire.requestFrom(I2C_SLAVE_ADDR, 16); //슬레이브(1)에 13byte 요청
  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }
  Serial.println();
  delay(1000);
}
