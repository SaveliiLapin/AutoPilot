#define RADIO_PIN PB3

uint32_t INPUT_FREQ = 0;
uint32_t PRESCALE = 10;
uint32_t FALLING_CHANNEL;
volatile uint32_t PREVIOUS_CAPTURE = 0;
volatile uint32_t CURRENT_CAPTURE;
volatile uint32_t MEASURED_HIGH;

TIM_TypeDef *RADIO_INSTANCE = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(RADIO_PIN), PinMap_PWM);
uint32_t RISING_CHANNEL = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(RADIO_PIN), PinMap_PWM));

HardwareTimer *RADIO_TIMER = new HardwareTimer(RADIO_INSTANCE);

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
  Serial.println(MEASURED_HIGH);
  delay(100);
}
