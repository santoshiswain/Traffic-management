
#ifndef ROAD_NETWORK_H
#define ROAD_NETWORK_H

#include <vector>

using namespace std;

struct Road {
    int destination;
    int distance;
    int trafficLevel; // 1 = Low, 2 = Medium, 3 = High
    bool closed;
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

   bool shortestPath(int source, int destination);
};

#endif


