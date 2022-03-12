/**
 * Simple ring buffer implementation adapted from Andre Renaud's code at https://github.com/AndreRenaud/RingBuffer
 * Lockless and thread safe, as long as there is guaranteed one reader and one writer
 * No dynamic memory allocation. No external dependencies (beyond memcpy & memset)
 */

#ifndef RINGBUF_H_
#define RINGBUF_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct ringbuf {
    uint8_t *data;
    unsigned int data_len;
    unsigned int head;
    unsigned int tail;

    uint32_t bytes_written;  // Stores the total number of bytes written to the buffer
    uint32_t bytes_read;     // Stores the total number of bytes read to the buffer
} ringbuf_t;

/**
 * Initialise the ring buffer
 * @param rb Ring buffer
 * @param buffer Area to store data in
 * @param size Number of bytes in buffer (must be a power of 2)
 */
void ringbuf_init(ringbuf_t *rb, void *buffer, unsigned int size);

/**
 * Write data into the ring buffer
 * @param rb Ring buffer to write to
 * @param data Data to write in
 * @param data_len maximum number of bytes in `data` to try and write
 * @return Number of bytes written
 */
unsigned int ringbuf_write(ringbuf_t *rb, const void *data, unsigned int data_len);

/**
 * Read data out of the ring buffer
 * @param rb Ring buffer to read from
 * @param data Area to store data in
 * @param max_len Maximum number of bytes to read into `data`
 * @return Number of bytes read into `data`
 */
unsigned int ringbuf_read(ringbuf_t *rb, void *data, unsigned int max_len);

/**
 * Determine if the ring buffer is full or not
 */
bool ringbuf_full(const ringbuf_t *rb);

/**
 * Determine if the ring buffer is empty or not
 */
bool ringbuf_empty(const ringbuf_t *rb);

/**
 * Determine how many bytes are currently stored in the ring buffer
 */
unsigned int ringbuf_used_space(const ringbuf_t *rb);

/**
 * Determine how many bytes can currently be written into the ring
 * buffer
 */
unsigned int ringbuf_free_space(const ringbuf_t *rb);

#endif /* RINGBUF_H_ */