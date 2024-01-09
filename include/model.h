#pragma once

#include <climits>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

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
        train_ id;                       /**< The ID of the train. */
        std::string code;                /**< The code of the train. */
        railway_code_ rw_code;           /**< The character of the railway. */
        std::map<int, station_> stops;   /**< The stops of the train. */
        std::map<int, time_> stop_times; /**< The stop times of the train. */
    };

    struct Railway
    {
        railway_ id;                            /**< The ID of the railway. */
        std::map<station_, Station *> stations; /**< The stations of the railway. */
        std::map<train_, Train *> trains;       /**< The trains of the railway. */
    };

    struct Station
    {
        station_ id;                            /**< The ID of the station. */
        std::map<railway_, Railway *> railways; /**< The railways of the station. */
        // std::vector<Train> getTrains(train_ train_id); /**< Get the trains of the station. */
    };

    struct Train
    {
        train_ id;                    /**< The ID of the train. */
        railway_ railway_id;          /**< The ID of the railway. */
        std::vector<Station *> stops; /**< The stops of the train. */
        Train_ *train_ptr;
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
        train_ train_id;
        station_ from;       /**< The starting station of the edge. */
        station_ to;         /**< The ending station of the edge. */
        railway_ railway_id; /**< The ID of the railway. */
        // Edge_ *weight(Edge_ *prev); /**< The weight of the edge. */
    };

    struct Node
    {
        station_ id;           /**< The ID of the station. */
        time_ dist;            /**< The distance from the starting station. */
        station_ prev_station; /**< The previous edge. */
        train_ prev_train_id;  /**< The ID of the previous train. */
        bool operator>(const Node &rhs) const
        {
            return dist > rhs.dist;
        }
    };

    // Data Manager - generate some data from 3 essential data
    class DataManager
    {

    public:
        std::map<railway_, Railway_> railway_map;
        std::map<station_, Station_> station_map;
        std::map<train_, Train_> train_map;

        std::map<railway_, Railway *> railways;
        std::map<station_, Station *> stations;
        std::map<train_, Train *> trains;
        // std::map<std::pair<station_, station_>, std::vector<Edge_ *>> edge_s_map;
        std::map<std::pair<station_, station_>, std::map<train_, Edge_ *>> edge_s_map;
        std::map<std::pair<station_, station_>, Edge *> edge_map;

    private:
        DataManager(
            std::map<railway_, Railway_> &railway_map,
            std::map<station_, Station_> &station_map,
            std::map<train_, Train_> &train_map)
            : railway_map(railway_map),
              station_map(station_map),
              train_map(train_map)
        {
            initialize_basic_data();
            initialize_walk_edges();
            initialize_edges();
        }
        DataManager(const DataManager &) = delete;
        DataManager &operator=(const DataManager &) = delete;
        static DataManager *instance;

    protected:
        virtual ~DataManager()
        {
            for (auto &rw : railways)
            {
                delete rw.second;
            }
            for (auto &st : stations)
            {
                delete st.second;
            }
            for (auto &tr : trains)
            {
                delete tr.second;
            }
            for (auto &e : edge_map)
            {
                delete e.second;
            }
            for (auto &e : edge_s_map)
            {
                for (auto &e_ : e.second)
                {
                    delete e_.second;
                }
            }
        };

    public:
        static DataManager &getInstance(
            std::map<railway_, Railway_> &railway_map,
            std::map<station_, Station_> &station_map,
            std::map<train_, Train_> &train_map)
        {
            if (instance == nullptr)
            {
                instance = new DataManager(railway_map, station_map, train_map);
            }
            return *instance;
        }
        static DataManager &getInstance()
        {
            if (instance == nullptr)
            {
                throw std::runtime_error("DataManager has not been initialized.");
            }
            return *instance;
        }
        void initialize_basic_data()
        {
            // create Box of Railway, Station, Train
            for (auto &rw : railway_map)
            {
                Railway *r = new Railway;
                r->id = rw.first;
                railways[rw.first] = r;
            }
            for (auto &st : station_map)
            {
                Station *s = new Station;
                s->id = st.first;
                stations[st.first] = s;
            }
            for (auto &tr : train_map)
            {
                Train *t = new Train;
                t->id = tr.first;
                trains[tr.first] = t;
            }
            // initialize references
            for (auto &rw : railway_map)
            {
                Railway *r = railways[rw.first];
                for (auto &st : station_map)
                {
                    if (rw.second.rw_code == st.second.rw_code)
                    {
                        Station *s = stations[st.first];
                        r->stations[st.first] = s; // add station to railway
                        s->railways[rw.first] = r; // add railway to station
                    }
                }
                for (auto &tr : train_map)
                {
                    if (rw.second.rw_code == tr.second.rw_code)
                    {
                        Train *t = trains[tr.first];
                        r->trains[tr.first] = t; // add train to railway
                        t->railway_id = r->id;
                    }
                }
            }
            for (auto &st : station_map)
            {
                Station *s = stations[st.first];
                for (auto &tr : train_map)
                {
                    for (auto &stop : tr.second.stops)
                    {
                        if (stop.second == s->id)
                        {
                            Train *t = trains[tr.first];
                            t->stops.push_back(s); // add station to train
                        }
                    }
                }
            }
        }
        void initialize_edges()
        {
            for (auto &tr : train_map)
            {
                Train *t = trains[tr.first];
                for (int i = 0; i < t->stops.size() - 1; i++)
                {
                    std::pair<station_, station_> key = std::make_pair(tr.second.stops[i], tr.second.stops[i + 1]);
                    if (edge_map.find(key) == edge_map.end())
                    {
                        Edge *e = new Edge;
                        e->train_id = t->id;
                        e->from = key.first;
                        e->to = key.second;
                        e->railway_id = t->railway_id;
                        edge_map[key] = e;
                    }
                    Edge_ *e_ = new Edge_;
                    e_->train_id = t->id;
                    e_->from = key.first;
                    e_->to = key.second;
                    e_->departure = tr.second.stop_times[i];
                    e_->arrival = tr.second.stop_times[i + 1];
                    edge_s_map[key][t->id] = e_;
                }
            }
        }
        void initialize_walk_edges()
        {
            // extract last part of Station_.name by using regex
            // ex: `odpt.Station:Xtrains.alpha.center` -> `center`
            std::map<station_, std::string> station_name_map;
            for (auto &st : station_map)
            {
                std::regex re(".*\\.(.*)");
                std::smatch match;
                std::regex_match(st.second.name, match, re);
                station_name_map[st.first] = match[1];
            }
            // find same station name & add to edges
            // note: use 2d for loop to find all edges
            for (auto &st1 : station_name_map)
            {
                for (auto &st2 : station_name_map)
                {
                    if (st1.second == st2.second && st1.first != st2.first)
                    {
                        std::pair<station_, station_> key = std::make_pair(st1.first, st2.first);
                        if (edge_map.find(key) == edge_map.end())
                        {
                            Edge *e = new Edge;
                            e->train_id = -2;
                            e->from = st1.first;
                            e->to = st2.first;
                            e->railway_id = -2;
                            edge_map[key] = e;
                        }
                        Edge_ *e_ = new Edge_;
                        e_->train_id = -2;
                        e_->from = st1.first;
                        e_->to = st2.first;
                        e_->departure = -1;
                        e_->arrival = -1;
                        edge_s_map[key][-1] = e_;
                    }
                }
            }
        }
        // for debug
        void display(std::string prefix)
        {
            std::ofstream rw_ofs("data/" + prefix + "_railway.txt");
            for (auto &rw : railways)
            {
                rw_ofs << "rw-" << rw.second->id << std::endl;
                for (auto &st : rw.second->stations)
                {
                    rw_ofs << "  st-" << st.second->id << std::endl;
                }
                for (auto &tr : rw.second->trains)
                {
                    rw_ofs << "  tr-" << tr.second->id << std::endl;
                }
            }

            std::ofstream edge_ofs("data/" + prefix + "_edge.txt");
            for (auto &e : edge_s_map)
            {
                edge_ofs << "Edge: " << e.first.first << " " << e.first.second << std::endl;
                edge_ofs << "  Railway: " << edge_map[e.first]->railway_id << std::endl;
                for (auto &e_ : e.second)
                {
                    edge_ofs << "  "
                             << "tr-" << e_.second->train_id << " "
                             << "d-" << e_.second->departure << " "
                             << "a-" << e_.second->arrival << std::endl;
                }
            }

            std::ofstream st_ofs("data/" + prefix + "_station.txt");
            for (auto &st : station_map)
            {
                st_ofs << "st-" << st.first << " "
                       << "rw-" << st.second.rw_st_num << " "
                       << "name-" << st.second.name << std::endl;
            }

            int edge_count = 0;
            for (auto &e : edge_s_map)
            {
                edge_count += e.second.size();
            }

            std::ofstream ofs("data/" + prefix + "_edge_.txt");
            for (auto &e : edge_s_map)
            {
                ofs << "(" << e.first.first << "," << e.first.second << "): ";
                for (auto &e_ : e.second)
                {
                    ofs << e_.second->train_id << ",";
                }
                ofs << std::endl;
            }

            std::cout << "--------------------------" << std::endl;
            std::cout << "RailwayMapCount: " << railway_map.size() << std::endl;
            std::cout << "StationMapCount: " << station_map.size() << std::endl;
            std::cout << "TrainMapCount: " << train_map.size() << std::endl;
            std::cout << std::endl;
            std::cout << "RailwayCount: " << railways.size() << std::endl;
            std::cout << "StationCount: " << stations.size() << std::endl;
            std::cout << "TrainCount: " << trains.size() << std::endl;
            std::cout << std::endl;
            std::cout << "Edges_Count: " << edge_map.size() << std::endl;
            std::cout << "Edge_s_Count: " << edge_count << std::endl;
            std::cout << "--------------------------" << std::endl
                      << std::endl;
        };
    };
} // namespace model