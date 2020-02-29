# allocator-specifier

[Circle build 96](https://www.circle-lang.org/) introduces the _allocator-specifier_ for easing the use of allocator-aware containers. 

The _allocator-specifier_ applies in these contexts:

1. Braced-initializers in declarations or postfix expressions.
2. Class template deduction.
3. List comprehensions.
4. Array initializers.
5. Aggregate class initializers.
6. Constructor subobject initializers.
7. Member default initializers.

## 1. Braced-initializers in declarations or postfix expressions.

[**alloc1.cxx**](alloc1.cxx)
```cpp
#include <list>
#include "logger.hxx"

int main() {
  logging_resource_t logger;

  // using-allocator in a braced initializer for a declaration.
  // creates a PMR list when the allocator expression derives memory_resource.
  pmr::list<int> my_list { 1, 2, 3 } using logger;

  // using-allocator in a braced initializer on an expression.
  auto my_list2 = pmr::list<int> { 4.4, 5.5, 6.6 } using logger;
}
```

Use the _allocator-specifier_ immediately after the braces in an declaration or postfix expression initializer. This transforms the initializer to the equivalent of `{{1, 2, 3}, &logger}`. An allocator, rather than a `memory_resource` may also be provided to the _allocator-specifier_, in which case it is passed to the constructor directly, rather than passed by address.

## 2. Class template deduction.

[**alloc2.cxx**](alloc2.cxx)
```cpp
#include <list>
#include "logger.hxx"

int main() {
  logging_resource_t logger;

  {
    // using-allocator in a braced initializer for a declaration.
    // creates a PMR list when the allocator expression derives memory_resource.
    std::list my_list { 1, 2, 3 } using logger;
    puts(@type_string(decltype(my_list)));
  }

  {
    // using-allocator in a braced initializer on an expression.
    auto my_list2 = std::list { 4.4, 5.5, 6.6 } using logger;
    puts(@type_string(decltype(my_list2)));
  }

  {
    // Static pack expansion in braced-initializer.
    const int terms[] { 1, 2, 3, 4, 5 };
    std::list my_list3 { terms...[:] ..., terms...[::-1] ...} using logger;
    puts(@type_string(decltype(my_list3)));
    printf("%d ", my_list3[:])...; printf("\n");
  }
}
```

Use the class template name to deduce its arguments. Do not use the alias template in the `std::pmr` namespace, as deduction doesn't work on alias templates. When the argument to the _allocator-specifier_ is a class object deriving `memory_resource`, the `std::pmr::polymorphic_allocator` type is selected. The template argument for the PMR allocator matches that deduced from the arguments in the provided `std::initializer_list` braces.

## 3. List comprehensions.

[**alloc3.cxx**](alloc3.cxx)
```cpp
#include <vector>
#include <set>
#include <cstdlib>
#include "logger.hxx"

int main() {
  logging_resource_t logger;

  std::string s = "Hello World A Set Of Chars";
  std::set unique_chars = [(char)tolower(s[:])...] using logger;
  printf("%c ", unique_chars[:])...; printf("\n");
}
```
```
$ circle alloc3.cxx
$ ./alloc3
alloc 26 bytes at 0xb2dea0
alloc 40 bytes at 0xb2e2e0
alloc 40 bytes at 0xb2e310
alloc 40 bytes at 0xb2e340
alloc 40 bytes at 0xb2e370
alloc 40 bytes at 0xb2e3a0
alloc 40 bytes at 0xb2e3d0
alloc 40 bytes at 0xb2e400
alloc 40 bytes at 0xb2e430
alloc 40 bytes at 0xb2e460
alloc 40 bytes at 0xb2e490
alloc 40 bytes at 0xb2e4c0
alloc 40 bytes at 0xb2e4f0
alloc 40 bytes at 0xb2e520
dealloc 26 bytes at 0xb2dea0
  a c d e f h l o r s t w 
dealloc 40 bytes at 0xb2e3d0
dealloc 40 bytes at 0xb2e4c0
dealloc 40 bytes at 0xb2e490
dealloc 40 bytes at 0xb2e400
dealloc 40 bytes at 0xb2e370
dealloc 40 bytes at 0xb2e340
dealloc 40 bytes at 0xb2e2e0
dealloc 40 bytes at 0xb2e4f0
dealloc 40 bytes at 0xb2e310
dealloc 40 bytes at 0xb2e430
dealloc 40 bytes at 0xb2e520
dealloc 40 bytes at 0xb2e460
dealloc 40 bytes at 0xb2e3a0
```

The 

## Array initializers.

[**alloc4.cxx**](alloc4.cxx)
```cpp
#include <vector>
#include <cstdio>
#include "logger.hxx"

int main() {
  logging_resource_t logger;

  {  
    // Constructs all 4 array elements with the explicit allocator ctor.
    printf("One-dimensional array:\n");
    pmr::vector<int> vecs[4] { } using logger;
    vecs[1].push_back(@range(10))...;
  }

  {
    // Also works on multi-dimensional arrays.
    printf("\nThree-dimensional array:\n");
    pmr::vector<int> vecs[2][3][4] { } using logger;
    vecs[1][2][3].push_back(@range(10))...;
  }
}
```
```
$ circle alloc4.cxx
$ ./alloc4
One-dimensional array:
alloc 4 bytes at 0x1077280
alloc 8 bytes at 0x10772a0
dealloc 4 bytes at 0x1077280
alloc 16 bytes at 0x1077280
dealloc 8 bytes at 0x10772a0
alloc 32 bytes at 0x10772c0
dealloc 16 bytes at 0x1077280
alloc 64 bytes at 0x10772f0
dealloc 32 bytes at 0x10772c0
dealloc 64 bytes at 0x10772f0

Three-dimensional array:
alloc 4 bytes at 0x1077280
alloc 8 bytes at 0x10772a0
dealloc 4 bytes at 0x1077280
alloc 16 bytes at 0x1077280
dealloc 8 bytes at 0x10772a0
alloc 32 bytes at 0x10772c0
dealloc 16 bytes at 0x1077280
alloc 64 bytes at 0x10772f0
dealloc 32 bytes at 0x10772c0
dealloc 64 bytes at 0x10772f0
```

The _allocator-specifier_ supports deep initialization of array declarations of any dimension. Normally when no initializer is provided for an element, the default constructor/zero initializer is selected for that element. When an _allocator-specifier_ is provided, the allocator argument is passed to the one-parameter explicit constructor. When an initializer is provided, the allocator is appended as an additional argument to the constructor.

## Aggregate class initializers.

[**alloc5.cxx**](alloc5.cxx)
```cpp
struct vec3_t {
  double x, y, z;
};

struct foo_t {
  pmr::vector<int> vec;
  pmr::list<double> list;
  vec3_t xyz;
  pmr::set<pmr::string> set[3];
};

int main() {
  logging_resource_t logger;

  foo_t obj {
    // Initialize the vector
    { 5, 6, 7, },

    // Initialize the list.
    { 1.1, 2.2, 3.3 },

    // Initialize the vec3_t.
    { 101, 202, 303 },

    // Initialize the pmr::set<pmr::string> set[3] array.
    // The allocator is passed to the ctor for all 3 set objects.
    // It's also passed to the pmr::string ctors that construct strings
    // from the literals.
    { { "apple", "a string that's 25 bytes", "apple", "orange" }, { }, { } }

  } using logger;

  puts(obj.set[0][:].c_str())...;
}
```
```
$ circle alloc5.cxx
$ ./alloc5
alloc 12 bytes at 0x25f7e70
alloc 24 bytes at 0x25f82a0
alloc 24 bytes at 0x25f82c0
alloc 24 bytes at 0x25f82e0
alloc 72 bytes at 0x25f8330
alloc 72 bytes at 0x25f8380
alloc 25 bytes at 0x25f83d0
alloc 72 bytes at 0x25f8400
a string that's 25 bytes
apple
orange
dealloc 72 bytes at 0x25f8400
dealloc 72 bytes at 0x25f8330
dealloc 25 bytes at 0x25f83d0
dealloc 72 bytes at 0x25f8380
dealloc 24 bytes at 0x25f82a0
dealloc 24 bytes at 0x25f82c0
dealloc 24 bytes at 0x25f82e0
dealloc 12 bytes at 0x25f7e70
```

The _allocator-specifier_ also modifies the initialization of aggregate class templates by recursively passing the allocator to the initializer for its data members. Array members are allocator-initialized according to the previous example.

This example uses uniform initialization with an _allocator-specifier_ to call the two-parameter constructors on `std::vector`, `std::list` and `std::set`. The first parameter is `std::initializer_list`. The second parameter is a reference to the allocator, which is a `pmr::polymorphic_allocator` around the address of the provided `pmr::memory_resource`.

Notice that the logging allocator even applies to the `pmr::string` key of the `pmr::set`; the allocator logs a 25 byte allocation for that string's storage. (Short string optimization is used for the other keys.)

# Constructor subobject initializers.

[**alloc6.cxx**](alloc6.cxx)
```cpp
struct foo_t {
  typedef pmr::polymorphic_allocator<int> allocator_type;
  allocator_type get_allocator() const { return a.get_allocator(); }

  // May use direct-initializer for a non-aggregate allocator-aware member.
  // May use allocator-specifier for aggregate/array.
  foo_t(int x, const allocator_type& alloc) :
    a({x, 2 * x, 3 * x}, alloc),                        // Use ctor.
    b{{4, 5, 6}, {7, 8, 9}, {10, 11, 12}} using(alloc), // Use specifier.
    c{100, 200, 300},
    x(x) { }

  pmr::vector<int> a;
  pmr::vector<int> b[3];
  std::vector<double> c;
  int x;
};

int main() {
  logging_resource_t logger;

  {
    // Initialize a non-aggregate object with a ctor.
    printf("One-object initializer:\n");
    foo_t one(1, &logger);
    printf("%d ", one.a[:])...; printf("\n");
  }

  {
    printf("\nArray initializer:\n");
    // Initialize an array with a braced initializer and allocator-specifier.
    foo_t two[3] {
      // Initialize with values of x. The alloc argument is provided through
      // the allocator-specifier.
      2, 3, 4
    } using logger;

    for(foo_t& obj : two) {
      printf("%d ", obj.a[:])...; printf("\n");
    }
  }
}
```

If any constructor is declared for a class, it no longer qualifies as an aggregate type, and aggregate initialization will not be used. In this case, the class's author must provide a means to provide base class and member subobject initializers with a suitable allocator. 

Following the convention of all allocator-aware STL containers, define constructors where the trailing parameter is a reference to the incoming allocator. Use the constructor member initializer list to pass this onto subobject initializers.

When constructing a single non-aggregate object, it might be most convenient to just call the constructor directly and provide the allocator or `pmr::memory_resource`, as is shown with the initializer for `foo_t one`. However when constructing an array of these objects, the _allocator-specifier_ is more concise, as shown by the initializer of `foo_t two[3]`. In this case, the array initializer calls the two-parameter constructor for each of its three subobjects: `foo_t(2, &logger)`, `foo_t(3, &logger)`, and `foo_t(4, &logger)`.

The user-defined constructor is responsible for configuring the object's subobjects. The _allocator-specifier_ syntax has been extended here as well. Write `using(allocator-name)` after braced initializer in the constructor subobject initializer list to pass the allocator to that subobject. This is a more constrained syntax than _allocator-specifier_ in the context of complete object initialization, in which the parenthesis are merely optional. The parenthesis are required due to the deferred parsing of a member function's body until after the _class-specifier_ has been made complete.

Note the use of _allocator-specifier_ in the subobject initializer for the array member `b`. This implicitly appends the allocator as an argument to each call to the `std::initializer_list` constructor of `pmr::vector<int>`.

A final note is that user-defined constructors are required when aggregate initialization would otherwise attempt to initialize an allocator-aware container with the wrong kind of allocator. `std::vector<double> c` is a data member using the default allocator. We'd generate a compiler error when initializing with a `pmr::polymorphic_allocator<double>`. Since `foo_t`'s constructor controls the initialization of all subobjects, it chooses to initialize members `a` and `b` with the PMR allocator, and let the remaining members use their default allocators (or no allocator at all).

# 7. Member default initializers.

[**alloc7.cxx**](alloc7.cxx)
```cpp
struct obj_t {
  static logging_resource_t logger;

  pmr::vector<int> a = { 1, 2, 3, 4 } using(logger);
  pmr::vector<int> b = { 5, 6, 7, 8, 9 } using(logger);
};

logging_resource_t obj_t::logger { };

int main() {
  logging_resource_t logger_local;

  obj_t obj {
    // Initialize obj_t::a explicitly, and select a resource.
    std::vector { 100, 200, 300, 400 } using logger_local

    // By omission, select the member-initializer for obj_t::b.
  };
  printf("%d ", obj.a[:])...; printf("\n");
  printf("%d ", obj.b[:])...; printf("\n");

  printf("&logger_local: %p\n", &logger_local);
  printf("obj.a's resource: %p\n", obj.a.get_allocator().resource());

  printf("&obj_t::logger: %p\n", &obj_t::logger);
  printf("obj.b's resource: %p\n", obj.b.get_allocator().resource());
}
```
```
$ circle alloc7.cxx
$ ./alloc7
alloc 16 bytes at 0x1af2e70
alloc 20 bytes at 0x1af32a0
100 200 300 400 
5 6 7 8 9 
&logger_local: 0x7fff524a0088
obj.a's resource: 0x7fff524a0088
&obj_t::logger: 0x602060
obj.b's resource: 0x602060
dealloc 20 bytes at 0x1af32a0
dealloc 16 bytes at 0x1af2e70
```

Default member initializers are chosen when no member initializer is provided either in a constructor's subobject initializer list or in the uniform initializer for an aggregate class object. _allocator-specifier_ has also been defined in this context.

`obj_t` is an aggregate class type with two non-static data members, each with _allocator-specifiers_ referring to a static data member `pmr::memory_resource`, `obj_t::logger`. The uniform initializer provides an explicit initializer for member `a`, which uses class template deduction, as shown in [alloc2.cxx](alloc2.cxx). That selects the local memory resource. As no initializer for member `b` is provided, the default member initializer is selected, which provides an `std::initializer_list` and sets `b`'s allocator to`&obj_t::logger`. 

