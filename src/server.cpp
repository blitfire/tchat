#include <ctime>
#include <iostream>
#include <array>
#include <asio.hpp>

using asio::ip::tcp;

int main() {
    try {
        asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 3000));

        for (;;) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            
            while (true) {
                std::error_code err;
                std::array<char, 128> buf;
                size_t len = socket.read_some(asio::buffer(buf), err);

                if (err == asio::error::eof)
                    break;
                else if (err)
                    throw std::system_error(err);

                std::cout << buf.data() << std::endl;
                std::cout.flush();
            }
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
