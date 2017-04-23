#pragma once

#include "poller.h"

class InputPoller : public Poller {
public:
    void poll() override;
};
