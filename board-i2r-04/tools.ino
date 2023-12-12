void parseJSONPayload(byte* payload, unsigned int length) {
  char payloadStr[length + 1];
  memcpy(payloadStr, payload, length);
  payloadStr[length] = '\0';  // Null-terminate the string
  Serial.println(payloadStr);

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, payloadStr);

  if (error) {
    Serial.println("JSON 파싱 실패!");
    return;
  }

  int order = doc["order"] | -1;
  if (order == 0) {
    writeToBle(order);
    download_program("i2r-04.ino.bin");
  }
  else if (order == 1) {
    const char *ssid = doc["ssid"] | "";
    const char *password = doc["password"] | "";
    const char *mqttBroker = doc["mqttBroker"] | "";
    const char *email = doc["email"] | "";
    const bool use = doc["wifiUse"] | false;

    wifiSave.ssid = ssid;
    wifiSave.password = password;
    wifiSave.mqttBroker=mqttBroker;
    wifiSave.email=email;
    wifiSave.use = use;
    returnMsg=wifiSave.ssid+" 정보가 저장 되었습니다.";
    writeToBle(101);

    //Serial.print("wifi.ssid: "); Serial.println(wifiSave.ssid);
    //Serial.print("wifi.password: "); Serial.println(wifiSave.password);
    //Serial.print("wifi.mqttBroker: "); Serial.println(wifiSave.mqttBroker);
    //Serial.print("wifi.use: "); Serial.println(wifiSave.use);
    saveConfigToSPIFFS();
  }
  else if (order == 2) {
    // JSON 메시지에서 "no"와 "value" 값을 읽어옵니다.
    int no = doc["no"] | -1;  // 유효하지 않은 인덱스로 초기화
    bool value = doc["value"] | false;
    dev.out[no]=value;

    // "no" 값이 유효한 범위 내에 있는지 확인하고, "out" 배열에 "value"를 설정합니다.
    if (no >= 0 && no < 8) {
      dev.out[no] = value ? 1 : 0;  // "true"이면 1로, "false"이면 0으로 설정
      Serial.print("out[");
      Serial.print(no);
      Serial.print("] 값이 ");
      Serial.print(value ? "true" : "false");
      Serial.println("로 설정되었습니다.");
      returnMsg=String(no)+"번이 "+value+" 설정 되었습니다.";
    } else {
      Serial.println("유효하지 않은 'no' 값입니다.");
    }
    //Serial.println(outputPins[no]);
    //Serial.println(dev.out[no]);
    digitalWrite(outputPins[no], dev.out[no]);
  }
  else if (order==3) {
    bool value = doc["value"] | false;
    wifi.selectMqtt = value;
    if(value == true)
      returnMsg="mqtt로 통신 합니다.";
    else
      returnMsg="블루투스로 통신 합니다.";
  }
  returnMessage();
}

void returnMessage() {
  DynamicJsonDocument responseDoc(1024);
  responseDoc["order"] = 101;
  responseDoc["message"] = returnMsg;
  dev.sendData="";
  serializeJson(responseDoc, dev.sendData);
  Serial.print("returnMessage: ");
  Serial.println(dev.sendData);
  if(wifi.selectMqtt == true) 
    publishMqtt();
  else
    writeToBle(101);
}

//1초 마다 실행되는 시간함수
void doTick() {
  unsigned long currentTime = millis();  // 현재 시간을 가져옵니다
  String strIn,strOut;
  if ( currentTime - lastTime >= interval) {
    lastTime = currentTime;
    // 센서 값 읽기
    for (int i = 0; i < 8; i++) {
      dev.in[i] = digitalRead(inputPins[i]);
    }
     // Bat 센서에서 값 읽기 및 전압 계산
    int iVoltBat = analogRead(34);
    dev.voltAdc = 0.00164 * iVoltBat + 0.1723;

    // ADC 센서에서 값 읽기 및 전압 계산
    iVoltBat = analogRead(35);
    dev.voltBat = 0.0049 * iVoltBat + 0.7452;

    
    // in, out 값 문자열로 변환
    strIn = String(dev.in[0])+String(dev.in[1])+String(dev.in[2])+String(dev.in[3])
            +String(dev.in[4])+String(dev.in[5])+String(dev.in[6])+String(dev.in[7]);
    strOut = String(dev.out[0])+String(dev.out[1])+String(dev.out[2])+String(dev.out[3])
            +String(dev.out[4])+String(dev.out[5])+String(dev.out[6])+String(dev.out[7]);
    dev.strInPre=dev.strIn;
    // 데이터 변경 여부 확인
    bool dataChanged = !dev.strIn.equals(dev.strInPre) || dev.voltBat != lastVoltBat || dev.voltAdc != lastVoltAdc;

    if (dataChanged) {
      // JSON 문서 생성 및 초기화
      DynamicJsonDocument responseDoc(1024);
      responseDoc["order"] = 3;
      responseDoc["in"] = strIn;
      responseDoc["out"] = strOut;
      responseDoc["bat"] = String(dev.voltBat, 1); 
      responseDoc["adc"] = String(dev.voltAdc, 1);

      // 전송 데이터 설정
      dev.sendData="";
      serializeJson(responseDoc, dev.sendData);
      // Serial.print("wifi.selectMqtt: ");
      // Serial.println(wifi.selectMqtt);
  // 전송 로직
    // 조건 1: BLE 연결되어 있으면 BLE를 통해 전송
    if (ble.isConnected && pCharacteristic) {
      writeToBle(2);
      Serial.println("Data sent via BLE");
    }
    // 조건 2: BLE 연결이 없고, MQTT 연결 정보가 있으며, wifi.selectMqtt가 true일 경우 MQTT를 통해 전송
    else if (!ble.isConnected && wifi.ssid[0] != '\0' && wifi.selectMqtt) {
      if (!client.connected()) {
        reconnectMQTT();
      }
      if (client.connected()) {
        publishMqtt();
        Serial.println("Data sent via MQTT");
      }
    }
    // 조건 3: BLE 연결이 없고, MQTT 연결 정보가 있으나 wifi.selectMqtt가 false일 경우 MQTT를 통해 전송
    else if (!ble.isConnected && wifi.ssid[0] != '\0' && !wifi.selectMqtt) {
      if (!client.connected()) {
        reconnectMQTT();
      }
      if (client.connected()) {
        publishMqtt();
        Serial.println("Data sent via MQTT with stored settings");
      }
    }
    // 이전 bat와 adc 값을 업데이트
      lastVoltBat = dev.voltBat;
      lastVoltAdc = dev.voltAdc;
      Serial.println(dev.sendData);
    } // changed
  } // internal
} // dotick
// Config 파일을 SPIFFS에서 읽어오는 함수
void loadConfigFromSPIFFS() {
  Serial.println("파일 읽기");

  if (!initializeSPIFFS()) {
    Serial.println("Failed to initialize SPIFFS.");
    return;
  }

  if (!SPIFFS.exists("/config.txt")) {
    Serial.println("Config file does not exist.");
    return;
  }

  File configFile = SPIFFS.open("/config.txt", FILE_READ);
  if (!configFile) {
    Serial.println("Failed to open config file");
    return;
  }

  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, buf.get());
  
  if (error) {
    Serial.println("Failed to parse config file");
    return;
  }

  wifi.ssid = doc["ssid"] | "";
  wifi.password = doc["password"] | "";
  wifi.mqttBroker = doc["mqttBroker"] | "";
  wifi.email = doc["email"] | "";
  wifi.use = doc["use"] | false;

  Serial.print("wifi.ssid: "); Serial.println(wifi.ssid);
  Serial.print("wifi.password: "); Serial.println(wifi.password);
  Serial.print("wifi.mqttBroker: "); Serial.println(wifi.mqttBroker);
  Serial.print("wifi.email: "); Serial.println(wifi.email);
  Serial.print("wifi.use: "); Serial.println(wifi.use);

  configFile.close();
}

void saveConfigToSPIFFS() {
  Serial.println("config.txt 저장");

  if (!initializeSPIFFS()) {
    Serial.println("SPIFFS 초기화 실패.");
    return;
  }

  // SPIFFS 초기화를 시도합니다.
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS failed to initialize. Formatting...");
    // 초기화 실패 시 포맷을 시도합니다.
    if (!SPIFFS.format()) {
      Serial.println("SPIFFS format failed.");
      return;
    }
    // 포맷 후에 다시 초기화를 시도합니다.
    if (!SPIFFS.begin()) {
      Serial.println("SPIFFS failed to initialize after format.");
      return;
    }
  }

  File configFile = SPIFFS.open("/config.txt", FILE_WRITE);
  
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return;
  }

  DynamicJsonDocument doc(1024);

  // 데이터를 구조체에서 가져온다고 가정합니다.
  doc["ssid"] = wifiSave.ssid;
  doc["password"] = wifiSave.password;
  doc["mqttBroker"] = wifiSave.mqttBroker;
  doc["email"] = wifiSave.email;
  doc["use"] = wifiSave.use;

  Serial.print("wifi.ssid: "); Serial.println(wifiSave.ssid);
  Serial.print("wifi.password: "); Serial.println(wifiSave.password);
  Serial.print("wifi.mqttBroker: "); Serial.println(wifiSave.mqttBroker);
  Serial.print("wifi.email: "); Serial.println(wifiSave.email);
  Serial.print("wifi.use: "); Serial.println(wifiSave.use);

  if (serializeJson(doc, configFile) == 0) {
    Serial.println("Failed to write to file");
    configFile.close();
    return;
  }

  configFile.close();
  // 파일이 제대로 닫혔는지 확인합니다.
  if (configFile) {
    Serial.println("파일이 여전히 열려있습니다.");
  } else {
    Serial.println("파일이 성공적으로 닫혔습니다.");
  }
  Serial.println("파일 저장 끝");

  // 파일이 제대로 저장되었는지 확인합니다.
  if (SPIFFS.exists("/config.txt")) {
    Serial.println("Config file saved successfully.");
    // 저장이 확인된 후 재부팅을 진행합니다.
    Serial.println("Rebooting...");
    delay(1000); // 재부팅 전에 짧은 지연을 줍니다.
    ESP.restart();
  } else {
    Serial.println("Config file was not saved properly.");
  }
  
  // ESP32 재부팅
  delay(1000);
  ESP.restart();
}

// SPIFFS를 초기화하고 필요한 경우 포맷하는 함수를 정의합니다.
bool initializeSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS 초기화 실패!");
    if (!SPIFFS.format()) {
      Serial.println("SPIFFS 포맷 실패!");
      return false;
    }
    if (!SPIFFS.begin()) {
      Serial.println("포맷 후 SPIFFS 초기화 실패!");
      return false;
    }
  }
  return true;
}

void saveConfigToSPIFFS01() {
  Serial.println("Save config.txt");
  // SPIFFS 초기화 체크
  if (!SPIFFS.begin(true)) { // true 파라미터는 SPIFFS가 초기화되지 않았을 때 자동으로 포맷하도록 합니다.
    Serial.println("SPIFFS failed to initialize. Formatting...");
    SPIFFS.format();
  }

  // config.txt 파일로 설정 정보 저장
  File configFile = SPIFFS.open("/config.txt", FILE_WRITE);
  
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return;
  }

  DynamicJsonDocument doc(1024);
  bool a=true;

  // JSON 객체에 데이터 저장
  doc["ssid"] = "aa";
  doc["password"] = "bb";
  doc["mqttBroker"] = "cc";
  doc["email"] = "dd";
  doc["use"] = a;

  // 파일에 JSON 시리얼라이즈
  if (serializeJson(doc, configFile) == 0) {
    Serial.println("Failed to write to file");
  }

  // 파일 닫기
  configFile.close();
  Serial.println("파일저장 끝");

  // ESP32 재부팅
  delay(1000);
  ESP.restart();
  
}

void checkFactoryDefault() {
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    Serial.println("Please wait over 3 min");
    SPIFFS.format();
    delay(1000);
    ESP.restart();
    delay(1000);
  }
}
// httpupdate()
void download_program(String fileName) {
  Serial.println(fileName);
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    // The line below is optional. It can be used to blink the LED on the board during flashing
    // The LED will be on during download of one buffer of data from the network. The LED will
    // be off during writing that buffer to flash
    // On a good connection the LED should flash regularly. On a bad connection the LED will be
    // on much longer than it will be off. Other pins than LED_BUILTIN may be used. The second
    // value is used to put the LED on. If the LED is on with HIGH, that value should be passed
    httpUpdate.setLedPin(ledPin, LOW);

    // Add optional callback notifiers
    httpUpdate.onStart(update_started);
    httpUpdate.onEnd(update_finished);
    httpUpdate.onProgress(update_progress);
    httpUpdate.onError(update_error);

    String ss;
    //ss=(String)URL_fw_Bin+fileName;
    ss="http://i2r.link/download/"+fileName;
    Serial.println(ss);
    t_httpUpdate_return ret = httpUpdate.update(client, ss);
    //t_httpUpdate_return ret = ESPhttpUpdate.update(client, URL_fw_Bin);
    // Or:
    //t_httpUpdate_return ret = ESPhttpUpdate.update(client, "server", 80, "file.bin");
    
    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
        break;
  
      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;
  
      case HTTP_UPDATE_OK:
        Serial.println("HTTP_UPDATE_OK");
        break;
    }
  }
}

void update_started() {
  Serial.println("Update Started");
}

void update_finished() {
  Serial.println("Update Finished");
}

void update_progress(int cur, int total) {
  Serial.printf("Progress: %d%%\n", (cur * 100) / total);
}

void update_error(int error) {
  Serial.printf("Update Error: %d\n", error);
}

