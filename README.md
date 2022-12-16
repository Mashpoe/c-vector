# C Vector Library
A simple vector library for C. This libary's vectors work in a similar manner to C++ vectors: they can store any type, their elements can be accessed via the `[]` operator, and elements may be added or removed with simple library calls.

You can easily create a vector like so:

```c
int* num_vec = vector_create();
```

You can store different data types in vectors by simply modifying the pointer type:

```c
// vector of chars
char* char_vec = vector_create();

// vector of integers
int* int_vec = vector_create();

// you can even store structs!
struct foo* foo_vec = vector_create();
```

A vector's elements can be accessed/modified using the `[]` operator:

```c
char* vec_str = vector_create();

// add an item
vector_add(&vec_str, 'a');

// print the item from the vector
printf("first item: %c\n", vec_str[0]);
```

Vectors require that their pointer type corresponds to the contents of the vector (e.g. `int*` for a vector containing `int`s); otherwise, you have to explicitly cast the vector pointer to the correct type when accessing elements or making library calls:

```c
// recommened vector use
int* foo = vector_create();

vector_add(&foo, 5);

// reassignment
foo[0] = 3;


// generic vector (not recommended)
vector bar = vector_create();

vector_add(&(int*)bar, 3);

((int*)bar)[0] = 5; // we have to cast to an int* so we can properly access
```
Note: the `vector` type is just an alias for `void*` defined in `vec.h`.

# How It Works

These vectors can be manipulated in a similar manner to C++ vectors; their elements can be accessed via the `[]` operator and elements may be added or removed through the use of simple library calls.

This works because these vectors are stored directly alongside special header in memory, which keeps track of the vector's size and capacity:

    +--------+-------------+
    | Header | Vector data |
    +--------+-------------+
             |
             `-> Pointer returned to the user.

This design was inspired by anitrez's [Simple Dynamic Strings](https://github.com/antirez/sds/).

This library uses the preprocessor to perform compile-time type checks, which make sure the pointer type of a vector corresponds to the type of its contents. The type checks are done using GCC's `typeof` operator, which is only implemented in some compilers, such as Clang and GCC, and will not work under the Visual Studio C compiler.

If you're using this library for a C++ project, you can swap each occurrence of `typeof` with `decltype`, which *is* standardized in C++.

# Usage

Just because these vectors can be accessed and modified like regular arrays doesn't mean they should be treated the same in all cases.

Because of the hidden header data, these vectors can only be properly freed by calling `vector_free(vec)`.

Since the header data is stored in the same location as the the vector's elements, the entire vector might be moved to a new location when its size is changed. This means that the library calls that increase the vector's size need to take the address of the vector pointer, e.g `&vec`, so it can be reassigned. This obviously means that you can't reference the same vector in multiple places without encapsulating it in some kind of structure, which is likely your intention anyway.

Here is an example using a function that reassigns the vector pointer:

```c
int* baz = vector_create();

// adds the number "5" to the vector
vector_add(&baz, 5); // takes the address of the `int*` in case the pointer needs to be changed
```

This is another similarity to the *Simple Dynamic Strings* library, which has function calls that are a little more clear in their reassignment, e.g. `s = sdscat(s,"foo")`, but this functionality would break the `vector_add` and `vector_insert` macros because they already expand to an assignment expression. The method these macros use does have the advantage of producing a compiler error when you omit the `&` (unless you're using the Visual Studio compiler), which means you're less likely to get a segfault as it forces reassignment on you.

Some functions never move a vector to a new location, so they just take a regular vector pointer as an argument:

```c
int* age_vec = vector_create();

// get the vector size
int num_ages = vector_size(age_vec); // just pass the `int*` without taking its address

// insert age 18 to the 0 position in a vector
vector_insert(age_vec, 0, 18);
```

# Best Practices

Because of the differences between regular arrays and vectors, it's probably a good idea to try to distinguish them from one another.

One way to do this is to create an alias for various vector types using `typedef`:

```c
typedef float* vec_float; // vector alias for float

vec_float qux = vector_create();
```

`vec.h` already includes an alias for `int*` and `char*` types (`vec_int` and `vec_char` respectively), but you can add as many as you want :)

The *reccomended* way to differentiate between vectors and arrays is to simply name them differently, for example, an array of eggs could be named `eggs` while a vector of eggs could be named `egg_vec`.

# Reference Sheet

Here is a cheat sheet for this library's functions and macros:

Some functions take a normal vector argument, e.g. `vec`, while other functions can change the vector's memory location and thus require the pointer address, e.g. `&vec`. You should get an error if you use the vector address incorrectly.

| Action                                        | Code                                       | Changes vector address? |
|-----------------------------------------------|--------------------------------------------|-------------------------|
| create a vector                               | `type* vec_a = vector_create();`           | N/A                     |
| free a vector                                 | `vector_free(vec_a);`                      | N/A                     |
| add `item` to the vector `vec_a`              | `vector_add(&vec_a, item);`                | yes                     |
| insert `item` into `vec_a` at index `9`       | `vector_insert(&vec_a, 9, item)`           | yes                     |
| erase `4` items from `vec_a` at index `3`     | `vector_erase(vec_a, 3, 4);`               | no (moves elements)     |
| remove item at index `3` from `vec_a`         | `vector_remove(vec_a, 3);`                 | no (moves elements)     |
| get the number of items in `vec_a`            | `int num_items = vector_size(vec_a);`      | no                      |
| get the amount of allocated memory in `vec_a` | `int alloc_amt = vector_get_alloc(vec_a);` | no                      |

# Visual Studio Reference Sheet

Because the Visual Studio C compiler doesn't support the `typeof` operator, which is used for static type checks in the library's macros, you have to use a slightly different set of macros. Unfortunatley, this also means you won't get an error when passing incorrect types to these macros, so if you're getting segfaults, make sure you are properly using `vec` and `&vec` for their corresponding calls.

| Action                                        | Code                                       | Changes vector address? |
|-----------------------------------------------|--------------------------------------------|-------------------------|
| create a vector                               | `type* vec_a = vector_create();`           | N/A                     |
| free a vector                                 | `vector_free(vec_a);`                      | N/A                     |
| add `item` to the vector `vec_a`              | `vector_add(&vec_a, type) = item;`         | yes                     |
| insert `item` into `vec_a` at index `9`       | `vector_insert(&vec_a, type, 9) = item;`   | yes                     |
| erase `4` items from `vec_a` at index `3`     | `vector_erase(vec_a, type, 3, 4);`         | no (moves elements)     |
| remove item at index `3` from `vec_a`         | `vector_remove(vec_a, type, 3);`           | no (moves elements)     |
| get the number of items in `vec_a`            | `int num_items = vector_size(vec_a);`      | no                      |
| get the amount of allocated memory in `vec_a` | `int alloc_amt = vector_get_alloc(vec_a);` | no                      |

# What About Structures?

If you have a vector storing some kind of structure, you can't initialize new elements of the vector like you would a variable, e.g. `{ a, b, c }`. To get around this, there's a set of special macros defined for both the Visual Studio compiler and compilers that support the `typeof` operator. These macros allow you to have more control over how you initialize a new element of a vector.

Here are some examples:

```c
typedef struct { ... } foo;

foo* foo_vec = vector_create();

// the lifetime of temp is not guaranteed to be long; don't use this pointer after instantiation
foo* temp = vector_add_asg(&foo_vec);
temp->a = 1;
temp->b = 2;
temp->c = 3;
temp = NULL; // stop using temp now that the element is initialized
```

Here's a quick cheat sheet for these macros:

| Action                                  | Code                                         |
|-----------------------------------------|----------------------------------------------|
| add `item` to the vector `vec_a`        | `type* temp = vector_add_asg(&vec_a);`       |
| insert `item` into `vec_a` at index `9` | `type* temp = vector_insert_asg(&vec_a, 9);` |

The corresponding Visual Studio macros:

| Action                                  | Code                                               |
|-----------------------------------------|----------------------------------------------------|
| add `item` to the vector `vec_a`        | `type* temp = vector_add_asg(&vec_a, type);`       |
| insert `item` into `vec_a` at index `9` | `type* temp = vector_insert_asg(&vec_a, type, 9);` |
