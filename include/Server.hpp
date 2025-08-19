#include <asio.hpp>
#include "Connection.h"

using asio::ip::tcp;

class Server {
    tcp::acceptor acceptor_;

    void start_accept();

public:
    explicit Server(asio::io_context& io_context) :
        acceptor_(io_context, tcp::endpoint(tcp::v4(), 3000)) {
        
        start_accept();
    }
};
