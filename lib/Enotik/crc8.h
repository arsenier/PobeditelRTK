#pragma once
#include <avr/io.h>
#include <Arduino.h>
#include "stdint.h"
uint8_t crc8(const byte * array, uint32_t len);

