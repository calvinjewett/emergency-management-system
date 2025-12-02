#include "graph.h"
#include <queue>
#include <limits>
#include <functional>

using namespace std;

Graph::Graph() {}

void Graph::addCity(const string& city) {
    adjList[city]; // ensures key exists
}

void Graph::addConnection(const string& from, const string& to, int distance) {
    adjList[from].push_back({to, distance});
    adjList[to].push_back({from, distance}); // undirected
}

unordered_map<string, int> Graph::dijkstra(const string& startCity) const 
{
    const int INF = numeric_limits<int>::max();

    unordered_map<string, int> distances;

    // Initialize all distances to infinity
    for (const auto& entry : adjList) {
        distances[entry.first] = INF;
    }
        distances[startCity] = 0;

        using Pair = pair<int, string>;

        // Min-heap priority queue (shortest distance popped first)
        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;
        pq.push({0, startCity});

        while (!pq.empty()) {
        auto top = pq.top();
        int currentDist = top.first;
        string currentCity = top.second;
        pq.pop();

        if (currentDist > distances[currentCity])
            continue; // stale entry

        // Relax edges
        for (const auto& neighbor : adjList.at(currentCity)) {
            string nextCity = neighbor.first;
            int weight = neighbor.second;
            int newDist = currentDist + weight;

            if (newDist < distances[nextCity]) {
                distances[nextCity] = newDist;
                pq.push({newDist, nextCity});
            }
        }
    }

    return distances;
}
