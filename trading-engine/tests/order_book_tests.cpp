#include "trading_engine/order_book.hpp"

#include <stdexcept>

#define CHECK(condition)                                      \
    do {                                                      \
        if (!(condition)) throw std::runtime_error(#condition); \
    } while (false)

int main() {
    using namespace trading_engine;

    OrderBook book;
    CHECK(!book.best_bid());
    CHECK(!book.best_ask());
    CHECK(book.add_limit({1, Side::Buy, 100, 10}) == AddResult::Added);
    CHECK(book.add_limit({2, Side::Buy, 100, 5}) == AddResult::Added);
    CHECK(book.add_limit({3, Side::Buy, 101, 3}) == AddResult::Added);
    CHECK(book.add_limit({4, Side::Sell, 103, 7}) == AddResult::Added);
    CHECK(book.add_limit({1, Side::Sell, 104, 1}) == AddResult::DuplicateId);
    CHECK(book.add_limit({5, Side::Buy, 0, 1}) == AddResult::InvalidPrice);
    CHECK(book.add_limit({5, Side::Buy, 100, 0}) == AddResult::InvalidQuantity);

    CHECK(book.order_count() == 4);
    CHECK(book.best_bid()->price == 101);
    CHECK(book.best_ask()->price == 103);
    CHECK(book.cancel(3));
    CHECK(book.best_bid()->price == 100);
    CHECK(book.best_bid()->total_quantity == 15);
    CHECK(book.best_bid()->order_count == 2);
    CHECK(book.best_bid()->oldest_order_id == 1);
    CHECK(book.cancel(1));
    CHECK(book.best_bid()->oldest_order_id == 2);
    CHECK(book.best_bid()->total_quantity == 5);
    CHECK(!book.cancel(1));
    CHECK(book.cancel(2));
    CHECK(!book.best_bid());
    CHECK(book.best_ask()->oldest_order_id == 4);
}

