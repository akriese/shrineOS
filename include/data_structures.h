#ifndef _DATA_STRUCTURES_H_
#define _DATA_STRUCTURES_H_

/**
 * @class ring_buffer
 * @brief Ring buffer for [unsigned int] with a length and two indices marking
 * current input and output pointers
 */
typedef struct ring_buffer {
  unsigned int length;
  unsigned int next_in;
  unsigned int next_out;
  unsigned int *buffer;
} ring_buffer;

/**
 * @brief Creates a ring buffer where the buffer lies directly after the *buffer
 * pointer.
 *
 * Expects that (size-4) bytes are reserved (allocated) at *start.
 *
 * @param size Number of bytes that can be used by the data structure.
 * @param start Pointer to the start of the object in memory.
 * @return Pointer to the created object.
 */
volatile ring_buffer *ring_buffer_create(unsigned int size,
                                         unsigned int *start) {
  volatile ring_buffer *b = (ring_buffer *)start;
  b->length = (size / 4) - 4;
  b->buffer = start + 4;
  b->next_in = 0;
  b->next_out = 0;

  return b;
}

/**
 * @brief Checks if the given ring_buffer has a next element.
 */
int ring_buffer_available(volatile ring_buffer *b) {
  return b->next_out != b->next_in;
}

void ring_buffer_put(volatile ring_buffer *b, unsigned int element) {
  b->buffer[b->next_in] = element;
  b->next_in = (b->next_in + 1) % b->length;
}

/**
 * Returns the next object of the buffer.
 *
 * Assumes that a next item is available.
 * If not, the returned value will be unexpected and the internal pointers
 * are broken.
 */
unsigned int ring_buffer_get(volatile ring_buffer *b) {
  unsigned int element = *(b->buffer + b->next_out);
  b->next_out = (b->next_out + 1) % b->length;

  return element;
}

#endif /* ifndef _DATA_STRUCTURES_H_ */
