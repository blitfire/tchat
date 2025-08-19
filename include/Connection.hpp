#ifndef INCLUDE_CONNECTION
#define INCLUDE_CONNECTION

#include <memory>
#include <asio.hpp>
#include <sstream>

using asio::ip::tcp;

class Connection : public std::enable_shared_from_this<Connection> {
    tcp::socket socket_;
    asio::streambuf buffer_;
    std::stringstream inStream_;

    explicit Connection(tcp::socket socket) : socket_(std::move(socket)) {}

    void read();
    void write();

public:
    typedef std::shared_ptr<Connection> pointer;

    static pointer create(tcp::socket socket) {
        return pointer(new Connection(std::move(socket)));
    }

    tcp::socket& socket() { return socket_; }

    void start() { read(); }
};

#endif // INCLUDE_CONNECTION
