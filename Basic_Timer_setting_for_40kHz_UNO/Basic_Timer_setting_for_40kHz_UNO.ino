#include <avr/sleep.h>
#include <avr/power.h>

#define N_PORTS 1
#define N_DIVS 24
/*
#define WAIT_LOT(a) __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop")
#define WAIT_MID(a) __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop")
#define WAIT_LIT(a) __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop")
*/

#define OUTPUT_WAVE(pointer, d)  PORTC = pointer[d*N_PORTS + 0]

#define N_BUTTONS 5
//half a second
#define BUTTON_SENS 10000 
#define N_FRAMES 29

void setup()
{
/*
 for (int i = 0; i < (N_PORTS*N_DIVS); ++i){
    animation[frame][i] =  0;
  }

  for (int i = 0; i < (N_PORTS*N_DIVS/2); ++i){
     animation[frame][i] = 0b11111111;
  }
  
  for(int i = 0; i < N_DIVS; ++i){
    if (i % 2 == 0){
      animation[frame][i * N_PORTS] |= 0b00000001;
    }else{
      animation[frame][i * N_PORTS] &= 0b11111110;
    }
  }
*/

   DDRC = 0b00111111; //A0 to A5 are the signal outputs
   PORTC = 0b00000000; 
   
   pinMode(10, OUTPUT); //pin 10 (B2) will generate a 40kHz signal to sync 
   pinMode(11, INPUT_PULLUP); //pin 11 (B3) is the sync in
   //please connect pin 10 to pin 11

   for (int i = 2; i < 8; ++i){ //pin 2 to 7 (D2 to D7) are inputs for the buttons
    pinMode(i, INPUT_PULLUP); 
   }

  // generate a sync signal of 40khz in pin 10
  noInterrupts();           // disable all interrupts
  TCCR1A = bit (WGM10) | bit (WGM11) | bit (COM1B1); // fast PWM, clear OC1B on compare
  TCCR1B = bit (WGM12) | bit (WGM13) | bit (CS10);   // fast PWM, no prescaler
  OCR1A =  (F_CPU / 40000L) - 1;
  OCR1B = (F_CPU / 40000L) / 2;
  interrupts();             // enable all interrupts

   // disable everything that we do not need 
  ADCSRA = 0;  // ADC
  power_adc_disable ();
  power_spi_disable();
  power_twi_disable();
  power_timer0_disable();
  power_usart0_disable();
  //Serial.begin(115200);
}

void loop() {

}
