#include <vector>
#include "logger.hxx"

struct obj_t {
  static logging_resource_t logger;

  pmr::vector<int> a = { 1, 2, 3, 4 } using(logger);
  pmr::vector<int> b = { 5, 6, 7, 8, 9 } using(logger);
};

logging_resource_t obj_t::logger { };

int main() {
  logging_resource_t logger_local;

  obj_t obj {
    // Initialize obj_t::a explicitly, and select a resource.
    std::vector { 100, 200, 300, 400 } using logger_local

    // By omission, select the member-initializer for obj_t::b.
  };
  printf("%d ", obj.a[:])...; printf("\n");
  printf("%d ", obj.b[:])...; printf("\n");

  printf("&logger_local: %p\n", &logger_local);
  printf("obj.a's resource: %p\n", obj.a.get_allocator().resource());

  printf("&obj_t::logger: %p\n", &obj_t::logger);
  printf("obj.b's resource: %p\n", obj.b.get_allocator().resource());
}