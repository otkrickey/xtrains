#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "../include/db.h"
#include "../include/api.h"
#include "../include/config.h"

namespace preprocess
{
    using json = nlohmann::json;

    int stations();
    int trains();
    int main();
} // namespace preprocess