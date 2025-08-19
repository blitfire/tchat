#include "Connection.h"
#include <iostream>
#include <string>

void Connection::read() {
    asio::async_read_until(socket_, buffer_, '\n',
        [self = shared_from_this()](std::error_code ec, std::size_t length) {
            if (ec) {
               return; 
            }
            std::istream is(&self->buffer_);
            std::string message;
            std::getline(is, message);
            self->inStream_ << message;
            self->read();
        });
}

void Connection::write() {
    // std::string message;
    // std::cout << "Enter message: ";
    // std::getline(std::cin, message);
    // if (message.empty()) return;
    // message += '\n';
    
    asio::async_write(socket, asio::buffer(buf),
        [&](std::error_code ec, size_t /*length*/) {
            if (!ec) {
                std::cout << "Message sent successfully" << std::endl;
            };
        });
}

