#pragma once
#include <vector>
#include <string>

#include "db.h"

namespace model
{
    typedef int time_; /**< seconds since 00:00:00. */

    typedef int railway_; /**< Railway Number. Automatically generated. */
    typedef int station_; /**< Station Number. Automatically generated. */
    typedef int train_;   /**< Train Number. Automatically generated. */

    typedef railway_ railwayKey;                      /**< The type of a railway key. */
    typedef std::pair<railway_, station_> stationKey; /**< The type of a station key. */
    typedef std::pair<railway_, train_> trainKey;     /**< The type of a train key. */

    struct Railway;
    struct Station;
    struct Train;

    struct Railway_
    {
        railway_ id;         /**< The ID of the railway. */
        std::string char_id; /**< The character of the railway. */
        std::string name;    /**< The name of the railway. */
    };

    struct Station_
    {
        station_ id;         /**< The ID of the station. */
        std::string char_id; /**< The character of the station. */
        std::string name;    /**< The name of the station. */
    };

    struct Train_
    {
        train_ id;        /**< The ID of the train. */
        std::string code; /**< The code of the train. */
        // std::vector<Stop> stops; /**< The stops of the train. */
    };

    struct Railway
    {
        railway_ id;                    /**< The ID of the railway. */
        std::vector<Station> &stations; /**< The stations of the railway. */
        std::vector<Train> &trains;     /**< The trains of the railway. */
    };

    struct Station
    {
        station_ id;                    /**< The ID of the station. */
        std::vector<Railway> &railways; /**< The railways of the station. */
        // std::vector<Train> getTrains(train_ train_id); /**< Get the trains of the station. */
    };

    struct Train
    {
        train_ id;                   /**< The ID of the train. */
        railway_ railway_id;         /**< The ID of the railway. */
        std::vector<Station> &stops; /**< The stops of the train. */
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

    int main();
} // namespace model