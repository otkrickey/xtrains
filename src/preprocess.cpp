#include "../include/preprocess.h"

namespace preprocess
{
    using json = nlohmann::json;

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

        return hours * 3600 + minutes * 60;
    }

    int railways()
    {
        api::URLBuilder builder(api::Endpoint::Railway);
        builder.addQuery(api::Query::Operator, api::operator_map[api::Operator::TokyoMetro]);
        std::string res = api::getRequest(builder);
        json j = json::parse(res);
        railway::DB railwayDB;
        int railwayID = 0;
        for (const auto &railway : j)
        {
            if (railway.contains("odpt:lineCode") && railway.contains("owl:sameAs"))
            {
                std::string railwayCode = railway["odpt:lineCode"].get<std::string>();
                std::string railwayLongID = railway["owl:sameAs"].get<std::string>();
                railwayDB.add({railwayID, 0}, {railwayCode, railwayLongID});
            }
            railwayID++;
        }
        railwayDB.save();
        return 0;
    }

    int stations()
    {
        railway::DB railwayDB;
        station::DB stationDB;

        for (const auto &[k, v] : railwayDB.show())
        {
            api::URLBuilder builder(api::Endpoint::Station);
            builder.addQuery(api::Query::Railway, v.long_id);
            std::string res = api::getRequest(builder);
            json j = json::parse(res);
            for (const auto &station : j)
            {
                if (station.contains("odpt:stationCode") && station.contains("owl:sameAs"))
                {
                    std::string stationCode = station["odpt:stationCode"].get<std::string>();
                    std::string stationID = station["owl:sameAs"].get<std::string>();
                    std::string stationCodeNumStr = stationCode.substr(stationCode.size() - 2);
                    int stationCodeNum = stoi(stationCodeNumStr);
                    stationDB.add({k.first, stationCodeNum}, {stationCode, stationID});
                }
            }
        }

        stationDB.save();
        return 0;
    }

    int trains()
    {
        station::DB stationDB;
        railway::DB railwayDB;
        train::DB trainDB;
        std::vector<api::Calendar> calendars = {
            api::Calendar::Weekday,
        };

        for (const auto &[k, v] : railwayDB.show())
        {
            for (const auto &calendar : calendars)
            {
                api::URLBuilder builder(api::Endpoint::TrainTimetable);
                builder.addQuery(api::Query::Railway, v.long_id);
                builder.addQuery(api::Query::Calendar, api::calendar_map[calendar]);
                std::string res = api::getRequest(builder);
                json j = json::parse(res);
                int trainID = 0;
                for (const auto &train : j)
                {
                    if (train.contains("odpt:trainNumber") && train.contains("odpt:trainTimetableObject"))
                    {
                        std::string trainNumber = train["odpt:trainNumber"].get<std::string>();
                        std::vector<train::Stop> stops;
                        json trainTimetable = train["odpt:trainTimetableObject"];
                        for (const auto &station : trainTimetable)
                        {
                            if (station.contains("odpt:departureStation") && station.contains("odpt:departureTime"))
                            {
                                std::string time = station["odpt:departureTime"].get<std::string>();
                                int seconds = timeToSeconds(time);
                                std::string stationLongID = station["odpt:departureStation"].get<std::string>();
                                const auto &[sk, sv] = stationDB.read("long", stationLongID);
                                stops.push_back({sk.first, sk.second, seconds});
                            }
                            else if (station.contains("odpt:arrivalStation") && station.contains("odpt:arrivalTime"))
                            {
                                std::string time = station["odpt:arrivalTime"].get<std::string>();
                                int seconds = timeToSeconds(time);
                                std::string stationLongID = station["odpt:arrivalStation"].get<std::string>();
                                const auto &[sk, sv] = stationDB.read("long", stationLongID);
                                stops.push_back({sk.first, sk.second, seconds});
                            }
                        }
                        trainDB.add({k.first, trainID}, {trainNumber, stops});
                    }
                    trainID++;
                }
            }
        }
        trainDB.save();
        return 0;
    }

    int main()
    {
        // check --api option
        // bool requireAPI = false;
        // bool test = false;
        // for (int i = 0; i < argc; i++)
        // {
        //     std::string arg = argv[i];
        //     if (arg == "--api" || arg == "-A")
        //     {
        //         requireAPI = true;
        //     }
        //     else if (arg == "--test" || arg == "-T")
        //     {
        //         test = true;
        //     }
        // }

        bool requireAPI = readOption("api");
        bool test = readOption("test");

        if (requireAPI)
        {
            railways();
            stations();
            trains();
        }
        if (test)
        {
            railway::test();
            station::test();
            train::test();
        }

        model::main();
        return 0;
    }

} // namespace preprocess