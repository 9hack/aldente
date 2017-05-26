#pragma once

class PlayerStats {
public:
    PlayerStats()
            : coins(START_COINS) {}

    int get_coins();
    void add_coins(int delta);
    void set_coins(int amount);
private:
    int coins;
    static const int START_COINS = 100;
};
