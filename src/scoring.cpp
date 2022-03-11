#include "scoring.h"
#include "constants.h"

int ScoringSystem::getLevel() const {
    return m_level;
}

int ScoringSystem::getGoal() const {
    return m_goal;
}

int ScoringSystem::getFallSpeedMs() const {
    return m_fall_speed_ms;
}

void ScoringSystem::updateFallSpeed() {
    switch (m_level) {
    case 1:
        m_fall_speed_ms = 1000;
        break;
    case 2:
        m_fall_speed_ms = 793;
        break;
    case 3:
        m_fall_speed_ms = 618;
        break;
    case 4:
        m_fall_speed_ms = 473;
        break;
    case 5:
        m_fall_speed_ms = 355;
        break;
    case 6:
        m_fall_speed_ms = 262;
        break;
    case 7:
        m_fall_speed_ms = 190;
        break;
    case 8:
        m_fall_speed_ms = 135;
        break;
    case 9:
        m_fall_speed_ms = 94;
        break;
    case 10:
        m_fall_speed_ms = 64;
        break;
    case 11:
        m_fall_speed_ms = 43;
        break;
    case 12:
        m_fall_speed_ms = 28;
        break;
    case 13:
        m_fall_speed_ms = 18;
        break;
    case 14:
        m_fall_speed_ms = 11;
        break;
    default:
        m_fall_speed_ms = 7;
        break;
    }
}

FixedGoalScoring::FixedGoalScoring() : FixedGoalScoring(1){};

FixedGoalScoring::FixedGoalScoring(int starting_level) {
    m_level = starting_level;
    // When starting on a level higher than one, the first goal is equal to the
    // sum of all the goals up to the current one
    m_goal = 5; // starting_level * LINES_PER_LEVEL;
    updateFallSpeed();
}

void FixedGoalScoring::onLinesCleared(int n_lines) {
    // If the amount of cleared lines is bigger than the current goal, subtract
    // the 'overhead' from the subsequent goal
    int overhead = n_lines - m_goal;
    // Subtract cleared lines from current goal
    m_goal = std::max(m_goal - n_lines, 0);

    // Current goal was reached
    if (overhead >= 0) {
        // Increase level
        m_level++;
        // Set new goal
        m_goal = LINES_PER_LEVEL;
        m_goal -= overhead;
    }

    updateFallSpeed();
}
