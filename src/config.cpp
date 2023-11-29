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