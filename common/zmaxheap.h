/* $COPYRIGHT_UM
$LICENSE_BSD
*/

#pragma once

#include <stdio.h>

typedef struct zmaxheap zmaxheap_t;

typedef struct zmaxheap_iterator zmaxheap_iterator_t;
struct zmaxheap_iterator {
    zmaxheap_t *heap;
    int in, out;
};

zmaxheap_t *zmaxheap_create(size_t el_sz);

void zmaxheap_vmap(zmaxheap_t *heap, void (*f)());

void zmaxheap_destroy(zmaxheap_t *heap);

void zmaxheap_add(zmaxheap_t *heap, void *p, float v);

int zmaxheap_size(zmaxheap_t *heap);

// returns 0 if the heap is empty, so you can do
// while (zmaxheap_remove_max(...)) { }
int zmaxheap_remove_max(zmaxheap_t *heap, void *p, float *v);

////////////////////////////////////////////
// This is a peculiar iterator intended to support very specific (and
// unusual) applications, and the heap is not necessarily in a valid
// state until zmaxheap_iterator_finish is called.  Consequently, do
// not call any other methods on the heap while iterating through.

// you must provide your own storage for the iterator, and pass in a
// pointer.
void zmaxheap_iterator_init(zmaxheap_t *heap, zmaxheap_iterator_t *it);

// Traverses the heap in top-down/left-right order. makes a copy of
// the content into memory (p) that you provide.
int zmaxheap_iterator_next(zmaxheap_iterator_t *it, void *p, float *v);

// will set p to be a pointer to the heap's internal copy of the dfata.
int zmaxheap_iterator_next_volatile(zmaxheap_iterator_t *it, void *p, float *v);

// remove the current element.
void zmaxheap_iterator_remove(zmaxheap_iterator_t *it);

// call after all iterator operations are done. After calling this,
// the iterator should no longer be used, but the heap methods can be.
void zmaxheap_iterator_finish(zmaxheap_iterator_t *it);
