#define OC1A 9 // OC1A PIN
#define OC1B 10 // OC1B PIN

uint16_t pulseWidth1 = 0;
uint16_t pulseWidth2 = 0;

//////////////////////////////////////////////////////////////////////////
//ADC Setup
//-ADMUX = 0b 0100 0000; 
// >> (01) Voltage source: AVcc (REFS[1:0] == 01)
// >> Input channel: A0 or A1 based on Pin (MUX[3:0])
//-ADCSRA = 0b 1000 0110; 
// >> (1) Enable ADC 
// >> (110) Prescaler 64
//////////////////////////////////////////////////////////////////////////
uint16_t init_ADC(uint8_t pin) {
    
    // 1. ADMUX Register
    // Volatage reference: AVcc (REFS[1:0] == 01)
    ADMUX = (0 << REFS1) | (1 << REFS0);

    // ADC Right Adjust Result
    ADMUX |= (0 << ADLAR);

    // Analog Channel Selection: ADC0 (MUX[3:0] == 0000)
    if(pin == A0){
      ADMUX |= (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0); 
    }

    // Analog Channel Selection: ADC1 (MUX[3:0] == 0001)
    else if(pin == A1){
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
    DDRB |= OC1A; 
    DDRB |= OC1B;

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

    // >> (20000) Set Top Value to 20000, Creating a 20ms Period for the PWM Signal
    // System Clock Frequency = 16 MHz
    // Prescaler = 8
    // Timer Clock Frequency = System Clock Frequency / Prescaler = 16,000,000 / 8 = 2,000,000 Hz
    // Desired PWM Period = 20 ms = 0.020 s
    // ICR1 = (Desired PWM Period * Timer Clock Frequency) - 1
    // ICR1 = (0.020 seconds * 2,000,000 Hz) - 1
    // ICR1 = 40,000 - 1
    // ICR1 = 39,999
    ICR1 = 39999;

    // Setting for OC1A, OC1B
    TIMSK1 |= (1 << OCIE1A) | (1 << OCIE1B); 

    // Enable Global Interrupt
    sei();
}

void loop() {
    // Read Joystick Value from Pin A0,A1
    pulseWidth1 = init_ADC(A0); 
    pulseWidth2 = init_ADC(A1); 

    // Map the Joystick Value to Pulse Width for Servo Control
    // PulseWidth Range: 1000 to 4800 (Correspond to 1ms to 4.8ms pulse)
    pulseWidth1 = (((float)(pulseWidth1 - 0) / (1023 - 0)) * (4800 - 1000) + 1000);
    pulseWidth2 = (((float)(pulseWidth2 - 0) / (1023 - 0)) * (4800 - 1000) + 1000);

    // Set Pulse Width
    OCR1A = pulseWidth1;
    OCR1B = pulseWidth2;

    // Delay for 20ms to Match Servo Motiorr Update Cycle
    delay(20); 
}
