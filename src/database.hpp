#pragma once

#include <pqxx/pqxx>
#include <string>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

namespace database
{
  // Wrapper around pqxx::connection to automatically release back to the pool
  class PooledConnection
  {
    std::shared_ptr<pqxx::connection> conn;
    class Pool *pool;

  public:
    PooledConnection(std::shared_ptr<pqxx::connection> conn, Pool *pool);
    // On destruction, the connection is returned to the pool
    ~PooledConnection();

    // Provide access to the underlying pqxx::connection
    operator pqxx::connection &();
    pqxx::connection &operator*();
    pqxx::connection *operator->();
  };

  class Pool
  {
    std::string connectionString;
    std::queue<std::shared_ptr<pqxx::connection>> connections;
    std::mutex mtx;
    std::condition_variable cv;
    int maxSize;

  public:
    Pool() = default;
    Pool(const std::string &connectionString, int size);

    // Disable copy and move operations
    Pool(const Pool &) = delete;
    Pool &operator=(const Pool &) = delete;

    PooledConnection acquire();
    void release(std::shared_ptr<pqxx::connection> conn);
  };

  Pool &getPool();
}
