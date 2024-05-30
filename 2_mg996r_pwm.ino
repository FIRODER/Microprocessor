#define OC2A 0X08 // OC2A 핀 정의
#define OC2B 0X09 // OC2B 핀 정의

uint8_t pwm_level_a = 0; // PWM 레벨 변수 (모터 A)
uint8_t pwm_level_b = 0; // PWM 레벨 변수 (모터 B)
uint16_t pulse_width_a = 0; // 펄스 폭 변수 (모터 A)
uint16_t pulse_width_b = 0; // 펄스 폭 변수 (모터 B)

ISR(TIMER2_COMPA_vect){
  OCR2A = pulse_width_a; // 펄스 폭 설정 (모터 A)
}

ISR(TIMER2_COMPB_vect){
  OCR2B = pulse_width_b; // 펄스 폭 설정 (모터 B)
}

void loop() {
  if (Serial.available()){
    pwm_level_a = Serial.parseInt();
    pwm_level_a = constrain(pwm_level_a, 0, 180); // PWM 레벨 범위 제한 (0~180)
    pulse_width_a = map(pwm_level_a, 0, 180, 500, 2500); // PWM 레벨을 펄스 폭으로 변환 (모터 A)

    pwm_level_b = Serial.parseInt();
    pwm_level_b = constrain(pwm_level_b, 0, 180); // PWM 레벨 범위 제한 (0~180)
    pulse_width_b = map(pwm_level_b, 0, 180, 500, 2500); // PWM 레벨을 펄스 폭으로 변환 (모터 B)
  }
}

void setup() {
  cli(); // 인터럽트 비활성화

  // OC2A, OC2B 핀 출력 설정
  DDRB |= (OC2A | OC2B);

  // 타이머2 설정
  TCCR2A |= (1<<WGM21)|(1<<WGM20); // CTC 모드 설정
  TCCR2B |= (0<<CS22)|(0<<CS21)|(1<<CS20); // 프리스케일러 1:1 설정
  TCCR2A |= (1<<COM2A1)|(0<<COM2A0); // OC2A 출력 모드 설정 (Clear OC2A on Compare Match)
  TCCR2A |= (1<<COM2B1)|(0<<COM2B0); // OC2B 출력 모드 설정 (Clear OC2B on Compare Match)

  // 인터럽트 활성화
  OCR2A = pulse_width_a; // 펄스 폭 설정 (모터 A)
  OCR2B = pulse_width_b; // 펄스 폭 설정 (모터 B)
  TIMSK2 |= (1<<OCIE2A) | (1<<OCIE2B); // OC2A, OC2B 비교 일치 인터럽트 활성화
  sei(); // 인터럽트 활성화

  Serial.begin(9600);
  Serial.println("Enter PWM level for Motor A (0-180):");
  Serial.println("Enter PWM level for Motor B (0-180):");
  delay(10); // 반복 주기 설정 (10ms)
}
