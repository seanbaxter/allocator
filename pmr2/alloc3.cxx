#include "pmr2.hxx"

struct obj1_t {
  pmr::vector<int> vec;

  typedef pmr::polymorphic_allocator<int> allocator_type;

  // Declare a defaulted copy-alloc ctor.
  // During subobject initializer codegen, if the special member function
  // is implicitly declared/defaulted OR the type is allocator-automatic, 
  // it will note the last parameter as an automatic-allocator parameter and 
  // use it on all subobjects.
  obj1_t(const obj1_t& lhs, const allocator_type& alloc) = default;
  allocator_type get_allocator() const = default;
};

int main() {
  logging_resource_t res1("logger1");
  printf("default = %p, res1 = %p\n", pmr::get_default_resource(), &res1);

  // 
  obj1_t obj { { 1, 2, 3 } } using res1;
  printf("obj.resource = %p\n", obj.vec.get_allocator().resource());

  // Copy/move-alloc ctors only generated for automatic allocator types.
  obj1_t obj2 = { obj } using res1;
  printf("obj.resource = %p\n", obj2.get_allocator().resource());
}