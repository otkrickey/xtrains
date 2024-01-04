#include "../include/dijkstra.h"

namespace dijkstra
{
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
            std::string currentStationName = dm.station_map[currentNode.id].name;
            std::map<train_, Edge_ *> prevEdges = dm.edge_s_map[{currentNode.prev_station, currentNode.id}];
            Edge_ *prevEdge =
                prevEdges.find(currentNode.prev_train_id) != prevEdges.end()
                    ? prevEdges[currentNode.prev_train_id]
                : currentNode.prev_train_id == -2
                    ? new Edge_({-2, currentNode.prev_station, currentNode.id, -2, currentNode.dist})
                    : new Edge_({-1, -1, currentNode.id, -1, currentNode.dist});

            // !FOR DEBUG!
            // std::cout << "currentNode: " << currentStationName
            //           << " prevEdge: " << prevEdge->from << " -> " << prevEdge->to
            //           << " train: " << prevEdge->train_id
            //           << " arrival: " << prevEdge->arrival / 3600 << ":" << (prevEdge->arrival % 3600) / 60 << ":" << prevEdge->arrival % 60
            //           << " departure: " << prevEdge->departure / 3600 << ":" << (prevEdge->departure % 3600) / 60 << ":" << prevEdge->departure % 60
            //           << std::endl;

            for (auto &edge : graph[currentNode.id])
            {
                std::pair<station_, station_> key = {edge.second->from, edge.second->to};
                time_ prev_arrival = prevEdge->arrival;
                time_ min = INT_MAX;
                Edge_ *min_edge_ = nullptr;
                time_ transfer_time = 4 * 60;
                for (auto &edge_ : dm.edge_s_map[key])
                {
                    time_ _min = edge_.second->departure - prev_arrival;

                    // case 1: walk
                    if (edge_.second->train_id == -2)
                    {
                        // use below line if you want to use adaptive transfer time
                        // min = edge_.second->departure - edge_.second->arrival;
                        min = transfer_time;
                        min_edge_ = new Edge_({-2, edge_.second->from, edge_.second->to, currentNode.dist, currentNode.dist + transfer_time});
                        break;
                    }

                    // case 2: same train
                    if (edge_.second->train_id == prevEdge->train_id)
                    {
                        min = _min;
                        min_edge_ = edge_.second;
                        break;
                    }

                    // case 3: transfer
                    if (min > _min && _min > 0)
                    {
                        min = _min;
                        min_edge_ = edge_.second;
                    }
                }

                if (min_edge_ != nullptr && min_edge_->arrival < shortestPaths[edge.second->to]->arrival)
                {
                    shortestPaths[edge.second->to] = min_edge_;
                    pq.push({edge.second->to, min_edge_->arrival, edge.second->from, edge.second->train_id});
                }
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

        // real data
        DataManager &dm = DataManager::getInstance(RailwayDB->data, StationDB->data, TrainDB->data);
        station_ startStationId = 6;
        station_ endStationId = 90;
        time_ startTime = 18 * 60 * 60;

        // test data
        // DataManager &dm = DataManager::getInstance(model::__test__Railway_s, model::__test__Station_s, model::__test__Train_s);
        // station_ startStationId = 3;
        // station_ endStationId = 7;
        // time_ startTime = 10 * 60 * 60;

        dm.display("db");

        Graph graph;
        for (auto &e : dm.edge_map)
        {
            graph[e.second->from][e.second->to] = e.second;
        }

        ShortestPath shortestPaths = dijkstra(graph, startStationId, endStationId, startTime);

        // 結果を表示 !FIXME!@Baketsu150L!
        // std::cout << "Shortest path from station "
        //           << dm.station_map[startStationId].name
        //           << " to station "
        //           << dm.station_map[endStationId].name
        //           << ":" << std::endl;

        // for (auto &path : shortestPaths)
        // {
        //     if (path.second->train_id != -1 && path.second->train_id != -2)
        //     {
        //         std::string rw_name = dm.railway_map[dm.trains[path.second->train_id]->railway_id].name;
        //         std::string st_name = dm.station_map[path.second->to].name;
        //         time_ td = path.second->departure;
        //         time_ ta = path.second->arrival;
        //         std::cout << "st-" << dm.station_map[path.second->from].name
        //                   << " -> "
        //                   << "rw-" << dm.railway_map[dm.trains[path.second->train_id]->railway_id].name
        //                   << " -> "
        //                   << " st-" << dm.station_map[path.second->to].name
        //                   << " time " << td / 3600 << ":" << (td % 3600) / 60 << ":" << td % 60
        //                   << " -> "
        //                   << " time " << ta / 3600 << ":" << (ta % 3600) / 60 << ":" << ta % 60
        //                   << std::endl;
        //     }
        //     // std::cout << "    st-" << path.second->from << " -> st-" << path.second->to
        //     //           << " train-" << path.second->train_id
        //     //           << std::endl;
        // }
        return 0;
    }
}
