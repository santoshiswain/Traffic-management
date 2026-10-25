
#include <iostream>
#include "EV.h"

using namespace std;

void EmergencyManager::addVehicle(
    string type, int current,
    int destination, int priority) {

    EmergencyVehicle vehicle{
        type, current, destination, priority
    };

    vehicles.push(vehicle);

    cout << type << " added to emergency queue.\n";
}

void EmergencyManager::processNext(RoadNetwork& network) {
    if (vehicles.empty()) {
        cout << "No emergency vehicles waiting.\n";
        return;
    }

    EmergencyVehicle vehicle = vehicles.top();
    vehicles.pop();

    cout << "\n=== EMERGENCY ROUTING ===\n";
    cout << "Vehicle: " << vehicle.type << '\n';
    cout << "Starting intersection: "
         << vehicle.currentIntersection << '\n';
    cout << "Destination: " << vehicle.destination << '\n';

    cout << "Finding the fastest available route...\n";

    bool routeFound = network.shortestPath(
        vehicle.currentIntersection,
        vehicle.destination
    );

    if (routeFound) {
        cout << "Emergency routing completed successfully.\n";
    } else {
        cout << "Emergency routing failed: no available route.\n";
    }
}
void EmergencyManager::displayQueue() {
    if (vehicles.empty()) {
        cout << "\nNo emergency vehicles in the queue.\n";
        return;
    }

    // Copy the queue so displaying it doesn't remove vehicles.
    priority_queue<
        EmergencyVehicle,
        vector<EmergencyVehicle>,
        ComparePriority
    > temp = vehicles;

    cout << "\n=== EMERGENCY VEHICLE QUEUE ===\n";

    int position = 1;

    while (!temp.empty()) {
        EmergencyVehicle vehicle = temp.top();
        temp.pop();

        cout << position++ << ". "
             << vehicle.type
             << " | Current: " << vehicle.currentIntersection
             << " | Destination: " << vehicle.destination
             << " | Priority: " << vehicle.priority
             << '\n';
    }
}







