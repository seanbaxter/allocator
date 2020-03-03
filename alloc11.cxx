#include <cstdio>
#include <list>
#include "logger.hxx"

struct object_t {
  int x, y, z;
  pmr::list<int> a, b, c;
  std::list<int> d;

  typedef pmr::polymorphic_allocator<int> allocator_type;

  // By omission, a is implicitly default-constructed with alloc-ctor.
  // b is explicitly constructed using its default constructor.
  object_t(int x, int y, int z, 
    const allocator_type& alloc = allocator_type()) :
    x(x), 
    y(y), 
    z(z), 
    a{10, 11, 12} using(alloc),
    b{20, 21, 22},
    d{30, 31, 32}
    // Implicit alloc-ctor initialization of c.
    { 

    printf("alloc ctor called: %p\n", alloc.resource());
  }
};

int main() {
  logging_resource_t resource("resource");
  printf("default = %p\n", pmr::get_default_resource());
  printf("resource = %p\n", &resource);

  object_t obj { 1, 2, 3 } using resource;
  printf("obj.a.alloc = %p\n", obj.a.get_allocator().resource());
  printf("obj.b.alloc = %p\n", obj.b.get_allocator().resource());
  printf("obj.c.alloc = %p\n", obj.c.get_allocator().resource());
}