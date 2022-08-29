#include <Servo.h>

#define MOTOR_1_PIN PB12
#define MOTOR_2_PIN PB13
#define MOTOR_3_PIN PB14
#define MOTOR_4_PIN PB15
#define RADIO_IN PB9

Servo motor_1, motor_2, motor_3, motor_4;

int duration = 0;

void setup()
{
  motor_1.attach(MOTOR_1_PIN, 1000, 2000); // (pin, min pulse width, max pulse width in microseconds)
  motor_2.attach(MOTOR_2_PIN, 1000, 2000);
  motor_3.attach(MOTOR_3_PIN, 1000, 2000);
  motor_4.attach(MOTOR_4_PIN, 1000, 2000);
  Serial.begin(9600);
  pinMode(RADIO_IN, INPUT);
}

void loop()
{
  duration = pulseIn(RADIO_IN, HIGH);
  duration = map(duration, 1100, 1900, 0, 180);

  motor_1.write(duration);
  motor_2.write(duration);
  motor_3.write(duration);
  motor_4.write(duration);
  
  Serial.println(duration);
}
