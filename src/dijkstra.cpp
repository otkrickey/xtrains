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
                        // min = transfer_time;
                        min = prevEdge->train_id == -1 ? 0 : transfer_time;
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
                    pq.push({edge.second->to, min_edge_->arrival, edge.second->from, min_edge_->train_id});
                }
            }
        }
        return shortestPaths;
    }

    int main()
    {
        using namespace model;

        clock_t start = clock();

        auto RailwayDB = database_v3::Database<railway_, Railway_>::loadDB();
        auto StationDB = database_v3::Database<station_, Station_>::loadDB();
        auto TrainDB = database_v3::Database<train_, Train_>::loadDB();

        clock_t load = clock();
        std::cout << "Load time: " << (double)(load - start) / CLOCKS_PER_SEC << "s" << std::endl;

        DataManager &dm = DataManager::getInstance(RailwayDB->data, StationDB->data, TrainDB->data);

        clock_t init = clock();
        std::cout << "Init time: " << (double)(init - load) / CLOCKS_PER_SEC << "s" << std::endl;

        auto [startStationId, endStationId, startTime] = readDijkstraOptions();

        clock_t read = clock();
        std::cout << "Read time: " << (double)(read - init) / CLOCKS_PER_SEC << "s" << std::endl;

        Graph graph;
        for (auto &e : dm.edge_map)
        {
            graph[e.second->from][e.second->to] = e.second;
        }

        clock_t graph_ = clock();
        std::cout << "Graph time: " << (double)(graph_ - read) / CLOCKS_PER_SEC << "s" << std::endl;

        std::cout << "Start Dijkstra" << std::endl;
        ShortestPath shortestPaths = dijkstra(graph, startStationId, endStationId, startTime);
        clock_t end = clock();
        std::cout << "End Dijkstra" << std::endl;
        std::cout << "Time: " << (double)(end - graph_) / CLOCKS_PER_SEC << "s" << std::endl;

        // 結果を表示
        std::cout << "Shortest path from station "
                  << dm.station_map[startStationId].name
                  << " to station "
                  << dm.station_map[endStationId].name
                  << ":" << std::endl;

        std::vector<model::Edge_ *> shortestPaths_;
        station_ currentStationId = endStationId;

        while (currentStationId != startStationId)
        {
            for (auto &path : shortestPaths)
            {
                if (path.second->to == currentStationId)
                {
                    shortestPaths_.push_back(path.second);
                    currentStationId = path.second->from;
                    break;
                }
            }
        }

        for (auto &path : shortestPaths_)
        {
            if (path->train_id != -1 && path->train_id != -2)
            {
                std::string rw_name = dm.railway_map[dm.trains[path->train_id]->railway_id].name;
                std::string st_name = dm.station_map[path->to].name;
                time_ td = path->departure;
                time_ ta = path->arrival;
                std::cout << "st-" << dm.station_map[path->from].name
                          << " -> "
                          << "rw-" << dm.railway_map[dm.trains[path->train_id]->railway_id].name
                          << " -> "
                          << " st-" << dm.station_map[path->to].name
                          << " time " << td / 3600 << ":" << (td % 3600) / 60 << ":" << td % 60
                          << " -> "
                          << " time " << ta / 3600 << ":" << (ta % 3600) / 60 << ":" << ta % 60
                          << std::endl;
            }
        }

        return 0;
    }
}
