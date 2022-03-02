#include <algorithm>
#include <iostream>
#include <random>

#include "bag.h"

SevenBag::SevenBag() {
    // Initialize PRNG
    srand(time(0));
    // Initialize Seven-Bag
    shuffleBag();
    // Initialize Tetromino queue
    initQueue();
}

void SevenBag::shuffleBag() {
    for (int i = 0; i < N_TETROMINOS; i++) {
        bag[i] = i;
    }
    std::random_shuffle(bag.begin(), bag.end());
    next_bag_element = 0;
}

TetrominoKind_t SevenBag::pullFromBag() {
    // Return the next element in the bag

    // Run out of elements? -> Shuffle the bag
    if (next_bag_element >= bag.size()) {
        shuffleBag();
    }
    // Return the next_bag_element'th element from the bag and increment
    // next_bag_element
    TetrominoKind_t ret = bag[next_bag_element];
    next_bag_element++;
    return ret;
}

void SevenBag::initQueue() {
    for (int i = 0; i < QUEUE_LEN; i++) {
        queue[i] = pullFromBag();
    }
}

TetrominoKind_t SevenBag::popQueue() {
    // Pop one element from top of the Queue and push a new one to the bottom

    // Get the topmost element, which will be returned
    TetrominoKind_t ret = queue[queue_top];
    // Replace the old element that is now at the new bottom of the queue
    queue[queue_top] = pullFromBag();
    // Move the top of the queue to its new position
    queue_top = (queue_top + 1) % queue.size();

    return ret;
}

std::array<TetrominoKind_t, QUEUE_LEN> SevenBag::getQueue() {
    // Return the Queue in its virtual, not its actual shape (I know this
    // description sucks -- just read the the code and you will understand)

    std::array<TetrominoKind_t, QUEUE_LEN> ret_queue;
    for (int i = 0; i < QUEUE_LEN; i++) {
        ret_queue[i] = queue[(queue_top + i) % QUEUE_LEN];
    }
    return ret_queue;
}