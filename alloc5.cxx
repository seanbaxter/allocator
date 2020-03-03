#include <vector>
#include <list>
#include <cstdio>
#include <set>
#include <string>
#include "logger.hxx"

struct vec3_t {
  double x, y, z;
};

struct foo_t {
  pmr::vector<int> vec;
  pmr::list<double> list;
  vec3_t xyz;
  pmr::set<pmr::string> set[3];
};

int main() {
  logging_resource_t logger("logger");

  foo_t obj {
    // Initialize the vector
    { 5, 6, 7, },

    // Initialize the list.
    { 1.1, 2.2, 3.3 },

    // Initialize the vec3_t.
    { 101, 202, 303 },

    // Initialize the pmr::set<pmr::string> set[3] array.
    // The allocator is passed to the ctor for all 3 set objects.
    // It's also passed to the pmr::string ctors that construct strings
    // from the literals.
    { { "apple", "a string that's 25 bytes", "apple", "orange" }, { }, { } }

  } using logger;

  puts(obj.set[0][:].c_str())...;
}
