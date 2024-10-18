#pragma once

#include <string>
#include <vector>

class GridBase {
public:
    virtual std::string toString() const = 0;
    virtual std::string toString(const std::vector<std::vector<bool>>& printGrid) const = 0;
    virtual ~GridBase() = default;
};