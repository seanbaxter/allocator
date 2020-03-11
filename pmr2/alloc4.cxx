#include "pmr2.hxx"

struct obj2_t {
  pmr2::vector<int> vec;
};

// obj2_t is allocator automatic because pmr2::vector is AA.
static_assert(__is_allocator_automatic(obj2_t) == true);

int main() {
  logging_resource2_t res("logger1");
  printf("default = %p, res = %p\n", pmr::get_default_resource(), &res);

  obj2_t obj { { 1, 2, 3 } } using res;
  printf("obj.resource = %p\n", obj.get_allocator().resource());

  // Copy/move-alloc ctors only generated for automatic allocator types.
  obj2_t obj2 = { obj } using res;

  // Implicit declaration of get_allocator() in all allocator-automatic
  // types.
  printf("obj.resource = %p\n", obj2.get_allocator().resource());
}