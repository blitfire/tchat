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
            std::cout << "Message received: " << message << std::endl;
            self->read();
        });
}
