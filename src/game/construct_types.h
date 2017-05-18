#pragma once

// Type of construct built on tile.
enum ConstructType {
    CHEST, SPIKES, REMOVE, NONE
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
    const static ConstructData REMOVE = { ConstructType::REMOVE, 0, "Remove", "Remove a block.", "Tomato.jpg" };
}
