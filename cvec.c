//
//  cvec.c
//
//  Created by Mashpoe on 2/26/19.
//

#include "cvec.h"
#include <string.h>

typedef struct vector_data vector_data;

struct vector_data {
	size_t size; // size of each element
	size_t alloc; // stores the number of bytes allocated
	size_t length;
	char buff[]; // use char to store bytes of an unknown type
};

vector_data* vector_alloc(size_t alloc, size_t size) {
	vector_data* v_data = malloc(sizeof(vector_data) + alloc * size);
	v_data->alloc = alloc;
	return v_data;
}

vector_data* vector_get_data(vector v) {
	return &((vector_data*)v)[-1];
}

vector vector_create(size_t size) {
	vector_data* v = malloc(sizeof(vector_data));
	v->size = size;
	v->alloc = 0;
	v->length = 0;
	
	return &v->buff;
}

void vector_free(vector v) {
	free(vector_get_data(v));
}

size_t vector_size(vector v) {
	return vector_get_data(v)->length;
}

size_t vector_get_alloc(vector v) {
	return vector_get_data(v)->alloc;
}

vector_data* vector_realloc(vector_data* v_data) {
	size_t new_alloc = (v_data->alloc == 0) ? 1 : v_data->alloc * 2;
	vector_data* new_v_data = realloc(v_data, sizeof(vector_data) + new_alloc * v_data->size);
	new_v_data->alloc = new_alloc;
	return new_v_data;
}

bool vector_has_space(vector_data* v_data) {
	return v_data->alloc - v_data->length > 0;
}

void* vector_add(vector* v) {
	vector_data* v_data = vector_get_data(*v);
	
	if (!vector_has_space(v_data)) {
		v_data = vector_realloc(v_data);
		*v = v_data->buff;
	}
	
	return (void*)&v_data->buff[v_data->size * v_data->length++];
}

void* vector_insert(vector* v, size_t pos) {
	vector_data* v_data = vector_get_data(*v);
	
	size_t new_length = v_data->length + 1;
	
	// make sure there is enough room for the new element
	if (!vector_has_space(v_data)) {
		v_data = vector_realloc(v_data);
	}
	memmove(&v_data->buff[(pos+1) * v_data->size],
			&v_data->buff[pos * v_data->size],
			(v_data->length - pos) * v_data->size); // move trailing elements
	
	v_data->length = new_length;
	
	return &v_data->buff[pos * v_data->size];
}

void vector_erase(vector* v, size_t pos, size_t len) {
	vector_data* v_data = vector_get_data(v);
	// anyone who puts in a bad index can face the consequences on their own
	memmove(&v_data->buff[pos * v_data->size],
			&v_data->buff[(pos+len) * v_data->size],
			(v_data->length - pos) * v_data->size);
	
	v_data->length -= len;
}

void vector_remove(vector* v, size_t pos) {
	vector_erase(v, pos, 1);
}
