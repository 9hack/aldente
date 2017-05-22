#include "player_stats.h"

int PlayerStats::get_coins() {
    return coins;
}

void PlayerStats::add_coins(int delta) {
    coins += delta;
    if (coins < 0)
        coins = 0;
}

void PlayerStats::set_coins(int amount) {
    coins = amount < 0 ? 0 : amount;
}
