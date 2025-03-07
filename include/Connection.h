#ifndef INCLUDE_CONNECTION
#define INCLUDE_CONNECTION

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <string_view>
#include <thread>
#include <asio.hpp>

using asio::ip::tcp;

class Connection : public std::enable_shared_from_this<Connection> {
    asio::io_context& context_;
    asio::strand<asio::io_context::executor_type> buffer_strand_;
    std::stringstream front_buffer_;
    std::string back_buffer_;
    
    tcp::socket socket_;
    
    explicit Connection(asio::io_context& context);
    void handle_write(const std::error_code & /*error*/, size_t /*bytes transfered*/) {}
    // Wrapper for async_read_some
    void init_read();
    void handle_read(const asio::error_code& error);

public:
    typedef std::shared_ptr<Connection> pointer;

    static pointer create(asio::io_context& context) {
        return pointer(new Connection(context));
    }

    tcp::socket& socket() const { return socket_; }
    
    void write(std::string_view message) const;
    std::string read(); 

};

#endif // INCLUDE_CONNECTION

