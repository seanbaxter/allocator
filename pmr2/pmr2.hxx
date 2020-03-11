#pragma once

#include <memory_resource>
#include <vector>
#include <list>
#include <string>

namespace std {
namespace pmr2 {

  class memory_resource : public std::pmr::memory_resource { 
    using std::pmr::memory_resource::memory_resource;
  };

  template<typename type_t>
  class polymorphic_allocator : public std::pmr::polymorphic_allocator<type_t> {
    using std::pmr::polymorphic_allocator<type_t>::polymorphic_allocator;

    polymorphic_allocator(const polymorphic_allocator& rhs) :
      pmr::polymorphic_allocator<type_t>(rhs.resource()) { }

    polymorphic_allocator(polymorphic_allocator&& rhs) :
      pmr::polymorphic_allocator<type_t>(rhs.resource()) { }

    enum { allocator_automatic };
  };

  template<typename _Tp>
    using vector = std::vector<_Tp, polymorphic_allocator<_Tp>>;

  template<typename _Tp>
    using list = std::list<_Tp, polymorphic_allocator<_Tp>>;

  template<typename _CharT, typename _Traits = char_traits<_CharT>>
    using basic_string = std::basic_string<_CharT, _Traits,
             polymorphic_allocator<_CharT>>;
  using string    = basic_string<char>;

}
}

namespace pmr = std::pmr;
namespace pmr2 = std::pmr2;

class logging_resource_t : public pmr::memory_resource {
public:
  logging_resource_t(const char* name) : name(name) { }

protected:
  void* do_allocate(size_t bytes, size_t align) override {
    void* p = pmr::get_default_resource()->allocate(bytes, align);
    printf("(pmr1) alloc %d bytes at %p (%s)\n", bytes, p, name.c_str());
    return p;
  }

  void do_deallocate(void* p, size_t bytes, size_t align) override {
    printf("(pmr1) dealloc %d bytes at %p (%s)\n", bytes, p, name.c_str());
    pmr::get_default_resource()->deallocate(p, bytes, align);
  }

  bool do_is_equal(const pmr::memory_resource& rhs) const noexcept override {
    return this == &rhs;
  }

private:
  std::string name;
};

class logging_resource2_t : public pmr2::memory_resource {
public:
  logging_resource2_t(const char* name) : name(name) { }

protected:
  void* do_allocate(size_t bytes, size_t align) override {
    void* p = pmr::get_default_resource()->allocate(bytes, align);
    printf("(pmr2) alloc %d bytes at %p (%s)\n", bytes, p, name.c_str());
    return p;
  }

  void do_deallocate(void* p, size_t bytes, size_t align) override {
    printf("(pmr2) dealloc %d bytes at %p (%s)\n", bytes, p, name.c_str());
    pmr::get_default_resource()->deallocate(p, bytes, align);
  }

  bool do_is_equal(const pmr::memory_resource& rhs) const noexcept override {
    return this == &rhs;
  }

private:
  std::string name;
};

