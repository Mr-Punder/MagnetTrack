//=========================================================================== //
// Катушка
//==============================================================================

#ifndef COIL_H
#define COIL_H

#include <Arduino.h>

class Coil {
public: 
  Coil(const uint8_t &pin);

  // Включение катушки
  void on();
  // Выключение катушки
  void off();
  // Включена ли катушка
  bool isOn();
  
protected:
  uint8_t _pin;
};

#endif