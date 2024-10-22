#include "server.hpp"
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <thread>

Server::Server(std::function<void(httplib::Server &)> handler, int port, std::optional<TLSOptions> tlsOptions)
    : port(port), isHttps(tlsOptions.has_value()), protocol(this->isHttps ? "HTTPS" : "HTTP")
{
  std::cout << "Initializing " << this->protocol << " server on port " << port << "..." << std::endl;

  if (isHttps)
  {
    auto [cert, key] = this->convert(tlsOptions.value());
    this->server = std::make_unique<httplib::SSLServer>(cert, key);
  }
  else
  {
    this->server = std::make_unique<httplib::Server>();
  }

  handler(*this->server);

  std::cout << this->protocol << " server has been initialized" << std::endl;
}

void Server::run()
{
  std::cout << "Attempting to run " << this->protocol << " server on port " << port << "..." << std::endl;
  if (!this->server->bind_to_port("0.0.0.0", this->port))
  {
    throw std::runtime_error("Failed to bind to port, maybe port in use or lacking permissions?");
  }

  std::thread([](std::unique_ptr<httplib::Server> server) 
  {
    if (!server->listen_after_bind())
    {
      throw std::runtime_error("Failed to listen");
    }
  }, std::move(this->server)).detach();

  std::cout << this->protocol << " server is listening on port " << port << std::endl;
}

Server::TLSOptionsOpenSSLFormat Server::convert(TLSOptions tlsOptions)
{
  const auto [certStr, keyStr] = tlsOptions;

  int ret = 0;
  X509 *cert = nullptr;
  EVP_PKEY *key = nullptr;

  auto bio = BIO_new(BIO_s_mem());
  if (bio == nullptr)
  {
    throw std::bad_alloc();
  }

  // Write the certificate
  ret = BIO_puts(bio, certStr.c_str());
  if (ret <= 0)
  {
    BIO_free_all(bio);
    throw std::invalid_argument("Invalid certificate");
  }

  cert = PEM_read_bio_X509(bio, nullptr, nullptr, nullptr);
  if (cert == nullptr)
  {
    BIO_free_all(bio);
    throw std::invalid_argument("Can't parse the certificate");
  }

  // Reset
  BIO_reset(bio);

  // Write the key
  ret = BIO_puts(bio, keyStr.c_str());
  if (ret <= 0)
  {
    X509_free(cert);
    BIO_free_all(bio);
    throw std::invalid_argument("Invalid key");
  }

  key = PEM_read_bio_PrivateKey(bio, nullptr, nullptr, nullptr);
  if (!key)
  {
    X509_free(cert);
    BIO_free_all(bio);
    throw std::invalid_argument("Can't parse the private key");
  }

  // Cleanup
  BIO_free_all(bio);

  return {cert, key};
}
