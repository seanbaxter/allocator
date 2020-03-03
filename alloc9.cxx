#include <list>
#include "logger.hxx"

struct agg_t {
  pmr::list<int> a, b;
};

int main() {
  logging_resource_t logger1("logger1");
  logging_resource_t logger2("logger2");
  
  printf("logger1 = %p\n", &logger1);
  printf("logger2 = %p\n", &logger2);
  printf("default = %p\n", pmr::get_default_resource());

  {
    puts("move-ctor with new allocator:");

    // Create an object with the default memory_resource.
    agg_t agg1 { { 1, 2, 3 } };
    printf("agg1.resource = %p\n", agg1.a.get_allocator().resource());

    // Move-construct it into agg2. Performs deep copy and allocation on 
    // logger1.
    agg_t agg2 = { std::move(agg1) } using logger1;
    printf("agg2.resource = %p\n", agg1.a.get_allocator().resource());
  }

  {
    puts("\nmove-ctor with inherited allocator:");

    // Create an object with the default memory_resource.
    agg_t agg1 { { 1, 2, 3 } } using logger1;
    printf("agg1.resource = %p\n", agg1.a.get_allocator().resource());

    // Move-construct it into agg2. Uses the non-allocator copy ctor.
    agg_t agg2 = { std::move(agg1) };
    printf("agg2.resource = %p\n", agg2.a.get_allocator().resource());
  }

  {
    puts("\nmove-ctor with mismatched allocator:");

    // Create an object with the default memory_resource.
    agg_t agg1 { { 1, 2, 3 } } using logger1;
    printf("agg1.resource = %p\n", agg1.a.get_allocator().resource());

    // Move-construct it into agg2. Uses the non-allocator copy ctor.
    agg_t agg2 = { std::move(agg1) } using logger2;
    printf("agg2.resource = %p\n", agg2.a.get_allocator().resource());
  }
}