#pragma once

#include "poller.h"
#include "window.h"

class InputPoller : public Poller {
public:
    void poll() override;
};
