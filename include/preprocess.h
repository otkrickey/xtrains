#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "db.h"
#include "ndb.h"
#include "api.h"
#include "config.h"
#include "model.h"

namespace preprocess
{
    using json = nlohmann::json;

    int stations();
    int trains();
    int main();
} // namespace preprocess