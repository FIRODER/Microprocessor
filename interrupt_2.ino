#include <avr/power.h>
#include <avr/interrupt.h>

#define N_Transducers 24 // Number of Transducers

#define WAIT_LOT(a) __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop")
//14 clock

#define WAIT_MID(a) __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop")
//13 clock

#define WAIT_LIT(a) __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop")
//9 clock

#define OUTPUT_WAVE(pointer, d)  PORTC = pointer[d]

#define N_BUTTONS 5
#define BUTTON_SENS 50
#define N_FRAMES 29

static byte frame = 19;
static byte animation[N_FRAMES][N_Transducers] = //29x24
{{0x6,0xc,0xc,0xc,0xc,0xc,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x3,0x3,0x3,0x3,0x3,0x6,0x6,0x6,0x6,0x6,0x6},
{0x6,0x6,0x6,0x6,0x6,0xc,0xc,0xc,0xc,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x3,0x3,0x3,0x3,0x6,0x6,0x6},
{0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0xc,0xc,0xc,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x3,0x3,0x3},
{0x3,0x3,0x3,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0xc,0xc,0xc,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9},
{0x9,0x9,0x9,0x9,0x3,0x3,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0xc,0xc,0x9,0x9,0x9,0x9,0x9,0x9},
{0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x3,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0xc,0x9,0x9,0x9},
{0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x3,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0xc},
{0x6,0x6,0x6,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6},
{0x6,0x6,0x6,0x6,0x6,0x6,0x3,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0xc,0x6,0x6,0x6,0x6,0x6},
{0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x3,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0xc,0x6,0x6},
{0xc,0xc,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x3,0x3,0x3,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0xc},
{0x9,0x9,0xc,0xc,0xc,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x3,0x3,0x3,0x9,0x9,0x9,0x9,0x9,0x9,0x9},
{0x9,0x9,0x9,0x9,0xc,0xc,0xc,0xc,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x3,0x3,0x3,0x3,0x9,0x9,0x9,0x9},
{0x9,0x9,0x9,0x9,0x9,0x9,0x9,0xc,0xc,0xc,0xc,0xc,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x3,0x3,0x3,0x3,0x3},
{0x3,0x3,0x3,0x9,0x9,0x9,0x9,0x9,0x9,0xc,0xc,0xc,0xc,0xc,0xc,0x6,0x6,0x6,0x6,0x6,0x6,0x3,0x3,0x3},
{0x3,0x3,0x3,0x3,0x3,0x3,0x9,0x9,0x9,0x9,0x9,0xc,0xc,0xc,0xc,0xc,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6},
{0x6,0x6,0x6,0x6,0x3,0x3,0x3,0x3,0x3,0x9,0x9,0x9,0x9,0x9,0xc,0xc,0xc,0xc,0x6,0x6,0x6,0x6,0x6,0x6},
{0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x3,0x3,0x3,0x3,0x3,0x9,0x9,0x9,0x9,0x9,0xc,0xc,0xc,0xc,0x6,0x6,0x6},
{0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x3,0x3,0x3,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0xc,0x6,0x6},
{0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x3,0x3,0x3,0x3,0x3,0x9,0x9,0x9,0x9,0x9,0x9},
{0x9,0x9,0x9,0x9,0x9,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x3,0x3,0x3,0x9,0x9,0x9,0x9},
{0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x3,0x3,0x3},
{0x3,0x3,0x3,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6},
{0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6},
{0x6,0x6,0x6,0x6,0x3,0x3,0x3,0x3,0x3,0x3,0x9,0x9,0x9,0x9,0x9,0x9,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6},
{0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x3,0x3,0x3,0x3,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x6,0x6,0x6,0x6,0x6},
{0xc,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x3,0x3,0x3,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x6,0x6,0x6,0x6},
{0xc,0xc,0xc,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x3,0x3,0x3,0x3,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x6},
{0xc,0xc,0xc,0xc,0xc,0x6,0x6,0x6,0x6,0x6,0x6,0x3,0x3,0x3,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9},
{0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0x6,0x6,0x6,0x6,0x6,0x6,0x3,0x3,0x3,0x3,0x9,0x9,0x9,0x9,0x9,0x9}};


#define PATTERN_IS(x) (memcmp_P (current_pattern, (byte*)pgm_read_word(&patterns[x]), sizeof (current_pattern)) == 0)

#define STATE_OFF   0
#define STATE_ON    1
#define STATE_STOP  2

static uint8_t state = STATE_OFF;
static uint8_t pressed_button = 0;
static uint8_t current_button = 0;
static uint8_t current_button_state[N_BUTTONS] = {0};

// ISR to handle button press
ISR(PCINT1_vect) {
    // Debounce the button press
    _delay_ms(10);
    uint8_t buttons = PINC & 0x1F; // Read the state of the first 5 pins of PORTC
    for (uint8_t i = 0; i < N_BUTTONS; i++) {
        if (bit_is_clear(buttons, i)) { // Check if the button is pressed (active low)
            if (current_button_state[i] == 0) {
                current_button_state[i] = 1;
                pressed_button = i + 1; // Store the pressed button number
                break;
            }
        } else {
            current_button_state[i] = 0;
        }
    }
}

void setup() {
    DDRC = 0x3F; // Set the first 6 pins of PORTC as output
    PORTC = 0x00; // Set all outputs to low

    // Enable pin change interrupt on PCINT8-PCINT14 (PORTC)
    PCICR |= (1 << PCIE1);
    PCMSK1 |= 0x1F; // Enable interrupts on the first 5 pins of PORTC

    sei(); // Enable global interrupts
}

void loop() {
    if (pressed_button) {
        switch (pressed_button) {
            case 1:
                state = STATE_ON;
                break;
            case 2:
                state = STATE_OFF;
                break;
            case 3:
                state = STATE_STOP;
                break;
            // Add cases for other buttons if needed
        }
        pressed_button = 0;
    }

    if (state == STATE_ON) {
        for (int d = 0; d < N_Transducers; d++) {
            OUTPUT_WAVE(animation[frame], d);
            WAIT_LOT(d);
        }
        frame = (frame + 1) % N_FRAMES;
    } else if (state == STATE_OFF) {
        PORTC = 0x00;
    }
    // Add additional functionality for STATE_STOP or other states if needed
}

int main(void) {
    setup();
    while (1) {
        loop();
    }
}