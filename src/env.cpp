#include "env.hpp"
#include "../include/laserpants/dotenv/dotenv.h"
#include <cstdlib>

namespace env 
{
  EnvData data;

  static std::string getEnvVariable(const std::string& name, bool required = true)
  {
    const auto variable = std::getenv(name.c_str());
    if (variable) {
      return variable;
    }

    if (required) {
      throw std::runtime_error(std::format("env variable '{}' not found", name));
    }

    return std::string();
  }

  void init() 
  {
    dotenv::init();
  }
}
