#ifndef _DBGU_H_
#define _DBGU_H_

// Reads a char from the serial interface
char dbgu_getc();

// Writes a char to the serial interface
void dbgu_putc(char c);

// Writes a string to the serial interface. Stops at the first \0 in s
void serial_write_string(char *s);

// enables interrupts on receive
void dbgu_enable_interrupts();

// checks if the current interrupt was triggered by the dbgu
int dbgu_interrupt_active();

void dbgu_receive_interrupt_handler();

void dbgu_initialize();

char dbgu_grab_char();

void dbgu_enable();

int dbgu_has_next();

#endif // !_DBGU_H_
