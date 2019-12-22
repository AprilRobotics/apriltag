/* Copyright (C) 2013-2017, The Regents of The University of Michigan.
   All rights reserved.

   This software was developed in the APRIL Robotics Lab under the
   direction of Edwin Olson, ebolson@umich.edu. This software may be
   available under alternative licensing terms; contact the address above.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _ZQUEUE_H
#define _ZQUEUE_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct zqueue zqueue_t;

/**
 * Creates and returns a variable FIFO queue structure capable of holding elements of
 * the specified size. It is the caller's responsibility to call zqueue_destroy()
 * on the returned array when it is no longer needed.
 */
zqueue_t *zqueue_create(size_t el_sz);

/**
 * Frees all resources associated with the variable queue structure which was
 * created by zqueue_create(). After calling, 'zq' will no longer be valid for storage.
 */
void zqueue_destroy(zqueue_t *zq);

/**
 * Copy a zqueue
 */
zqueue_t *zqueue_copy(zqueue_t *zq);

/**
 * Retrieves the number of elements currently being contained by the supplied
 * queue. The index of the last element in the queue will be one less than the
 * returned value.
 */
int zqueue_size(const zqueue_t *zq);

/**
 * Allocates enough internal storage in the supplied variable array structure to
 * guarantee that the supplied number of elements (capacity) can be safely stored.
 */
void zqueue_ensure_capacity(zqueue_t *zq, size_t num);

/**
 * Adds a new element to the back of the supplied queue, and sets its value
 * (by copying) from the data pointed to by the supplied pointer 'p'.
 * Automatically ensures that enough storage space is available for the new element.
 */
void zqueue_add_back(zqueue_t *zq, const void *p);

/**
 * Retrieves the element from the supplied queue located at the zero-based
 * index of 'idx' and copies its value into the variable pointed to by the pointer
 * 'p'. idx must be a valid index (i.e. 0 <= idx <= zqueue_size() - 1)
 */
void zqueue_get(const zqueue_t *zq, int idx, void *p);

/**
 * Similar to zqueue_get(), but returns a "live" pointer to the internal
 * storage, avoiding a memcpy. This pointer is not valid across
 * operations which might move memory around (i.e. zqueue_remove_front(),
 * zarray_add_back(), zarray_clear()).
 * 'p' should be a pointer to the pointer which will be set to the internal address.
 */
void zqueue_get_volatile(const zqueue_t *zq, int idx, void *p);

/**
 * Removes the entry at the front of the supplied queue.
 */
void zqueue_remove_front(zqueue_t *zq);

/**
 * Adds the supplied element to the back of the queue (see zqueue_add_back())
 */
#define zqueue_push(zq, p)    zqueue_add_back(zq, p)

/**
 * Removes the first element from the front of the queue and copies its value
 * into the variable pointed to by the pointer 'p' (see zqueue_get() and
 * zqueue_remove()).
 *
 */
#define zqueue_pop(zq, p)                                                                          \
    {                                                                                              \
        zqueue_get(zq, 0, p);                                                                      \
        zqueue_remove_front(zq);                                                                   \
    }


/**
 * Removes all elements from the queue and sets its size to zero. Pointers to
 * any data elements obtained i.e. by zqueue_get_volatile() will no longer be
 * valid.
 */
void zqueue_clear(zqueue_t *zq);

/**
 * Calls the supplied function for every element in the queue in order.
 * The map function will be passed a pointer to each element in turn and must
 * have the following format:
 *
 * void map_function(element_type *element)
 */
void zqueue_map(zqueue_t *zq, void (*f)());

/**
 * Calls the supplied function for every element in the queue in index order.
 * HOWEVER values are passed to the function, not pointers to values. In the
 * case where the zqueue stores object pointers, zqueue_vmap allows you to
 * pass in the object's destroy function (or free) directly. Can only be used
 * with zqueue's which contain pointer data. Should not be used simultaneously
 * with any operations which might move memory around (i.e. zqueue_remove_front(),
 * zarray_add_back(), zarray_clear()).
 *
 * The map function should have the following format:
 *
 * void map_function(element_type *element)
 */
void zqueue_vmap(zqueue_t *zq, void (*f)());

    void zqueue_remove_index(zqueue_t *zq, int idx);

#ifdef __cplusplus
}
#endif

#endif
