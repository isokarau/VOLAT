#pragma once

#include <vector>

#include "../map/Hex.h"
#include "../strategy/Strategy.h"

enum class TankType { // new ones will be added next time
    ST
};

class Tank {
public:
    using HexList = std::vector<Hex>;

    virtual HexList AvailableHexesForMove() const = 0;
    virtual std::vector<HexList> GetShootingHexesAreas() const = 0;

    Hex GetPosition() const;

protected:
    TankType type_;

    int x_;
    int y_;
    int z_;
    int health_points;
    int speed_points;
    int damage;
    int destruction_points;

    Strategy current_strategy;
};

class St : public Tank {
public:
    virtual HexList AvailableHexesForMove() const;
    virtual std::vector<HexList> GetShootingHexesAreas() const;
};
