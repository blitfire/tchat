#include <ctime>
#include <iostream>
#include <string>
#include <asio.hpp>

using asio::ip::tcp;

std::string make_daytime_string() {
    std::time_t now = time(0);
    return ctime(&now);
}

int main() {
    try {
        asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13));

        for (;;) {
            tcp::socket socket (io_context);
            acceptor.accept(socket);
            
            std::string message;
            std::getline(std::cin, message);

            std::error_code ignored_error;
            asio::write(socket, asio::buffer(message), ignored_error);
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
