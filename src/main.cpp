#include "env.hpp"
#include <iostream>

int main()
{
  try {
    env::init();
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
  }
}
