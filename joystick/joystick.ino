#define OC1A 9 // OC2A 핀 정의 (9번 핀)
#define OC1B 10 // OC2B 핀 정의 (10번 핀)

// 서보 모터 PWM 레벨 (UINT_8 형식)
uint8_t pwmLevel1 = 0;
uint8_t pwmLevel2 = 0;

// 서보 모터 펄스 너비 (UINT_16 형식)
uint16_t pulseWidth1 = 0;
uint16_t pulseWidth2 = 0;

// ISR 함수 정의 - 타이머1 비교 일치 인터럽트 서비스 루틴
ISR(TIMER1_COMPA_vect){
    OCR1A = pulseWidth1; // 펄스 폭 설정
}

ISR(TIMER1_COMPB_vect){
    OCR1B = pulseWidth2; // 펄스 폭 설정
}

void setup() {
    cli(); // 인터럽트 비활성화

    // 핀 출력 설정
    DDRB |= (1 << DDB1); // 9번 핀 출력으로 설정 (OC1A)
    DDRB |= (1 << DDB2); // 10번 핀 출력으로 설정 (OC1B)

    // 타이머1 설정
    // TCCR2A 레지스터 설정 - CTC 모드, 프리 스케일러 1:8
    TCCR1A = (1 << WGM11) | (1 << COM1A1) | (1 << COM1B1); // Fast PWM, TOP = ICR1
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);    // 프리 스케일러 1:8

    // TOP 값 설정
    ICR1 = 20000; // 20ms 주기

    // 비교 일치 인터럽트 활성화
    TIMSK1 = (1 << OCIE1A) | (1 << OCIE1B); 

    sei(); // 인터럽트 활성화
}

void loop() {
    // 조이스틱 입력을 읽어 서보 모터의 펄스 폭 조절
    pulseWidth1 = analogRead(A0); // A0 핀에서 조이스틱 값을 읽음
    pulseWidth2 = analogRead(A1); // A1 핀에서 조이스틱 값을 읽음

    // 조이스틱 값에 따라 펄스 폭 조정 (서보 모터의 각도 제어)
    pulseWidth1 = map(pulseWidth1, 0, 1023, 1000, 2000); // 0-1023 -> 1000-2000us
    pulseWidth2 = map(pulseWidth2, 0, 1023, 1000, 2000); // 0-1023 -> 1000-2000us

    delay(20); // 서보 모터 갱신 주기 (20ms)
}