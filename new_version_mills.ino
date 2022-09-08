#define motor1 PA8
#define motor2 PA9
#define motor3 PA10
#define motor4 PA11

#define channel1 1
#define channel2 2
#define channel3 3
#define channel4 4

uint8_t frequency = 50;                  // Herz
uint8_t minThrottle = 1000;             // microseconds
uint8_t maxThrottle = 2000;             // microseconds
uint8_t currentThrottle = minThrottle; // microseconds

void Update_IT_callback(void)
{
  digitalWrite(motor1, LOW);
}

void Compare_IT_callback(void)
{ 
  digitalWrite(motor1, HIGH);
}

HardwareTimer *motorTimer = new HardwareTimer(TIM1);

void setup()
{
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
  
  motorTimer->attachInterrupt(Update_IT_callback);
  motorTimer->attachInterrupt(channel1, Compare_IT_callback);
  motorTimer->resume();
}


void loop()
{
  if (Serial.available() > 0)
  {
    currentThrottle = Serial.parseInt();

    motorTimer->setCaptureCompare(channel1, currentThrottle, MICROSEC_COMPARE_FORMAT);
    motorTimer->setCaptureCompare(channel2, currentThrottle, MICROSEC_COMPARE_FORMAT);
    motorTimer->setCaptureCompare(channel3, currentThrottle, MICROSEC_COMPARE_FORMAT);
    motorTimer->setCaptureCompare(channel4, currentThrottle, MICROSEC_COMPARE_FORMAT);
  }
}
