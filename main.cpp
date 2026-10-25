
#include <iostream>
#include <string>
#include "TrafficSignal.h"
#include "RoadNetwork.h"
#include "EV.h"

using namespace std;

int main() {
    TrafficSignal signal;
    RoadNetwork network(5);
    EmergencyManager emergencyManager;

    int choice;

    do {
        cout << "\n===== INTELLIGENT TRAFFIC MANAGEMENT "
             << "AND EMERGENCY ROUTING SYSTEM =====\n";

        cout << "1. Manage Traffic Signals\n";
        cout << "2. Display Road Network\n";
        cout << "3. Update Road Traffic Level\n";
        cout << "4. Close/Reopen a Road\n";
        cout << "5. Find Fastest Available Route\n";
        cout << "6. Add Emergency Vehicle\n";
        cout << "7. Process Next Emergency Vehicle\n";
       cout << "8. Display Emergency Queue\n";
        cout << "9. Exit\n";

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int count;
                bool valid = true;

                string directions[] = {
                    "North", "South", "East", "West"
                };

                for (int i = 0; i < 4; i++) {
                    cout << "Enter vehicles waiting in "
                         << directions[i] << ": ";
                    cin >> count;

                    if (count < 0) {
                        cout << "Count cannot be negative.\n";
                        valid = false;
                        break;
                    }

                    signal.setVehicleCount(i, count);
                }

                if (valid) {
                    signal.displaySignal();
                }
                break;
            }

            case 2:
                network.displayRoads();
                break;

            case 3: {
                int u, v, level;

                cout << "Enter first intersection (0-4): ";
                cin >> u;
                cout << "Enter second intersection (0-4): ";
                cin >> v;

                cout << "1. Low traffic\n";
                cout << "2. Medium traffic\n";
                cout << "3. High traffic\n";
                cout << "Enter traffic level: ";
                cin >> level;

                network.setTrafficLevel(u, v, level);
                break;
            }

            case 4: {
                int u, v, status;

                cout << "Enter first intersection (0-4): ";
                cin >> u;
                cout << "Enter second intersection (0-4): ";
                cin >> v;

                cout << "1. Close road\n";
                cout << "2. Reopen road\n";
                cout << "Enter choice: ";
                cin >> status;

                if (status == 1) {
                    network.setRoadClosure(u, v, true);
                } else if (status == 2) {
                    network.setRoadClosure(u, v, false);
                } else {
                    cout << "Invalid choice.\n";
                }
                break;
            }

            case 5: {
                int source, destination;

                cout << "Enter source intersection (0-4): ";
                cin >> source;
                cout << "Enter destination intersection (0-4): ";
                cin >> destination;

                network.shortestPath(source, destination);
                break;
            }

            case 6: {
                int typeChoice, source, destination;
                string type;
                int priority;

                cout << "1. Ambulance\n";
                cout << "2. Fire Engine\n";
                cout << "3. Police Vehicle\n";
                cout << "Select vehicle type: ";
                cin >> typeChoice;

                if (typeChoice == 1) {
                    type = "Ambulance";
                    priority = 1;
                } else if (typeChoice == 2) {
                    type = "Fire Engine";
                    priority = 2;
                } else if (typeChoice == 3) {
                    type = "Police Vehicle";
                    priority = 3;
                } else {
                    cout << "Invalid vehicle type.\n";
                    break;
                }

                cout << "Enter current intersection (0-4): ";
                cin >> source;
                cout << "Enter destination intersection (0-4): ";
                cin >> destination;

                if (source < 0 || source > 4 ||
                    destination < 0 || destination > 4) {
                    cout << "Invalid intersection number.\n";
                    break;
                }

                emergencyManager.addVehicle(
                    type, source, destination, priority
                );
                break;
            }

            case 7:
                emergencyManager.processNext(network);
                break;

            case 8:
                emergencyManager.displayQueue();
                break;

            case 9:
                cout << "Exiting system. Goodbye!\n";
                break;

            default:
                cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 9);

    return 0;
}




