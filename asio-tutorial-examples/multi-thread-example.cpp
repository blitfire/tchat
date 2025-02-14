#include <functional>
#include <iostream>
#include <asio.hpp>
#include <thread>

class Printer {
    asio::strand<asio::io_context::executor_type> strand_;
    asio::steady_timer timer1_;
    asio::steady_timer timer2_;
    int count_;
public:
    explicit Printer(asio::io_context& io) :
        strand_(asio::make_strand(io)),
        timer1_(io, asio::chrono::seconds(1)),
        timer2_(io, asio::chrono::seconds(1)),
        count_(0) { 
        timer1_.async_wait(std::bind(&Printer::print1, this));
        timer2_.async_wait(std::bind(&Printer::print2, this));
    }
    ~Printer() {
        std::cout << "Final count is: " << count_ << std::endl;
    }
    void print1() {
        if (count_ < 10) {
            std::cout << "Timer 1: " << count_ << std::endl;
            ++count_;

            timer1_.expires_at(timer1_.expiry() + asio::chrono::seconds(1));
            timer1_.async_wait(std::bind(&Printer::print1, this));
        }
    }
    void print2() {
        if (count_ < 10) {
            std::cout << "Timer 2: " << count_ << std::endl;
            ++count_;

            timer2_.expires_at(timer2_.expiry() + asio::chrono::seconds(1));
            timer2_.async_wait(std::bind(&Printer::print2, this));
        }
    }

};

int main() {
    asio::io_context io;
    Printer p(io);
    std::thread t ([&]{ io.run(); });
    io.run();
    t.join();

    return 0;
}
