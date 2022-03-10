#pragma once

class ScoringSystem {
  protected:
    int m_level;
    int m_goal;
    int m_fall_speed_ms;
    void updateFallSpeed();

  public:
    int getLevel();
    int getGoal();
    int getFallSpeedMs();
    virtual void onLinesCleared(int n_lines) = 0;
};

class FixedGoalScoring : public ScoringSystem {
  public:
    FixedGoalScoring();
    FixedGoalScoring(int starting_level);

    void onLinesCleared(int n_lines) override;
};