float voltBat; //35
int iVoltBat; //35

void setup() {
  Serial.begin(115200);
}

void loop() {
  // 센서에서 값 읽기:
  iVoltBat = analogRead(34);
  // AD 컨버터 값을 사용하여 전압 예측:
  voltBat = 0.00164 * iVoltBat + 0.1723;
  Serial.print("ADC Voltage: ");
  Serial.println(voltBat);

  // 센서에서 값 읽기:
  iVoltBat = analogRead(35);
  // AD 컨버터 값을 사용하여 전압 예측:
  voltBat = 0.0049 * iVoltBat + 0.7452;
  Serial.print("Bat Voltage: ");
  Serial.println(voltBat);

  delay(2000);
}
