#include <print.h>

void undef_handler(unsigned int registers[16]) {
  print("An undefined instruction was executed at %p\n\r",
        (void *)(registers[14] - 4));

  return;
}

void swi_handler(unsigned int registers[16]) {
  print("SWI at %p\n\r", (void *)(registers[14] - 4));

  return;
}

void irq_handler(unsigned int registers[16]) {
  print("interrupt at %p\n\r", (void *)(registers[14] - 8));

  return;
}

void fiq_handler(unsigned int registers[16]) {
  print("fast interrupt %p\n\r", (void *)(registers[14] - 8));

  return;
}

void prefetch_abort_handler(unsigned int registers[16]) {
  print("prefetch abort %p\n\r", (void *)(registers[14] - 4));

  return;
}

void data_abort_handler(unsigned int registers[16]) {
  print("A data abort happened at %p\n\r", (void *)(registers[14] - 8));

  return;
}