#include <dbgu.h>
#include <mem_addresses.h>
#include <print.h>

#define SYS_INTERRUPT 1

extern void handle_sys_interrupt();

struct aic {
  unsigned int smr[32];
  void *svr[32];
  unsigned int __ivr;
  unsigned int __fvr;
  unsigned int __isr;
  unsigned int __ipr;
  unsigned int __imr;
  unsigned int __cisr;
  unsigned int __reserved[2];
  unsigned int iecr;
  unsigned int idcr;
  unsigned int iccr;
  unsigned int iscr;
  unsigned int eoicr;
  unsigned int spu;
  unsigned int dcr;
};

volatile struct aic *const aic = (struct aic *)AIC;

void timer_interrupt_handler() { print("!\n\r"); }

void dbgu_interrupt_handler() { print("?\n\r"); }

void system_interrupt_handler() {
  print("system interrupt received!\n\r");

  // read status registers of the system peripherals
  // to determine where the interrupt comes from
  if (st_interrupt_active()) {
    timer_interrupt_handler();
  } else if (dbgu_interrupt_active()) {
    dbgu_interrupt_handler();
  }

  // finally, signal that the interrupt handling is over
  aic->eoicr = 1;
}

void install_interrupt_handlers() {
  aic->iecr = 1 << SYS_INTERRUPT;
  aic->smr[SYS_INTERRUPT] = 7; // set highest priority
  aic->svr[SYS_INTERRUPT] = *handle_sys_interrupt;

  // print("address of iecr: %p \n\r", &(aic->iecr));
}
