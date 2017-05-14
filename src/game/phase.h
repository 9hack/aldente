#pragma once

#include "events.h"
#include "construct_types.h"
#include "context.h"
#include <iostream>

class Phase {
public:
    Phase(Context context) : context(context) {};
    virtual void setup() {};
    virtual Phase* update() { return nullptr; };
    virtual void teardown() {};
    virtual void client_setup() {};
    virtual void client_teardown() {};
protected:
    Context context;
};

class MenuPhase : public Phase {
public:
    MenuPhase(Context context) : Phase(context) {};
};

class BuildPhase : public Phase {
public:
    BuildPhase(Context context) : Phase(context) {};
    void setup();
    Phase* update();
    void teardown();
    void client_setup();
    void client_teardown();
private:
    static bool is_menu;
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
};

class MinigamePhase : public Phase {
public:
    MinigamePhase(Context context) : Phase(context) {};
};
