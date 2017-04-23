#pragma once

#include "poller.h"

class GlfwPoller : public Poller {
public:
    virtual void poll() override;
};
