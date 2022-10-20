#pragma once
#include <assert.h>
#include <stddef.h>

/*! \file vec.h
 *  \brief Generic implementation of a "Vec". A generic list of elements, that
 *  can automatically resize if needed.
 *
 *  \par Usage example
 *  \parblock \code
 *  #include "vec.h"
 *
 *  int main() {
 *      int *divisers = vec_new(int);
 *      for (int i = 0; i < 100; i++) {
 *          *vec_push(&divisers, int) = 100 - i;
 *      }
 *      while (vec_size(divisers) > 0) {
 *          printf(" - %d\n", vec_pop(divisers));
 *      }
 *
 *      // DONT FORGET
 *      vec_destroy(divisers);
 *  }
 *  \endcode \endparblock
 */

/*! \brief Creates a new empty vec of the given type.
 *
 *  \param type The type of the elements the array will contain.
 *  \returns A new Vec for elements of the given type.
 */
#define vec_new(type) (type *)vec_new(sizeof(type))
/*! \brief Get a new empty space at the end of the vec.
 *
 *  \param vec Pointer to the Vec (=> a double pointer!!) into which to push.
 *  \returns A pointer to the newly created last element of the Vec.
 */
#define vec_push(vec, type) ((type *)vec_append((void **)vec))
/*! \brief Removes the last value of the Vec.
 *
 *  \param vec The Vec from which to pop.
 *  \returns The now removed value.
 */
#define vec_pop(vec, type) (*(type *)vec_remove_last((void *)vec))

/*! \brief Creates a new Vec, prefer using #vec_new.
 *
 *  \param elem_size Individual size of each elements in the Vec.
 *  \param capacity How much memory should be allocated. If set to zero memory
 *                  for the metadata will still be allocated.
 */
void *vec_create(size_t elem_size, size_t capacity);
/*! \brief Dellocates any memory used by the given Vec.
 *
 *  The vec MUST NOT be used after calling this function.
 *
 *  \param vec The vec to deallocate.
 */
void vec_destroy(void *vec);

/*! \brief Get the amount of elements currently in the Vec.
 *
 *  \param vec The vec from which to get the size.
 *  \returns How many elements currently are in the Vec.
 */
size_t vec_size(void *vec);
/*! \brief Get the current total capacity of the Vec.
 *
 *  The capacity is additional memory not used by any elements used for eventual
 *  insertions, NOT the same as #vec_size.
 *
 *  \param vec The vec from which to get the capacity.
 *  \returns Total capacity of the Vec.
 */
size_t vec_capacity(void *vec);
/*! \brief Get the size of individual elements of the Vec.
 *
 *  \param vec The vec from which to get the elem_size.
 *  \returns The elem_size of the Vec.
 */
size_t vec_elem_size(void *vec);

/*! \brief Makes sure there is at least the given amount of capacity left in
 *         given vec.
 *
 *  The given vec MUST NOT be used after calling this function. This is because
 *  if additional capacity must be allocated, the pointer will change, this
 *  in effect deallocates the given Vec.
 *  The returned Vec should be used instead, but it will be equal to the given
 *  one if no allocation occured.
 *
 *  \param vec Vec from which to reserve space.
 *  \param n How many element should be reserved.
 *  \returns The elem_size of the Vec.
 */
void *vec_reserve(void *vec, size_t n);
/*! \brief Tries to add an element to the Vec if there is enough capacity.
 *
 *  If there is more capacity then elements in the Vec, that is, there is still
 *  space left in the buffer, than this increase the size of the Vec and returns
 *  a pointer to the last element.
 *  If there isn't enough capacity, returns NULL.
 *
 *  You probably wan to use #vec_push instead of this directly.
 *
 *  \param vec Vec from which to append a new slot.
 *  \returns Pointer to the newly created slot.
 */
void *vec_try_new_slot(void *vec);
/*! \brief Removes the last element of the Vec.
 *
 *  The returned pointer should be used immediately as it will be invalidated
 *  by many other vec operations.
 *  You may want to use the convinient macro #vec_pop.
 *
 *  \param vec The Vec from which to remove the last element.
 *  \returns Vector to the removed element, should be considered invalid after
 *  any call to another vec function. */
void *vec_remove_last(void *vec);

/*! \brief Removes all elements from the Vec.
 *
 *  O(1) operation used to remove all elements currently in the Vec.
 *
 *  \param vec The Vec from which to remove all elements.
 */
void vec_clear(void *vec);
/*! \brief Shortcut function used for implementing #vec_push.
 *
 *  The given pointer should be a pointer to a Vec created with #vec_create or
 * #vec_new. This pointer may be changed if an allocation was required.
 *
 *  This is equivalent to:
 *  \parblock \code
 *  *vec = vec_reserve(*vec, 1);
 *  return vec_try_new_slot(*vec);
 *  \endcode \endparblock
 *
 *  \param vec Pointer to a Vec (which is a pointer so vec is a double pointer).
 *  \returns A non null pointer to the newly created slot.
 */
void *vec_append(void **vec);
/*! \brief Removes the element at position index in the Vec.
 *
 *  The removed element is replaced with the last element of the Vec.
 *  This means the ordering isn't preserved, but is computed in O(1) time.
 *
 *  \param vec Vec from which to remove the element at index.
 *  \param index Index of the element to remove.
 */
void vec_swap_remove(void *vec, size_t index);
