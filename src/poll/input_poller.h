#pragma once

#include "poller.h"
#include "window.h"

class InputPoller : public Poller {
private:
    Window &window;

public:
    InputPoller(Window &window) :
            window(window) {};
    void poll() override;
};
