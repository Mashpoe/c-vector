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
// v is a vector* (aka void**)
#define vector_add(v, type)				(*(type*)_vector_add((vector*)v, sizeof(type)))
#define vector_insert(v, type, pos)		(*(type)_vector_insert((vector*)v, sizeof(type), pos))
#define vector_erase(v, pos, len)		(_vector_erase((vector*)v, sizeof(type), pos, len))
#define vector_remove(v, pos)			(_vector_remove((vector*)v, sizeof(type), pos))

vector vector_create(void);

void vector_free(vector v);

void* _vector_add(vector* v, vec_type_size type_size);

void* _vector_insert(vector* v, vec_type_size type_size, vec_size pos);

void _vector_erase(vector* v, vec_type_size type_size, vec_size pos, vec_size len);

void _vector_remove(vector* v, vec_type_size type_size, vec_size pos);

vec_size vector_size(vector v);

vec_size vector_get_alloc(vector v);

#endif /* vec_h */
