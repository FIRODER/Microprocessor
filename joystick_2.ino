#define OC1A 10 // OC1A PIN up
#define OC1B 9 // OC1B PIN down

// 임계값 설정 (조이스틱 움직임의 최소 범위)
uint16_t threshold = 90; // 필요에 따라 조정
volatile uint16_t pulseWidth1 = 0;
volatile uint16_t pulseWidth2 = 0;

uint16_t targetPulseWidth1 = 0;
uint16_t targetPulseWidth2 = 0;

unsigned long previousMillis = 0;
const long interval = 20; // 서보모터 업데이트 주기 (20ms)

ISR(TIMER1_COMPA_vect) {
    OCR1A = pulseWidth1; // 펄스 폭 설정, x
}

ISR(TIMER1_COMPB_vect) {
    OCR1B = pulseWidth2; // 펄스 폭 설정, y
}

//////////////////////////////////////////////////////////////////////////
// ADC Setup
//////////////////////////////////////////////////////////////////////////
uint16_t init_ADC(uint8_t pin) {
    
    // 1. ADMUX Register
    // Volatage reference: AVcc (REFS[1:0] == 01)
    ADMUX = (0 << REFS1) | (1 << REFS0);

    // ADC Right Adjust Result
    ADMUX |= (0 << ADLAR);

    // Analog Channel Selection: ADC0 (MUX[3:0] == 0000)
    if (pin == A0) {
        ADMUX |= (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0); 
    }
    // Analog Channel Selection: ADC1 (MUX[3:0] == 0001)
    else if (pin == A1) {
        ADMUX |= (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (1 << MUX0); 
    }

    // 2. ADCSRA Register
    // ADC Enable
    ADCSRA |= (1 << ADEN);

    // ADC Prescaler Selct: 64 (ADPS [2:0] == 110)
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0);

    // Start ADC Conversion
    ADCSRA |= (1 << ADSC);

    // Wait until ADC is completed
    while (ADCSRA & (1 << ADSC));

    // Return ADC Result
    return ADC;
}

void setup() {
    
    // Disable Global Interrupt
    cli();

    // Setting Output Port
    DDRB |= (1 << DDB1); 
    DDRB |= (1 << DDB2);

    /////////////////////////////////////////////////////////////////////////////////////////
    // Timer/Counter 1
    // -- TCCR1A = 0b 10100010;
    // >> (10) Waveform Generation Mode: Fast PWM, TOP = ICR1
    // >> (10) Compare Output Mode: Clear OC1A/OC1B on compare match (non-inverting mode)
    // -- TCCR1B = 0b 00011010;
    // >> (11) Waveform Generation Mode: Fast PWM, TOP = ICR1
    // >> (010) Clock Select: Prescaler 8
    // TIMSK1 = 0b 00000110;
    // >> (11) Enable Output Compare A Match Interrupt and Output Compare B Match Interrupt
    ////////////////////////////////////////////////////////////////////////////////////////

    // Common Settings
    TCCR1A = (1 << WGM11) | (1 << COM1A1) | (1 << COM1B1);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); 

    // Set Top Value to 320000, Creating a 160ms Period for the PWM Signal
    ICR1 = 20000;

    // Setting for OC1A, OC1B
    TIMSK1 = (1 << OCIE1A) | (1 << OCIE1B); 

    // Enable Global Interrupt
    sei();
}

void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        // Read Joystick Value from Pin A0, A1
        uint16_t adcValue1 = init_ADC(A0); // x
        uint16_t adcValue2 = init_ADC(A1); // y

        // Map the Joystick Value to Pulse Width for Servo Control
        // PulseWidth Range: 1000 to 3500 (Correspond to 1ms to 3.5ms pulse)
        targetPulseWidth1 = map(adcValue1, 0, 1023, 1000, 3500);
        targetPulseWidth2 = map(adcValue2, 0, 1023, 1000, 3500);

        // Smooth transition to the target pulse width
        if (pulseWidth1 < targetPulseWidth1) {
            pulseWidth1++;
        } else if (pulseWidth1 > targetPulseWidth1) {
            pulseWidth1--;
        }

        if (pulseWidth2 < targetPulseWidth2) {
            pulseWidth2++;
        } else if (pulseWidth2 > targetPulseWidth2) {
            pulseWidth2--;
        }
    }
}