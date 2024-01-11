#ifndef _SHRINE_OS_LIB_H_
#define _SHRINE_OS_LIB_H_

void sys_call_exit_thread();
int sys_call_create_thread(int (*fun)(void *), void *input);
void sys_call_sleep(unsigned int duration);
char sys_call_read_char();
void sys_call_put_char(char c);
void sys_call_register_irq_callback(unsigned int callback_id,
                                    int (*fun)(void *));

#endif /* ifndef _SHRINE_OS_LIB_H_ */
