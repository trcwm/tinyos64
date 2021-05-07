#pragma once

#include <stdint.h>

void halt();

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

uint64_t cpuid(uint64_t index);

uint64_t read_cr0();
uint64_t read_cr4();
uint64_t read_efer();
uint64_t read_cs();

uint64_t read_stackptr();

uint64_t read_msr(const uint32_t msr);
void write_msr(const uint32_t msr, const uint64_t value);

void setISR(uint8_t intnum, uint64_t address);
void enableInterrupts();
void disableInterrupts();
void int10();