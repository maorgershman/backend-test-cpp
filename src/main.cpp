#include "env.hpp"
#include "database.hpp"
#include "server.hpp"
#include <iostream>
#include <chrono>

int main()
{
  try
  {
    env::init();

    const auto handler = [&](httplib::Server &server)
    {
      server.Get("/", [](const httplib::Request &req, httplib::Response &res)
                 { return res.set_content("Hello World", "text/plain"); });
    };

    Server httpsServer(handler, env::data.server.httpsPort);
    httpsServer.run();

    // Keep the main thread busy
    while (true)
    {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }
  catch (const std::exception &ex)
  {
    std::cerr << ex.what() << std::endl;
  }
}
