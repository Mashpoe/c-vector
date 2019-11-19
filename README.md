# C Vector Library
A simple vector library written in C. It can store any type, it can be accessed like a traditional array, e.g. `arr[i]`, and it has methods for adding, inserting, and removing elements. 

You can easily create a vector like so:

```c
int* num_vec = vector_create();
```

Vectors require that their pointer type corresponds to the contents of the vector; otherwise, you have to explicitly cast it to the correct pointer type (e.g. `int*` for a vector containing `int`s) when accessing elements or calling this library's macros:

```c
// recommened vector use
int* foo = vector_create();

vector_add(&foo, 5);

foo[0] = 3;

// generic vector (not recommended)
vector bar = vector_create();

vector_add(&(int*)bar, 3);

((int*)bar)[0] = 5; // we have to cast to an int* so we can properly access
```
Note: the `vector` type is just an alias for `void*` defined in `vec.h`.

# How It Works

These vectors can be manipulated very simalarly to regular C++ vectors; their elements can be accessed via the `[]` operator and elements can be added or removed with simple library calls.

This works because these vectors are stored in the same chunk of memory as a special header, which keeps track of the number of elements and the amount of allocated memory:

    +--------+-------------+
    | Header | Vector data |
    +--------+-------------+
             |
             `-> Pointer returned to the user.

This design was inspired by anitrez's [Simple Dynamic Strings](https://github.com/antirez/sds/).

# Usage

Just because these vectors can be accessed and modified like regular arrays doesn't mean they should be treated the same in all cases.

Because of the header data that's hidden from the user, these vectors can only be properly freed by calling `vector_free(vec)`.

Since the header data is stored in the same location as the the vector's elements, the entire vector might be moved to a new location when its size is changed. This means that the library calls that increase the vector's size need to take the address of the vector pointer, e.g `&vec`, so it can be reassigned. This obviously means that you can't reference the same vector in multiple places without encapsulating it in some kind of structure, which is likely your intention anyway.

Here is an example using a function that reassigns the vector pointer:

```c
int* baz = vector_create();

// adds the number "5" to the vector
vector_add(&baz, 5); // takes the address of the `int*` in case the pointer needs to be changed
```

This is another similarity to the *Simple Dynamic Strings* library, which has function calls that are a little more clear in their reassignment, e.g. `s = sdscat(s,"foo")`, but this functionality would break the `vector_add` and `vector_insert` macros because they already expand to an assignment expression. The method these macros use does have the advantage of giving a compiler error when you omit the `&` (unless you're using the Visual Studio compiler), which means you're less likely to get a segfault because it forces reassignment on you.

Some functions never move the vector to a new location, so they just take a regular vector as an argument:

```c
int* age_vec = vector_create();

// get the vector size
int num_ages = vector_size(age_vec); // just pass the `int*` without taking its address
```

# Best Practices

Because of their differences, it's probably a good idea to make a way to distinguish vectors from regular arrays.

One way to do this is to `typedef` an alias for vectors of a specific type like so:

```c
typedef float* float_vec; // vector alias for float

float_vec qux = vector_create();
```

`vec.h` already includes an alias for `int*` and `char*` types (`vec_int` and `vec_char` respectively), but you can add as many as you want :)

The reccomended way to differentiate between vectors and arrays is to simply name them differently, for example, an array of eggs could be named `eggs` while a vector of eggs could be named `egg_vec`.

# Reference Sheet

Here is a cheat sheet for using this library:

Some functions take a normal vector argument, e.g. `vec`, while other functions can change the vector's memory location and thus require the pointer address, e.g. `&vec`. You should get an error if you use the vector address incorrectly.

| Action                                       | Code                                    | Changes memory address? |
|----------------------------------------------|-----------------------------------------|-------------------------|
| create a vector                              |`type* vec_a = vector_create();`         | N/A                     |
| add `item` to the vector `vec_a`             |`vector_add(&vec_a, item);`              | yes                     |
| insert `item` into `vec_a` at index `9`      |`vector_insert(&vec_a, 9, item)`         | yes                     |
| erase `4` items from `vec_a` at index `3`    |`vector_erase(vec_a, 3, 4);`             | yes                     |
| remove item at index `3` from `vec_a`        |`vector_remove(vec_a, 3);`               | yes                     |
| get the number of items in `vec_a`           |`int num_items = vector_size(vec_a);`    | no                      |
| get the amount of allocated memory in `vec_a`|`int alloc_amt = vector_alloc(vec_a);`   | no                      |

# Visual Studio Reference Sheet

Because the Visual Studio C compiler doesn't support the `typeof` operator, which is used for static type comparisons in the library's macros, you have to use a slightly different set of macros. Unfortunatley, this also means you won't get an error when passing incorrect types to these macros, so if you are getting segfaults, make sure you are properly using `vec` and `&vec` for their corresponding calls.

| Action                                       | Code                                    | Changes memory address? |
|----------------------------------------------|-----------------------------------------|-------------------------|
| create a vector                              |`type* vec_a = vector_create();`         | N/A                     |
| add `item` to the vector `vec_a`             |`vector_add(&vec_a, type) = item;`       | yes                     |
| insert `item` into `vec_a` at index `9`      |`vector_insert(&vec_a, type, 9) = item;` | yes                     |
| erase `4` items from `vec_a` at index `3`    |`vector_erase(vec_a, type, 3, 4);`       | yes                     |
| remove item at index `3` from `vec_a`        |`vector_remove(vec_a, type, 3);`         | yes                     |
| get the number of items in `vec_a`           |`int num_items = vector_size(vec_a);`    | no                      |
| get the amount of allocated memory in `vec_a`|`int alloc_amt = vector_alloc(vec_a);`   | no                      |

# What About Data Structures?

As you know, if you have a vector storing some kind of data structure, you can't initialize new elements of the vector in the traditional way, e.g. `{ a, b, c }`. To get around this, there's a set of special macros defined for both the Visual Studio compiler and compilers that support the `typeof` operator. These macros allow you to have more control over how you initialize a new element of a vector.

Here are some examples:

```c
struct { ... } foo;

foo* foo_vec = vector_create();

// the lifetime of temp is not guaranteed to be long, don't use this outside of initialization
foo* temp = vector_add_asg(&foo_vec);
temp->a = 1;
temp->b = 2;
temp->c = 3;
temp = NULL; // stop using temp now that the element is initialized
```

Here's a quick cheat sheet for these macros:

| Action                                  | Code                                        |
|-----------------------------------------|---------------------------------------------|
| add `item` to the vector `vec_a`        |`type* temp = vector_add_asg(&vec_a);`       |
| insert `item` into `vec_a` at index `9` |`type* temp = vector_insert_asg(&vec_a, 9);` |

The corresponding Visual Studion macros:

| Action                                  | Code                                              |
|-----------------------------------------|---------------------------------------------------|
| add `item` to the vector `vec_a`        |`type* temp = vector_add_asg(&vec_a, type);`       |
| insert `item` into `vec_a` at index `9` |`type* temp = vector_insert_asg(&vec_a, type, 9);` |
