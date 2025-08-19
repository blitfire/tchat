#include "Server.h"

void Server::start_accept() {
    acceptor_.async_accept([this](std::error_code ec, tcp::socket socket) {
        if (!ec) {
            Connection::pointer connection(Connection::create(std::move(socket)));
            connection->start();
        }
        start_accept();
    });
}

