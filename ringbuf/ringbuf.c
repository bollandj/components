#include <string.h>

#include "ringbuf.h"

void ringbuf_init(ringbuf_t *rb, void *buf, unsigned int size)
{
    memset(rb, 0, sizeof(*rb));
    rb->data_len = size;
    rb->data = (uint8_t *)buf;
}

unsigned int ringbuf_write(ringbuf_t *rb, const void *data, unsigned int data_len)
{
    const uint8_t *d8 = (const uint8_t *)data;
    unsigned int free_space = ringbuf_free_space(rb);

    if (data_len >= free_space)
        data_len = free_space;

    unsigned int to_end = rb->data_len - rb->head;
    if (to_end >= data_len)
        to_end = data_len;

    memcpy(&rb->data[rb->head], data, to_end);

    if (to_end < data_len)
        memcpy(rb->data, d8 + to_end, data_len - to_end);

    rb->head = (rb->head + data_len) % rb->data_len;

    rb->bytes_written += data_len;

    return data_len;
}

unsigned int ringbuf_read(ringbuf_t *rb, void *data, unsigned int max_len)
{
    unsigned int used = ringbuf_used_space(rb);
    uint8_t *d8 = (uint8_t *)data;

    if (used < max_len)
        max_len = used;
    unsigned int to_end = rb->data_len - rb->tail;

    if (to_end >= max_len)
        to_end = max_len;

    memcpy(d8, &rb->data[rb->tail], to_end);

    if (to_end < max_len)
        memcpy(d8 + to_end, rb->data, max_len - to_end);

    rb->tail = (rb->tail + max_len) % rb->data_len;

    rb->bytes_read += max_len;

    return max_len;
}

bool ringbuf_full(const ringbuf_t *rb)
{
    return rb->head == ((rb->tail - 1) % rb->data_len);
}

bool ringbuf_empty(const ringbuf_t *rb)
{
    return rb->head == rb->tail;
}

unsigned int ringbuf_used_space(const ringbuf_t *rb)
{
    return (rb->head - rb->tail) % rb->data_len;
}

unsigned int ringbuf_free_space(const ringbuf_t *rb)
{
    return rb->data_len - ((rb->head - rb->tail) % rb->data_len) - 1;
}
