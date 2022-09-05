#define MOTOR_1_PIN PB3
#define MOTOR_2_PIN PB4
#define MOTOR_3_PIN PB5
#define MOTOR_4_PIN PB6

TIM_TypeDef *INSTANCE_1 = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(MOTOR_1_PIN), PinMap_PWM);
uint32_t CHANNEL_1 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(MOTOR_1_PIN), PinMap_PWM));

TIM_TypeDef *INSTANCE_2 = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(MOTOR_2_PIN), PinMap_PWM);
uint32_t CHANNEL_2 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(MOTOR_2_PIN), PinMap_PWM));

TIM_TypeDef *INSTANCE_3 = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(MOTOR_3_PIN), PinMap_PWM);
uint32_t CHANNEL_3 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(MOTOR_3_PIN), PinMap_PWM));

TIM_TypeDef *INSTANCE_4 = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(MOTOR_4_PIN), PinMap_PWM);
uint32_t CHANNEL_4 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(MOTOR_4_PIN), PinMap_PWM));

HardwareTimer *motor_1 = new HardwareTimer(INSTANCE_1);
HardwareTimer *motor_2 = new HardwareTimer(INSTANCE_2);
HardwareTimer *motor_3 = new HardwareTimer(INSTANCE_3);
HardwareTimer *motor_4 = new HardwareTimer(INSTANCE_4);

int duty = 0;
int frequency = 50;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  if (Serial.available() > 0)
  {
    duty = Serial.parseInt();
    Serial.print("Your duty is ");
    Serial.println(duty);
    
    motor_1->setPWM(CHANNEL_1, MOTOR_1_PIN, frequency, duty);
    motor_2->setPWM(CHANNEL_2, MOTOR_2_PIN, frequency, duty);
    motor_3->setPWM(CHANNEL_3, MOTOR_3_PIN, frequency, duty);
    motor_4->setPWM(CHANNEL_4, MOTOR_4_PIN, frequency, duty);
  }
}
