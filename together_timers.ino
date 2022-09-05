#define RADIO_PIN PB3
#define MOTOR_1_PIN PB4
#define MOTOR_2_PIN PB5
#define MOTOR_3_PIN PB6
#define MOTOR_4_PIN PB7

uint32_t INPUT_FREQ = 0;
uint32_t PRESCALE = 10;
uint32_t FALLING_CHANNEL;
volatile uint32_t PREVIOUS_CAPTURE = 0;
volatile uint32_t CURRENT_CAPTURE;
volatile uint32_t MEASURED_HIGH;
int duty = 0;
int frequency = 48000;

TIM_TypeDef *RADIO_INSTANCE = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(RADIO_PIN), PinMap_PWM);
uint32_t RISING_CHANNEL = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(RADIO_PIN), PinMap_PWM));

TIM_TypeDef *INSTANCE_1 = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(MOTOR_1_PIN), PinMap_PWM);
uint32_t CHANNEL_1 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(MOTOR_1_PIN), PinMap_PWM));

TIM_TypeDef *INSTANCE_2 = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(MOTOR_2_PIN), PinMap_PWM);
uint32_t CHANNEL_2 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(MOTOR_2_PIN), PinMap_PWM));

TIM_TypeDef *INSTANCE_3 = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(MOTOR_3_PIN), PinMap_PWM);
uint32_t CHANNEL_3 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(MOTOR_3_PIN), PinMap_PWM));

TIM_TypeDef *INSTANCE_4 = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(MOTOR_4_PIN), PinMap_PWM);
uint32_t CHANNEL_4 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(MOTOR_4_PIN), PinMap_PWM));

HardwareTimer *RADIO_TIMER = new HardwareTimer(RADIO_INSTANCE);
HardwareTimer *motor_1 = new HardwareTimer(INSTANCE_1);
HardwareTimer *motor_2 = new HardwareTimer(INSTANCE_2);
HardwareTimer *motor_3 = new HardwareTimer(INSTANCE_3);
HardwareTimer *motor_4 = new HardwareTimer(INSTANCE_4);

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
}

void setup()
{
  Serial.begin(115200);

  switch (RISING_CHANNEL) {
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
  Serial.print(MEASURED_HIGH);
  Serial.print("   ");
  delay(100);

  duty = map(MEASURED_HIGH, 10660, 18270, 16, 100);

  Serial.println(duty);
  
  motor_1->setPWM(CHANNEL_1, MOTOR_1_PIN, frequency, duty);
  motor_2->setPWM(CHANNEL_2, MOTOR_2_PIN, frequency, duty);
  motor_3->setPWM(CHANNEL_3, MOTOR_3_PIN, frequency, duty);
  motor_4->setPWM(CHANNEL_4, MOTOR_4_PIN, frequency, duty);
}
