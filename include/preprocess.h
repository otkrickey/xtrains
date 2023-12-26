#pragma once

#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "api.h"
#include "config.h"
#include "database.h"
#include "db.h"
#include "model.h"

namespace preprocess
{
    using json = nlohmann::json;
    int timeToSeconds(const std::string &time);
    int main();
    int test();
} // namespace preprocess

namespace preprocess_v2
{
    using json = nlohmann::json;
    int timeToSeconds(const std::string &time);
    int main();
    int test();
} // namespace preprocess_v2