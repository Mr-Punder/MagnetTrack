//=========================================================================== //
// Планировщик задач
//==============================================================================

#include "tasks.h"

volatile Task taskQueue[MAX_TASKS] = {};
volatile uint8_t queueEnd = 0;
Tasks tasks = {};

//------------------------------------------------------------------------------
// Таймер
//------------------------------------------------------------------------------

// Инициализация
void Tasks::init()
{
  //----------------------------------------------------------------------------
  // Используется Timer4/2. Таймер будет срабатывать каждую 1 мс
  //----------------------------------------------------------------------------

  #if defined(__AVR_ATmega32U4__)
    #define TCCRA  TCCR4A
    #define TCCRB  TCCR4B
    #define CS0    CS40
    #define CS1    CS41
    #define CS2    CS42
    #define OCRA   OCR4A
    #define OCIEA  OCIE4A
    #define TIMSK  TIMSK4
    
  #elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__)
    #define TCCRA  TCCR2A
    #define TCCRB  TCCR2B
    #define CS0    CS20
    #define CS1    CS21
    #define CS2    CS22
    #define OCRA   OCR2A
    #define OCIEA  OCIE2A
    #define TIMSK  TIMSK2
  #endif

  #define TDIV 64

  // Проверяем на соответствие частоты
  #if F_CPU != 16000000UL && F_CPU != 8000000UL
    #error "[STOP] Board F_CPU not matches, tasks will be work incorrectly."
  #endif

  // init
  TCCRA = 0x00;
  TCCRB = 0x00;
  // Делитель 64 для частот 8 и 16 МГц
  #if defined(__AVR_ATmega32U4__)
    setBit(TCCRA, WGM01);
    setBit(TCCRB, CS0);
    setBit(TCCRB, CS1);
    setBit(TCCRB, CS2);
  #elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__)
    setBit(TCCRA, WGM21);
    setBit(TCCRB, CS2);
  #endif
  /*
    Установка границы совпадения для делителя 64 и периода в 1мс
    OCR = timer_period / (1 / (F_CPU / timer_divider)) - 1
  */
  OCRA = ((F_CPU / TDIV) / 1000) - 1;

  setBit(TIMSK, OCIEA); // Comp Interrupt Enable
  sei();
}

// Прерывание по срабатыванию таймера
#if defined(__AVR_ATmega32U4__)
ISR(TIMER4_COMPA_vect)
#else
ISR(TIMER2_COMPA_vect)
#endif
{
  Tasks::tick();
};

// Обработка тика таймера
void Tasks::tick()
{
  // Сканируем очередь задач
  for (uint8_t i = 0; i < queueEnd; i++)
  {
    // Если пришло время запуска задачи, то ставим флаг готовности
    if (taskQueue[i].startDelay == 0)
      taskQueue[i].readyToRun = true;
    // Иначе просто уменьшаем время до запуска
    else taskQueue[i].startDelay--;
  }
}

//------------------------------------------------------------------------------
// Лаунчер
//------------------------------------------------------------------------------

// Запуск готовых задач
void Tasks::callLauncher()
{
  if (queueEnd == 0)
    return;

  // Сканируем очередь задач
  for (uint8_t i = 0; i < queueEnd; i++)
  {
    // Ищем готовые к запуску задачи
    if (!taskQueue[i].readyToRun)
      continue;

    // Нашли, копируем указатель
    void (*function)(void) = taskQueue[i].pTask;
    
    // Для циклической задачи обновим время задержки, задача при этом остается
    if (taskQueue[i].isInfinite)                              
    {
      taskQueue[i].readyToRun = false;
      taskQueue[i].startDelay = taskQueue[i].period;
    }
    // Если задача не циклична:
    else
    {
      // Если задача должна выполниться еще N раз, уменьшаем счетчик
      if (taskQueue[i].repeats > 1)
      {
        taskQueue[i].readyToRun = false;
        taskQueue[i].startDelay = taskQueue[i].period;
        taskQueue[i].repeats--;
      }
      // Для однократной задачи или если все повторы закончились:
      else
      {
        deleteByID(taskQueue[i].id);
      }
    }

    // Выполняем задачу
    (*function)();
  }
}

//------------------------------------------------------------------------------
// Добавление задач
//------------------------------------------------------------------------------

uint8_t Tasks::addSingle(void (*tFunc)(void), const uint16_t startDelay)
{
  return _addTask(tFunc, startDelay, 0, 0, false);
}

uint8_t Tasks::addRepeats(void (*tFunc)(void), const uint16_t startDelay, 
                          const uint16_t taskPeriod, const uint8_t repeats)
{
  return _addTask(tFunc, startDelay, taskPeriod, repeats, false);
}

uint8_t Tasks::addInfinite(void (*tFunc)(void), const uint16_t startDelay, 
                            const uint16_t taskPeriod)
{
  return _addTask(tFunc, startDelay, taskPeriod, 0, true);
}

// Добавление задачи
uint8_t Tasks::_addTask(void (*tFunc)(void), 
                    const uint16_t _startDelay, 
                    const uint16_t _period,
                    const uint8_t  _repeats,
                    const bool     _isInfinite)
{
  // Если есть место в очереди, добавляем задачу
  if (queueEnd < MAX_TASKS)
  {
    uint8_t id = 0;
    // АвтоID. Увеличиваем на единицу относительно последней задачи в очереди
    if (queueEnd > 0)
    {
      id = taskQueue[(queueEnd - 1)].id + 1;
    }
    else
      id = 1;

    taskQueue[queueEnd].id          = id;
    taskQueue[queueEnd].pTask       = tFunc;
    taskQueue[queueEnd].startDelay  = _startDelay;
    taskQueue[queueEnd].period      = _period;
    taskQueue[queueEnd].repeats     = _repeats;
    taskQueue[queueEnd].isInfinite  = _isInfinite;
    taskQueue[queueEnd].readyToRun  = false;
    queueEnd++;

    return id;
  }
  else
  {
    ERROR("Очередь"," > MAX_TASKS!");
    return 0;
  }
}

//------------------------------------------------------------------------------
// Удаление задач
//------------------------------------------------------------------------------

// Удаление задачи по ID
bool Tasks::deleteByID(const uint8_t id)
{
  // Ищем задачу по ID
  for (uint8_t i = 0; i < queueEnd; i++)
  {
    // Поиск успешен
    if (taskQueue[i].id == id)
    {
      _deleteByIndex(i);
      return true;
    }
  }
  return false;
}

// Удаление первой найденной по ссылке задачи
bool Tasks::deleteByLink(void (*tFunc)(void))
{
  return _deleteByLink(tFunc, false);
}

// Удаление всех найденных по ссылке задач
bool Tasks::deleteAllByLink(void (*tFunc)(void))
{
  return _deleteByLink(tFunc, true);
}

// Удаление задачи в очереди по ссылке
bool Tasks::_deleteByLink(void (*tFunc)(void), bool all)
{
  bool founded = false;

  for (uint8_t i = 0; i < queueEnd; i++)
  {
    // Поиск успешен
    if (taskQueue[i].pTask == tFunc)
    {
      founded = true;
      _deleteByIndex(i);
      if (!all)
        return true;
    }
  }
  return founded;
}

// Удаление задачи по индексу в очереди
void Tasks::_deleteByIndex(uint8_t index)
{
  // Сдвигаем очередь, если удаляемая задача не последняя
  if (index != (queueEnd-1))
  {
    for (uint8_t i = index; i < queueEnd; i++)
    {
      // Пересортировываем последующие элементы
      if (i < (queueEnd-1))
      {
        taskQueue[i].id         = taskQueue[(i + 1)].id;
        taskQueue[i].pTask      = taskQueue[(i + 1)].pTask;
        taskQueue[i].startDelay = taskQueue[(i + 1)].startDelay;
        taskQueue[i].period     = taskQueue[(i + 1)].period;
        taskQueue[i].repeats    = taskQueue[(i + 1)].repeats;
        taskQueue[i].isInfinite = taskQueue[(i + 1)].isInfinite;
        taskQueue[i].readyToRun = taskQueue[(i + 1)].readyToRun;
      }
    }
  }
  // Очередь меньше на 1 элемент
  queueEnd--;
}

//------------------------------------------------------------------------------

// Очистка очереди
void Tasks::clear()
{
  // Зачистка массива
  // for (uint8_t i = 0; i < queueEnd; i++)
  // {
  //   taskQueue[i].id         = 0;
  //   taskQueue[i].pTask      = NULL;
  //   taskQueue[i].startDelay = 0;
  //   taskQueue[i].period     = 0;
  //   taskQueue[i].repeats    = 0;
  //   taskQueue[i].isInfinite = false;
  //   taskQueue[i].readyToRun = false;
  // }

  queueEnd = 0;
}

// Получение текущего числа оставшихся повторений (для уканного Repeats)
uint8_t Tasks::getRemainedRepeats(const uint8_t id)
{
  for (uint8_t i = 0; i < queueEnd; i++)
  {
    if (taskQueue[i].id != id)
      continue;

    return taskQueue[i].repeats;
  }
  return 0;
}