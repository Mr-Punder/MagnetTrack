//=========================================================================== //
// Инициализация
//==============================================================================

#include "init.h"

//------------------------------------------------------------------------------

void systemInit()
{
  // Статус устройства
  device.status = STATE_INIT;

  // Отладка
  //----------------------------------------------------------------------------
  DEBUG_PORT_INIT();
  LOG("Система", "Cтарт", "");

  // Задачи
  //----------------------------------------------------------------------------
  // Старт таймера планировщика задач
  tasks.init();

  // Индикация
  led.blinkOK();

  // Периферия
  //----------------------------------------------------------------------------

  // Подключаем катушки с датчиками
  units.addUnit(COIL_1_PIN,  HALL_1_PIN);
  units.addUnit(COIL_2_PIN,  HALL_2_PIN);
  units.addUnit(COIL_3_PIN,  HALL_3_PIN);
  units.addUnit(COIL_4_PIN,  HALL_4_PIN);
  units.addUnit(COIL_5_PIN,  HALL_5_PIN);
  units.addUnit(COIL_6_PIN,  HALL_6_PIN);
  units.addUnit(COIL_7_PIN,  HALL_7_PIN);
  units.addUnit(COIL_8_PIN,  HALL_8_PIN);
  units.addUnit(COIL_9_PIN,  HALL_9_PIN);
  units.addUnit(COIL_10_PIN, HALL_10_PIN);
  units.addUnit(COIL_11_PIN, HALL_11_PIN);
  units.addUnit(COIL_12_PIN, HALL_12_PIN);

  // Запускаем опрос датчиков
  tasks.addInfinite([]() {
    units.forAll([](Unit(*unit)) {
      unit->hall->update();
    });
  }, 0, HALL_UPDATE_INTERVAL);

  // Простое миганиие индикаторного светодиода
  tasks.addInfinite([]() {
    if (!led.isOn)
      led.on();
    else
      led.off();
  }, 0, 500);
}