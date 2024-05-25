const int joystickXPin = A0; // 조이스틱 X 축 입력 핀
const int joystickYPin = A1; // 조이스틱 Y 축 입력 핀
const int servo1Pin = 9;    // MG996R 모터 1 제어 핀
const int servo2Pin = 10;   // MG996R 모터 2 제어 핀

int servo1Angle = 90;     // MG996R 모터 1 목표 각도 (초기값 90도)
int servo2Angle = 90;     // MG996R 모터 2 목표 각도 (초기값 90도)

int xVal;                // 조이스틱 X 축 값
int yVal;                // 조이스틱 Y 축 값

int servo1PulseWidth;    // MG996R 모터 1 펄스폭
int servo2PulseWidth;    // MG996R 모터 2 펄스폭

void setup() {
  pinMode(servo1Pin, OUTPUT);
  pinMode(servo2Pin, OUTPUT);

  // 타이머 1, 2 설정 (10kHz 주파수)
  TCCR1A = 0b01000000; // 10kHz 분할 비율 (prescaler) 설정
  TCCR1B = 0b00001001; // CTC 모드, 타이머 클럭 설정
  OCR1A = 255;           // 자동 재설정 값 (255 * 1/10kHz = 25.5ms)
  OCR1B = 255;           // 자동 재설정 값 (255 * 1/10kHz = 25.5ms)
  TIMSK1 = 0b00001001; // 타이머 1, 2 인터럽트 활성화

  sei(); // 인터럽트 허용
}

void loop() {
  // 조이스틱 X, Y 축 값 읽기
  xVal = analogRead(joystickXPin);
  yVal = analogRead(joystickYPin);

  // 목표 각도 계산 (예: 맵핑 함수 사용)
  servo1Angle = map(xVal, 0, 1023, 0, 180);
  servo2Angle = map(yVal, 0, 1023, 0, 180);

  // 각도를 펄스폭으로 변환 (7.5ms/180° * 목표 각도)
  servo1PulseWidth = 7.5 * servo1Angle / 180 + 500;
  servo2PulseWidth = 7.5 * servo2Angle / 180 + 500;

  // 지연 (필요 시)
  delay(10);
}