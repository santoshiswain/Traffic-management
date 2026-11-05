
#ifndef EV_H
#define EV_H

#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include "roadNetwork.h"

using namespace std;

struct EmergencyVehicle {
    string type;
    int currentIntersection;
    int destination;
    int priority;
};

struct EmergencyProcessResult {
    bool processed = false;
    EmergencyVehicle vehicle;
    RouteResult route;
};

struct ComparePriority {
    bool operator()(EmergencyVehicle a, EmergencyVehicle b) {
        return a.priority > b.priority;
    }
};

class EmergencyManager {
private:
    priority_queue<
        EmergencyVehicle,
        vector<EmergencyVehicle>,
        ComparePriority
    > vehicles;

public:
    void addVehicle(string type, int current, int destination, int priority);

    void displayQueue();

    vector<EmergencyVehicle> getQueue() const;

    EmergencyProcessResult processNext(RoadNetwork &network);

    bool isEmpty() const;
};

#endif



