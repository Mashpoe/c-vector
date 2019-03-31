//
//  cvec.h
//  CHeckScript
//
//  Created by Mashpoe on 2/26/19.
//  Copyright © 2019 Mashpoe. All rights reserved.
//

#ifndef cvec_h
#define cvec_h

#include <stdlib.h>
#include <stdbool.h>

typedef void* vector;

typedef int*	vec_int;
typedef char*	vec_char;

// shortcut defines
#define _VECTOR_CREATE(type)		(VECTOR_CREATE(sizeof(type)))
// v is a vector* (aka void**)
#define _VECTOR_ADD(v, type)		(*(type*)VECTOR_ADD((vector*)v))
#define _VECTOR_INSERT(v, type)		(*(type)VECTOR_INSERT((vector*)v))
#define _VECTOR_ERASE(v, pos, len)	(VECTOR_ERASE((vector*)v, pos, len))
#define _VECTOR_REMOVE(v)			(VECTOR_ERASE((vector*)v, pos, 1))

vector VECTOR_CREATE(size_t type);

void VECTOR_FREE(vector v);

void* VECTOR_ADD(vector* v);

void* VECTOR_INSERT(vector* v, size_t pos);

void VECTOR_ERASE(vector* v, size_t pos, size_t len);

void VECTOR_REMOVE(vector* v, size_t pos);

size_t VECTOR_SIZE(vector v);

size_t VECTOR_GET_ALLOC(vector v);

#endif /* cvec_h */
