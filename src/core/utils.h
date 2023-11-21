
//=========================================================================== //
// Вспомогательные функции
//==============================================================================

#ifndef UTILS_H
#define UTILS_H

#include "../config.h"

//------------------------------------------------------------------------------
// Логирование и дебаг
//------------------------------------------------------------------------------

#define PRINT(com) (DEBUG_PORT.print(com))
#define PRINTL(com) (DEBUG_PORT.println(com))
#define WRITE(com) (DEBUG_PORT.write(com))

// Вывод логов в консоль
#define _PRINT_LOG(source, text, var) \
  PRINT(F("["));                      \
  PRINT(F(source));                   \
  PRINT(F("] "));                     \
  PRINT(F(text));                     \
  PRINTL(var);

// Вывод ошибки
#define _ERROR(text, error) \
  PRINT(F("(!) "));         \
  PRINT(F(text));           \
  PRINTL(error);

//---

#if (LOG_ENABLE)

#define DEBUG_PORT_INIT() DEBUG_PORT.begin(DEBUG_PORT_SPEED)

#if (LOG_LEVEL == 1)
#define LOG(source, text, var) \
  _PRINT_LOG(source, text, var);
#define LOG2(source, text, var)
#define ERROR(source, text, var) \
  _ERROR(text, error);

#elif (LOG_LEVEL == 2)
#define LOG(source, text, var) \
  _PRINT_LOG(source, text, var);
#define LOG2(source, text, var) \
  _PRINT_LOG(source, text, var);
#define ERROR(text, error) \
  _ERROR(text, error);
#endif
#else
#define DEBUG_PORT_INIT()
#define LOG(source, text, var)
#define LOG2(source, text, var)
#define ERROR(text, error)

#endif

//==============================================================================

#if (DEBUG_ENABLE)

#define DEBUG_PORT_INIT() DEBUG_PORT.begin(DEBUG_PORT_SPEED)

#if (DEBUG_LEVEL == 1)
#define DEBUG(com) (DEBUG_PORT.print(F(com)))
#define DEBUGL(com) (DEBUG_PORT.println(F(com)))
#define DEBUGVAR(prefix, var) \
  DEBUG_PORT.print(F(prefix));    \
  DEBUG_PORT.println(var)
#define DEBUG2(com)
#define DEBUG2L(com)

#elif (DEBUG_LEVEL == 2)
#define DEBUG(com) (DEBUG_PORT.print(F(com)))
#define DEBUGL(com) (DEBUG_PORT.println(F(com)))
#define DEBUG2(com) (DEBUG_PORT.print(F(com)))
#define DEBUG2L(com) (DEBUG_PORT.println(F(com)))
#define DEBUGVAR(prefix, var) \
  DEBUG_PORT.print(F(prefix));    \
  DEBUG_PORT.println(var)
#define DEBUG2VAR(prefix, var) \
  DEBUG_PORT.print(F(prefix));     \
  DEBUG_PORT.println(var)
#endif
#else
#define DEBUG_PORT_INIT()
#define DEBUG(com)
#define DEBUGL(com)
#define DEBUG2(com)
#define DEBUG2L(com)
#define DEBUGVAR(com)
#define DEBUG2VAR(com)
#endif

//------------------------------------------------------------------------------
// Битовые операции
//------------------------------------------------------------------------------

#define setBit(reg, bit) (reg |= (1<<bit))
#define clearBit(reg, bit) (reg &= ~(1<<bit))

#endif