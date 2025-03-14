#include <memory>
#include <string>
#include <iostream>
#include <asio.hpp>

using asio::ip::tcp;
typedef asio::strand<asio::io_context::executor_type> out_strand;

class tcp_connection : public std::enable_shared_from_this<tcp_connection> {
    tcp::socket socket_;
    asio::streambuf buffer_;

    explicit tcp_connection(tcp::socket socket) : 
        socket_(std::move(socket)) {}

    void read() {
        asio::async_read_until(socket_, buffer_, '\n',
            [self = shared_from_this()](std::error_code ec, std::size_t length) {
                if (!ec) {
                    std::istream is(&self->buffer_);
                    std::string message;
                    std::getline(is, message);
                    std::cout << "Message received: " << message << std::endl;
                    self->respond();
                }
            });
    }

    void respond() {
        asio::async_write(socket_, asio::buffer("\n"),
            [self = shared_from_this()](std::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    self->read();  // Continue reading
                }
            });
    }

public:
    typedef std::shared_ptr<tcp_connection> pointer;

    static pointer create(tcp::socket socket) {
        return pointer(new tcp_connection(std::move(socket)));
    }

    tcp::socket& socket() { return socket_; }

    void start() {
        respond();
    }
};

class tcp_server {
    tcp::acceptor acceptor_;

    void start_accept() {
        acceptor_.async_accept([this](std::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::cout << "Connection made successfully" << std::endl;
                tcp_connection::pointer connection(tcp_connection::create(std::move(socket)));
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
