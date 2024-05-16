#define SERVO_PIN 9  // Pin connected to the servo signal wire

void setup() {
  // Configure pin as output
  pinMode(SERVO_PIN, OUTPUT);

  // Set Timer1 to Fast PWM mode, non-inverting, with prescaler of 8
  TCCR1A = (1 << WGM11) | (1 << COM1A1);  // Fast PWM mode, non-inverting
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);  // Prescaler of 8

  // Set TOP value for 200 Hz frequency (5 ms period)
  ICR1 = 9999;  // (16MHz / (200 * 8)) - 1

  // Start with servo at 0 degrees (1.5 ms pulse width)
  OCR1A = 2999;  // (1.5ms / 20ms) * 39999
}

void loop() {
  // Sweep servo from -90 to +90 degrees and back
  for (int pulseWidth = 1999; pulseWidth <= 4999; pulseWidth += 10) {
    OCR1A = pulseWidth;  // Update pulse width
    delay(5);  // Wait 5 ms for the servo to reach position
  }

  for (int pulseWidth = 4999; pulseWidth >= 1999; pulseWidth -= 10) {
    OCR1A = pulseWidth;  // Update pulse width
    delay(5);  // Wait 5 ms for the servo to reach position
  }
}
