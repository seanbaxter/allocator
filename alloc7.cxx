#include <vector>
#include "logger.hxx"

struct obj_t {
  static logging_resource_t logger;

  pmr::vector<int> a = { 1, 2, 3, 4 } using(logger);
  pmr::vector<int> b = { 5, 6, 7, 8, 9 } using(logger);
};

logging_resource_t obj_t::logger("obj_t::logger");

int main() {
  logging_resource_t logger_local("logger_local");

  obj_t obj {
    // Initialize obj_t::a explicitly, and select a resource.
    std::vector { 100, 200, 300, 400 } using logger_local

    // By omission, select the member-initializer for obj_t::b.
  };
  printf("%d ", obj.a[:])...; printf("\n");
  printf("%d ", obj.b[:])...; printf("\n");
}