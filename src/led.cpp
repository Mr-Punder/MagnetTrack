//=========================================================================== //
// Светодиодная индикация
//==============================================================================

#include "led.h"

// Светодиод
LED led = LED(LED_INT);

//------------------------------------------------------------------------------
// Класс светодиода
//------------------------------------------------------------------------------

LED::LED(const uint8_t _pin)
{
  pinMode(pin, OUTPUT);
  pin = _pin;
  pwm = 0;
  isOn = false;
}

LED::~LED()
{
  pinMode(pin, INPUT);
}

void LED::setPWM(uint8_t _pwm)
{
  analogWrite(pin, _pwm);
  pwm = _pwm;

  if (pwm > 0 ) 
    isOn = true;
  else
    isOn = false;
}

uint8_t LED::getPWM()
{
  return pwm;
}

void LED::on()
{
  setPWM(255);
  isOn = true;
}

void LED::off()
{
  setPWM(0);
  isOn = false;
}

// Макрос мигания светодиода (по ссылке)
#define LED_BLINK(link, blinkTime, count) \
  tasks.addRepeats([]() \
  { \
    link.on(); \
    tasks.addSingle([]() \
    { \
      link.off();\
    }, blinkTime); \
\
  }, 0, (blinkTime * 2), count);

// Мигание, подтверждение
void LED::blinkOK() {
  off();
  LED_BLINK(led, 150, 2);
}

// Мигание, ошибка
void LED::blinkERROR() {
  off();
  LED_BLINK(led, 200, 3);
}