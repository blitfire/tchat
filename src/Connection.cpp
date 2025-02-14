#include "Connection.h"

Connection::Connection(asio::io_context& context) :
    context_(context),
    buffer_strand_(asio::make_strand(context)),
    socket_(context) {}

void Connection::init_read() {
    socket.async_read_some(asio::buffer(back_buffer_),
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

void Connection::start() {
    init_read();
    thread_ = std::thread([&]{ context_.run(); });
}

void Connection::read() {
        std::string out {std::move(front_buffer_.str())};
        front_buffer_.str("");
        front_buffer_.clear();
        return out;
}
