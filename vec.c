//
//  vec.c
//
//  Created by Mashpoe on 2/26/19.
//

#include "vec.h"
#include <string.h>

typedef struct vector_data vector_data;

struct vector_data {
	vec_size alloc;				// stores the number of bytes allocated
	vec_size length;
	char buff[]; // use char to store bytes of an unknown type
};

vector_data* vector_alloc(vec_size alloc, vec_size size) {
	vector_data* v_data = malloc(sizeof(vector_data) + alloc * size);
	v_data->alloc = alloc;
	return v_data;
}

vector_data* vector_get_data(vector v) {
	return &((vector_data*)v)[-1];
}

vector vector_create(void) {
	vector_data* v = malloc(sizeof(vector_data));
	v->alloc = 0;
	v->length = 0;
	
	return &v->buff;
}

void vector_free(vector v) {
	free(vector_get_data(v));
}

vec_size vector_size(vector v) {
	return vector_get_data(v)->length;
}

vec_size vector_get_alloc(vector v) {
	return vector_get_data(v)->alloc;
}

vector_data* vector_realloc(vector_data* v_data, vec_type_size type_size) {
	vec_size new_alloc = (v_data->alloc == 0) ? 1 : v_data->alloc * 2;
	vector_data* new_v_data = realloc(v_data, sizeof(vector_data) + new_alloc * type_size);
	new_v_data->alloc = new_alloc;
	return new_v_data;
}

bool vector_has_space(vector_data* v_data) {
	return v_data->alloc - v_data->length > 0;
}

void* _vector_add(vector* v, vec_type_size type_size) {
	vector_data* v_data = vector_get_data(*v);
	
	if (!vector_has_space(v_data)) {
		v_data = vector_realloc(v_data, type_size);
		*v = v_data->buff;
	}
	
	return (void*)&v_data->buff[type_size * v_data->length++];
}

void* _vector_insert(vector* v, vec_type_size type_size, vec_size pos) {
	vector_data* v_data = vector_get_data(*v);
	
	vec_size new_length = v_data->length + 1;
	
	// make sure there is enough room for the new element
	if (!vector_has_space(v_data)) {
		v_data = vector_realloc(v_data, type_size);
	}
	memmove(&v_data->buff[(pos+1) * type_size],
			&v_data->buff[pos * type_size],
			(v_data->length - pos) * type_size); // move trailing elements
	
	v_data->length = new_length;
	
	return &v_data->buff[pos * type_size];
}

void _vector_erase(vector* v, vec_type_size type_size, vec_size pos, vec_size len) {
	vector_data* v_data = vector_get_data(v);
	// anyone who puts in a bad index can face the consequences on their own
	memmove(&v_data->buff[pos * type_size],
			&v_data->buff[(pos+len) * type_size],
			(v_data->length - pos - len) * type_size);
	
	v_data->length -= len;
}

void _vector_remove(vector* v, vec_type_size type_size, vec_size pos) {
	_vector_erase(v, type_size, pos, 1);
}
