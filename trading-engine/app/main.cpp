#include "trading_engine/order_book.hpp"

#include <iostream>

int main() {
    using namespace trading_engine;

    OrderBook book;
    book.add_limit({1, Side::Buy, 100, 10});
    book.add_limit({2, Side::Buy, 100, 5});
    book.add_limit({3, Side::Sell, 102, 7});

    const auto bid = book.best_bid();
    const auto ask = book.best_ask();
    std::cout << "Best bid: " << bid->price << " ticks, " << bid->total_quantity
              << " units, oldest order " << bid->oldest_order_id << '\n';
    std::cout << "Best ask: " << ask->price << " ticks, " << ask->total_quantity
              << " units\n";
}

