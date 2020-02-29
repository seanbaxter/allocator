// allocator-specifier in initializers and postfix-expressions

#include <list>
#include "logger.hxx"

int main() {
  logging_resource_t logger;

  // using-allocator in a braced initializer for a declaration.
  // creates a PMR list when the allocator expression derives memory_resource.
  pmr::list<int> my_list { 1, 2, 3 } using logger;

  // using-allocator in a braced initializer on an expression.
  auto my_list2 = pmr::list<int> { 4.4, 5.5, 6.6 } using logger;
}
