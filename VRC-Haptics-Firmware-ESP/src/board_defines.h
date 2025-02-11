#ifndef BOARD_DEFINES_H
#define BOARD_DEFINES_H

// switch between board imports
#ifdef BOARD_ESP32C3_SUPERMINI
#include "boards/esp32c3-supermini.h"
#else 
#include "boards/default.h"
#warning "no specific board defined, using default values. This typically isn't desired behaviour"
#endif

#endif
