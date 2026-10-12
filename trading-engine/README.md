# High-Performance C++ Matching Engine

A portfolio project for building and measuring a price/time priority limit order book and matching engine. The current starter implements **resting limit orders only**; crossing orders can coexist until matching is added.

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
- This first version favors clarity. Benchmark before changing containers or allocation.

## Next milestones

1. Add a matching engine that consumes incoming limit and market orders, emits trades, handles partial fills, and never rests an unfilled market order.
2. Define replace-order priority and rejection rules; test fills, cancellations, and price/time priority with deterministic event sequences.
3. Add repeatable latency and throughput benchmarks with documented hardware, compiler flags, and workloads.
4. Add event replay. Distinguish reconstructing a book from historical add/cancel/execute messages from simulating incoming orders through this engine.

