#pragma once

#include <string>

namespace env
{
  struct EnvData
  {
    std::string env; // development/production

    struct
    {
      std::string name, host, user, password;
      int poolSize;
    } db;

    struct
    {
      int httpPort, httpsPort;
      std::string tlsCert, tlsKey;
    } server;
  };

  extern EnvData data;

  void init();
}
