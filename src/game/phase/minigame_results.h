#pragma once

#include "phase.h"

class MinigameResultsPhase : public TimedPhase {
public:
    MinigameResultsPhase(Context &context);
    void s_setup() override;
    void s_teardown() override;
    void c_setup() override;
    void c_update() override;
    void c_teardown() override;
    std::string to_string() override;
    proto::Phase s_phase_when_done() override;

private:
    int n_rounds;
};
