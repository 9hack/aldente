#pragma once

#include "phase.h"

class MenuPhase : public Phase {
public:
    MenuPhase(Context& context) : Phase(context) {};
    void s_setup() override;
    proto::Phase s_update() override;
    void s_teardown() override;
    void c_setup() override;
    void c_update() override;
    void c_teardown() override;

private:
};
