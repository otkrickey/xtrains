#include "../include/dijkstra.h"

namespace dijkstra
{
    // 構造体と型定義（省略）

    // グローバルな駅、鉄道、電車のリスト
    // std::vector<model::Station> allStations;
    // std::vector<model::Railway> allRailways;
    // std::vector<model::Train> allTrains;

    // // 最短経路を見つけるためのダイクストラ法
    // std::map<std::pair<model::station_, model::railway_>, std::pair<model::time_, model::train_>> dijkstra(model::station_ startStationId)
    // {
    //     std::map<std::pair<model::station_, model::railway_>, std::pair<model::time_, model::train_>> shortestPaths;
    //     std::priority_queue<model::Node, std::vector<model::Node>, std::greater<model::Node>> pq;

    //     // 初期化
    //     for (model::Station &station : allStations)
    //     {
    //         if (station.id == startStationId)
    //         {
    //             pq.push({station.id, 0, 0});
    //             shortestPaths[{station.id, 0}] = {0, -1};
    //         }
    //         else
    //         {
    //             shortestPaths[{station.id, 0}] = {INT_MAX, -1};
    //         }
    //     }

    // ダイクストラ法
    //     while (!pq.empty())
    //     {
    //         model::Node currentNode = pq.top();
    //         pq.pop();

    //         if (currentNode.dist > shortestPaths[{currentNode.id, currentNode.prev_railway}].first)
    //         {
    //             continue;
    //         }

    //         model::Station &currentStation = allStations[currentNode.id];

    //          for (model::Railway *railway : currentStation.railways)
    //         {
    //             for (model::Train *train : railway->trains)
    //             {
    //                 auto it = std::find(train->stops.begin(), train->stops.end(), &currentStation);
    //                 if (it != train->stops.end())
    //                 {
    //                     model::time_ departureTime = train->train_ptr->stops[currentStation.id];
    //                     model::time_ arrivalTime = currentNode.dist + departureTime;
    //                     model::Station *nextStation = nullptr;

    //                     for (model::Station *stop : train->stops)
    //                     {
    //                         if (stop->id != currentStation.id)
    //                         {
    //                             nextStation = stop;
    //                             break;
    //                         }
    //                     }

    //                     if (nextStation)
    //                     {
    //                         int weight = arrivalTime;
    //                         std::pair<model::station_, model::railway_> nextStationKey = {nextStation->id, railway->id};

    //                         if (weight < shortestPaths[nextStationKey].first)
    //                         {
    //                             shortestPaths[nextStationKey] = {weight, train->id};
    //                             pq.push({nextStation->id, weight, railway->id});
    //                         }
    //                     }
    //                 }
    //             }
    //         }
    //     }
    //     while (!pq.empty())
    //     {
    //         model::Node currentNode = pq.top();
    //         pq.pop();

    //         if (currentNode.dist > shortestPaths[{currentNode.id, currentNode.prev_railway}].first)
    //         {
    //             continue;
    //         }

    //         model::Station &currentStation = allStations[currentNode.id];

    //         for (model::Railway *railway : currentStation.railways)
    //         {
    //         }

    //     }
    //             return shortestPaths;
    // }

    // // 結果を表示する関数
    // void printShortestPathInfo(model::station_ startStationId, model::station_ endStationId)
    // {
    //     std::map<std::pair<model::station_, model::railway_>, std::pair<model::time_, model::train_>> shortestPaths = dijkstra(startStationId);
    //     std::pair<model::station_, model::railway_> currentKey = {endStationId, 0};

    //     if (shortestPaths.find(currentKey) == shortestPaths.end())
    //     {
    //         std::cout << "There is no path from start station to end station." << std::endl;
    //         return;
    //     }

    //     std::vector<model::train_> path;
    //     std::vector<model::time_> arrivalTimes;

    //     while (currentKey.first != startStationId || currentKey.second != 0)
    //     {
    //         path.push_back(shortestPaths[currentKey].second);
    //         arrivalTimes.push_back(shortestPaths[currentKey].first);
    //         currentKey.first = allTrains[shortestPaths[currentKey].second].railway_id;
    //         currentKey.second = 0;
    //     }

    //     std::cout << "Shortest path from station " << startStationId << " to station " << endStationId << ":\n";

    //     for (int i = path.size() - 1; i >= 0; --i)
    //     {
    //         std::cout << "Take train " << path[i] << " and arrive at time " << arrivalTimes[i] << std::endl;
    //     }
    // }

    ShortestPath dijkstra(
        Graph &graph,
        model::station_ startStationId,
        model::station_ endStationId,
        model::time_ startTime)
    {
        using namespace model;
        ShortestPath shortestPaths;
        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
        auto &dm = DataManager::getInstance();

        // 初期化
        for (auto &station : dm.station_map)
        {
            if (station.second.id == startStationId)
            {
                pq.push({station.second.id, startTime, -1, -1});
                shortestPaths[station.second.id] = new Edge_({-1, -1, startStationId, -1, startTime});
            }
            else
            {
                shortestPaths[station.second.id] = new Edge_({-1, -1, station.second.id, -1, INT_MAX});
            }
        }

        // ダイクストラ法
        while (!pq.empty())
        {
            Node currentNode = pq.top();
            pq.pop();

            if (currentNode.dist > shortestPaths[currentNode.id]->arrival)
            {
                continue;
            }

            Station *currentStation = dm.stations[currentNode.id];
            Edge_ *prevEdge = dm.edge_s_map[{currentNode.prev_station, currentNode.id}][currentNode.prev_train_id];
            if (prevEdge == nullptr)
            {
                prevEdge = new Edge_({-1, -1, currentNode.prev_station, currentNode.id, currentNode.dist});
            }

            for (auto &edge : graph[currentNode.id])
            {
                std::pair<station_, station_> key = {edge.second->from, edge.second->to};
                time_ prev_arrival = prevEdge->arrival;
                time_ min = INT_MAX;
                Edge_ *min_edge = nullptr;
                time_ transfer_time = 4 * 60;
                prev_arrival += transfer_time;
                for (auto &edge_ : dm.edge_s_map[key])
                {
                    if (edge_.second->train_id == prevEdge->train_id)
                    {
                        min_edge = edge_.second;
                        break;
                    }
                    else if (edge_.second->departure > prev_arrival)
                    {
                        if (edge_.second->departure - prev_arrival < min)
                        {
                            min = edge_.second->departure - prev_arrival;
                            min_edge = edge_.second;
                        }
                    }
                }
                // time_ departureTime = edge.second->from;
                // time_ arrivalTime = currentNode.dist + departureTime;
                // Station_ *nextStation = &dm.station_map[edge.second->to];

                // int weight = arrivalTime;
                // station_ nextStationId = nextStation->id;

                // if (weight < shortestPaths[nextStationId].arrival)
                // {
                //     shortestPaths[nextStationId] = {weight, edge.second->train_id};
                //     pq.push({nextStationId, weight, edge.second->train_id});
                // }
                // using ShortestPath = std::map<model::station_, model::Edge_ *>;
                shortestPaths[edge.second->to] = min_edge;
            }
        }
        return shortestPaths;
    }

    int main()
    {
        using namespace model;
        auto RailwayDB = database_v3::Database<railway_, Railway_>::loadDB();
        auto StationDB = database_v3::Database<station_, Station_>::loadDB();
        auto TrainDB = database_v3::Database<train_, Train_>::loadDB();

        DataManager &dm = DataManager::getInstance(RailwayDB->data, StationDB->data, TrainDB->data);
        dm.display("db");

        // 開始駅のIDと終了駅のIDを入力

        station_ startStationId = 6;
        station_ endStationId = 90;
        time_ startTime = 18 * 60 * 60;

        Graph graph;
        for (auto &e : dm.edge_map)
        {
            graph[e.second->from][e.second->to] = e.second;
        }

        ShortestPath shortestPaths = dijkstra(graph, startStationId, endStationId, startTime);

        // 結果を表示
        std::cout << "Shortest path from station " << startStationId << " to station " << endStationId << ":\n";

        for (auto &path : shortestPaths)
        {
            if (path.second->train_id != -1)
            {
                std::cout << "tr-" << path.second->train_id << " st-" << path.second->to << " time " << path.second->arrival << std::endl;
            }
        }
        return 0;
    }
}
