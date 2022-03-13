#pragma once

class ScoringSystem {
  protected:
    int m_level;
    int m_goal;
    int m_score;
    int m_fall_speed_ms;
    void updateFallSpeed();

  public:
    int getLevel() const;
    int getGoal() const;
    int getScore() const;
    int getFallSpeedMs() const;
    void onSoftDrop();
    void onHardDrop(int n_lines);
    // This is dependent on the specific scoring system, so subclasses must
    // define it
    virtual void onLinesCleared(int n_lines) = 0;
};

class FixedGoalScoring : public ScoringSystem {
  public:
    FixedGoalScoring();
    FixedGoalScoring(int starting_level);

    void onLinesCleared(int n_lines) override;
};