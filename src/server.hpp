#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <string>
#include <functional>
#include <optional>
#include <openssl/types.h>

class Server
{
  struct TLSOptionsOpenSSLFormat
  {
    X509 *cert;
    EVP_PKEY *key;
  };

  int port = 0;
  std::unique_ptr<httplib::Server> server = nullptr;
  bool isRunning = false;
  
  const bool isHttps;
  const char *const protocol;

public:
  struct TLSOptions
  {
    std::string cert, key;
  };

  Server(std::function<void(httplib::Server &)> handler,
         int port,
         std::optional<TLSOptions> tlsOptions = std::nullopt);

  void run();

private:
  TLSOptionsOpenSSLFormat convert(TLSOptions tlsOptions);
};
