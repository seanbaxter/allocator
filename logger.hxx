#pragma once
#include <memory_resource>

namespace pmr = std::pmr;

class logging_resource_t : public pmr::memory_resource {
public:
  logging_resource_t(const char* name) : name(name) { }

protected:
  void* do_allocate(size_t bytes, size_t align) override {
    void* p = pmr::get_default_resource()->allocate(bytes, align);
    printf("alloc %d bytes at %p (%s)\n", bytes, p, name.c_str());
    return p;
  }

  void do_deallocate(void* p, size_t bytes, size_t align) override {
    printf("dealloc %d bytes at %p (%s)\n", bytes, p, name.c_str());
    pmr::get_default_resource()->deallocate(p, bytes, align);
  }

  bool do_is_equal(const pmr::memory_resource& rhs) const noexcept override {
    return this == &rhs;
  }

private:
  std::string name;
};

