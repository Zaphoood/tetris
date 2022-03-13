#include <iostream>

#include "constants.h"
#include "scoring.h"

int ScoringSystem::getLevel() const {
    return m_level;
}

int ScoringSystem::getGoal() const {
    return m_goal;
}

int ScoringSystem::getScore() const {
    return m_score;
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

void ScoringSystem::onSoftDrop() {
    m_score++;
}

void ScoringSystem::onHardDrop(int n_lines) {
    m_score += n_lines * 2;
}

void ScoringSystem::onTSpin(int n_lines_cleared) {
    awardAction(T_SPIN_REWARD[n_lines_cleared]);
}

void ScoringSystem::onMiniTSpin(int n_lines_cleared) {
    awardAction(MINI_T_SPIN_REWARD[n_lines_cleared]);
}

void ScoringSystem::awardAction(int points) {
    /**
     * Award points depending on whether a back-to-back sequence is active
     *
     */

    if (m_b2b) {
        points = (int)(points * 1.5);
    }
    m_score += points;
}

FixedGoalScoring::FixedGoalScoring() : FixedGoalScoring(1){};

FixedGoalScoring::FixedGoalScoring(int starting_level) {
    m_level = starting_level;
    // When starting on a level higher than one, the first goal is equal to
    // the sum of all the goals up to the current one
    m_goal = 5; // starting_level * LINES_PER_LEVEL;
    m_score = 0;
    updateFallSpeed();
}

void FixedGoalScoring::onLinesCleared(int n_lines) {
    // Award points for line clear
    if (n_lines > 0) {
        if (n_lines == 4) {
            m_b2b = true;
        } else {
            // A line clear that doesn't clear 4 lines at once end's a
            // back-to-back sequence
            m_b2b = false;
        }
        awardAction(LINE_CLEAR_REWARD[n_lines - 1] * m_level);
    } else if (n_lines > 4) {
        std::cout << "Unreachable: More than four lines cleared\n";
        exit(1);
    }

    // If the amount of cleared lines is bigger than the current goal,
    // subtract the 'overhead' from the subsequent goal
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
