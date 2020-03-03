#include <list>
#include <string>
#include <array>
#include <iostream>
#include "logger.hxx"

struct agg_t {
  pmr::list<int> a;
  pmr::list<float> b;
};

// Implicit allocator_type declaration uses the allocator_type of the
// first subobject.
@meta printf("agg_t::allocator_type = %s\n", 
  @type_string(agg_t::allocator_type, true));

@meta printf("__is_allocator_aware(std::array<int, 5>) = %d\n", 
  __is_allocator_aware(std::array<int, 5>));

@meta printf("__is_allocator_aware(std::string) = %d\n", 
  __is_allocator_aware(std::string));

@meta printf("__is_allocator_aware(double) = %d\n", 
  __is_allocator_aware(double));

@meta printf("__is_allocator_aware(agg_t) = %d\n", 
  __is_allocator_aware(agg_t));

int main() {
  logging_resource_t logger("logger");

  printf("logger = %p\n", &logger);
  printf("default = %p\n", pmr::get_default_resource());

  // Create an object with the default memory_resource.
  agg_t agg1 { { 1, 2, 3 } };
  printf("agg1.resource = %p\n", agg1.a.get_allocator().resource());

  // Copy-construct it into an aggregate with the logger.
  // This uses the implicitly copy-alloc ctor.
  agg_t agg2 = { agg1 } using logger;
  printf("agg2.resource = %p\n", agg2.a.get_allocator().resource());
}