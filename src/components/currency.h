#pragma once

/*
    Used to keep track of a player's current amount of gold.
*/
class Currency {
public:
    Currency() : gold(0) {};

    int get_gold() { return gold; };
    void set_gold(int amt) { gold = amt; };
    void add_gold(int amt) { gold += amt; };
    void subtract_gold(int amt) { gold -= amt; if (gold < 0) gold = 0; };

private:
    int gold; // Current amount of gold being carried
};