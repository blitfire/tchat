#include "../include/Connection.h"
#include <system_error>

Connection::Connection(asio::io_context& context) :
    context_(context),
    buffer_strand_(asio::make_strand(context)),
    socket_(context) {
    
    init_read();
}

void Connection::init_read() {
    socket_.async_read_some(asio::buffer(back_buffer_),
        std::bind_executor(buffer_strand_, std::bind(handle_read, this, asio::placeholders::error));
}

void Connection::handle_read(const asio::error_code& error) {
   if (!error) {
       front_buffer_ << back_buffer_;
       back_buffer_.clear();
       init_read();
       return;
   }
   throw std::system_error(error);
}

void Connection::handle_write(const asio::error_code& error, size_t /* bytes_transferred */) {
    if (error) {
        throw std::system_error(error);
    }
}

void Connection::write(std::string_view message) {
    asio::async_write(socket_, asio::buffer(message), handle_write);
}

std::string Connection::read() {
    std::string out {std::move(front_buffer_.str())};
    front_buffer_.str("");
    front_buffer_.clear();
    return out;
}
