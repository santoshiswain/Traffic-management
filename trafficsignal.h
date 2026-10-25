
#ifndef TRAFFIC_SIGNAL_H
#define TRAFFIC_SIGNAL_H

class TrafficSignal {
private:
    int vehicles[4];
    int greenTime[4];

public:
    TrafficSignal();

    void setVehicleCount(int direction, int count);
    void calculateGreenTime();
    void displaySignal();
};

#endif



