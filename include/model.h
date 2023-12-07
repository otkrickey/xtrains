#pragma once

#include <fstream>
#include <iostream>
#include <map>
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
        std::map<station_, time_> stops; /**< The stops of the train. */
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
        station_ from;                    /**< The starting station of the edge. */
        station_ to;                      /**< The ending station of the edge. */
        railway_ railway_id;              /**< The ID of the railway. */
        time_ weight(time_ prev_arrival) const; /**< The weight of the edge. */
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
        std::map<railway_, Railway *> railways;
        std::map<station_, Station *> stations;
        std::map<train_, Train *> trains;
        std::map<std::pair<station_, station_>, std::vector<Edge_ *>> edge_s_map;
        std::map<std::pair<station_, station_>, Edge *> edge_map;

    public:
        Manager(
            std::map<railway_, Railway_> &railway_map,
            std::map<station_, Station_> &station_map,
            std::map<train_, Train_> &train_map)
        {
            // Basic 3 models: Railway, Station, Train
            // initialize railways, stations, trains
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
                        if (stop.first == s->id)
                        {
                            Train *t = trains[tr.first];
                            t->stops.push_back(s); // add station to train
                        }
                    }
                }
            }

            // initialize edges
            for (auto &tr : train_map)
            {
                Train *t = trains[tr.first];
                for (int i = 0; i < t->stops.size() - 1; i++)
                {
                    std::pair<station_, station_> key = std::make_pair(t->stops[i]->id, t->stops[i + 1]->id);
                    if (edge_map.find(key) == edge_map.end())
                    {
                        Edge *e = new Edge;
                        e->from = t->stops[i]->id;
                        e->to = t->stops[i + 1]->id;
                        e->railway_id = t->railway_id;
                        edge_map[key] = e;
                    }
                    Edge_ *e_ = new Edge_;
                    e_->train_id = t->id;
                    e_->from = t->stops[i]->id;
                    e_->to = t->stops[i + 1]->id;
                    e_->departure = tr.second.stops[t->stops[i]->id];
                    e_->arrival = tr.second.stops[t->stops[i + 1]->id];
                    edge_s_map[key].push_back(e_);
                }
            }
        };
        virtual ~Manager()
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
                    delete e_;
                }
            }
        };
        std::map<station_, Station *> getStations()
        {
            return stations;
        };
        std::map<railway_, Railway *> getRailways()
        {
            return railways;
        };
        std::map<train_, Train *> getTrains()
        {
            return trains;
        };
        std::map<std::pair<station_, station_>, Edge *> getEdges()
        {
            return edge_map;
        };
        std::map<std::pair<station_, station_>, std::vector<Edge_ *>> getEdge_s()
        {
            return edge_s_map;
        };

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
                             << "tr-" << e_->train_id << " "
                             << "d-" << e_->departure << " "
                             << "a-" << e_->arrival << std::endl;
                }
            }
        };
    };

    // test data
    extern std::map<railway_, Railway_> __test__Railway_s;
    extern std::map<station_, Station_> __test__Station_s;
    extern std::map<train_, Train_> __test__Train_s;
} // namespace model