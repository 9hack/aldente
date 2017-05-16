#pragma once

#include "events.h"
#include "timer.h"

class Phase {
public:
    Phase(Context& context) : context(context), next(nullptr) {};
    virtual void setup() = 0;
    virtual Phase* update() { return next; };
    virtual void teardown() = 0;
    virtual void client_setup() = 0;
    virtual void client_update() {};
    virtual void client_teardown() = 0;
protected:
    Context& context;
    Phase *next;
};

class TimedPhase : public Phase {
public:
	TimedPhase(Context& context) : Phase(context) {};
protected:
    std::function<void()> cancel_clock_every;

    // Use to set up timer to transition to next phase after specified time.
    void transition_after(const Timer::Duration &time, Phase *const to);
};
