#include "../include/api.h"

namespace api
{
    using namespace std;
    using json = nlohmann::json;

    const string base_url = "https://api.odpt.org/api/v4/";

    // const string consumer_key = [consumer_key = getenv("ODPT_CONSUMER_KEY")]() -> string
    const string consumer_key = readApiKey();

    map<Endpoint, string> endpoint_map = {
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
    map<Query, string> query_map = {
        {Query::Calendar, "odpt:calendar"},
        {Query::Operator, "odpt:operator"},
        {Query::Railway, "odpt:railway"},
    };
    map<Operator, string> operator_map = {
        {Operator::JREast, "odpt.Operator:JR-East"},
        {Operator::TokyoMetro, "odpt.Operator:TokyoMetro"},
    };
    map<Calendar, string> calendar_map = {
        {Calendar::Weekday, "odpt.Calendar:Weekday"},
        {Calendar::SaturdayHoliday, "odpt.Calendar:SaturdayHoliday"},
    };

    URLBuilder::URLBuilder(const Endpoint &endpoint)
    {
        url = base_url + endpoint_map[endpoint] + "?acl:consumerKey=" + consumer_key;
    }

    void URLBuilder::addQuery(const Query &key, const string &val)
    {
        query[key] = val;
    }

    string URLBuilder::build()
    {
        for (auto const &[key, val] : query)
        {
            url += "&" + query_map[key] + "=" + val;
        }
        return url;
    }

    size_t WriteCallback(void *contents, size_t size, size_t nmemb, string *userp)
    {
        size_t totalSize = size * nmemb;
        userp->append((char *)contents, totalSize);
        return totalSize;
    }

    string getRequest(URLBuilder &builder)
    {
        CURL *curl;
        CURLcode res;
        string readBuffer;

        string url = builder.build();
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

    vector<string> getRailwayList()
    {
        vector<string> railway_list;
        URLBuilder builder(Endpoint::Railway);
        builder.addQuery(Query::Operator, operator_map[Operator::TokyoMetro]);
        string response = getRequest(builder);
        json j = json::parse(response);
        for (auto &railway : j)
        {
            railway_list.push_back(railway["owl:sameAs"]);
        }
        return railway_list;
    }
} // namespace api