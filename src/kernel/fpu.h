#pragma once

#include <stdint.h>

void fpu_init();
void fpu_load_control_word(const uint16_t control);