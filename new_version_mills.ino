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

/* END MOTOR INITIALIZATION */

/* RADIO INITIALIZATION */

#define radio PB5
#define radioChannel 2

volatile int measured;
volatile int current = 0;
volatile int previous = 0;
int prescale = 100;

HardwareTimer *radioTimer = new HardwareTimer(TIM3);

void inputCapture(void)
{
  current = radioTimer->getCaptureCompare(radioChannel);

  if (previous == 0)
  {
    previous = current;
  }
  else
  {
    if (current > previous)
    {
      measured = current - previous;
    }
    else
    {
      measured = 0x10000 + current - previous;
    }

    previous = 0;

    currentThrottle = map(measured, 1063, 1827, 1000, 2000);

    if (currentThrottle < 1000)
    {
      currentThrottle = 1000;
    }
    else if (currentThrottle > 2000)
    {
      currentThrottle = 2000;
    }
    
    motorTimer->setCaptureCompare(channel1, currentThrottle, MICROSEC_COMPARE_FORMAT);
    motorTimer->setCaptureCompare(channel2, currentThrottle, MICROSEC_COMPARE_FORMAT);
    motorTimer->setCaptureCompare(channel3, currentThrottle, MICROSEC_COMPARE_FORMAT);
    motorTimer->setCaptureCompare(channel4, currentThrottle, MICROSEC_COMPARE_FORMAT);
  }
}

/* END RADIO INITIALIZATION */

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


  /* END MOTOR SETUP */

  /* RADIO SETUP */
  
  pinMode(radio, INPUT);

  radioTimer->setMode(radioChannel, TIMER_INPUT_CAPTURE_BOTHEDGE, radio);
  radioTimer->setPrescaleFactor(prescale);
  radioTimer->setOverflow(0x10000);
  radioTimer->attachInterrupt(radioChannel, inputCapture);
  radioTimer->resume();

  /* END RADIO SETUP */
}

void loop()
{
   Serial.println((String)"Measured is " + measured + "  Throttle is " + currentThrottle);
}
