#include "pmr2.hxx"

struct obj1_t {
  pmr::vector<int> vec;
};

struct obj2_t {
  pmr2::vector<int> vec;
};

@meta printf("__is_allocator_aware(obj1_t) = %d\n", 
  __is_allocator_aware(obj1_t));
@meta printf("__is_automatic_allocator(obj1_t) = %d\n", 
  __is_allocator_automatic(obj1_t));

@meta printf("__is_allocator_aware(obj2_t) = %d\n", 
  __is_allocator_aware(obj2_t));
@meta printf("__is_automatic_allocator(obj2_t) = %d\n", 
  __is_allocator_automatic(obj2_t));


int main() {
  {
    logging_resource_t res1("logger1");
    obj1_t obj { { 1, 2, 3 } } using res1;

    // auto obj2 = { obj } using res1;
  }

  {
    logging_resource2_t res2("logger1");
    obj2_t obj { { 1, 2, 3 } } using res2;

    auto obj2 = { obj } using res2;
  }
}