#pragma once

#include <algorithm>
#include <climits>
#include <iostream>
#include <map>
#include <queue>
#include <vector>

#include "database.h"
#include "model.h"

namespace dijkstra
{
    using ShortestPath = std::map<model::station_, model::Edge_ *>;
    using Graph = std::map<model::station_, std::map<model::station_, model::Edge *>>;

    ShortestPath dijkstra();
    int main();
}