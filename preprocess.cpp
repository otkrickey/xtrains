#include <iostream>
#include <vector>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

std::string downloadJSON(const std::string &url)
{
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

class URLGenerator
{
private:
    const std::string base_url = "https://api.odpt.org/api/v4/odpt:TrainTimetable";
    const std::string consumer_key = "2bbf82cd3d5647265c321eab3cd84ef0178ec8f4b472bf6655d13f4f11f92c67";

public:
    std::string generateURL(const std::string &railway, const std::string &calendar) const
    {
        return base_url + "?odpt:railway=" + railway + "&odpt:calendar=" + calendar + "&acl:consumerKey=" + consumer_key;
    }
};

int preprocess()
{
    std::string url = "https://api.odpt.org/api/v4/odpt:Railway?odpt:operator=odpt.Operator:TokyoMetro&acl:consumerKey=2bbf82cd3d5647265c321eab3cd84ef0178ec8f4b472bf6655d13f4f11f92c67"; // 実際のAPIエンドポイントURLに置き換えてください
    std::string jsonDataStr = downloadJSON(url);
    json jsonData = json::parse(jsonDataStr);

    std::vector<std::string> railwayIDs;

    // 各路線データから"owl:sameAs"の値を取得
    for (const auto &railway : jsonData)
    {
        if (railway.contains("owl:sameAs"))
        {
            railwayIDs.push_back(railway["owl:sameAs"].get<std::string>());
        }
    }

    // 結果を表示
    URLGenerator generator;
    for (const auto &id : railwayIDs)
    {
        std::cout << id << std::endl;
        std::string weekday_url = generator.generateURL(id, "odpt.Calendar:Weekday");
        std::string saturday_holiday_url = generator.generateURL(id, "odpt.Calendar:SaturdayHoliday");
        std::cout << weekday_url << std::endl;
        std::cout << saturday_holiday_url << std::endl;
    }

    return 0;
}
