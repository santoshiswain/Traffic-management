
#include "roadNetwork.h"
#include <queue>
#include <algorithm>
#include <limits>

RoadNetwork::RoadNetwork(int n) {
    intersections = n;
    roads.resize(n);

    addRoad(0, 1, 4);
    addRoad(0, 2, 2);
    addRoad(1, 2, 1);
    addRoad(1, 3, 5);
    addRoad(2, 3, 8);
    addRoad(2, 4, 10);
    addRoad(3, 4, 2);
}

void RoadNetwork::addRoad(int u, int v, int distance) {
    roads[u].push_back({v, distance, 1, false});
    roads[v].push_back({u, distance, 1, false});
}

void RoadNetwork::displayRoads() {
    for (int i = 0; i < intersections; i++) {
        for (auto road : roads[i]) {
            if (i < road.destination) {
                cout << i << " -> " << road.destination
                     << " Distance: " << road.distance
                     << " Traffic: " << road.trafficLevel
                     << " Closed: " << road.closed << endl;
            }
        }
    }
}

void RoadNetwork::setTrafficLevel(int u, int v, int level) {
    for (auto &road : roads[u]) {
        if (road.destination == v) {
            road.trafficLevel = level;
        }
    }

    for (auto &road : roads[v]) {
        if (road.destination == u) {
            road.trafficLevel = level;
        }
    }
}

void RoadNetwork::setRoadClosure(int u, int v, bool close) {
    for (auto &road : roads[u]) {
        if (road.destination == v) {
            road.closed = close;
        }
    }

    for (auto &road : roads[v]) {
        if (road.destination == u) {
            road.closed = close;
        }
    }
}

RouteResult RoadNetwork::findShortestRoute(int source, int destination) {
    RouteResult result;

    if (source < 0 || destination < 0 ||
        source >= intersections || destination >= intersections) {
        return result;
    }

    const double INF = numeric_limits<double>::max();

    vector<double> dist(intersections, INF);
    vector<int> parent(intersections, -1);

    priority_queue<
        pair<double, int>,
        vector<pair<double, int>>,
        greater<pair<double, int>>
    > pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        int u = pq.top().second;
        double currentCost = pq.top().first;
        pq.pop();

        if (currentCost > dist[u]) {
            continue;
        }

        for (auto road : roads[u]) {
            if (road.closed) {
                continue;
            }

            double speed;

            if (road.trafficLevel == 1) {
                speed = 40;
            }
            else if (road.trafficLevel == 2) {
                speed = 25;
            }
            else {
                speed = 10;
            }

            double time = (road.distance / speed) * 60.0;
            double newCost = dist[u] + time;

            if (newCost < dist[road.destination]) {
                dist[road.destination] = newCost;
                parent[road.destination] = u;

                pq.push({newCost, road.destination});
            }
        }
    }

    if (dist[destination] == INF) {
        return result;
    }

    vector<int> path;

    int current = destination;

    while (current != -1) {
        path.push_back(current);
        current = parent[current];
    }

    reverse(path.begin(), path.end());

    result.found = true;
    result.path = path;
    result.travelTime = dist[destination];

    for (int i = 0; i < (int)path.size() - 1; i++) {
        int u = path[i];
        int v = path[i + 1];

        for (auto road : roads[u]) {
            if (road.destination == v) {
                result.totalDistance += road.distance;
                break;
            }
        }
    }

    return result;
}

bool RoadNetwork::shortestPath(int source, int destination) {
    RouteResult result = findShortestRoute(source, destination);

    if (!result.found) {
        cout << "No route available!" << endl;
        return false;
    }

    cout << "Shortest Route: ";

    for (int i = 0; i < (int)result.path.size(); i++) {
        cout << result.path[i];

        if (i != (int)result.path.size() - 1) {
            cout << " -> ";
        }
    }

    cout << endl;
    cout << "Distance: " << result.totalDistance << " km" << endl;
    cout << "Travel Time: " << result.travelTime << " minutes" << endl;

    return true;
}

const vector<vector<Road>>& RoadNetwork::getRoads() const {
    return roads;
}

int RoadNetwork::getIntersections() const {
    return intersections;
}



