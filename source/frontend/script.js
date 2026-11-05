
const API = "http://127.0.0.1:8080";

const $ = id => document.getElementById(id);

let roads = [];

const positions = {
    0: [65, 55],
    1: [260, 55],
    2: [160, 190],
    3: [390, 145],
    4: [475, 55]
};

const trafficName = {
    1: "Low",
    2: "Medium",
    3: "High"
};

$("today").textContent = new Date().toLocaleDateString();

function fillSelect(id) {
    const element = $(id);

    if (!element) return;

    element.innerHTML = "";

    for (let i = 0; i < 5; i++) {
        element.add(new Option("Intersection " + i, i));
    }
}

["source", "destination", "emSource", "emDestination"].forEach(fillSelect);

$("destination").value = "4";
$("emDestination").value = "4";

async function api(url, method = "GET", body = null) {
    const options = { method };

    if (body !== null) {
        options.headers = { "Content-Type": "application/json" };
        options.body = JSON.stringify(body);
    }

    const response = await fetch(API + url, options);
    const data = await response.json();

    if (!response.ok) {
        throw new Error(data.error || "Request failed");
    }

    return data;
}

function showMessage(id, message) {
    if ($(id)) {
        $(id).textContent = message;
    }
}

// LOAD ROADS
async function loadRoads() {
    roads = await api("/api/roads");
    renderRoads();
}

// DISPLAY ROADS
function renderRoads() {
    if ($("roadList")) {
        $("roadList").innerHTML = "";
    }

    if ($("roadSelect")) {
        $("roadSelect").innerHTML = "";
    }

    roads.forEach(road => {
        const option = document.createElement("option");
        option.value = `${road.u}-${road.v}`;
        option.textContent = `${road.u} - ${road.v}`;
        $("roadSelect")?.appendChild(option);

        if ($("roadList")) {
            const item = document.createElement("div");

            item.textContent =
                `Road ${road.u} - ${road.v} | ` +
                `${road.distance} km | ` +
                `${trafficName[road.traffic]} Traffic | ` +
                `${road.closed ? "Closed" : "Open"}`;

            $("roadList").appendChild(item);
        }
    });

    if ($("roadCount")) {
        $("roadCount").textContent = roads.length;
    }

    if ($("closedCount")) {
        $("closedCount").textContent =
            roads.filter(road => road.closed).length;
    }

    drawRoadMap();
}

// DRAW ROAD MAP
function drawRoadMap() {
    if ($("roadLines")) {
        $("roadLines").innerHTML = "";

        roads.forEach(road => {
            const [x1, y1] = positions[road.u];
            const [x2, y2] = positions[road.v];

            const line = document.createElementNS(
                "http://www.w3.org/2000/svg",
                "line"
            );

            line.setAttribute("x1", x1);
            line.setAttribute("y1", y1);
            line.setAttribute("x2", x2);
            line.setAttribute("y2", y2);
            line.setAttribute("stroke", road.closed ? "red" : "#22c55e");
            line.setAttribute("stroke-width", "5");

            $("roadLines").appendChild(line);
        });
    }

    if ($("roadNodes")) {
        $("roadNodes").innerHTML = "";

        Object.entries(positions).forEach(([id, position]) => {
            const [x, y] = position;

            const circle = document.createElementNS(
                "http://www.w3.org/2000/svg",
                "circle"
            );

            circle.setAttribute("cx", x);
            circle.setAttribute("cy", y);
            circle.setAttribute("r", "15");
            circle.setAttribute("fill", "#2563eb");

            $("roadNodes").appendChild(circle);

            const label = document.createElementNS(
                "http://www.w3.org/2000/svg",
                "text"
            );

            label.setAttribute("x", x);
            label.setAttribute("y", y + 5);
            label.setAttribute("text-anchor", "middle");
            label.setAttribute("fill", "white");
            label.textContent = id;

            $("roadNodes").appendChild(label);
        });
    }
}

// ADAPTIVE SIGNAL
async function calculateSignals() {
    try {
        const vehicles = [
            Number($("north").value),
            Number($("south").value),
            Number($("east").value),
            Number($("west").value)
        ];

        const result = await api("/api/signals", "POST", { vehicles });

        $("signalResults").innerHTML = "";

        result.forEach(signal => {
            const item = document.createElement("div");

            item.textContent =
                `${signal.direction}: ${signal.vehicles} vehicles | ` +
                `${signal.greenTime} seconds green`;

            $("signalResults").appendChild(item);
        });
    }
    catch (error) {
        showMessage("signalResults", error.message);
    }
}

$("calculateSignals").addEventListener("click", calculateSignals);

// FIND SHORTEST ROUTE
$("findRoute").addEventListener("click", async () => {
    try {
        const source = Number($("source").value);
        const destination = Number($("destination").value);

        const result = await api("/api/route", "POST", {
            source,
            destination
        });

        if (!result.found) {
            showMessage("routeOutput", "No route available!");
            return;
        }

        showMessage(
            "routeOutput",
            `Shortest Route: ${result.path.join(" -> ")}\n` +
            `Distance: ${result.distance} km\n` +
            `Travel Time: ${result.travelTime.toFixed(2)} minutes`
        );
    }
    catch (error) {
        showMessage("routeOutput", error.message);
    }
});

// UPDATE TRAFFIC
$("updateTraffic").addEventListener("click", async () => {
    try {
        const [u, v] = $("roadSelect").value.split("-").map(Number);
        const level = Number($("trafficLevel").value);

        const result = await api("/api/traffic", "POST", {
            u, v, level
        });

        await loadRoads();
        alert(result.message);
    }
    catch (error) {
        alert(error.message);
    }
});

// CLOSE OR REOPEN ROAD
$("toggleClosure").addEventListener("click", async () => {
    try {
        const [u, v] = $("roadSelect").value.split("-").map(Number);

        const road = roads.find(
            item => item.u === u && item.v === v
        );

        const result = await api("/api/closure", "POST", {
            u,
            v,
            closed: !road.closed
        });

        await loadRoads();
        alert(result.message);
    }
    catch (error) {
        alert(error.message);
    }
});

// ADD EMERGENCY VEHICLE
$("addEmergency").addEventListener("click", async () => {
    try {
        const type = $("vehicleType").value;
        const current = Number($("emSource").value);
        const destination = Number($("emDestination").value);

        const priority = {
            Ambulance: 1,
            Fire: 2,
            Police: 3
        }[type] || 1;

        const result = await api("/api/emergency/add", "POST", {
            type,
            current,
            destination,
            priority
        });

        showMessage("emergencyMessage", result.message);

        await loadEmergencyQueue();
    }
    catch (error) {
        showMessage("emergencyMessage", error.message);
    }
});

// DISPLAY EMERGENCY QUEUE
async function loadEmergencyQueue() {
    try {
        const queue = await api("/api/emergency/queue");

        if ($("queueCount")) {
            $("queueCount").textContent = queue.length;
        }

        if ($("queueLabel")) {
            $("queueLabel").textContent =
                queue.length ? "Vehicles Waiting" : "Queue Empty";
        }

        if ($("queueList")) {
            $("queueList").innerHTML = "";

            queue.forEach((vehicle, index) => {
                const item = document.createElement("div");

                item.textContent =
                    `${index + 1}. ${vehicle.type} | ` +
                    `From ${vehicle.current} to ${vehicle.destination} | ` +
                    `Priority ${vehicle.priority}`;

                $("queueList").appendChild(item);
            });
        }
    }
    catch (error) {
        showMessage("emergencyMessage", error.message);
    }
}

// PROCESS EMERGENCY VEHICLE
$("processEmergency").addEventListener("click", async () => {
    try {
        const result = await api("/api/emergency/process", "POST");

        if (!result.processed) {
            showMessage("emergencyMessage", result.message);
        }
        else if (!result.found) {
            showMessage(
                "emergencyMessage",
                `${result.type} processed, but no route is available.`
            );
        }
        else {
            showMessage(
                "emergencyMessage",
                `${result.type} processed!\n` +
                `Route: ${result.path.join(" -> ")}\n` +
                `Distance: ${result.distance} km\n` +
                `Travel Time: ${result.travelTime.toFixed(2)} minutes`
            );
        }

        await loadEmergencyQueue();
    }
    catch (error) {
        showMessage("emergencyMessage", error.message);
    }
});

// INITIALIZE DASHBOARD
async function initializeDashboard() {
    try {
        await loadRoads();
        await calculateSignals();
        await loadEmergencyQueue();
    }
    catch (error) {
        alert(
            "Cannot connect to C++ server. Start server.exe first.\n" +
            error.message
        );
    }
}

initializeDashboard();
