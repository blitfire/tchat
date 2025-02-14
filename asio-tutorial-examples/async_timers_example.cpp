#include <functional>
#include <iostream>
#include <asio.hpp>

class Printer {
    asio::steady_timer timer_;
    int count_;
public:
    Printer(asio::io_context& io) : timer_(io, asio::chrono::seconds(1)), count_(0) {
        timer_.async_wait(std::bind(&Printer::print, this));
    }
    ~Printer() {
        std::cout << "Final count is: " << count_ << std::endl;
    }
    void print() {
        if (count_ < 5) {
            std::cout << count_ << std::endl;
            ++count_;

            timer_.expires_at(timer_.expiry() + asio::chrono::seconds(1));
            timer_.async_wait(std::bind(&Printer::print, this));
        }
    }
};

int main() {
    asio::io_context io;
    Printer p(io);
    io.run();

    return 0;
}
