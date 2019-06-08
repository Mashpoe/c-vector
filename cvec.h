//
//  cvec.c
//
//  Created by Mashpoe on 2/26/19.
//

#ifndef cvec_h
#define cvec_h

#include <stdlib.h>
#include <stdbool.h>

typedef void* vector;

typedef int*	vec_int;
typedef char*	vec_char;

// shortcut defines
#define _vector_create(type)		(vector_create(sizeof(type)))
// v is a vector* (aka void**)
#define _vector_add(v, type)		(*(type*)vector_add((vector*)v))
#define _vector_insert(v, type)		(*(type)vector_insert((vector*)v))
#define _vector_erase(v, pos, len)	(vector_erase((vector*)v, pos, len))
#define _vector_remove(v)			(vector_remove((vector*)v, pos, 1))

vector vector_create(size_t type);

void vector_free(vector v);

void* vector_add(vector* v);

void* vector_insert(vector* v, size_t pos);

void vector_erase(vector* v, size_t pos, size_t len);

void vector_remove(vector* v, size_t pos);

size_t vector_size(vector v);

size_t vector_get_alloc(vector v);

#endif /* cvec_h */
