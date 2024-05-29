#define OC2A 0X08 // OC2A핀 정의

uint8_t pwm_level = 0; // PWM 레벨 변수
uint16_t pulse_width = 0; // 펄스 폭 변수

ISR(TIMER2_COMPA_vect){
    OCR2A = pulse_width; // 펄스 폭 설정
  }

void loop() {
    if (Serial.available()){
    pwm_level = Serial.parseInt();
    pwm_level = constrain(pwm_level, 0, 180); // PWM 레벨 범위 제한 (0~180)
    pulse_width = map(pwm_level, 0, 180, 500, 2500); // PWM 레벨을 펄스 폭으로 변환
    }
}

void setup() {
  cli(); // 인터럽트 비활성화

  // OC2A 핀 출력 설정
  DDRB |= OC2A;

  // 타이머2 설정
  TCCR2A |= (1<<WGM21)|(1<<WGM20); // CTC 모드 설정
  TCCR2B |= (0<<CS22)|(0<<CS21)|(1<<CS20); // 프리스케일러 1:1 설정
  TCCR2A |= (1<<COM2A1)|(0<<COM2A0); // OC2A 출력 모드 설정 (Clear OC2A on Compare Match)

  // 인터럽트 활성화
  OCR2A = pulse_width; // 펄스 폭 설정
  TIMSK2 |= (1<<OCIE2A); // OC2A 비교 일치 인터럽트 활성화
  sei(); // 인터럽트 활성화

  Serial.begin(9600);
  Serial.println("Enter PWM level (0-180):");
  //delay(10); // 반복 주기 설정 (10ms)
}