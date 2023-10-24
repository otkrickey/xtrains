#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <climits>
#include <fstream>

#include "gen_data.cpp"

using namespace std;

struct Edge // train path
{
    int to;
    int weight; // 所要時間
    int line;   // 路線ID
};

struct Node // station
{
    int id;
    int dist;      // 現在地からの距離
    int prev_line; // 以前に使用した路線

    bool operator>(const Node &rhs) const
    {
        return dist > rhs.dist;
    }
};

int dijkstra(const vector<vector<Edge>> &graph, int start, int goal, int transfer_time)
{
    vector<int> dist(graph.size(), INT_MAX);
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    dist[start] = 0;
    pq.push({start, 0, -1});

    while (!pq.empty())
    {
        Node current = pq.top();
        pq.pop();

        cout << "(" << current.id << "," << current.dist << ")";

        if (current.id == goal)
        {
            cout << endl;
            return current.dist;
        }

        for (Edge e : graph[current.id])
        {
            int next_dist = current.dist + e.weight;

            // 乗り換え時間の追加
            if (current.prev_line != e.line && current.prev_line != -1)
            {
                next_dist += transfer_time;
            }

            if (next_dist < dist[e.to])
            {
                dist[e.to] = next_dist;
                pq.push({e.to, next_dist, e.line});
            }
        }
    }

    return -1;
}

int d_main()
{
    // gen_data();
    ifstream inFile("large_data.txt");
    if (!inFile)
    {
        cerr << "Unable to open file data.txt";
        return 1; // exit with failure
    }

    int n, m;
    inFile >> n >> m;

    vector<vector<Edge>> graph(n);

    for (int i = 0; i < m; ++i)
    {
        int u, v, w, l;
        inFile >> u >> v >> w >> l;

        graph[u].push_back({v, w, l});
        graph[v].push_back({u, w, l});
    }

    int transfer_time;
    inFile >> transfer_time;

    int start, goal;
    // inFile >> start >> goal;
    cout << "Enter start and goal: ";
    cin >> start >> goal;

    int shortest_time = dijkstra(graph, start, goal, transfer_time);

    if (shortest_time == -1)
    {
        cout << "There's no route from " << start << " to " << goal << endl;
    }
    else
    {
        cout << "Shortest time from " << start << " to " << goal << " is: " << shortest_time << endl;
    }

    inFile.close();

    return 0;
}