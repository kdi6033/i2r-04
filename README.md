# i2r-04 IoT PLC
WiFi Bluetooth PLC (4채널 릴레이, , ESP32) KC인증  
인터넷 환경에서 원격으로 모니터링/제어 할수 있는 PLC 입니다.  
![i2r-04-포트설명](자료/i2r-04-pin.jpg)

📌 사양
- 정격전압 : 5V DC, 보드내에서는 5V로 설계했습니다.
- 입력전압 : 7 ~ 26V DC Free Volt, 7 ~ 26V 사이 전압을 공급하면         레귤레이터에서 5V 로 전원을 공급합니다.
- 작동온도 : -40 ℃ - 85 ℃
- 입력 : 8개, 접점만 연결되면 동작합니다. 별도의 전압을 인가하면 고장의 원인이 됩니다.
- 출력 : 1개는 30A 250VAC/30VDC
7개는 10A VAC, 10A VDC, 10A 125VAC, 10A 28VDC
- 통신: WIFI 802.11 b / g / n (802.11n에서 최대 150Mbps) 및 Bluetooth 4.2 BR / EDR + BLE
와이파이는 2.4G에 연결하세요. 5G는 동작하지 않습니다.
- RS232 통신 : 보드내에 TTL Level의 rx, tx 단자가 있습니다.

스마트폰에 어플 설치와 와이파이 연결을 보여줍니다. 그림을 크릭하세요  
[아두이노 소스프로그램 링크](https://github.com/kdi6033/i2r-04/tree/main/0%20Source-Program-IoT/board-i2r-04)  
[AWS 아두이노 소스프로그램 링크](https://github.com/kdi6033/i2r-04/tree/main/0%20Source-Program-IoT/board-i2r-04-aws)  
[![21-3 안드로이드 어플 사용 블루투스 와이파이 MQTT 통신](https://img.youtube.com/vi/FT0muFM24xc/0.jpg)](https://youtu.be/FT0muFM24xc)
 1) 8채널 릴레이  
8채널 릴레이가 탑재된 보드입니다. 릴레이 출력단에 A접점을 활용해 장치를 연결할 수 있습니다.
다양한 장치를 연결해서 손쉽게 원격제어 시스템을 구현해보세요. 모든 소스프로그램은 설명글 하단을 참조하세요.
릴레이에 연결된 ESP32핀은 왼쪽부터 각각 <br>
출력 : 25 23 22 21 33 32 27 26  <br>
입력 : 18 19 4 5 21 22 23 15
![i2r-02-포트설명](https://drive.google.com/uc?id=1X0wcNuqFN-zJ07sOzUsBimr1k4QfJlmA)
1) WiFi, BLE 통신  
ESP32가 탑재되어 WiFi, BLE 통신 가능합니다. WiFi 를 활용해 PC 및 스마트폰에서 4채널 릴레이를
원격제어 및 모니터링 가능합니다. 
BLE 를 활용해 근거리 제어가 가능합니다. IoT와 관련해 다양하게 활용 가능합니다.
![i2r-04-포트설명](https://drive.google.com/uc?id=1ADQGv7T-kg5jTpx5lF-GrZ1OTK7YDSBV)
# AD 컨버터 입력전압 측정
보드의 34, 35 핀에 AD콘버터가 있어서 전압을 측정할수 있습니다. 이를 측정하는 프로그램 설명 영상입니다.<br>
[아두이노 소스프로그램 링크](https://github.com/kdi6033/i2r-04/blob/main/2%20volt%20value/voltValue/voltValue.ino)  
[![Input Output 아두이노 프로그램](https://drive.google.com/uc?id=1XybM7WA4IEEQsOG3KhKcRVjbnA9dZqdu)](https://www.youtube.com/watch?v=lZQ763ljGjE)

# MQTT 통신 연결하기
아두이노로 mqtt 통신을 연결한다.
ArduinoJson.h 를 사용해 데이터 처리방법을 설명한다.
IoT MQTT Panel을 이용해 스마트폰으로 보드의 Relay를 제어 한다.
이 프로그램을 이용해 인터넷 상에서 원격으로 입력i과 출력을 제어 할 수 있습니다.
[![MQTT 통신 연결하기](https://img.youtube.com/vi/u4NejCu5xnw/0.jpg)](https://youtu.be/u4NejCu5xnw)

# i2r-04-motor 
<img src="https://github.com/user-attachments/assets/e2b28820-5f75-4787-9c6b-06e767e0ff05" alt="i2r-04-motor" width="500">
<img width="271" height="186" alt="image" src="https://github.com/user-attachments/assets/7e89cdee-cd46-428f-a51d-386c9fe9e315" />    

## 1. (주)우성하이텍 WSM-4035 모터 제어

📌 제품 개요
WSM-4035는 환기창(루버, 온실, 축사 등)의 개폐 제어를 위해 사용되는 DC24V 구동 모터입니다.
헤리컬 & 유성 기어를 통한 감속 방식을 적용하여 높은 내구성과 안정성을 제공하며, LED 표시등을 통해 동작 상태를 확인할 수 있습니다.
본 프로젝트에서는 ESP32 기반 IoT 제어 보드와 MQTT 통신을 활용하여 WSM-4035 모터를 원격 제어할 수 있도록 구현하였습니다.

⚙️ 주요 사양 (WSM-4035 기준)
| 구분            | 사양                |
| ------------- | ----------------- |
| **정격 전압**     | DC 24V            |
| **안전 사용 전류**  | 2.6A              |
| **회전수**       | 3.5 rpm           |
| **토크**        | 4 Kg·m            |
| **최대 전류**     | 3.5A              |
| **최대 회전수**    | 3.3 rpm           |
| **최대 토크**     | 6 Kg·m            |
| **리미트 설정 범위** | 35                |
| **본체 무게**     | 2.4 kg            |
| **감속 방식**     | 헤리컬 & 유성기어        |
| **작동 표시**     | 2컬러 LED (적색/녹색)   |
| **기본 제공 부품**  | ①②③④⑤⑥ (제공 그림 참조) |

🔧 리미트 스위치 설정 방법 (WSM-4035 기준)
WSM 시리즈 모터(4035 포함)는 **내부에 리미트 스위치(limit switch)**가 내장되어 있어서, 사용자가 원하는 개폐 위치에서 리미트 나사를 돌려 설정할 수 있습니다.
1. 전원 차단
- 반드시 DC24V 전원을 끈 상태에서 작업하세요.
2.모터 구동 → 원하는 위치까지 이동
- 환기창(루버)을 열고/닫아 원하는 위치까지 움직입니다.
3. 리미트 스위치 조정 나사 확인
- 모터 본체 측면 또는 기어박스 커버 안쪽에 **리미트 조정 나사(2개)**가 있습니다.
- 하나는 열림(OPEN) 위치, 다른 하나는 닫힘(CLOSE) 위치를 담당합니다.
4. 리미트 나사 돌려 설정
- 드라이버로 나사를 돌려서 원하는 위치에서 멈추도록 설정합니다.
- ↻ 시계 방향: 동작 범위를 좁힘
- ↺ 반시계 방향: 동작 범위를 넓힘
5. 테스트
- 전원을 다시 넣고 열림/닫힘 동작을 반복하여 정확히 원하는 지점에서 멈추는지 확인합니다.

📌 회로도    
다음과 같이 배선하세요 총 4개의 모터를 제어 할 수 있습니다.    
<img src="https://github.com/user-attachments/assets/a9e3da2a-086e-427b-ba0d-243f25d9452f" alt="회로도" width="700">

## 2. CrowPanel Pico Display 3.5" HMI 모듈

이 보드는 RP2040 MCU + 3.5" 480×320 TFT LCD + 정전식 터치스크린이 결합된 HMI(Human Machine Interface) 모듈입니다. LVGL, C/C++, MicroPython을 지원하여 다양한 UI 및 IoT 응용에 활용할 수 있습니다.

<img src="https://github.com/user-attachments/assets/15416181-7861-4fea-976b-fcb33cc896f0" alt="회로도" width="700">

📌 주요 기능
- RP2040 듀얼코어 MCU 내장 → 외부 보드 없이 단독 동작 가능
- 3.5인치 480×320 해상도 TFT 디스플레이 → 선명한 그래픽 표현
- 정전식 터치스크린 → 직관적인 UI 제어 가능
- LVGL 지원 → 버튼, 슬라이더, 차트, 키보드 등 고급 GUI 구성
- USB-C 포트 → 전원 공급 및 펌웨어 업로드 지원
- C/C++ & MicroPython 개발 환경 → 초보자부터 전문가까지 사용 가능
- GPIO 확장 핀 제공 → 센서, 액추에이터 등 외부 장치 연결 가능
- HMI 전용 설계 → IoT, 스마트 제어, 교육용 UI 개발에 최적화

⚙️GPIO Pin Definition

| Pin | Function       | Pin | Function               |
| --- | -------------- | --- | ---------------------- |
| P1  | GP0 / UART0 TX | P9  | GP19                   |
| P2  | GP1 / UART0 RX | P10 | GP20 / I2C0 SDA        |
| P3  | GP2 / I2C1 SDA | P11 | GP21 / I2C0 SCL        |
| P4  | GP3 / I2C1 SCL | P12 | GP26 / ADC1 / I2C1 SDA |
| P5  | GP4 / UART1 TX | P13 | GP27 / ADC0 / I2C1 SCL |
| P6  | GP5 / UART1 TX | P14 | GP28 / ADC2            |
| P7  | GP6 / I2C1 SDA | P15 | GND                    |
| P8  | GP7 / I2C1 SCL | P16 | VCC 3V3                |

## 3. 조도센서 GY302 BH1750

<img src="https://github.com/user-attachments/assets/de3ce3e5-becb-4446-8a8b-4344aabcc9b7" alt="조도센서" width="350">

BH1750FVI 칩을 사용한 디지털 조도 센서 모듈로, I2C 인터페이스를 통해 간단하게 광량(lux)을 측정할 수 있습니다.
직접 ADC 변환이나 보정 과정이 필요 없으며, 다양한 IoT 및 임베디드 프로젝트에서 주변 밝기 감지 및 자동 제어 용도로 활용할 수 있습니다.

📌 주요 기능
- I2C 인터페이스 → 간단한 연결 및 통신
- 16bit A/D 변환기 내장 → 고해상도 측정 지원
- Lux 단위 직접 출력 → 보정/계산 과정 불필요
- 인간 시각 특성과 유사한 스펙트럼 감도 → 실제 체감 밝기에 근접
- 넓은 측정 범위 (1 ~ 65535 lux) → 저조도부터 강한 빛까지 커버
- 자동화/IoT 제어에 적합 → 조명 제어, 화면 밝기 자동 조절 등

⚙️ 사양
- 칩셋: ROHM BH1750FVI
- 동작 전압: 3.0V ~ 5.0V
- 데이터 범위: 0 ~ 65535 lux
- 모듈 크기: 13.9mm × 18.5mm
