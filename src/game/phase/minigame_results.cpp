#include <util/config.h>
#include "minigame_results.h"

MinigameResultsPhase::MinigameResultsPhase(Context &context)
        : TimedPhase(context) {}

void MinigameResultsPhase::s_setup() {
    Config::config->get_value(Config::str_num_rounds, n_rounds);

    transition_after(0, 15, s_phase_when_done());

    // Do any extra scene stuff here
}

void MinigameResultsPhase::s_teardown() {
}

void MinigameResultsPhase::c_setup() {
    Config::config->get_value(Config::str_num_rounds, n_rounds);

    // TODO: replace with real data
    events::ui::scoreboard_sequence(
            {{"pig", 123, 500},
             {"lizar", 456, 7},
             {"cat", 999, 5},
             {"tomatoe", 1, 66666}});

    // Do any extra scene stuff here
}

void MinigameResultsPhase::c_update() {
}

void MinigameResultsPhase::c_teardown() {
    events::ui::disable_scoreboard();
}

std::string MinigameResultsPhase::to_string() {
    return "Minigame Results";
}

proto::Phase MinigameResultsPhase::s_phase_when_done() {
    return context.current_round == n_rounds ? proto::Phase::END : proto::Phase::BUILD;
}
