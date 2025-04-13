#include <memory>
#include <string>
#include <iostream>
#include <asio.hpp>
#include "Connection.h"

using asio::ip::tcp;

class tcp_server {
    tcp::acceptor acceptor_;

    void start_accept() {
        acceptor_.async_accept([this](std::error_code ec, tcp::socket socket) {
            if (!ec) {
                Connection::pointer connection(Connection::create(std::move(socket)));
                connection->start();
            }
            start_accept();
        });
    }

public:
    explicit tcp_server(asio::io_context& io_context) :
        acceptor_(io_context, tcp::endpoint(tcp::v4(), 3000)) {
        
        start_accept();
    }
};

int main() {
    try {
        asio::io_context io_context;
        tcp_server server(io_context);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
