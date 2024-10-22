#include "database.hpp"
#include "env.hpp"
#include <format>
#include <iostream>

namespace database
{
  Pool &getPool()
  {
    static Pool pool(std::format("host={} dbname={} user={} password={}",
                                 env::data.db.host,
                                 env::data.db.name,
                                 env::data.db.user,
                                 env::data.db.password),
                     env::data.db.poolSize);
    return pool;
  }

  //
  // Pool
  //

  Pool::Pool(const std::string &connectionString, int size) : connectionString(connectionString), maxSize(size)
  {
    // Pre-create all connections to fill the pool
    for (int i = 0; i < size; i++)
    {
      auto conn = std::make_shared<pqxx::connection>(connectionString);
      if (!conn->is_open())
      {
        throw std::runtime_error("Failed to open database connection");
      }
      connections.push(conn);
    }
  }

  PooledConnection Pool::acquire()
  {
    std::unique_lock<std::mutex> lock(this->mtx);

    // Wait until a connection becomes available
    this->cv.wait(lock, [this]()
                  { return !this->connections.empty(); });

    auto conn = this->connections.front();
    this->connections.pop();

    return PooledConnection(conn, this);
  }

  void Pool::release(std::shared_ptr<pqxx::connection> conn)
  {
    std::lock_guard<std::mutex> lock(this->mtx);
    this->connections.push(std::move(conn));
    this->cv.notify_one(); // Notify a waiting thread, if any
  }

  //
  // PooledConnection
  //

  PooledConnection::PooledConnection(std::shared_ptr<pqxx::connection> conn, Pool *pool)
      : conn(std::move(conn)), pool(pool)
  {
  }

  PooledConnection::~PooledConnection()
  {
    this->pool->release(this->conn);
  }

  PooledConnection::operator pqxx::connection &()
  {
    return *this->conn;
  }

  pqxx::connection &PooledConnection::operator*()
  {
    return *this->conn;
  }

  pqxx::connection *PooledConnection::operator->()
  {
    return this->conn.get();
  }
}
