#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "db.h"
#include "api.h"
#include "data.h"

namespace preprocess
{
    using namespace std;
    using json = nlohmann::json;

    int stations();
    int trains();
    int main();
}