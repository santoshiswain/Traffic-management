
#include <iostream>
#include <queue>
#include <algorithm>
#include <string>
#include <utility>
#include "roadNetwork.h"

using namespace std;

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
    if (u < 0 || u >= intersections ||
        v < 0 || v >= intersections ||
        distance <= 0 || u == v) {
        cout << "Invalid road details.\n";
        return;
    }

    // A new road is open by default.
    roads[u].push_back({v, distance, 1, false});
    roads[v].push_back({u, distance, 1, false});
}

void RoadNetwork::displayRoads() {
    string levels[] = {"", "Low", "Medium", "High"};

    cout << "\n--- Road Network ---\n";

    for (int i = 0; i < intersections; i++) {
        for (const Road& road : roads[i]) {
            if (i < road.destination) {
                cout << i << " <-> " << road.destination
                     << " | Distance: " << road.distance << " km"
                     << " | Traffic: " << levels[road.trafficLevel]
                     << " | Status: "
                     << (road.closed ? "CLOSED" : "OPEN")
                     << '\n';
            }
        }
    }
}

void RoadNetwork::setTrafficLevel(int u, int v, int level) {
    if (u < 0 || u >= intersections ||
        v < 0 || v >= intersections ||
        level < 1 || level > 3 || u == v) {
        cout << "Invalid intersection or traffic level.\n";
        return;
    }

    bool found = false;

    for (Road& road : roads[u]) {
        if (road.destination == v) {
            road.trafficLevel = level;
            found = true;
        }
    }

    for (Road& road : roads[v]) {
        if (road.destination == u) {
            road.trafficLevel = level;
        }
    }

    if (found) {
        cout << "Traffic level updated successfully.\n";
    } else {
        cout << "No road connects these intersections.\n";
    }
}

void RoadNetwork::setRoadClosure(int u, int v, bool close) {
    if (u < 0 || u >= intersections ||
        v < 0 || v >= intersections || u == v) {
        cout << "Invalid intersection numbers.\n";
        return;
    }

    bool found = false;

    for (Road& road : roads[u]) {
        if (road.destination == v) {
            road.closed = close;
            found = true;
        }
    }

    for (Road& road : roads[v]) {
        if (road.destination == u) {
            road.closed = close;
        }
    }

    if (!found) {
        cout << "No road connects these intersections.\n";
    } else if (close) {
        cout << "Road closed successfully.\n";
    } else {
        cout << "Road reopened successfully.\n";
    }
}

bool RoadNetwork::shortestPath(int source, int destination){
    if (source < 0 || source >= intersections ||
        destination < 0 || destination >= intersections) {
        cout << "Invalid intersection number.\n";
        return false;
    }

    if (source == destination) {
        cout << "You are already at the destination.\n";
        cout << "Estimated travel time: 0 minutes\n";
        return true;
    }

    // Estimated speed in km/h for each traffic level.
    double speed[] = {0, 40.0, 25.0, 10.0};

    const double INF = 1e18;

    vector<double> time(intersections, INF);
    vector<int> parent(intersections, -1);

    priority_queue<
        pair<double, int>,
        vector<pair<double, int>>,
        greater<pair<double, int>>
    > pq;

    time[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        double currentTime = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (currentTime > time[u]) {
            continue;
        }

        for (const Road& road : roads[u]) {
            // Never use a closed road.
            if (road.closed) {
                continue;
            }

            int v = road.destination;

            double travelTime =
                road.distance / speed[road.trafficLevel];

            if (time[u] + travelTime < time[v]) {
                time[v] = time[u] + travelTime;
                parent[v] = u;
                pq.push({time[v], v});
            }
        }
    }

   if (time[destination] == INF) {
    cout << "No route available! All possible routes "
         << "may be blocked.\n";
    return false;
}

    vector<int> path;

    for (int current = destination;
         current != -1;
         current = parent[current]) {
        path.push_back(current);
    }

    reverse(path.begin(), path.end());

    cout << "\nFastest available route: ";

    for (int i = 0; i < static_cast<int>(path.size()); i++) {
        cout << path[i];

        if (i + 1 < static_cast<int>(path.size())) {
            cout << " -> ";
        }
    }

    cout << "\nEstimated travel time: "
         << time[destination] * 60.0 << " minutes\n";
         return true;
}

