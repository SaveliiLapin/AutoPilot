#define RADIO_PIN PB3
#define MOTOR_1_PIN PB4
#define MOTOR_2_PIN PB5
#define MOTOR_3_PIN PB6
#define MOTOR_4_PIN PB7

uint32_t INPUT_FREQ = 0;
uint32_t PRESCALE = 100;
uint32_t FALLING_CHANNEL;
uint32_t DUTY = 0;
uint32_t MIN_DUTY = 50;
uint32_t MAX_DUTY = 100;
uint32_t FREQUENCY = 500;
volatile uint32_t PREVIOUS_CAPTURE = 0;
volatile uint32_t CURRENT_CAPTURE;
volatile uint32_t MEASURED_HIGH;

TIM_TypeDef *RADIO_INSTANCE = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(RADIO_PIN), PinMap_PWM);
uint32_t RISING_CHANNEL = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(RADIO_PIN), PinMap_PWM));

TIM_TypeDef *MOTOR_INSTANCE_1 = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(MOTOR_1_PIN), PinMap_PWM);
uint32_t CHANNEL_1 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(MOTOR_1_PIN), PinMap_PWM));

TIM_TypeDef *MOTOR_INSTANCE_2 = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(MOTOR_2_PIN), PinMap_PWM);
uint32_t CHANNEL_2 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(MOTOR_2_PIN), PinMap_PWM));

TIM_TypeDef *MOTOR_INSTANCE_3 = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(MOTOR_3_PIN), PinMap_PWM);
uint32_t CHANNEL_3 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(MOTOR_3_PIN), PinMap_PWM));

TIM_TypeDef *MOTOR_INSTANCE_4 = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(MOTOR_4_PIN), PinMap_PWM);
uint32_t CHANNEL_4 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(MOTOR_4_PIN), PinMap_PWM));

HardwareTimer *RADIO_TIMER = new HardwareTimer(RADIO_INSTANCE);
HardwareTimer *MOTOR_1_TIMER = new HardwareTimer(MOTOR_INSTANCE_1);
HardwareTimer *MOTOR_2_TIMER = new HardwareTimer(MOTOR_INSTANCE_2);
HardwareTimer *MOTOR_3_TIMER = new HardwareTimer(MOTOR_INSTANCE_3);
HardwareTimer *MOTOR_4_TIMER = new HardwareTimer(MOTOR_INSTANCE_4);

void CAPTURE_RISING_FUNC(void)
{
  CURRENT_CAPTURE = RADIO_TIMER->getCaptureCompare(RISING_CHANNEL);
  PREVIOUS_CAPTURE = CURRENT_CAPTURE;
}

void CAPTURE_FALLING_FUNC(void)
{
  CURRENT_CAPTURE = RADIO_TIMER->getCaptureCompare(FALLING_CHANNEL);

  if (CURRENT_CAPTURE > PREVIOUS_CAPTURE)
  {
    MEASURED_HIGH = CURRENT_CAPTURE - PREVIOUS_CAPTURE;
  }
  else
  {
    MEASURED_HIGH = 0x10000 + CURRENT_CAPTURE - PREVIOUS_CAPTURE;
  }

  DUTY = map(MEASURED_HIGH, 1056, 1830, MIN_DUTY, MAX_DUTY);
  Serial.println((String)MEASURED_HIGH + "  " + DUTY);
  
  MOTOR_1_TIMER->setPWM(CHANNEL_1, MOTOR_1_PIN, FREQUENCY, DUTY);
  MOTOR_2_TIMER->setPWM(CHANNEL_2, MOTOR_2_PIN, FREQUENCY, DUTY);
  MOTOR_3_TIMER->setPWM(CHANNEL_3, MOTOR_3_PIN, FREQUENCY, DUTY);
  MOTOR_4_TIMER->setPWM(CHANNEL_4, MOTOR_4_PIN, FREQUENCY, DUTY);
}

void setup()
{
  Serial.begin(115200);

  switch (RISING_CHANNEL)
  {
    case 1:
      FALLING_CHANNEL = 2;
      break;
    case 2:
      FALLING_CHANNEL = 1;
      break;
    case 3:
      FALLING_CHANNEL = 4;
      break;
    case 4:
      FALLING_CHANNEL = 3;
      break;
  }

  RADIO_TIMER->setMode(RISING_CHANNEL, TIMER_INPUT_FREQ_DUTY_MEASUREMENT, RADIO_PIN);
  RADIO_TIMER->setPrescaleFactor(PRESCALE);
  RADIO_TIMER->setOverflow(0x10000);
  RADIO_TIMER->attachInterrupt(RISING_CHANNEL, CAPTURE_RISING_FUNC);
  RADIO_TIMER->attachInterrupt(FALLING_CHANNEL, CAPTURE_FALLING_FUNC);
  RADIO_TIMER->resume();
}

void loop()
{
 
}
