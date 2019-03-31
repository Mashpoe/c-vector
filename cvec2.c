//
//  cvec.c
//  CHeckScript
//
//  Created by Mashpoe on 2/26/19.
//  Copyright © 2019 Mashpoe. All rights reserved.
//

#include "cvec2.h"
#include <string.h>

typedef struct vector_data vector_data;

struct vector_data {
	size_t size; // size of each element
	size_t alloc; // stores the number of bytes allocated
	size_t length;
	char buff[]; // use char to store bytes of an unknown type
};

vector_data* VECTOR_ALLOC(size_t alloc, size_t size) {
	vector_data* v_data = malloc(sizeof(vector_data) + alloc * size);
	v_data->alloc = alloc;
	return v_data;
}

vector_data* VECTOR_DATA(vector v) {
	return &((vector_data*)v)[-1];
}

vector VECTOR_CREATE(size_t size) {
	vector_data* v = malloc(sizeof(vector_data));
	v->size = size;
	v->alloc = 0;
	v->length = 0;
	
	return &v->buff;
}

void VECTOR_FREE(vector v) {
	free(VECTOR_DATA(v));
}

size_t VECTOR_SIZE(vector v) {
	return VECTOR_DATA(v)->length;
}

size_t VECTOR_GET_ALLOC(vector v) {
	return VECTOR_DATA(v)->alloc;
}

vector_data* VECTOR_REALLOC(vector_data* v_data) {
	if (v_data->alloc == 0) {
		vector_data* new_v_data = VECTOR_ALLOC(1, v_data->size);
		new_v_data->size = v_data->size;
		new_v_data->alloc = 1;
		new_v_data->length = 0;
		return new_v_data;
	} else {
		size_t new_alloc = v_data->alloc * 2;
		vector_data* new_v_data = realloc(v_data, sizeof(vector_data) + new_alloc * v_data->size);
		return new_v_data;
	}
}

bool VECTOR_HAS_SPACE(vector_data* v_data) {
	return v_data->alloc - v_data->length > 0;
}

void* VECTOR_ADD(vector* v) {
	vector_data* v_data = VECTOR_DATA(*v);
	
	if (!VECTOR_HAS_SPACE(v_data)) {
		v_data = VECTOR_REALLOC(v_data);
	}
	
	*v = v_data->buff;
	
	return (void*)&v_data->buff[v_data->size * v_data->length++];
}

void* VECTOR_INSERT(vector* v, size_t pos) {
	vector_data* v_data = VECTOR_DATA(*v);
	
	size_t new_length = v_data->length + 1;
	
	// make sure there is enough room for the new element
	if (!VECTOR_HAS_SPACE(v_data)) {
		v_data = VECTOR_REALLOC(v_data);
	}
	memmove(&v_data->buff[(pos+1) * v_data->size],
			&v_data->buff[pos * v_data->size],
			(v_data->length - pos) * v_data->size); // move trailing elements
	
	v_data->length = new_length;
	
	return &v_data->buff[pos * v_data->size];
}

void VECTOR_ERASE(vector* v, size_t pos, size_t len) {
	vector_data* v_data = VECTOR_DATA(v);
	// anyone who puts in a bad index can face the consequences on their own
	memmove(&v_data->buff[pos * v_data->size],
			&v_data->buff[(pos+len) * v_data->size],
			(v_data->length - pos) * v_data->size);
	
	v_data->length -= len;
}

void VECTOR_REMOVE(vector* v, size_t pos) {
	VECTOR_ERASE(v, pos, 1);
}
