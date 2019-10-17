#include <cute.h>
#include <cute_runner.h>
#include <tap_listener.h>
#include <xml_listener.h>

#include <cstdlib>
#include <string>
#include <utility>

std::pair<std::string, cute::suite> get_suite();

int main(int argc, char ** argv)
{
  auto xml_file = cute::xml_file_opener{argc, argv};
  auto listener = cute::xml_listener<cute::tap_listener<>>{xml_file.out};
  auto runner = cute::makeRunner(listener, argc, argv);
  auto [name, tests] = get_suite();

  return runner(tests, name.c_str()) ? EXIT_SUCCESS : EXIT_FAILURE;
}