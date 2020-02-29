// allocator-specifier in class template deduction

#include <list>
#include "logger.hxx"

int main() {
  logging_resource_t logger;

  {
    // using-allocator in a braced initializer for a declaration.
    // creates a PMR list when the allocator expression derives memory_resource.
    std::list my_list { 1, 2, 3 } using logger;
    puts(@type_string(decltype(my_list)));
  }

  {
    // using-allocator in a braced initializer on an expression.
    auto my_list2 = std::list { 4.4, 5.5, 6.6 } using logger;
    puts(@type_string(decltype(my_list2)));
  }

  {
    // Static pack expansion in braced-initializer.
    const int terms[] { 1, 2, 3, 4, 5 };
    std::list my_list3 { terms...[:] ..., terms...[::-1] ...} using logger;
    puts(@type_string(decltype(my_list3)));
    printf("%d ", my_list3[:])...; printf("\n");
  }
}
