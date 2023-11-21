//=========================================================================== //
// Работа с датчиком холла
//==============================================================================

#include "hall.h"

// Конструктор
Hall::Hall(const uint8_t &pin, const uint8_t &number) {
  this->_pin = pin;
  pinMode(pin, INPUT);
  this->number = number;

  this->_wasMax = false;
}

// Чтение с датчика
uint16_t Hall::read() {
  return analogRead(this->_pin);
}

// Обновление показаний датчика
void Hall::update() {
  // Читаем значение
  uint16_t value = this->read();

  // Если максимум еще не пройден
  if (!this->_wasMax) {
    // Проверяем, попали ли в диапазон максимума
    if (value >= (HALL_VALUE_MAX - HALL_VALUE_DELTA)) {
      // Поднимаем флаг
      this->_wasMax = true;
    }
  }
  // Если максимум уже пройден
  else {
    // Ожидаем значение в районе середины диапазона
    if (value <= (HALL_VALUE_MED + HALL_VALUE_DELTA)) {
      // Выполняем действия
      this->zeroCallback(this->number);
      // Сбрасываем флаг
      this->_wasMax = false;
      this->_av_speed = 145.0 / (millis() - this->_time)*1000;
      this->_time = millis();
      //this->_time_micros = micros();
    }
  }

  // Сохраняем в историю
  this->_lastValue = value;
}
