#pragma once
#include <cstdint>

class Transition {
private:
    unsigned long start;
    unsigned long end;
    int64_t startTime;
    int64_t endTime;
    unsigned long current;
    bool transitionInProgress;

public:
    void startTransition(unsigned long startVal, unsigned long endVal, int durationMs, int64_t now);
    void tick(int64_t now);
    unsigned long getCurrent() {
        return current;
    }
    bool inProgress() {
        return transitionInProgress;
    }
};
