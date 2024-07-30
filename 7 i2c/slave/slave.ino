#include <Wire.h>

#define I2C_SLAVE_ADDR 0x01
#define SCL_PIN 17
#define SDA_PIN 16
String message = "";

// I2C 데이터 수신 이벤트 핸들러
void receiveEvent(int howMany) {
  while (Wire.available()) {
    char c = Wire.read();
    message += c;
  }
  Serial.print("Received: ");
  Serial.println(message);
  message = "";
}

// I2C 데이터 요청 이벤트 핸들러
void requestEvent() {
  Wire.write("Hello, Master!");
}

void setup() {
  Serial.begin(115200);
  
  // I2C 슬레이브 모드 설정
  Wire.begin(I2C_SLAVE_ADDR, SDA_PIN, SCL_PIN, 100000);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  Serial.println("I2C slave start");
}

void loop() {
  // 슬레이브 모드에서 데이터를 수신하는 콜백이 자동으로 호출됩니다.
  // 별도의 루프 처리가 필요하지 않습니다.
  delay(100);
}
