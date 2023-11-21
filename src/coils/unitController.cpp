//=========================================================================== //
// Контроллер катушек
//==============================================================================

#include "unitController.h"

UnitController units = UnitController();

//------------------------------------------------------------------------------

// Конструктор класса Unit
Unit::Unit(Coil *coil, Hall *hall) {
  this->coil = coil;
  this->hall = hall;
}

//------------------------------------------------------------------------------

// Конструктор класса Controller
UnitController::UnitController() {
  this->count = 0;
  this->_unitsArray[UNITS_COUNT] = {};
  this->currentUnit = 0;
}

// Добавление устройства
void UnitController::addUnit(const uint8_t &coilPin, const uint8_t &hallPin) {
  if (count == UNITS_COUNT) {
    ERROR("MAX units!",);
  }
  else {
    Coil *newCoil = new Coil(coilPin);
    Hall *newHall = new Hall(hallPin, (this->count + 1));
    // Добавляем коллбек при переходе центра магнита
    newHall->zeroCallback = &detectedOn;

    Unit *newUnit = new Unit(newCoil, newHall);
    newUnit->number = (this->count + 1);
    _unitsArray[count] = newUnit;
    this->count++;
  }
}

// Поиск устройства по номеру
Unit* UnitController::searchUnit(uint8_t number) {
  for (uint8_t num = 0; num < this->count; num++) {
    if (_unitsArray[num]->number == number) {
      Unit *ptr = _unitsArray[num];
      return ptr;
    }
  }
  return NULL;
}

// Действие для всех устройств
void UnitController::forAll(void (*callback)(Unit *unit)) {
  for (uint8_t num = 0; num < this->count; num++) {
    Unit *unit = this->_unitsArray[num];
    callback(unit);
  }
}

//------------------------------------------------------------------------------
// Работа с катушками
//------------------------------------------------------------------------------

// Поиск требуемой катушки по номеру
Coil* UnitController::searchCoil(uint8_t number) {
  Unit *unit = this->searchUnit(number);
  if (unit == NULL) return NULL;

  return unit->coil;
}

// Включение катушки по номеру
void UnitController::coilOn(uint8_t number) {
  Coil *coil = this->searchCoil(number);
  if (coil == NULL) return;

  coil->on();
}

// Выключение катушки по номеру
void UnitController::coilOff(uint8_t number) {
  Coil *coil = this->searchCoil(number);
  if (coil == NULL) return;

  coil->off();
}

//------------------------------------------------------------------------------
// Работа с датчиками
//------------------------------------------------------------------------------

// Поиск требуемого датчика по номеру
Hall* UnitController::searchHall(uint8_t number) {
  Unit *unit = this->searchUnit(number);
  if (unit == NULL) return NULL;

  return unit->hall;
}

//------------------------------------------------------------------------------

// static
// Обработка действия при переходе магнитом N-го датчика
void UnitController::detectedOn(uint8_t &number) {
  // Магнит был обнаружен на ... датчике.
  // Выключаем все катушки, если были включены
  units.forAll([](Unit(*unit)) {
    unit->coil->off();
  });

  // Дополнительно, завершаем активное задание планировщика по выключению катушки
  tasks.deleteByID(units._timeoutTaskID);

  // Получаем номер будущей активной катушки
  uint8_t num = number + 1;
  if (num > units.count)
    num = 1;
  units.currentUnit = num;

  // Отладка
  //PRINT("[H]: ");
  //PRINTL(number);

   //вывод //скорости  
  //  PRINT("v = ");
  //  PRINT(number);
  //  PRINT(" ");
  //  double temp_time =units._unitsArray[number-1]->hall->_time;
   
  //  PRINTL(temp_time);
 


  if(number == 5)
  {
      //PRINT("v = ");
      PRINTL(units._unitsArray[number]->hall->_av_speed);
  }

  // Включаем следующую катушку
  Coil *coil = units.searchCoil(units.currentUnit);
  if (coil == NULL) return;
  coil->on();

  // ...на определенное время работы
  units._timeoutTaskID = tasks.addSingle([]() {
    Coil *coil = units.searchCoil(units.currentUnit);
    if (coil == NULL) return;
    coil->off();
  }, COIL_TIMEOUT);
}