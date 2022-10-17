#include "vec.h"

#include <stdalign.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#ifndef __GNUC__
#error The Vec implementation rely on gcc (and clang) specific extensions.
#else
#define MAX_ALIGN __attribute__((aligned(alignof(max_align_t))))
#endif

typedef struct VecMetaData {
    size_t elem_size;
    size_t size;
    size_t capacity;
} MAX_ALIGN VecMetaData;

void *vec_create(size_t elem_size, size_t capacity) {
    size_t next_po2_capacity = capacity > 0 ? 1 : 0;
    while (next_po2_capacity < capacity)
        next_po2_capacity <<= 1;

    VecMetaData *ptr =
        malloc(sizeof(VecMetaData) + next_po2_capacity * elem_size);
    *ptr = (VecMetaData){
        .elem_size = elem_size,
        .size      = 0,
        .capacity  = next_po2_capacity,
    };

    return (void *)(ptr + 1);
}
void vec_destroy(void *vec) {
    free((VecMetaData *)vec - 1);
}

static VecMetaData *vec_get_metadata(void *vec) {
    return (VecMetaData *)vec - 1;
}

size_t vec_size(void *vec) {
    return vec_get_metadata(vec)->size;
}
size_t vec_capacity(void *vec) {
    return vec_get_metadata(vec)->capacity;
}
size_t vec_elem_size(void *vec) {
    return vec_get_metadata(vec)->elem_size;
}

void *vec_reserve(void *vec, size_t n) {
    VecMetaData *real_ptr = (VecMetaData *)vec - 1;
    if (real_ptr->capacity - real_ptr->size >=
        n) // Making sure alocation is required
        return vec;

    size_t next_po2_capacity = 1;
    while (next_po2_capacity < real_ptr->size + n)
        next_po2_capacity <<= 1;

    real_ptr->capacity = next_po2_capacity;

    VecMetaData *new_real_ptr = realloc(
        real_ptr, sizeof(VecMetaData) + real_ptr->capacity * real_ptr->elem_size
    );
    void *new_vec = (void *)(new_real_ptr + 1);
    return new_vec;
}
void *vec_try_new_slot(void *vec) {
    VecMetaData *metadata = vec_get_metadata(vec);

    assert(metadata->size < metadata->capacity);
    void *last_slot =
        (void *)((char *)vec + metadata->size * metadata->elem_size);
    metadata->size++;
    return last_slot;
}
void *vec_remove_last(void *vec) {
    VecMetaData *metadata = vec_get_metadata(vec);

    assert(metadata->size >= 1);
    metadata->size -= 1;
    return (void *)((char *)vec + metadata->elem_size * metadata->size);
}

void vec_clear(void *vec) {
    vec_get_metadata(vec)->size = 0;
}
void *vec_append(void **vec) {
    *vec = vec_reserve(*vec, 1);
    return vec_try_new_slot(*vec);
}
void vec_swap_remove(void *vec, size_t index) {
    VecMetaData *metadata = vec_get_metadata(vec);
    assert(index < metadata->size);
    metadata->size -= 1;

    // If index was the last element there is nothing to swap.
    // Also takes care of the one element vec case
    // (only one -> index must be last element).
    if (index == metadata->size)
        return;

    unsigned char *byte_array     = vec;
    unsigned char *to_remove_elem = byte_array + index * metadata->elem_size;
    // Since size was already decremented, the last element is
    // now outside the size of the vec.
    // But we still need to access it to replace
    // the element we really want to remove.
    unsigned char *last_elem =
        byte_array + metadata->size * metadata->elem_size;

    memcpy(to_remove_elem, last_elem, metadata->elem_size);
}
