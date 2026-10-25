
#ifndef EMERGENCY_VEHICLE_H
#define EMERGENCY_VEHICLE_H

#include <string>
#include <queue>
#include <vector>
#include "roadNetwork.h"

using namespace std;

struct EmergencyVehicle {
    string type;
    int currentIntersection;
    int destination;
    int priority;
};

struct ComparePriority {
    bool operator()(const EmergencyVehicle& a,
                    const EmergencyVehicle& b) const {
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
    void addVehicle(
        string type,
        int current,
        int destination,
        int priority
    );

    void displayQueue();

    void processNext(RoadNetwork& network);
};

#endif


