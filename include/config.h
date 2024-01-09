#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

#include "model.h"

std::string readApiKey();
bool readMode(const std::string &modeName);
bool readOption(const std::string &optionName);
int timeToSeconds(const std::string &time);
std::tuple<model::station_, model::station_, model::time_> readDijkstraOptions();