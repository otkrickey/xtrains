#include "../include/config.h"

std::string readApiKey()
{
    std::string configFilePath = "config/config.json";
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open())
    {
        std::cerr << "Unable to open config file: " << configFilePath << std::endl;
        return "";
    }

    nlohmann::json configJson;
    try
    {
        configFile >> configJson;
    }
    catch (nlohmann::json::parse_error &e)
    {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return "";
    }

    if (configJson["env"].find("ODPT_CONSUMER_KEY") != configJson["env"].end())
    {
        return configJson["env"]["ODPT_CONSUMER_KEY"];
    }
    else
    {
        std::cerr << "ODPT_CONSUMER_KEY not found in config file." << std::endl;
        return "";
    }
}

bool readMode(const std::string &modeName)
{
    std::string configFilePath = "config/config.json";
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open())
    {
        std::cerr << "Unable to open config file: " << configFilePath << std::endl;
        return false;
    }

    nlohmann::json configJson;
    try
    {
        configFile >> configJson;
    }
    catch (nlohmann::json::parse_error &e)
    {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return false;
    }

    if (configJson["mode"].find(modeName) != configJson["mode"].end())
    {
        return configJson["mode"][modeName];
    }
    else
    {
        std::cerr << modeName << " not found in config file." << std::endl;
        return false;
    }
}

bool readOption(const std::string &optionName)
{
    std::string configFilePath = "config/config.json";
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open())
    {
        std::cerr << "Unable to open config file: " << configFilePath << std::endl;
        return false;
    }

    nlohmann::json configJson;
    try
    {
        configFile >> configJson;
    }
    catch (nlohmann::json::parse_error &e)
    {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return false;
    }

    if (configJson["option"].find(optionName) != configJson["option"].end())
    {
        return configJson["option"][optionName];
    }
    else
    {
        std::cerr << optionName << " not found in config file." << std::endl;
        return false;
    }
}

// 07:00 -> 7 * 3600 + 0 * 60 = 25200
int timeToSeconds(const std::string &time)
{
    if (time.size() != 5 || time[2] != ':')
    {
        throw std::invalid_argument("Invalid time format");
    }

    int hours = (time[0] - '0') * 10 + (time[1] - '0');
    int minutes = (time[3] - '0') * 10 + (time[4] - '0');

    if (hours < 0 || hours >= 24 || minutes < 0 || minutes >= 60)
    {
        throw std::invalid_argument("Invalid time");
    }

    if (0 <= hours && hours < 3)
        hours += 24;

    return hours * 3600 + minutes * 60;
}

std::tuple<model::station_, model::station_, model::time_> readDijkstraOptions()
{
    std::string configFilePath = "config/config.json";
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open())
    {
        std::cerr << "Unable to open config file: " << configFilePath << std::endl;
        return std::make_tuple(model::station_(), model::station_(), model::time_());
    }

    nlohmann::json configJson;
    try
    {
        configFile >> configJson;
    }
    catch (nlohmann::json::parse_error &e)
    {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return std::make_tuple(model::station_(), model::station_(), model::time_());
    }

    if (configJson["dijkstra"].find("start") != configJson["dijkstra"].end() &&
        configJson["dijkstra"].find("end") != configJson["dijkstra"].end() &&
        configJson["dijkstra"].find("time") != configJson["dijkstra"].end())
    {
        if (configJson["dijkstra"]["time"].find("h") != configJson["dijkstra"]["time"].end() &&
            configJson["dijkstra"]["time"].find("m") != configJson["dijkstra"]["time"].end() &&
            configJson["dijkstra"]["time"].find("s") != configJson["dijkstra"]["time"].end())
        {
            int start = configJson["dijkstra"]["start"];
            int end = configJson["dijkstra"]["end"];
            int h = configJson["dijkstra"]["time"]["h"];
            int m = configJson["dijkstra"]["time"]["m"];
            int s = configJson["dijkstra"]["time"]["s"];

            if (0 <= h && h < 3)
                h += 24;

            return std::make_tuple(
                model::station_(start),
                model::station_(end),
                model::time_(h * 3600 + m * 60 + s));
        }
        else
        {
            std::cerr << "Time format is invalid." << std::endl;
            throw std::invalid_argument("Time format is invalid.");
        }
    }
    else
    {
        std::cerr << "Dijkstra options are not found in config file." << std::endl;
        throw std::invalid_argument("Dijkstra options are not found in config file.");
    }
}