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
#include "model.h"
namespace preprocess_v2
{
    using json = nlohmann::json;
    int main();
    int test();
} // namespace preprocess_v2