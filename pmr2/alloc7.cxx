
#include "pmr2.hxx"

namespace std { 
	template<typename type_t> requires __is_allocator_automatic(type_t)
	struct allocator<type_t> : pmr2::polymorphic_allocator<type_t> {
		using pmr2::polymorphic_allocator<type_t>::polymorphic_allocator;

      static constexpr size_t max_size() noexcept { return 1000; }

	};
}

int main() {
	logging_resource2_t logger("yo");
	std::vector<pmr2::string> vec { } using logger;
	vec.push_back("hello aeuaouaou au a aoeuaou auaeou a uua ");
}