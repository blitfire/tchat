#include <iostream>
#include <asio.hpp>

using asio::ip::tcp;

void send(tcp::socket& socket) {
    std::string message;
    std::cout << "Enter message: ";
    std::getline(std::cin, message);
    if (message.empty()) return;
    message += '\n';
    
    asio::async_write(socket, asio::buffer(message),
        [&](std::error_code ec, size_t /*length*/) {
            if (!ec) {
                send(socket);
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

        send(socket);

        io_context.run();

        socket.close();
        io_context.stop();

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
