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
    using namespace std;

    extern const string base_url;
    extern const string consumer_key;

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
    extern map<Endpoint, string> endpoint_map;
    extern map<Query, string> query_map;
    extern map<Operator, string> operator_map;
    extern map<Calendar, string> calendar_map;

    class URLBuilder
    {
    private:
        string url;
        map<Query, string> query;

    public:
        URLBuilder(const Endpoint &endpoint);
        void addQuery(const Query &key, const string &val);
        string build();
    };

    size_t WriteCallback(void *contents, size_t size, size_t nmemb, string *userp);
    string getRequest(URLBuilder &builder);
    vector<string> getRailwayList();
} // namespace api