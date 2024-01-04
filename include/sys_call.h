#ifndef _SYS_CALL_H_
#define _SYS_CALL_H_

#include <resource.h>

#define SYSCALL_NUM_THREAD_EXIT 1
#define SYSCALL_NUM_THREAD_CREATE 2
#define SYSCALL_NUM_THREAD_SLEEP 3
#define SYSCALL_NUM_IO_READ_CHAR 4
#define SYSCALL_NUM_IO_PUT_CHAR 5

void sys_call_handler(unsigned int number, void *context);
void sys_call_post_unblock(enum resource_type blocking_resource,
                           unsigned int unblocked_thread_id);

void sys_call_exit_thread();
int sys_call_create_thread(int (*fun)(void *), void *input);
void sys_call_sleep(unsigned int duration);
char sys_call_read_char();
void sys_call_put_char(char c);

#endif // !_SYS_CALL_H_
