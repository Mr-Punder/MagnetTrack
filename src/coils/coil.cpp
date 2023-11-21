//=========================================================================== //
// Катушка
//==============================================================================

#include "coil.h"

// Конструктор
Coil::Coil(const uint8_t &pin) {
  pinMode(pin, OUTPUT);
  this->_pin = pin;
}

// Включение катушки
void Coil::on() {
  digitalWrite(_pin, HIGH);
}

// Выключение катушки
void Coil::off() {
  digitalWrite(_pin, LOW);
}

// Включена ли катушка
bool Coil::isOn() {
  return digitalRead(_pin);
}
