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

    int main()
    {
        // save to DB
        database::railwayDB RW_DB;
        database::stationDB ST_DB;
        database::trainDB TR_DB;

        // get railway data as json
        api::URLBuilder builder(api::Endpoint::Railway);
        builder.addQuery(api::Query::Operator, api::operator_map[api::Operator::TokyoMetro]);
        std::string res = api::getRequest(builder);
        json j = json::parse(res);

        // add railway data to DB
        model::railway_ railway_id = 0;
        for (const auto &RW : j)
        {
            if (RW.contains("odpt:lineCode") && RW.contains("owl:sameAs"))
            {
                model::railway_code_ rw_code = RW["odpt:lineCode"].get<std::string>();
                std::string name = RW["owl:sameAs"].get<std::string>();
                RW_DB.add({railway_id, rw_code, name});
            }
            railway_id++;
        }
        RW_DB.save();

        // get stations data as json
        model::station_ station_id = 0;
        for (const auto &[k, v] : RW_DB.get())
        {
            // get station data as json
            api::URLBuilder builder(api::Endpoint::Station);
            builder.addQuery(api::Query::Railway, v.name);
            std::string res = api::getRequest(builder);
            json j = json::parse(res);

            // add station data to DB
            for (const auto &ST : j)
            {
                if (ST.contains("odpt:stationCode") && ST.contains("owl:sameAs"))
                {
                    std::string st_code = ST["odpt:stationCode"].get<std::string>();
                    model::railway_code_ rw_code = std::regex_replace(st_code, std::regex("\\d+"), "");
                    model::railway_station_number_ rw_st_num = std::stoi(std::regex_replace(st_code, std::regex("\\D+"), ""));
                    std::string name = ST["owl:sameAs"].get<std::string>();
                    ST_DB.add({station_id, rw_code, rw_st_num, name});
                }
                station_id++;
            }
        }
        ST_DB.save();

        // get trains data as json
        std::vector<api::Calendar> calendars = {
            api::Calendar::Weekday,
        };
        model::train_ train_id = 0;
        for (const auto &[k, v] : RW_DB.get())
        {
            for (const auto &calendar : calendars)
            {
                // get train data as json
                api::URLBuilder builder(api::Endpoint::TrainTimetable);
                builder.addQuery(api::Query::Railway, v.name);
                builder.addQuery(api::Query::Calendar, api::calendar_map[calendar]);
                std::string res = api::getRequest(builder);
                json j = json::parse(res);

                // add train data to DB
                for (const auto &TR : j)
                {
                    if (TR.contains("odpt:trainNumber") && TR.contains("odpt:trainTimetableObject"))
                    {
                        std::string code = TR["odpt:trainNumber"].get<std::string>();
                        std::map<model::station_, model::time_> stops;
                        json trainTimetable = TR["odpt:trainTimetableObject"];
                        for (const auto &station : trainTimetable)
                        {
                            if (station.contains("odpt:departureStation") && station.contains("odpt:departureTime"))
                            {
                                std::string st_name = station["odpt:departureStation"].get<std::string>();
                                std::string time_str = station["odpt:departureTime"].get<std::string>();
                                model::time_ time = timeToSeconds(time_str);
                                model::station_ st_id = ST_DB.get("name", st_name).id;
                                stops[st_id] = time;
                            }
                            else if (station.contains("odpt:arrivalStation") && station.contains("odpt:arrivalTime"))
                            {
                                std::string st_name = station["odpt:arrivalStation"].get<std::string>();
                                std::string time_str = station["odpt:arrivalTime"].get<std::string>();
                                model::time_ time = timeToSeconds(time_str);
                                model::station_ st_id = ST_DB.get("name", st_name).id;
                                stops[st_id] = time;
                            }
                        }
                        TR_DB.add({train_id, code, v.rw_code, stops});
                    }
                    train_id++;
                }
                j.clear();
            }
        }
        TR_DB.save();

        // display count
        std::cout << "Railways: " << railway_id << std::endl;
        std::cout << "Stations: " << station_id << std::endl;
        std::cout << "Trains: " << train_id << std::endl;

        return 0;
    }
    int test()
    {
        // load from DB
        database::railwayDB RW_DB;
        database::stationDB ST_DB;
        database::trainDB TR_DB;

        auto _rws = RW_DB.get();
        auto _sts = ST_DB.get();
        auto _trs = TR_DB.get();

        // generate cross-reference models
        model::Manager m(_rws, _sts, _trs);
        return 0;
    }
} // namespace preprocess