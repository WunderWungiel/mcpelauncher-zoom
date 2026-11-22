#include "transition.h"
#include "easing.h"

void Transition::startTransition(unsigned long startVal, unsigned long endVal, int durationMs, int64_t now) {
    start = transitionInProgress ? current : startVal;
    end = endVal;
    startTime = now;
    endTime = now + durationMs;
    current = start;
    transitionInProgress = durationMs > 0;
    if(!transitionInProgress) {
        current = end;
    }
}

void Transition::tick(int64_t now) {
    if(!transitionInProgress) {
        return;
    }

    int64_t duration = endTime - startTime;
    if(duration <= 0) {
        current = end;
        transitionInProgress = false;
        return;
    }

    double progress = (double)(now - startTime) / (double)duration;
    if(progress <= 0.0) {
        current = start;
        return;
    }
    if(progress >= 1.0) {
        current = end;
        transitionInProgress = false;
        return;
    }

    double eased = easeOutExp(progress);
    if(start > end) {
        unsigned long diff = start - end;
        current = start - (unsigned long)(diff * eased);
    } else {
        unsigned long diff = end - start;
        current = start + (unsigned long)(diff * eased);
    }
}
