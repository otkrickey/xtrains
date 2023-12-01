#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

std::string readApiKey();
bool readMode(const std::string &modeName);
bool readOption(const std::string &optionName);