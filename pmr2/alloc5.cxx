#include "pmr2.hxx"

template<typename vec_t>
struct inner_t {
  vec_t vec;
};

struct outer_t {
  pmr::vector<int> x1;
  pmr2::vector<int> x2;
};

static_assert(__is_allocator_automatic(outer_t) == true);

int main() {
  logging_resource2_t res1("logger1");
  logging_resource2_t res2("logger2");
  printf("res1 = %p\n", &res1);
  printf("res2 = %p\n", &res2);

  // Default works with or without resource.
  outer_t outer1 { { 1, 2, 3 }, { 4, 5, 6 }} using res1;

  // Copy-alloc ctor works on both pmr1 and pmr2. Why?
  // In this hacked example, pmr2 inherits pmr1, so we get extra
  // compatibility.
  outer_t outer2 = { outer1 } using res2;
  printf("outer2.resource = %p\n", outer2.get_allocator().resource());
  printf("outer2.x1.resource = %p\n", outer2.x1.get_allocator().resource());
  printf("outer2.x2.resource = %p\n", outer2.x2.get_allocator().resource());

}