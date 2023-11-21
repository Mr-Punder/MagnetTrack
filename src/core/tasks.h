//=========================================================================== //
// Планировщик задач
//==============================================================================

#ifndef TASKS_H
#define TASKS_H

#include <Arduino.h>

#include "../core/utils.h"
#include "../config.h"

//------------------------------------------------------------------------------

// Тип "Задача"
typedef struct Task
{
   uint8_t  id;
   void     (*pTask) (void);
   uint16_t startDelay;
   uint16_t period;
   uint8_t  repeats;
   bool     isInfinite : 1;
   bool     readyToRun : 1;
} Task;

//------------------------------------------------------------------------------

// Класс управления задачами
class Tasks
{
public:
   // Начальная инициализация
   void init();
   // Добавление задачи в очередь
   uint8_t addSingle  (void (*tFunc)(void), 
                       const uint16_t startDelay);
   uint8_t addRepeats (void (*tFunc)(void), 
                       const uint16_t startDelay, 
                       const uint16_t taskPeriod, 
                       const uint8_t repeats);
   uint8_t addInfinite(void (*tFunc)(void), 
                       const uint16_t startDelay, 
                       const uint16_t taskPeriod);
   // Удаление задачи из очереди по ID
   bool deleteByID(const uint8_t id);
   // Удаление задачи в очереди по ссылке, первое совпадение
   bool deleteByLink(void (*tFunc)(void));
   // Удаление всех задач в очереди, имеющих данную ссылку
   bool deleteAllByLink(void (*tFunc)(void));
   // Очистка очереди задач
   void clear();
   // Получение текущего числа оставшихся повторений (для уканного Repeats)
   uint8_t getRemainedRepeats(const uint8_t id);

   // Запуск готовых задач
   void callLauncher();
   // Обработка тика таймера
   static void tick();

private:
   uint8_t _addTask(void (*tFunc)(void), 
                   const uint16_t _startDelay, 
                   const uint16_t _period, 
                   const uint8_t  _repeats,
                   const bool     _isInfinite);
   void _deleteByIndex(uint8_t index);
   bool _deleteByLink(void (*tFunc)(void), bool all);
};

//------------------------------------------------------------------------------

// Очередь задач
extern volatile Task taskQueue[MAX_TASKS];
// Конец очереди (номер пустого элемента)
extern volatile uint8_t queueEnd;

//------------------------------------------------------------------------------

extern Tasks tasks;

#endif
