
#ifndef ROAD_NETWORK_H
#define ROAD_NETWORK_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Road {
    int destination;
    int distance;
    int trafficLevel;
    bool closed;
};

struct RouteResult {
    bool found = false;
    vector<int> path;
    double totalDistance = 0;
    double travelTime = 0;
};

class RoadNetwork {
private:
    int intersections;
    vector<vector<Road>> roads;

public:
    RoadNetwork(int n);

    void addRoad(int u, int v, int distance);
    void displayRoads();

    void setTrafficLevel(int u, int v, int level);
    void setRoadClosure(int u, int v, bool close);

    RouteResult findShortestRoute(int source, int destination);
    bool shortestPath(int source, int destination);

    const vector<vector<Road>>& getRoads() const;
    int getIntersections() const;
};

#endif



