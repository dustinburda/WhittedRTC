//
// Created by Dustin on 9/11/26.
//

#include <gtest/gtest.h>

#include "../include/TileQueue.h"

#include <algorithm>
#include <mutex>
#include <thread>
#include <vector>

TEST(TileQueueTest, EmptyQueueReturnsNullopt) {
    TileQueue queue;
    queue.Close();

    auto tile = queue.NextTile();

    EXPECT_FALSE(tile.has_value());
}

TEST(TileQueueTest, NextTileReturnsTilesInInsertionOrder) {
    TileQueue queue;
    queue.AddTile(Tile{0, 10, 0, 10});
    queue.AddTile(Tile{10, 20, 0, 10});

    queue.Close();

    auto first = queue.NextTile();
    auto second = queue.NextTile();
    auto third = queue.NextTile();

    ASSERT_TRUE(first.has_value());
    ASSERT_TRUE(second.has_value());
    EXPECT_FALSE(third.has_value());

    EXPECT_EQ(first->x_, 0);
    EXPECT_EQ(first->x_length_, 10);
    EXPECT_EQ(first->y_, 0);
    EXPECT_EQ(first->y_length_, 10);

    EXPECT_EQ(second->x_, 10);
    EXPECT_EQ(second->x_length_, 20);
    EXPECT_EQ(second->y_, 0);
    EXPECT_EQ(second->y_length_, 10);
}

TEST(TileQueueTest, ConcurrentNextTileClaimsEachTileOnce) {
    TileQueue queue;
    constexpr std::size_t num_tiles = 128;
    constexpr std::size_t num_threads = 8;

    for (std::size_t i = 0; i < num_tiles; i++)
        queue.AddTile(Tile{i, 1, 0, 1});

    queue.Close();

    std::vector<std::size_t> claimed;
    std::mutex claimed_mutex;
    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (std::size_t i = 0; i < num_threads; i++) {
        threads.emplace_back([&queue, &claimed, &claimed_mutex]() {
            while (auto tile = queue.NextTile()) {
                std::lock_guard<std::mutex> lock{claimed_mutex};
                claimed.push_back(tile->x_);
            }
        });
    }

    for (auto& thread : threads)
        thread.join();

    std::sort(claimed.begin(), claimed.end());

    ASSERT_EQ(claimed.size(), num_tiles);
    for (std::size_t i = 0; i < num_tiles; i++)
        EXPECT_EQ(claimed[i], i);
}
