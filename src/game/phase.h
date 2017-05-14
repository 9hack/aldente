#pragma once

#include "events.h"
#include "construct_types.h"
#include "context.h"
#include <iostream>

class Phase {
public:
    Phase(Context context) : context(context) {};
    virtual void setup() = 0;
    virtual Phase* update() = 0;
    virtual void teardown() = 0;
    virtual void client_setup() = 0;
    virtual void client_teardown() = 0;
protected:
    Context context;
};

class MenuPhase : public Phase {
public:
    MenuPhase(Context context) : Phase(context) {};
    void setup() {}
    Phase* update() { return nullptr; }
    void teardown() {}
    void client_setup() {}
    void client_teardown() {}
private:
};

class BuildPhase : public Phase {
public:
    BuildPhase(Context context) : Phase(context) {};
    void setup();
    Phase* update();
    void teardown();
    void client_setup();
    void client_teardown();
    static bool is_menu;
private:
    boost::signals2::connection joystick_conn;
    boost::signals2::connection button_conn;
};

class DungeonPhase : public Phase {
public:
    DungeonPhase(Context context) : Phase(context) {};
    void setup();
    Phase* update();
    void teardown();
    void client_setup();
    void client_teardown();
private:
    boost::signals2::connection joystick_conn;
    boost::signals2::connection button_conn;
    boost::signals2::connection collision_conn;
    boost::signals2::connection interact_conn;
};

class MinigamePhase : public Phase {
public:
    MinigamePhase(Context context) : Phase(context) {};
    void setup() {}
    Phase* update() { return nullptr; }
    void teardown() {}
    void client_setup() {}
    void client_teardown() {}
private:
};
