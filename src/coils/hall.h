//=========================================================================== //
// Датчик холла
//==============================================================================

#ifndef HALL_H
#define HALL_H

#include <Arduino.h>

#include "../core/utils.h"


class Hall {
public:
  Hall(const uint8_t &pin, const uint8_t &number);

  uint8_t number;

  // Функция, вызываемая при детектировании датчиком перехода через ноль
  void (*zeroCallback) (uint8_t &number);
  // Опрос датчика
  void update();
  // Чтение значения с датчика
  uint16_t read();

  // последнее время срабатывания
  long double _time = 1;
  long double _time_micros = 1;
  double _av_speed;
protected:
  uint8_t  _pin;
  uint16_t _lastValue;
  bool     _wasMax;
};

#endif