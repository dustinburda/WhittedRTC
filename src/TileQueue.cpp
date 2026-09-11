//
// Created by Dustin on 9/10/26.
//

#include "../include/TileQueue.h"

TileQueue::TileQueue() : state_{TileQueueState::Open}, current_tile_(0) {

}

void TileQueue::AddTile(Tile&& t) {
    if (state_ != TileQueueState::Open)
        throw std::logic_error("Can't add tiles if queue is not open.");

    q_.push_back(std::move(t));
}

void TileQueue::Close() {
    state_ = TileQueueState::Closed;
}

std::optional<Tile> TileQueue::NextTile() {
    if (state_ != TileQueueState::Closed)
        throw std::logic_error("Can't consume tiles while queue is open.");

    auto curr_index = current_tile_.fetch_add(1);

    if (curr_index >= q_.size() )
        return std::nullopt;

    return q_[curr_index];
}