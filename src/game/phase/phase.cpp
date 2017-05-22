#include "phase.h"

#include <iostream>

void TimedPhase::transition_after(int seconds, proto::Phase to) {
    cancel_clock_every();
    next = proto::Phase::NOOP;
    remaining_seconds = seconds;

    static proto::ServerMessage msg;
    // Announce the initial time to clients
    msg.set_time_update(remaining_seconds);
    events::server::announce(msg);

    cancel_clock_every = Timer::get()->do_every(std::chrono::seconds(1), [this, to](){
        remaining_seconds--;

        // Announce the time to clients
        msg.set_time_update(remaining_seconds);
        events::server::announce(msg);

        if (remaining_seconds <= 0) {
            cancel_clock_every();
            next = to;
        }
    });
}
