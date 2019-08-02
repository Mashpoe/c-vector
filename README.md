# C Vector Library
A simple vector library written in C. It can store any type, it can be accessed like a traditional array, e.g. `arr[i]`, and it has methods for adding, inserting, and removing elements. 

You can easily create a vector like so:

```c
vector foo = vector_create();
```

the `vector` type is actually just an alias for `void*`, but it's better to use a pointer of the type the vector is storing so you don't have to type cast when accessing a vector's elements:

```c
int* bar = vector_create();

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
int* baz = vector_create();

// adds the number "5" to the vector
vector_add(&baz, int) = 5; // takes the address of the `int*` in case the pointer needs to be changed
```

Because of these differences, it's probably a good idea to make a way to distinguish vectors from regular arrays.

One way to do this is to `typedef` an alias for vectors of a specific type like so:

```c
typedef float* float_vec; // vector alias for float

float_vec qux = vector_create();
```

`vec.h` already includes an alias for `int*` and `char*` types (`vec_int` and `vec_char` respectively), but you can add as many as you want :)

Another easy way to differentiate between vectors and arrays is to simply name them differently, for example, an array of eggs could be named `eggs` while a vector of eggs could be named `egg_vec`.

Some functions never move the vector to a new location, so they just take a regular `vector` (or any other `type*`) as an argument:

```c
int* age_vec = vector_create();

// get the vector size
int num_ages = vector_size(age_vec); // just pass the `int*` without taking its address
```

# Reference Sheet

Here is a cheat sheet for using this library:

| Action                                       | Code                                    | Changes memory address? |
|----------------------------------------------|-----------------------------------------|-------------------------|
| create a vector                              |`type* vec_a = vector_create();`         | N/A                     |
| add `item` to the vector `vec_a`             |`vector_add(&vec_a, type) = item;`       | yes                     |
| insert `item` into `vec_a` at index `9`      |`vector_insert(&vec_a, type, 9) = item;` | yes                     |
| erase `4` items from `vec_a` at index `3`    |`vector_erase(&vec_a, type, 3, 4);`      | yes                     |
| remove item at index `3` from `vec_a`        |`vector_remove(&vec_a, type, 3);`        | yes                     |
| get the number of items in `vec_a`           |`int num_items = vector_size(vec_a);`    | no                      |
| get the amount of allocated memory in `vec_a`|`int alloc_amt = vector_alloc(vec_a);`   | no                      |

# Macros

This library defines some macros in order to simulate the template arguments that make vectors possible in C++. Here is a list of this library's macros:

| Macro                                      | Expands to:                                            |
|--------------------------------------------|--------------------------------------------------------|
|`#define vector_add(v, type)`               |`(*(type*)_vector_add((vector*)v, sizeof(type)))`       |
|`#define vector_insert(v, type, pos)`       |`(*(type)_vector_insert((vector*)v, sizeof(type), pos))`|
|`#define vector_erase(v, type, pos, len)`   |`(_vector_erase((vector*)v, sizeof(type), pos, len))`   |
|`#define vector_remove(v, type, pos)`       |`(_vector_remove((vector*)v, sizeof(type), pos))`       |
