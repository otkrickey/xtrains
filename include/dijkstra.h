#pragma once

#include <iostream>
#include <vector>
#include "model.h"
#include <map>
#include <queue>
#include <climits>
#include <algorithm>

#include "database.h"
#include "db.h"
#include "model.h"

namespace dijkstra
{
    using ShortestPath = std::map<model::station_, model::Edge_ *>;
    using Graph = std::map<model::station_, std::map<model::station_, model::Edge *>>;

    ShortestPath dijkstra();
    int main();
}