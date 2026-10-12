#include "trading_engine/matching_engine.hpp"

#include <stdexcept>

#define CHECK(condition)                                         \
    do {                                                         \
        if (!(condition)) throw std::runtime_error(#condition); \
    } while (false)

int main() {
    using namespace trading_engine;

    MatchingEngine engine;
    CHECK(engine.submit({1, Side::Sell, OrderType::Limit, 101, 4}).rested_quantity == 4);
    CHECK(engine.submit({2, Side::Sell, OrderType::Limit, 101, 6}).rested_quantity == 6);
    CHECK(engine.submit({3, Side::Sell, OrderType::Limit, 102, 5}).rested_quantity == 5);

    const auto buy = engine.submit({4, Side::Buy, OrderType::Limit, 102, 12});
    CHECK(buy.status == SubmitStatus::Accepted);
    CHECK(buy.trades.size() == 3);
    CHECK(buy.trades[0].resting_order_id == 1);
    CHECK(buy.trades[0].price == 101);
    CHECK(buy.trades[0].quantity == 4);
    CHECK(buy.trades[1].resting_order_id == 2);
    CHECK(buy.trades[1].price == 101);
    CHECK(buy.trades[1].quantity == 6);
    CHECK(buy.trades[2].resting_order_id == 3);
    CHECK(buy.trades[2].price == 102);
    CHECK(buy.trades[2].quantity == 2);
    CHECK(buy.rested_quantity == 0);
    CHECK(engine.book().best_ask()->price == 102);
    CHECK(engine.book().best_ask()->total_quantity == 3);
    CHECK(engine.book().best_ask()->oldest_order_id == 3);

    CHECK(engine.submit({5, Side::Buy, OrderType::Limit, 100, 8}).rested_quantity == 8);
    const auto duplicate = engine.submit({5, Side::Buy, OrderType::Market, 0, 1});
    CHECK(duplicate.status == SubmitStatus::DuplicateActiveId);
    CHECK(duplicate.trades.empty());
    CHECK(engine.book().best_bid()->total_quantity == 8);

    const auto market = engine.submit({6, Side::Buy, OrderType::Market, 0, 5});
    CHECK(market.trades.size() == 1);
    CHECK(market.trades[0].quantity == 3);
    CHECK(market.expired_quantity == 2);
    CHECK(!engine.book().best_ask());

    const auto sell = engine.submit({7, Side::Sell, OrderType::Limit, 99, 10});
    CHECK(sell.trades.size() == 1);
    CHECK(sell.trades[0].resting_order_id == 5);
    CHECK(sell.trades[0].price == 100);
    CHECK(sell.trades[0].quantity == 8);
    CHECK(sell.rested_quantity == 2);
    CHECK(!engine.book().best_bid());
    CHECK(engine.book().best_ask()->price == 99);
    CHECK(engine.book().best_ask()->total_quantity == 2);

    CHECK(engine.submit({8, Side::Buy, OrderType::Limit, 0, 1}).status ==
          SubmitStatus::InvalidPrice);
    CHECK(engine.submit({8, Side::Buy, OrderType::Market, 0, 0}).status ==
          SubmitStatus::InvalidQuantity);
    CHECK(engine.cancel(7));
    CHECK(!engine.book().best_ask());
    CHECK(!engine.cancel(7));
}
