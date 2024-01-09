#include "../include/preprocess.h"

namespace preprocess_v2
{
    using json = nlohmann::json;

    int main()
    {
        // 1. get railway data as json from API
        //   - create RailwayDB
        //   - create API query for StationDB & TrainDB
        // 2. get station data as json from API
        //   - create StationDB
        // 3. get train data as json from API
        //   - create TrainDB
        using namespace model;
        using namespace database_v3;

        auto RailwayDB = Database<railway_, Railway_>::createDB();
        auto StationDB = Database<station_, Station_>::createDB();
        auto TrainDB = Database<train_, Train_>::createDB();

        // 1. get railway data as json from API
        api::URLBuilder builder(api::Endpoint::Railway);
        builder.addQuery(api::Query::Operator, api::operator_map[api::Operator::TokyoMetro]);
        std::string res = api::getRequest(builder);
        json j = json::parse(res);
        railway_ rw_id_counter = 0;
        for (const auto &RW : j)
        {
            if (RW.contains("odpt:lineCode") && RW.contains("owl:sameAs"))
            {
                railway_code_ rw_code = RW["odpt:lineCode"].get<std::string>();
                std::string name = RW["owl:sameAs"].get<std::string>();
                RailwayDB->add(rw_id_counter, {rw_id_counter, rw_code, name});
            }
            rw_id_counter++;
        }
        RailwayDB->save();

        // 2. get station data as json from API
        station_ st_id_counter = 0;
        for (const auto &[k, v] : RailwayDB->data)
        {
            api::URLBuilder builder(api::Endpoint::Station);
            builder.addQuery(api::Query::Railway, v.name);
            std::string res = api::getRequest(builder);
            json j = json::parse(res);
            for (const auto &ST : j)
            {
                if (ST.contains("odpt:stationCode") && ST.contains("owl:sameAs"))
                {
                    std::string st_code = ST["odpt:stationCode"].get<std::string>();
                    railway_code_ rw_code = std::regex_replace(st_code, std::regex("\\d+"), "");
                    railway_station_number_ rw_st_num = std::stoi(std::regex_replace(st_code, std::regex("\\D+"), ""));
                    std::string name = ST["owl:sameAs"].get<std::string>();
                    StationDB->add(st_id_counter, {st_id_counter, rw_code, rw_st_num, name});
                }
                st_id_counter++;
            }
        }
        StationDB->save();

        // 3. get train data as json from API
        std::vector<api::Calendar> calendars = {
            api::Calendar::Weekday,
        };
        train_ tr_id_counter = 0;
        for (const auto &[k, v] : RailwayDB->data)
        {
            for (const auto &calendar : calendars)
            {
                api::URLBuilder builder(api::Endpoint::TrainTimetable);
                builder.addQuery(api::Query::Railway, v.name);
                builder.addQuery(api::Query::Calendar, api::calendar_map[calendar]);
                std::string res = api::getRequest(builder);
                json j = json::parse(res);
                for (const auto &TR : j)
                {
                    if (TR.contains("odpt:trainNumber") && TR.contains("odpt:trainTimetableObject"))
                    {
                        std::string code = TR["odpt:trainNumber"].get<std::string>();
                        int stops_counter = 0;
                        std::map<int, station_> stops;
                        std::map<int, time_> stop_times;
                        json trainTimetable = TR["odpt:trainTimetableObject"];
                        for (const auto &station : trainTimetable)
                        {
                            if (station.contains("odpt:departureStation") && station.contains("odpt:departureTime"))
                            {
                                std::string st_name = station["odpt:departureStation"].get<std::string>();
                                std::string time_str = station["odpt:departureTime"].get<std::string>();
                                time_ time = timeToSeconds(time_str);
                                station_ st_id;
                                for (const auto &[k, v] : StationDB->data)
                                {
                                    if (v.name == st_name)
                                    {
                                        st_id = k;
                                        break;
                                    }
                                }
                                stops[stops_counter] = st_id;
                                stop_times[stops_counter] = time;
                            }
                            else if (station.contains("odpt:arrivalStation") && station.contains("odpt:arrivalTime"))
                            {
                                std::string st_name = station["odpt:arrivalStation"].get<std::string>();
                                std::string time_str = station["odpt:arrivalTime"].get<std::string>();
                                time_ time = timeToSeconds(time_str);
                                station_ st_id;
                                for (const auto &[k, v] : StationDB->data)
                                {
                                    if (v.name == st_name)
                                    {
                                        st_id = k;
                                        break;
                                    }
                                }
                                stops[stops_counter] = st_id;
                                stop_times[stops_counter] = time;
                            }
                            stops_counter++;
                        }
                        TrainDB->add(tr_id_counter, {tr_id_counter, code, v.rw_code, stops, stop_times});
                    }
                    tr_id_counter++;
                }
            }
        }
        TrainDB->save();

        DataManager &dm = DataManager::getInstance(RailwayDB->data, StationDB->data, TrainDB->data);
        dm.display("db");

        return 0;
    }

    int test()
    {
        using namespace model;
        using namespace database_v3;

        auto RailwayDB = Database<railway_, Railway_>::loadDB();
        auto StationDB = Database<station_, Station_>::loadDB();
        auto TrainDB = Database<train_, Train_>::loadDB();

        DataManager &dm = DataManager::getInstance(RailwayDB->data, StationDB->data, TrainDB->data);
        dm.display("db");

        return 0;
    }
} // namespace preprocess_v2