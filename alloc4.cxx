#include <vector>
#include <cstdio>
#include "logger.hxx"

int main() {
  logging_resource_t logger;

  {  
    // Constructs all 4 array elements with the explicit allocator ctor.
    printf("One-dimensional array:\n");
    pmr::vector<int> vecs[4] { } using logger;
    vecs[1].push_back(@range(10))...;
  }

  {
    // Also works on multi-dimensional arrays.
    printf("\nThree-dimensional array:\n");
    pmr::vector<int> vecs[2][3][4] { } using logger;
    vecs[1][2][3].push_back(@range(10))...;
  }
}
