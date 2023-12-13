#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "../include/config.h"

namespace api
{
    using json = nlohmann::json;

    extern const std::string base_url;
    extern const std::string consumer_key;

    enum class Endpoint
    {
        Calendar,
        Operator,
        Station,
        StationTimetable,
        TrainTimetable,
        TrainType,
        RailDirection,
        Railway,
        RailwayFare,
    };
    enum class Query
    {
        Calendar,
        Operator,
        Railway,
    };
    enum class Operator
    {
        JREast,
        TokyoMetro,
    };
    enum class Calendar
    {
        Weekday,
        SaturdayHoliday
    };
    extern std::map<Endpoint, std::string> endpoint_map;
    extern std::map<Query, std::string> query_map;
    extern std::map<Operator, std::string> operator_map;
    extern std::map<Calendar, std::string> calendar_map;

    class URLBuilder
    {
    private:
        std::string url;
        std::map<Query, std::string> query;

    public:
        URLBuilder(const Endpoint &endpoint);
        void addQuery(const Query &key, const std::string &val);
        std::string build();
        json getJson();
        std::string request();
    };

    size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp);
    std::string getRequest(URLBuilder &builder);
    std::vector<std::string> getRailwayList();
    std::vector<std::string> getStationList(const std::string &railway);
    std::vector<std::string> getTrainTimetableList(const std::string &railway);

    int main();
} // namespace api