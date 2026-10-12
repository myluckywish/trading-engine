# High-Performance C++ Matching Engine

A portfolio project for building and measuring a price/time priority limit order book and matching engine. The engine accepts limit and market orders, executes against resting orders, and reports trades.

## Build and run

Requires a C++20 compiler and CMake 3.20 or newer.

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/order_book_demo
```

## Current design

- Prices are positive integer ticks; quantities are positive integers.
- Bid levels sort high to low; ask levels sort low to high.
- Orders at the same price retain arrival order in a linked list.
- An order ID index supports direct lookup for cancellation.
- Empty levels disappear when their last order is canceled.
- Incoming orders match the best opposite price first, then the oldest order at that price. Trades execute at the resting order's price.
- Unfilled limit quantity rests; unfilled market quantity expires. Order IDs must be unique among active resting orders.
- This version favors clarity. Benchmark before changing containers or allocation.

## Next milestones

1. Define replace-order priority and rejection rules; expand deterministic tests for more edge cases.
2. Add repeatable latency and throughput benchmarks with documented hardware, compiler flags, and workloads.
3. Add event replay. Distinguish reconstructing a book from historical add/cancel/execute messages from simulating incoming orders through this engine.

