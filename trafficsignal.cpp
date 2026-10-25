
#include <iostream>
#include<string>
#include <algorithm>
#include "TrafficSignal.h"

using namespace std;

TrafficSignal::TrafficSignal() {
    for (int i = 0; i < 4; i++) {
        vehicles[i] = 0;
        greenTime[i] = 10;
    }
}

void TrafficSignal::setVehicleCount(int direction, int count) {
    if (direction >= 0 && direction < 4 && count >= 0) {
        vehicles[direction] = count;
    }
}

void TrafficSignal::calculateGreenTime() {
    int totalVehicles = 0;

    for (int i = 0; i < 4; i++) {
        totalVehicles += vehicles[i];
    }

    for (int i = 0; i < 4; i++) {
        if (totalVehicles == 0) {
            greenTime[i] = 10;
        } else {
            greenTime[i] = 10 +
                (vehicles[i] * 40) / totalVehicles;
        }

        greenTime[i] = min(greenTime[i], 60);
    }
}

void TrafficSignal::displaySignal() {
    string directions[4] = {
        "North", "South", "East", "West"
    };

    calculateGreenTime();

    cout << "\n--- Adaptive Traffic Signal Results ---\n";

    for (int i = 0; i < 4; i++) {
        cout << directions[i]
             << ": " << vehicles[i]
             << " vehicles, Green time: "
             << greenTime[i] << " seconds\n";
    }
}


