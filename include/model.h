#pragma once
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

namespace model
{
    typedef int time_; /**< seconds since 00:00:00. */

    typedef int railway_; /**< Railway Number. Automatically generated. */
    typedef int station_; /**< Station Number. Automatically generated. */
    typedef int train_;   /**< Train Number. Automatically generated. */

    typedef std::string railway_code_;   /**< Railway Code. */
    typedef int railway_station_number_; /**< Railway Station Number. */

    struct Railway;
    struct Station;
    struct Train;

    struct Railway_
    {
        railway_ id;           /**< The ID of the railway. */
        railway_code_ rw_code; /**< The character of the railway. */
        std::string name;      /**< The name of the railway. */
    };

    struct Station_
    {
        station_ id;                       /**< The ID of the station. */
        railway_code_ rw_code;             /**< The character of the railway. */
        railway_station_number_ rw_st_num; /**< The number of the station in the railway. */
        std::string name;                  /**< The name of the station. */
    };

    struct Train_
    {
        train_ id;                                      /**< The ID of the train. */
        std::string code;                               /**< The code of the train. */
        railway_code_ rw_code;                          /**< The character of the railway. */
        std::map<railway_station_number_, time_> stops; /**< The stops of the train. */
    };

    struct Railway
    {
        railway_ id;                     /**< The ID of the railway. */
        std::vector<Station *> stations; /**< The stations of the railway. */
        std::vector<Train *> trains;     /**< The trains of the railway. */
    };

    struct Station
    {
        station_ id;                     /**< The ID of the station. */
        std::vector<Railway *> railways; /**< The railways of the station. */
        // std::vector<Train> getTrains(train_ train_id); /**< Get the trains of the station. */
    };

    struct Train
    {
        train_ id;                    /**< The ID of the train. */
        railway_ railway_id;          /**< The ID of the railway. */
        std::vector<Station *> stops; /**< The stops of the train. */
    };

    struct Edge_
    {
        train_ train_id; /**< The ID of the train. */
        station_ from;   /**< The starting station of the edge. */
        station_ to;     /**< The ending station of the edge. */
        time_ departure; /**< The departure time of the edge. */
        time_ arrival;   /**< The arrival time of the edge. */
    };

    struct Edge
    {
        station_ from;                    /**< The starting station of the edge. */
        station_ to;                      /**< The ending station of the edge. */
        railway_ railway_id;              /**< The ID of the railway. */
        time_ weight(time_ prev_arrival); /**< The weight of the edge. */
    };

    struct Node
    {
        station_ id;           /**< The ID of the station. */
        time_ dist;            /**< The distance from the starting station. */
        railway_ prev_railway; /**< The railway number of the previous edge. */
        bool operator>(const Node &rhs) const
        {
            return dist > rhs.dist;
        }
    };

    class Manager
    {
    protected:
        std::vector<Station *> stations;
        std::vector<Railway *> railways;
        std::vector<Train *> trains;

    public:
        Manager(
            std::map<railway_, Railway_> railway_map,
            std::map<station_, Station_> station_map,
            std::map<train_, Train_> train_map)
        {
            // initialize stations, railways, trains
            for (auto &i : railway_map)
            {
                Railway *rw = new Railway;
                rw->id = i.second.id;
                railways.push_back(rw);
            }
            for (auto &i : station_map)
            {
                Station *st = new Station;
                st->id = i.second.id;
                stations.push_back(st);
            }
            for (auto &i : train_map)
            {
                Train *tr = new Train;
                tr->id = i.second.id;
                trains.push_back(tr);
            }
            // initialize references
            for (auto &i : railway_map)
            {
                Railway *rw = railways[i.second.id];
                for (auto &j : station_map)
                {
                    if (j.second.rw_code == i.second.rw_code)
                    {
                        Station *st = stations[j.second.id];
                        rw->stations.push_back(st); // add station to railway
                        st->railways.push_back(rw); // add railway to station
                    }
                }
                for (auto &j : train_map)
                {
                    if (j.second.rw_code == i.second.rw_code)
                    {
                        Train *tr = trains[j.second.id];
                        rw->trains.push_back(tr); // add train to railway
                        tr->railway_id = rw->id;
                    }
                }
            }
            for (auto &i : station_map)
            {
                Station *st = stations[i.second.id];
                for (auto &j : train_map)
                {
                    if (j.second.rw_code == i.second.rw_code)
                    {
                        Train *tr = trains[j.second.id];
                        tr->stops.push_back(st); // add station to train
                    }
                }
            }
        };
        ~Manager()
        {
            for (auto &i : stations)
            {
                delete i;
            }
            for (auto &i : railways)
            {
                delete i;
            }
            for (auto &i : trains)
            {
                delete i;
            }
        };
        std::vector<Station *> getStations()
        {
            return stations;
        };
        std::vector<Railway *> getRailways()
        {
            return railways;
        };
        std::vector<Train *> getTrains()
        {
            return trains;
        };

        void display()
        {
            std::cout << "Stations:" << std::endl;
            for (auto &i : stations)
            {
                std::cout << i->id << std::endl;
            }
            std::cout << "Railways:" << std::endl;
            for (auto &i : railways)
            {
                std::cout << i->id << std::endl;
            }
            std::cout << "Trains:" << std::endl;
            for (auto &i : trains)
            {
                std::cout << i->id << std::endl;
            }
        };
    };

    extern std::map<railway_, Railway_> __test__Railway_s;
    extern std::map<station_, Station_> __test__Station_s;
    extern std::map<train_, Train_> __test__Train_s;

    // int main();
} // namespace model