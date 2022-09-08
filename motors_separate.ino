/* MOTOR INITIALIZATION */

#define motor1 PB6
#define motor2 PB7
#define motor3 PB8
#define motor4 PB9

#define channel1 1
#define channel2 2
#define channel3 3
#define channel4 4

int frequency = 50;                  // Herz
int minThrottle = 1000;             // microseconds
int maxThrottle = 2000;             // microseconds
int currentThrottle = minThrottle; // microseconds

HardwareTimer *motorTimer = new HardwareTimer(TIM4);


void setup()
{
  /* MOTOR SETUP */
  
  Serial.begin(9600);
  
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(motor3, OUTPUT);
  pinMode(motor4, OUTPUT);
  
  motorTimer->setMode(channel1, TIMER_OUTPUT_COMPARE_PWM1, motor1);
  motorTimer->setMode(channel2, TIMER_OUTPUT_COMPARE_PWM1, motor2);
  motorTimer->setMode(channel3, TIMER_OUTPUT_COMPARE_PWM1, motor3);
  motorTimer->setMode(channel4, TIMER_OUTPUT_COMPARE_PWM1, motor4);
  
  motorTimer->setOverflow(frequency, HERTZ_FORMAT);
  
  motorTimer->setCaptureCompare(channel1, currentThrottle, MICROSEC_COMPARE_FORMAT);
  motorTimer->setCaptureCompare(channel2, currentThrottle, MICROSEC_COMPARE_FORMAT);
  motorTimer->setCaptureCompare(channel3, currentThrottle, MICROSEC_COMPARE_FORMAT);
  motorTimer->setCaptureCompare(channel4, currentThrottle, MICROSEC_COMPARE_FORMAT);

  motorTimer->resume();

 
}

void loop()
{
  if (Serial.available() > 0)
  {
    currentThrottle = Serial.parseInt();

    switch (currentThrottle / 10000)
    {
      case 1:
        motorTimer->setCaptureCompare(channel1, currentThrottle % 10000, MICROSEC_COMPARE_FORMAT);
        break;
        
       case 2:
        motorTimer->setCaptureCompare(channel2, currentThrottle % 10000, MICROSEC_COMPARE_FORMAT);
        break;
        
       case 3:
        motorTimer->setCaptureCompare(channel3, currentThrottle % 10000, MICROSEC_COMPARE_FORMAT);
        break;
        
       case 4:
        motorTimer->setCaptureCompare(channel4, currentThrottle % 10000, MICROSEC_COMPARE_FORMAT);
        break;
    }
  }

  Serial.println((String)"Current duty is " + currentThrottle % 10000);
  delay(500);
}
