////////////////////////////////////////////////////////////////////
//Arduino Uno ATmega328p
//초음파 센서
//PC0: 초음파 센서 1층 위 (아날로그 핀)
//PC1: 초음파 센서 2층 위 (아날로그 핀)
//PC2: 초음파 센서 1층 아래 (아날로그 핀)
//PC3: 초음파 센서 2층 아래 (아날로그 핀)
//스위치
//PD2: 스위치1
//PD3: 스위치2
//
//OC1A: PB1 -> INT0
//OC1B: PB2 -> INT1
//
//TCCR1A = 0b
//TCCR1B = 0b
//
////////////////////////////////////////////////////////////////////
#define numberOfTrans 24 // 초음파 개수

#define numberOfButtons 2 // 버튼 수

#define outputAll 0b00001111 // 초음파 센서 24개 전부

//초음파 센서 ON
ISR(INT0_vect){
  PORTC |= outputAll;
}

//초음파 센서 OFF
ISR(INT1_vect){
  PORTC &= ~outputAll;
}

/*
ISR(TIMER1_COMPA_vect){

}
*/

void setup() {

DDRC = 0b00001111; // PC0~PC3 output 신호 설정
PORTC = 0b00000000; // PORTC 초기값 LOW 설정

DDRD = 0b00000011; // PD2~PD3 output 신호 설정
PORTD = 0b00000000; // PORTD 초기값 LOW 설정

//Fast PWM
//no prescaling
TCCR1A |= (1<<WGM10) | (1<<WGM11) | (1<<COMA1) | (0<<COMA0);
TCCR1B |= (0<<WGM12) | (0<<CS22) | (0<<CS21) | (1<<CS20);

// 16MHz/40kHz=400
// 400번째 clock 마다 신호
OCR1A = (F_CPU / 40000)-1;

}

void loop() {

}
