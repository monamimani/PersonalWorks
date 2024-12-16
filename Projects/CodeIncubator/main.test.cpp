
#include <string>
#include "catch2/catch_session.hpp"



int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
  int result = Catch::Session().run( argc, argv );

  return result;
}
