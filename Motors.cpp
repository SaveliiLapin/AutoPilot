#include <Servo.h>

#define MOTOR_1_PIN PB12
#define MOTOR_2_PIN PB13
#define MOTOR_3_PIN PB14
#define MOTOR_4_PIN PB15
#define POT_PIN 0

Servo motor_1, motor_2, motor_3, motor_4;
int potValue;

void setup() 
{
  Serial.begin(9600);
  motor_1.attach(MOTOR_1_PIN, 1000, 2000); // (pin, min pulse width, max pulse width in microseconds)
  motor_2.attach(MOTOR_2_PIN, 1000, 2000);
  motor_3.attach(MOTOR_3_PIN, 1000, 2000);
  motor_4.attach(MOTOR_4_PIN, 1000, 2000);

}

void loop() 
{
  Serial.println(potValue);
  potValue = analogRead(POT_PIN);
  potValue = map(potValue, 0, 1023, 0, 180);
  
  motor_1.write(potValue);
  motor_2.write(potValue);
  motor_3.write(potValue);
  motor_4.write(potValue);
}
