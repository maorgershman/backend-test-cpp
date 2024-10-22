#pragma once

#include <string>

namespace env
{
  struct EnvData
  {
    struct
    {
      std::string name, host, user, password;
      int poolSize;
    } db;
  };

  extern EnvData data;

  void init();
}
