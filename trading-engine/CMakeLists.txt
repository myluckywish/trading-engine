cmake_minimum_required(VERSION 3.20)
project(trading_engine VERSION 0.1.0 LANGUAGES CXX)

add_library(trading_engine src/order_book.cpp src/matching_engine.cpp)
target_include_directories(trading_engine PUBLIC include)
target_compile_features(trading_engine PUBLIC cxx_std_20)
target_compile_options(trading_engine PRIVATE -Wall -Wextra -Wpedantic)

add_executable(order_book_demo app/main.cpp)
target_link_libraries(order_book_demo PRIVATE trading_engine)

include(CTest)
if(BUILD_TESTING)
    add_executable(order_book_tests tests/order_book_tests.cpp)
    target_link_libraries(order_book_tests PRIVATE trading_engine)
    add_test(NAME order_book_tests COMMAND order_book_tests)

    add_executable(matching_engine_tests tests/matching_engine_tests.cpp)
    target_link_libraries(matching_engine_tests PRIVATE trading_engine)
    add_test(NAME matching_engine_tests COMMAND matching_engine_tests)
endif()

