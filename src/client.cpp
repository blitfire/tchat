#include <string>
#include <iostream>
#include <asio.hpp>

using asio::ip::tcp;

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: client <host> <message>" << std::endl;
            return -1;
        }
        asio::io_context io_context;
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve(argv[1], "3000");

        tcp::socket socket(io_context);
        asio::connect(socket, endpoints);
        
        asio::write(socket, asio::buffer(argv[2], std::strlen(argv[2])));

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
