const int servoPin1 = 13; // 서보 모터 1 연결 핀
const int servoPin2 = 12; // 서보 모터 2 연결 핀

int pulseWidth1; // 서보 모터 1 펄스 너비
int pulseWidth2; // 서보 모터 2 펄스 너비
void setup() {
  pinMode(servoPin1, OUTPUT); // 서보 모터 1 핀 출력으로 설정
  pinMode(servoPin2, OUTPUT); // 서보 모터 2 핀 출력으로 설정
}

void loop() {
  int sensorValue1 = analogRead(A0); // 아날로그 센서 1 값 읽기
  int sensorValue2 = analogRead(A1); // 아날로그 센서 2 값 읽기

  // 센서 값을 0~180도 범위의 각도 값으로 변환
  int angle1 = map(sensorValue1, 0, 1023, 0, 180);
  int angle2 = map(sensorValue2, 0, 1023, 0, 180);

  // 각도 값을 서보 모터 펄스 너비로 변환 (500~2500 마이크로초)
  pulseWidth1 = map(angle1, 0, 180, 500, 2500);
  pulseWidth2 = map(angle2, 0, 180, 500, 2500);

  // 서보 모터 제어
  digitalWrite(servoPin1, HIGH);
  delayMicroseconds(pulseWidth1);
  digitalWrite(servoPin1, LOW);
  delay(20); // 다음 펄스까지 대기 시간

  digitalWrite(servoPin2, HIGH);
  delayMicroseconds(pulseWidth2);
  digitalWrite(servoPin2, LOW);
  delay(20); // 다음 펄스까지 대기 시간
}