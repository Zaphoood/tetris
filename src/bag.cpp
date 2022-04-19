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

/*
 * Return the next element in the bag
 */
TetrominoKind_t SevenBag::pullFromBag() {
    // Ran out of elements? -> Shuffle the bag
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

/*
 * Return one element from top of the Queue and push a new one to the bottom
 *
 * @return the popped element
 */
TetrominoKind_t SevenBag::popQueue() {
    // Get the topmost element, which will be returned
    TetrominoKind_t ret = queue[queue_head];
    // Replace the old element that is now at the new bottom of the queue
    queue[queue_head] = pullFromBag();
    // Move the top of the queue to its new position
    queue_head = (queue_head + 1) % queue.size();

    return ret;
}

/*
 * Return the queue in its virtual shape, i. e. starting with the element
 * pointed at by queue_head and wrapping around the end
 *
 * @return the queue
 */
std::array<TetrominoKind_t, QUEUE_LEN> SevenBag::getQueue() {
    std::array<TetrominoKind_t, QUEUE_LEN> ret_queue;
    for (int i = 0; i < QUEUE_LEN; i++) {
        ret_queue[i] = queue[(queue_head + i) % QUEUE_LEN];
    }
    return ret_queue;
}
