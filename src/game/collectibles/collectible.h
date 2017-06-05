#pragma once

#include "game_objects/player.h"

// Forward-declare
class Player;

namespace collectibles {

// Interface for collectible items
class Collectible {
public:
    // Implements the behavior which modifies a player when this collectible is collected.
    virtual void collected_by(Player *player) = 0;
};

}
