#include "env.hpp"
#include "../include/laserpants/dotenv/dotenv.h"
#include <cstdlib>
#include <cassert>
#include <regex>

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

    data.env = getEnvVariable("ENV");
    assert(data.env == "development" || data.env == "production");

    dotenv::init(std::format(".env.{}", data.env).c_str());

    data.db.host = getEnvVariable("POSTGRES_HOST");
    data.db.name = getEnvVariable("POSTGRES_DB");
    data.db.user = getEnvVariable("POSTGRES_USER");
    data.db.password = getEnvVariable("POSTGRES_PASSWORD");
    data.db.poolSize = std::stoi(getEnvVariable("POSTGRES_POOL_SIZE"));

    data.server.httpPort = std::stoi(getEnvVariable("HTTP_PORT"));
    data.server.httpsPort = std::stoi(getEnvVariable("HTTPS_PORT"));
    data.server.tlsCert = std::regex_replace(getEnvVariable("TLS_CERT"), std::regex(R"(\\n)"), "\n");
    data.server.tlsKey = std::regex_replace(getEnvVariable("TLS_KEY"), std::regex(R"(\\n)"), "\n");
  }
}
