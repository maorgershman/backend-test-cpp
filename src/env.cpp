#include "env.hpp"
#include "../include/laserpants/dotenv/dotenv.h"
#include <cstdlib>

namespace env
{
  EnvData data;

  static std::string getEnvVariable(const std::string &name, bool required = true)
  {
    const auto variable = std::getenv(name.c_str());
    if (variable)
    {
      return variable;
    }

    if (required)
    {
      throw std::runtime_error(std::format("env variable '{}' not found", name));
    }

    return std::string();
  }

  void init()
  {
    dotenv::init();

    data.db.host = getEnvVariable("POSTGRES_HOST");
    data.db.name = getEnvVariable("POSTGRES_DB");
    data.db.user = getEnvVariable("POSTGRES_USER");
    data.db.password = getEnvVariable("POSTGRES_PASSWORD");
    data.db.poolSize = std::stoi(getEnvVariable("POSTGRES_POOL_SIZE"));
  }
}
