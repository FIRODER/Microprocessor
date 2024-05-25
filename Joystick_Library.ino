#include <Servo.h>

Servo sv1;
Servo sv2;

void setup() {
  sv1.attach(13);
  sv2.attach(12);
}

void loop() {
  int x = analogRead(A0);
  int y = analogRead(A1);

  int angle1 = map(y, 0, 1023, 0, 120); // Map y value to angle for servo 1
  sv1.write(angle1);

  int angle2 = map(x, 0, 1023, 0, 120); // Map x value to angle for servo 2
  sv2.write(angle2);

  delay(15);
}

