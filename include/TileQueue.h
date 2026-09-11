//
// Created by Dustin on 9/10/26.
//

#ifndef WHITTED_TILEQUEUE_H
#define WHITTED_TILEQUEUE_H

#include <atomic>
#include <optional>
#include <vector>


struct Tile {
    std::size_t x_;
    std::size_t x_length_;
    std::size_t y_;
    std::size_t y_length_;
};

enum class TileQueueState {
    Open,
    Closed
};

class TileQueue {
public:
    TileQueue();

    void AddTile(Tile&& t);
    void Close();
    std::optional<Tile> NextTile();

private:
    TileQueueState state_;
    std::vector<Tile> q_;
    std::atomic<std::size_t> current_tile_;

};


#endif //WHITTED_TILEQUEUE_H
