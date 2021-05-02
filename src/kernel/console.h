#pragma once

#include <stdint.h>

void console_clear();
void console_write(const char *str);
void console_xy(const uint32_t x, const uint32_t y);
void console_outch(const char c);