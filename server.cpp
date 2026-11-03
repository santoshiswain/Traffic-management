
#include <iostream>
#include "httplib.h"
#include "json.hpp"
#include "trafficsignal.h"
#include "roadNetwork.h"
#include "EV.h"

using namespace std;
using json = nlohmann::json;

int main() {

    httplib::Server server;

    TrafficSignal signal;
    RoadNetwork network(5);
    EmergencyManager emergencyManager;

    server.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });

    server.Options(R"(.*)", [](const httplib::Request&, httplib::Response& res) {
        res.status = 200;
    });

    // TEST CONNECTION
    server.Get("/api/test", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(
            R"({"message":"C++ Backend Connected Successfully"})",
            "application/json"
        );
    });

    // ADAPTIVE TRAFFIC SIGNAL
    server.Post("/api/signals", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json data = json::parse(req.body);

            if (!data.contains("vehicles") ||
                !data["vehicles"].is_array() ||
                data["vehicles"].size() != 4) {
                res.status = 400;
                res.set_content(R"({"error":"Enter 4 vehicle counts"})", "application/json");
                return;
            }

            for (int i = 0; i < 4; i++) {
                if (!data["vehicles"][i].is_number_integer() ||
                    data["vehicles"][i].get<int>() < 0) {
                    res.status = 400;
                    res.set_content(R"({"error":"Vehicle counts must be non-negative integers"})", "application/json");
                    return;
                }

                signal.setVehicleCount(i, data["vehicles"][i].get<int>());
            }

            signal.calculateGreenTime();

            json result = json::array();

            string directions[] = {"North", "South", "East", "West"};

            for (int i = 0; i < 4; i++) {
                result.push_back({
                    {"direction", directions[i]},
                    {"vehicles", signal.getVehicleCount(i)},
                    {"greenTime", signal.getGreenTime(i)}
                });
            }

            res.set_content(result.dump(), "application/json");
        }
        catch (...) {
            res.status = 400;
            res.set_content(R"({"error":"Invalid signal data"})", "application/json");
        }
    });

    // GET ROADS
    server.Get("/api/roads", [&](const httplib::Request&, httplib::Response& res) {
        json result = json::array();

        const auto& roads = network.getRoads();

        for (int u = 0; u < network.getIntersections(); u++) {
            for (auto road : roads[u]) {
                if (u < road.destination) {
                    result.push_back({
                        {"u", u},
                        {"v", road.destination},
                        {"distance", road.distance},
                        {"traffic", road.trafficLevel},
                        {"closed", road.closed}
                    });
                }
            }
        }

        res.set_content(result.dump(), "application/json");
    });

    // UPDATE TRAFFIC
    server.Post("/api/traffic", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json data = json::parse(req.body);

            int u = data.at("u").get<int>();
            int v = data.at("v").get<int>();
            int level = data.at("level").get<int>();

            if (u < 0 || v < 0 || u >= 5 || v >= 5 ||
                u == v || level < 1 || level > 3) {
                res.status = 400;
                res.set_content(R"({"error":"Invalid traffic information"})", "application/json");
                return;
            }

            network.setTrafficLevel(u, v, level);

            res.set_content(R"({"message":"Traffic updated successfully"})", "application/json");
        }
        catch (...) {
            res.status = 400;
            res.set_content(R"({"error":"Invalid request"})", "application/json");
        }
    });

    // ROAD CLOSURE
    server.Post("/api/closure", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json data = json::parse(req.body);

            int u = data.at("u").get<int>();
            int v = data.at("v").get<int>();
            bool closed = data.at("closed").get<bool>();

            if (u < 0 || v < 0 || u >= 5 || v >= 5 || u == v) {
                res.status = 400;
                res.set_content(R"({"error":"Invalid road"})", "application/json");
                return;
            }

            network.setRoadClosure(u, v, closed);

            res.set_content(R"({"message":"Road status updated successfully"})", "application/json");
        }
        catch (...) {
            res.status = 400;
            res.set_content(R"({"error":"Invalid request"})", "application/json");
        }
    });

    // FIND SHORTEST ROUTE
    server.Post("/api/route", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json data = json::parse(req.body);

            int source = data.at("source").get<int>();
            int destination = data.at("destination").get<int>();

            if (source < 0 || destination < 0 ||
                source >= 5 || destination >= 5) {
                res.status = 400;
                res.set_content(R"({"error":"Invalid intersection"})", "application/json");
                return;
            }

            RouteResult route = network.findShortestRoute(source, destination);

            json result;

            result["found"] = route.found;

            if (route.found) {
                result["path"] = route.path;
                result["distance"] = route.totalDistance;
                result["travelTime"] = route.travelTime;
            }
            else {
                result["message"] = "No route available";
            }

            res.set_content(result.dump(), "application/json");
        }
        catch (...) {
            res.status = 400;
            res.set_content(R"({"error":"Invalid route request"})", "application/json");
        }
    });

    // ADD EMERGENCY VEHICLE
    server.Post("/api/emergency/add", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json data = json::parse(req.body);

            string type = data.at("type").get<string>();
            int current = data.at("current").get<int>();
            int destination = data.at("destination").get<int>();
            int priority = data.at("priority").get<int>();

            if (type.empty() || current < 0 || current >= 5 ||
                destination < 0 || destination >= 5 ||
                priority < 1 || priority > 3) {
                res.status = 400;
                res.set_content(R"({"error":"Invalid emergency vehicle information"})", "application/json");
                return;
            }

            emergencyManager.addVehicle(type, current, destination, priority);

            res.set_content(R"({"message":"Emergency vehicle added successfully"})", "application/json");
        }
        catch (...) {
            res.status = 400;
            res.set_content(R"({"error":"Invalid request"})", "application/json");
        }
    });

    // DISPLAY EMERGENCY QUEUE
    server.Get("/api/emergency/queue", [&](const httplib::Request&, httplib::Response& res) {
        json result = json::array();

        auto queue = emergencyManager.getQueue();

        for (auto vehicle : queue) {
            result.push_back({
                {"type", vehicle.type},
                {"current", vehicle.currentIntersection},
                {"destination", vehicle.destination},
                {"priority", vehicle.priority}
            });
        }

        res.set_content(result.dump(), "application/json");
    });

    // PROCESS EMERGENCY VEHICLE
    server.Post("/api/emergency/process", [&](const httplib::Request&, httplib::Response& res) {
        EmergencyProcessResult result = emergencyManager.processNext(network);

        json response;

        response["processed"] = result.processed;

        if (!result.processed) {
            response["message"] = "No emergency vehicles waiting";
        }
        else {
            response["type"] = result.vehicle.type;
            response["priority"] = result.vehicle.priority;
            response["found"] = result.route.found;

            if (result.route.found) {
                response["path"] = result.route.path;
                response["distance"] = result.route.totalDistance;
                response["travelTime"] = result.route.travelTime;
            }
            else {
                response["message"] = "Vehicle processed, but no route is available";
            }
        }

        res.set_content(response.dump(), "application/json");
    });

    cout << "Traffic Management Server Starting..." << endl;
    cout << "Server running at http://127.0.0.1:8080" << endl;

    server.listen("127.0.0.1", 8080);

    return 0;
}


