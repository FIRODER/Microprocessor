#define OC1A 9 // OC1A 핀 정의 (9번 핀)
#define OC1B 10 // OC1B 핀 정의 (10번 핀)

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

// ADC 초기화 및 값을 읽어오는 함수
uint16_t init_ADC(uint8_t pin) {
    // ADC 설정
    ADMUX |= (0 << REFS1) | (1 << REFS0); // AVcc with external capacitor at AREF pin
    ADMUX |= (0 << ADLAR); // ADC 결과를 오른쪽 정렬
    if(pin == A0){
      ADMUX |= (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0); // A0 핀 선택
    }
    else if(pin == A1){
      ADMUX |= (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (1 << MUX0); // A1 핀 선택
    }
    ADCSRA |= (1 << ADEN); // ADC 활성화
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0); // 프리스케일러 64 설정
    
    // ADC 변환 시작
    ADCSRA |= (1 << ADSC);

    // 변환이 완료될 때까지 대기
    while (ADCSRA & (1 << ADSC));

    // ADC 결과 반환
    return ADC;
}

void setup() {
    cli(); // 인터럽트 비활성화

    // 핀 출력 설정
    DDRB |= (1 << DDB1); // 9번 핀 출력으로 설정 (OC1A)
    DDRB |= (1 << DDB2); // 10번 핀 출력으로 설정 (OC1B)

    // 타이머1 설정
    TCCR1A |= (1 << WGM11) | (1 << COM1A1) | (1 << COM1B1); // Fast PWM, TOP = ICR1
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11);    // 프리 스케일러 1:8

    // TOP 값 설정
    ICR1 = 20000; // 20ms 주기 설정

    // 비교 일치 인터럽트 활성화
    TIMSK1 |= (1 << OCIE1A) | (1 << OCIE1B); 

    sei(); // 인터럽트 활성화
}

void loop() {
    // 조이스틱 입력을 읽어 서보 모터의 펄스 폭 조절
    pulseWidth1 = init_ADC(A0); // A0 핀에서 조이스틱 값을 읽음
    pulseWidth2 = init_ADC(A1); // A1 핀에서 조이스틱 값을 읽음

    // 조이스틱 값에 따라 펄스 폭 조정 (서보 모터의 각도 제어)
    pulseWidth1 = (((float)(pulseWidth1 - 0) / (1023 - 0)) * (4800 - 1000) + 1000);
    pulseWidth2 = (((float)(pulseWidth2 - 0) / (1023 - 0)) * (4800 - 1000) + 1000);


    delay(20); // 서보 모터 갱신 주기 (20ms)
}
