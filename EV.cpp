
#include "EV.h"

void EmergencyManager::addVehicle(
    string type,
    int current,
    int destination,
    int priority
) {
    EmergencyVehicle vehicle;

    vehicle.type = type;
    vehicle.currentIntersection = current;
    vehicle.destination = destination;
    vehicle.priority = priority;

    vehicles.push(vehicle);

    cout << type << " added to emergency queue!" << endl;
}

bool EmergencyManager::isEmpty() const {
    return vehicles.empty();
}

vector<EmergencyVehicle> EmergencyManager::getQueue() const {
    vector<EmergencyVehicle> result;

    auto temp = vehicles;

    while (!temp.empty()) {
        result.push_back(temp.top());
        temp.pop();
    }

    return result;
}

void EmergencyManager::displayQueue() {
    auto queueList = getQueue();

    if (queueList.empty()) {
        cout << "No emergency vehicles waiting." << endl;
        return;
    }

    cout << "\nEmergency Queue:\n";

    for (auto vehicle : queueList) {
        cout << vehicle.type
             << " | From: " << vehicle.currentIntersection
             << " | To: " << vehicle.destination
             << " | Priority: " << vehicle.priority
             << endl;
    }
}

EmergencyProcessResult EmergencyManager::processNext(RoadNetwork &network) {
    EmergencyProcessResult result;

    if (vehicles.empty()) {
        cout << "No emergency vehicles waiting." << endl;
        return result;
    }

    EmergencyVehicle vehicle = vehicles.top();
    vehicles.pop();

    result.processed = true;
    result.vehicle = vehicle;

    result.route = network.findShortestRoute(
        vehicle.currentIntersection,
        vehicle.destination
    );

    cout << "\nProcessing Emergency Vehicle: " << vehicle.type << endl;

    if (result.route.found) {
        cout << "Route: ";

        for (int i = 0; i < (int)result.route.path.size(); i++) {
            cout << result.route.path[i];

            if (i != (int)result.route.path.size() - 1) {
                cout << " -> ";
            }
        }

        cout << endl;
        cout << "Travel Time: " << result.route.travelTime << " minutes" << endl;
    }
    else {
        cout << "No route available for this vehicle!" << endl;
    }

    return result;
}








