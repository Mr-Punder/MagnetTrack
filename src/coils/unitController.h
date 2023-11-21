//=========================================================================== //
// Контроллер катушек
//==============================================================================

#ifndef COIL_CONTROLLER_H
#define COIL_CONTROLLER_H

#include <Arduino.h>

#include "coil.h"
#include "hall.h"
#include "../core/tasks.h"
#include "../core/utils.h"
#include "../config.h"

//------------------------------------------------------------------------------

// Класс пары катушка-датчик холла
class Unit {
public:
  Unit(Coil *coil, Hall *hall);
  // Номер модуля
  uint8_t number;
  // Ссылка на объект катушки
  Coil *coil;
  // Ссылка на объект датчика Холла
  Hall *hall;
};

//------------------------------------------------------------------------------

class UnitController {
public:
  UnitController();

  // Добавление катушки с датчиком в массив элементов
  void addUnit(const uint8_t &coilPin, const uint8_t &hallPin);
  // Фактическое число устройств
  uint8_t count;

  // Поиск ссылки на устройство по его номеру
  Unit *searchUnit(uint8_t number);
  // Поиск ссылки на катушку по ее номеру
  Coil *searchCoil(uint8_t number);
  // Поик ссылки на датчик по его номеру
  Hall *searchHall(uint8_t number);

  // Включение требуемой катушки
  void coilOn(uint8_t number);
  // Выключение требуемой катушки
  void coilOff(uint8_t number);

  // Действие для всех устройств
  void forAll(void (*callback)(Unit *unit));

  // Обработка действия при переходе магнитом N-го датчика
  static void detectedOn(uint8_t &number);
  // Номер текущего работающего устройства
  uint8_t currentUnit;

protected:
  // Массив объектов катушек с датчиками
  Unit *_unitsArray[UNITS_COUNT];
  // Задача на отключение активной катушки
  uint8_t _timeoutTaskID;
};

//------------------------------------------------------------------------------

extern UnitController units;

#endif