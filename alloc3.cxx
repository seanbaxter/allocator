// list comprehension and decay.

#include <vector>
#include <set>
#include <cstdlib>
#include "logger.hxx"

int main() {
  logging_resource_t logger;

  std::string s = "Hello World A Set Of Chars";
  std::set unique_chars = [(char)tolower(s[:])...] using logger;
  printf("%c ", unique_chars[:])...; printf("\n");
}
