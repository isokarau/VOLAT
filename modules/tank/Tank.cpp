#include "Tank.h"

St::HexList St::AvailableHexesForMove() const {
    return {};
}

std::vector<St::HexList> St::GetShootingHexesAreas() const {
    return {};
}

Hex Tank::GetPosition() const {
    return Hex(x_, y_, z_);
}