#include "pmr2.hxx"

struct obj1_t {
  pmr::vector<int> vec;
};

int main() {
  logging_resource_t res1("logger1");
  printf("default = %p, res1 = %p\n", pmr::get_default_resource(), &res1);

  // Aggregate initializers are always allocator-automatic.
  obj1_t obj { { 1, 2, 3 } } using res1;
  printf("obj.resource = %p\n", obj.vec.get_allocator().resource());

  // Copy/move-alloc ctors only generated for automatic allocator types.
  obj1_t obj2 = { obj } using res1;
  printf("obj.resource = %p\n", obj2.vec.get_allocator().resource());
}