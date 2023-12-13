#include "../include/api.h"

namespace api
{
    using json = nlohmann::json;

    const std::string base_url = "https://api.odpt.org/api/v4/";

    // const string consumer_key = [consumer_key = getenv("ODPT_CONSUMER_KEY")]() -> string
    const std::string consumer_key = readApiKey();

    std::map<Endpoint, std::string> endpoint_map = {
        {Endpoint::Calendar, "odpt:Calendar"},
        {Endpoint::Operator, "odpt:Operator"},
        {Endpoint::Station, "odpt:Station"},
        {Endpoint::StationTimetable, "odpt:StationTimetable"},
        {Endpoint::TrainTimetable, "odpt:TrainTimetable"},
        {Endpoint::TrainType, "odpt:TrainType"},
        {Endpoint::RailDirection, "odpt:RailDirection"},
        {Endpoint::Railway, "odpt:Railway"},
        {Endpoint::RailwayFare, "odpt:RailwayFare"},
    };
    std::map<Query, std::string> query_map = {
        {Query::Calendar, "odpt:calendar"},
        {Query::Operator, "odpt:operator"},
        {Query::Railway, "odpt:railway"},
    };
    std::map<Operator, std::string> operator_map = {
        {Operator::JREast, "odpt.Operator:JR-East"},
        {Operator::TokyoMetro, "odpt.Operator:TokyoMetro"},
    };
    std::map<Calendar, std::string> calendar_map = {
        {Calendar::Weekday, "odpt.Calendar:Weekday"},
        {Calendar::SaturdayHoliday, "odpt.Calendar:SaturdayHoliday"},
    };

    URLBuilder::URLBuilder(const Endpoint &endpoint)
    {
        url = base_url + endpoint_map[endpoint] + "?acl:consumerKey=" + consumer_key;
    }

    void URLBuilder::addQuery(const Query &key, const std::string &val)
    {
        query[key] = val;
    }

    std::string URLBuilder::build()
    {
        for (auto const &[key, val] : query)
        {
            url += "&" + query_map[key] + "=" + val;
        }
        return url;
    }

    json URLBuilder::getJson()
    {
        std::string response = getRequest(*this);
        return json::parse(response);
    }

    size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp)
    {
        size_t totalSize = size * nmemb;
        userp->append((char *)contents, totalSize);
        return totalSize;
    }

    std::string getRequest(URLBuilder &builder)
    {
        CURL *curl;
        CURLcode res;
        std::string readBuffer;

        std::string url = builder.build();
        printf("%s\n", url.c_str());

        curl = curl_easy_init();
        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

            // レスポンスを変数に保存するためのコールバック関数をセット
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            // APIリクエストの実行
            res = curl_easy_perform(curl);

            // エラーチェック
            if (res != CURLE_OK)
            {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }

            // リソースのクリーンアップ
            curl_easy_cleanup(curl);
        }

        return readBuffer;
    }

    std::vector<std::string> getRailwayList()
    {
        std::vector<std::string> railway_list;
        URLBuilder builder(Endpoint::Railway);
        builder.addQuery(Query::Operator, operator_map[Operator::TokyoMetro]);
        std::string response = getRequest(builder);
        json j = json::parse(response);
        for (auto &railway : j)
        {
            railway_list.push_back(railway["owl:sameAs"]);
        }
        return railway_list;
    }

    std::vector<std::string> getStationList(const std::string &railway)
    {
        std::vector<std::string> station_list;
        URLBuilder builder(Endpoint::Station);
        builder.addQuery(Query::Railway, railway);
        std::string response = getRequest(builder);
        json j = json::parse(response);
        for (auto &station : j)
        {
            station_list.push_back(station["owl:sameAs"]);
        }
        return station_list;
    }

    std::vector<std::string> getTrainTimetableList (const std::string &railway)
    {
        std::vector<std::string> timetable_list;
        URLBuilder builder(Endpoint::TrainTimetable);
        builder.addQuery(Query::Railway, railway);
        std::string response = getRequest(builder);
        json j = json::parse(response);
        for (auto &TrainTimetable : j)
        {
            timetable_list.push_back(TrainTimetable["owl:sameAs"]);
        }
        return timetable_list;   
    }

    int main()
    {
        // test getStationList
        std::vector<std::string> railway_list = getRailwayList();
        for (auto &railway : railway_list)
        {
            std::cout << railway << std::endl;
            std::vector<std::string> station_list = getStationList(railway);
            for (auto &station : station_list)
            {
                std::cout << station << std::endl;
            }
        }
        //std::cout << "Hello, world!" << std::endl;
        return 0;
    }

} // namstd::string




