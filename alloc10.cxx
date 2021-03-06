#include <list>
#include "logger.hxx"

struct object_t {
  typedef pmr::polymorphic_allocator<int> allocator_type;

  object_t() = default;
  object_t(const object_t& rhs) = default;
  object_t(object_t&& rhs) = default;

  // Allocator special member functions.
  object_t(const allocator_type& alloc) = default; 
  object_t(const object_t& rhs, const allocator_type& alloc) = default;
  object_t(object_t&& rhs, const allocator_type& alloc) = default;
  allocator_type get_allocator() const = default;

  pmr::list<int> x;
};

int main() {
  logging_resource_t logger1("logger1");
  logging_resource_t logger2("logger2");
  
  // Use default-alloc ctor.
  object_t obj1 { } using logger1;
  obj1.x.push_back(@range(5))...;

  // Use move-alloc ctor.
  object_t obj2 = { std::move(obj1) } using logger2;

  // Print the resource associatied with obj2.
  printf("obj2.resource = %p\n", obj2.get_allocator().resource());
  printf("&logger2 = %p\n", &logger2);
}