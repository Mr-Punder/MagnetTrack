*//=============================================================== // MagnetTrack
//                               MagnetTrack
//                          v1.0 Arduino Mega 2560
//==============================================================================

#include "src/dev-worker.h"

void setup() 
{
  // Начальные настройки
  systemInit();
}

void loop() 
{
  // Обрабатываем события
  // processEvents();

  // Выполнение задач из очереди
  tasks.callLauncher();
}
