#include <dbgu.h>
#include <print.h>
#include <scheduler.h>
#include <shrine_os_lib.h>
#include <system.h>
#include <util.h>

typedef struct print_thread_info {
  unsigned int sleep_or_repeat;
  char c;
  char free; // set to 1 (freed) by print threads
} print_thread_info;

print_thread_info thread_info_buffer[MAX_NUM_THREADS];

static int started_threads_counter = 0;
static int sleep_time = 500;
static int million_computation_cycles = 200;
const int PRINT_TIMES = 10;

/**
 * @brief Prints a char repeatedly with some computation inbetween.
 *
 * @param input Pointer to a print_thread_info object. This contains the
 * character to print and the number of comupation iterations.
 * @return 0
 */
int print_char_repeatedly_with_computation(void *input) {
  print_thread_info *info = (print_thread_info *)input;
  const char x = info->c;
  const unsigned int compute_repititions = info->sleep_or_repeat;
  info->free = 1;

  // print the character 6 times with small pauses
  volatile int c = PRINT_TIMES;
  while (c-- > 0) {
    print("%c", x);
    volatile int i;
    for (i = 0; i < compute_repititions * 1000000; i++) {
    }
  }

  sys_call_exit_thread();
  return 0;
}

/**
 * @brief Prints a character repeatedly while sleeping inbetween.
 *
 * @param input Pointer to a print_thread_info object. This contains the
 * character to print and the sleep duration for between each print.
 * @return 0
 */
int print_char_repeatedly_with_sleep(void *input) {
  print_thread_info *info = (print_thread_info *)input;
  const char x = info->c;
  const unsigned int sleep_duration = info->sleep_or_repeat;

  // mark the info block as free, as all the infos were moved to local variables
  info->free = 1;

  // print the character 6 times sleeping a short time inbetween
  volatile int c = PRINT_TIMES;
  while (c-- > 0) {
    print("%c", x);

    sys_call_sleep(sleep_duration);
  }

  sys_call_exit_thread();
  return 0;
}

/**
 * @brief Creates new printing threads depending on the character's case.
 *
 * @param c The character to be printed.
 * @return 0 on success; 1 if no thread was created.
 */
int input_callback(char c) {
  // check for an unused thread info object
  int count = 0;
  while (!thread_info_buffer[started_threads_counter % MAX_NUM_THREADS].free &&
         count++ < MAX_NUM_THREADS) {
    started_threads_counter++;
  }

  if (count == MAX_NUM_THREADS) {
    print("No thread info block available...\n\r");
    return 1;
  }

  print_thread_info *thread_info =
      &thread_info_buffer[started_threads_counter % MAX_NUM_THREADS];

  // lock this block from being accessed by another call of this function
  thread_info->free = 0;
  thread_info->c = c;

  int create_result;

  // try to create the new thread
  // depending on the case of the character, we use different print threads
  if (is_upper(c)) {
    thread_info->sleep_or_repeat = million_computation_cycles;
    create_result = sys_call_create_thread(
        print_char_repeatedly_with_computation, thread_info);
  } else if (is_lower(c)) {
    thread_info->sleep_or_repeat = sleep_time;
    create_result =
        sys_call_create_thread(print_char_repeatedly_with_sleep, thread_info);
  } else {
    print("Not a valid character! Doing nothing!\n\r");
    return 1;
  }

  started_threads_counter++;

  return 0;
}

/**
 * @brief The example program to execute threads with syscalls.
 * This has to set up interrupts, system timer, dbgu and the scheduler.
 * This thread starts other threads (print threads) as it listens on keyboard
 * input.
 *
 * @return 0
 */
int sys_call_application() {
  // get some run settings concerning time intervalls form the user
  int intervall = get_number(
      "Enter the intervall of thread switches [in ms] and press ENTER", 100);

  sys_call_st_set_pits_intervall(intervall);

  sleep_time =
      get_number("Enter the sleep duration [in ms] for threads!", sleep_time);
  million_computation_cycles = get_number(
      "Enter the number of computation cycles between prints (in millions)!",
      million_computation_cycles);

  print("The application starts now!\n\r");

  // mark all thread infos as free before usage
  int i;
  for (i = 0; i < MAX_NUM_THREADS; ++i) {
    thread_info_buffer[i].free = 1;
  }

  print("Type characters to start threads:\n\r"
        "- Upper case for actively waiting (computation)\n\r"
        "- Lower case for passively waiting (sleep)\n\r"
        "- ENTER for a linebreak\n\r\n\r");

  // use a custom idle function
  sys_call_set_idle_function(idling);

  /**
   * Runs in an endless loop and listens on input characters.
   * These are forwarded to the actual input callback.
   * This is supposed to be a user application as it only uses syscalls
   * to execute kernel functions.
   */
  while (1) {
    char c = sys_call_read_char();

    // print newline if the user wants to have a line break
    if (c == 13 || c == 10) {
      print("\n\r");
      continue;
    }

    input_callback(c);
  }

  sys_call_exit_thread();
  return 0;
}
