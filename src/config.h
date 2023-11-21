//=========================================================================== //
// Настройки системы
//==============================================================================

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

//------------------------------------------------------------------------------
// Катушки
//------------------------------------------------------------------------------

#define UNITS_COUNT 12

#define COIL_1_PIN  22
#define COIL_2_PIN  24
#define COIL_3_PIN  26
#define COIL_4_PIN  28
#define COIL_5_PIN  30
#define COIL_6_PIN  32
#define COIL_7_PIN  34
#define COIL_8_PIN  36
#define COIL_9_PIN  38
#define COIL_10_PIN 40
#define COIL_11_PIN 42
#define COIL_12_PIN 44

// Максимальное время работы катушки, мс
#define COIL_TIMEOUT 1600

//------------------------------------------------------------------------------
// Датчики Холла
//------------------------------------------------------------------------------

#define HALL_1_PIN  A11
#define HALL_2_PIN  A10
#define HALL_3_PIN  A9
#define HALL_4_PIN  A8
#define HALL_5_PIN  A7
#define HALL_6_PIN  A6
#define HALL_7_PIN  A5
#define HALL_8_PIN  A4
#define HALL_9_PIN  A3
#define HALL_10_PIN A2
#define HALL_11_PIN A1
#define HALL_12_PIN A0

#define HALL_UPDATE_INTERVAL 1

#define HALL_VALUE_MAX   650
// #define HALL_VALUE_MIN   10
#define HALL_VALUE_MED   500
#define HALL_VALUE_DELTA 60

//------------------------------------------------------------------------------
// Индикация
//------------------------------------------------------------------------------
#define LED_INT 13

//------------------------------------------------------------------------------
// Задачи
//------------------------------------------------------------------------------
#define MAX_TASKS 8

//------------------------------------------------------------------------------
// Отладка и логирование
//------------------------------------------------------------------------------
#define LOG_ENABLE   1
#define DEBUG_ENABLE 1

#define DEBUG_PORT       Serial
#define DEBUG_PORT_SPEED 115200

#define LOG_LEVEL    2
#define DEBUG_LEVEL  2

#endif