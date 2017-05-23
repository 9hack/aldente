#pragma once

// Type of construct built on tile.
enum ConstructType {
    NONE, CHEST, SPIKES, REMOVE
};

struct ConstructData {
    ConstructType type;
    int cost;
    std::string name;
    std::string description;
    std::string image;
};

namespace Constructs {
    const static ConstructData CHEST = { ConstructType::CHEST, 50, "Chest", "A treasure chest full of gold.", "test.png" };
    const static ConstructData SPIKES = { ConstructType::SPIKES, 50, "Spikes", "It was me, spikes.", "dio.jpg" };
    const static ConstructData REMOVE = { ConstructType::REMOVE, 200, "Remove", "Remove a block.", "Tomato.jpg" };
    const static std::map<ConstructType, ConstructData> CONSTRUCTS = {
        { ConstructType::CHEST, CHEST },
        { ConstructType::SPIKES, SPIKES },
        { ConstructType::REMOVE, REMOVE }
    };
}
