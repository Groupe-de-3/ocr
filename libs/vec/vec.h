#pragma once
#include <assert.h>
#include <stddef.h>

#define vec_new(type) (type *)vec_new(sizeof(type))
#define vec_push(vec, type) ((type *)vec_append((void **)&vec))
#define vec_pop(vec, type) (*(type *)vec_remove_last((void *)vec))

void *vec_create(size_t elem_size, size_t capacity);
void  vec_destroy(void *vec);

size_t vec_size(void *vec);
size_t vec_capacity(void *vec);
size_t vec_elem_size(void *vec);

void *vec_reserve(void *vec, size_t n);
void *vec_try_new_slot(void *vec);
void *vec_remove_last(void *vec);

void  vec_clear(void *vec);
void *vec_append(void **vec);
void  vec_swap_remove(void *vec, size_t index);
