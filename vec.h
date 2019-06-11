//
//  vec.h
//
//  Created by Mashpoe on 2/26/19.
//

#ifndef vec_h
#define vec_h

#include <stdlib.h>
#include <stdbool.h>

typedef void* vector;
typedef size_t vec_size; // stores the number of elements
typedef unsigned char vec_type_size; // stores the number of bytes for a type

typedef int*	vec_int;
typedef char*	vec_char;

// shortcut defines
#define _vector_create(type)			(vector_create(sizeof(type)))
// v is a vector* (aka void**)
#define _vector_add(v, type)			(*(type*)vector_add((vector*)v))
#define _vector_insert(v, type, pos)	(*(type)vector_insert((vector*)v, pos))
#define _vector_erase(v, pos, len)		(vector_erase((vector*)v, pos, len))
#define _vector_remove(v, pos)			(vector_remove((vector*)v, pos))

vector vector_create(vec_type_size type_size);

void vector_free(vector v);

void* vector_add(vector* v);

void* vector_insert(vector* v, vec_size pos);

void vector_erase(vector* v, vec_size pos, vec_size len);

void vector_remove(vector* v, vec_size pos);

vec_size vector_size(vector v);

vec_size vector_get_alloc(vector v);

#endif /* vec_h */
