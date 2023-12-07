#pragma once

#include <iostream>
#include <vector>
#include "model.h"
#include <map>
#include <queue>
#include <climits>
#include <algorithm>

#include "db.h"

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

    void dijkstra()
    {
        database::railwayDB RW_DB;
        database::stationDB ST_DB;
        database::trainDB TR_DB;

        auto _rws = RW_DB.get();
        auto _sts = ST_DB.get();
        auto _trs = TR_DB.get();

        model::Manager m(_rws, _sts, _trs);
        // m.display("db");
        auto edge_map = m.getEdges();
        auto edge_s_map = m.getEdge_s();

        // generate graph
        std::map<model::station_, std::map<model::station_, model::Edge *>> graph;
        for (auto &e : edge_map)
        {
            graph[e.second->from][e.second->to] = e.second;
        }

        // initialize
        std::vector<model::time_> dist(m.stations.size(), INT_MAX); //! use node
        
    }
    int main()
    {

        // for (auto &e : edge_map)
        // {
        //     std::cout << e.first.first << " " << e.first.second << std::endl;
        // }
        // for (auto &e : edge_s_map)
        // {
        //     std::cout << e.first.first << " " << e.first.second << std::endl;
        // }

        // // 開始駅のIDと終了駅のIDを入力

        // extern model::station_ startStationId;
        // extern model::station_ endStationId;
        // std::cin >> startStationId;
        // std::cin >> endStationId;
        // // 開始駅のIDと終了駅のIDを指定して最短経路と乗車情報を表示
        // printShortestPathInfo(startStationId, endStationId);

        return 0;
    }
}