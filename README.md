# C Vector Library
A simple vector library written in C. It can store any type, it can be accessed like a traditional array, e.g. `arr[i]`, and it has methods for adding, inserting, and removing elements. 

You can easily create a vector like so:

```c
vector foo = _vector_create(int);
```

the `vector` type is actually just an alias for `void*`, but it's better to use a pointer of the type the vector is storing so you don't have to type cast when accessing a vector's elements:

```c
int* bar = _vector_create(int);

/* imaginary code that adds elements to both vectors */

((int*)foo)[1] = 5; // we have to cast to an int* so we can properly access

bar[2] = 7; // we don't have to cast because b is already an int*
```

This works because these vectors are stored in the same chunk of memory as a header, which contains type information, the amount of elements in the vector, and the amount of memory that's been allocated for the vector:

    +--------+-------------+
    | Header | Vector data |
    +--------+-------------+
             |
             `-> Pointer returned to the user.

This design was inspired by anitrez's [Simple Dynamic Strings](https://github.com/antirez/sds/).


# Usage

Just because these vectors can be accessed and modified like regular arrays doesn't mean they should be treated the same in all cases.

Because of the header data that's obstructed from the user, these vectors can only be properly freed by calling `vector_free(vector)`.

Since the header is stored in the same location as the the vector's elements, the vector might have to move to a new location when its size is changed. This means many of the function calls for this library look like this:

```c
int* baz = _vector_create(int);

// adds the number "5" to the vector
vector_add(&baz) = 5; // takes the address of the `int*` in case the pointer needs to be changed
```

Because of these differences, it's probably a good idea to make a way to distinguish vectors from regular arrays.

One way to do this is to `typedef` an alias for vectors of a specific type like so:

```c
typedef float* float_vec; // vector alias for float

float_vec qux = _vector_create(float);
```

`vec.h` already includes an alias for `int*` and `char*` types (`vec_int` and `vec_char` respectively), but you can add as many as you want :)

Another easy way to differentiate between vectors and arrays is to simply name them differently, for example, an array of eggs could be named `eggs` while a vector of eggs could be named `egg_vec`.

Some functions never move the vector to a new location, so they just take a regular `vector` (or any other `type*`) as an argument:

```c
int* age_vec = _vector_create(int);

// get the vector size
int num_ages = vector_size(age_vec); // just pass the `int*` without taking its address
```

# Reference Sheet

Here is a cheat sheet for using this library:

| Action                                       | Code                                    | Changes memory address? |
|----------------------------------------------|-----------------------------------------|-------------------------|
| create a vector with a type of `type`        |`type* vec_a = _vector_create(type);`    |                         |
| add `item` to the vector `vec_a`             |`_vector_add(&vec_a, type) = item;`      | yes                     |
| insert `item` into `vec_a` at index `9`      |`_vector_insert(&vec_a, type, 9) = item;`| yes                     |
| erase `3` items from `vec_a` at index `4`    |`_vector_erase(&vec_a, 3, 4);`           | yes                     |
| remove item at index `3` from `vec_a`        |`_vector_remove(&vec_a, 3);`             | yes                     |
| get the number of items in `vec_a`           |`int num_items = vector_size(vec_a);`    | no                      |
| get the amount of allocated memory in `vec_a`|`int alloc_amt = vector_alloc(vec_a);`   | no                      |

# Macros

This library defines some macros in order to simulate the template arguments that make vectors possible in C++. All macros have an `_` prefix to distinguish them from their respective regular library functions.

| Macro                                | Expands to:                             |
|--------------------------------------|-----------------------------------------|
|`#define _vector_create(type)`        |`(vector_create(sizeof(type)))`          |
|`#define _vector_add(v, type)`        |`(*(type*)vector_add((vector*)v))`       |
|`#define _vector_insert(v, type, pos)`|`(*(type)vector_insert((vector*)v, pos))`|
|`#define _vector_erase(v, pos, len)`  |`(vector_erase((vector*)v, pos, len))`   |
|`#define _vector_remove(v, pos)`      |`(vector_remove((vector*)v, pos))`       |
