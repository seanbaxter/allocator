#include <vector>
#include "logger.hxx"

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
  logging_resource_t logger("logger");

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
