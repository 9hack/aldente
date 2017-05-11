#pragma once

#include "events.h"
#include "construct_types.h"
#include <iostream>
#include "game_objects\player.h"

class Phase {
public:
    virtual void setup() = 0;
    virtual Phase* update() = 0;
    virtual void teardown() = 0;
};

class MenuPhase : public Phase {
public:
    void setup() {}
    Phase* update() { return nullptr; }
    void teardown() {}
private:
};

class BuildPhase : public Phase {
public:
    void setup();
    Phase* update();
    void teardown();
    static bool is_menu;
private:
    boost::signals2::connection joystick_conn;
    boost::signals2::connection button_conn;
};

class DungeonPhase : public Phase {
public:
    void setup();
    Phase* update() { return nullptr; }
    void teardown();
private:
    boost::signals2::connection joystick_conn;
    boost::signals2::connection button_conn;
    std::map<Player*, bool> finished_players;
};

class MinigamePhase : public Phase {
public:
    void setup() {}
    Phase* update() { return nullptr; }
    void teardown() {}
private:
};
