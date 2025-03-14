#include <iostream>
#include <asio.hpp>

using asio::ip::tcp;

void wait(tcp::socket& socket);
void send(tcp::socket& socket);

void wait(tcp::socket& socket) {
    auto buffer = std::make_shared<asio::streambuf>();

    asio::async_read_until(socket, *buffer, '\n',
        [&socket, buffer](const std::error_code& ec, std::size_t /*length*/) {
            if (!ec) {
                buffer->consume(buffer->size()); // Discards the data

                send(socket);
            } else {
                std::cerr << "Error while reading: " << ec.message() << std::endl;
            }
        });
}

void send(tcp::socket& socket) {
    std::string message;
    std::cout << "Enter message: ";
    std::getline(std::cin, message);
    if (message.empty()) return;
    message += '\n';
    
    asio::async_write(socket, asio::buffer(message),
            [&](std::error_code ec, size_t /*length*/) {
                if (!ec) {
                    wait(socket);
                };
            });
}

int main() {
    try {
        asio::io_context io_context;
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "3000");

        tcp::socket socket(io_context);
        asio::connect(socket, endpoints);

        wait(socket);

        io_context.run();

        socket.close();
        io_context.stop();

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
