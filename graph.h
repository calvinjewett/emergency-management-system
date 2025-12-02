#pragma once
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Graph {
private:
    // adjacency list: city -> vector of (neighborCity, distance)
    unordered_map<string, vector<pair<string,int>>> adjList;

public:
    Graph();

    void addCity(const string& city);
    void addConnection(const string& from, const string& to, int distance);

    // Dijkstra FROM a starting city to all others
    unordered_map<string, int> dijkstra(const string& startCity) const;
};
