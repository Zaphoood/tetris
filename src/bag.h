#include <array>

#include "constants.h"

class SevenBag {
  private:
    // Bag of Tetromino types from which new types are "pulled". Once the bag is
    // empty it is refilled and shuffled
    std::array<TetrominoKind_t, N_TETROMINOS> bag;
    int next_bag_element = 0;
    // Queue of Tetrominos that will be placed on the Playfield
    // Since the Queue is of a fixed size, it's not necessary to actually delete
    // elements of the top off it and push new ones to the bottom using a
    // dynamic array. Instead, we simply store the virtual position of the top
    // of the Queue and move it around in the static array. When an element is
    // popped off the top, the index of the top is incremented by one (applying
    // modulo array length) and the popped element is replaced with a newly
    // generated value
    std::array<TetrominoKind_t, QUEUE_LEN> queue;
    // Stores where the top of the queue currently is
    int queue_top = 0;
    void shuffleBag();
    TetrominoKind_t pullFromBag();
    void initQueue();

  public:
    SevenBag();
    TetrominoKind_t popQueue();
    std::array<TetrominoKind_t, QUEUE_LEN> getQueue();
};