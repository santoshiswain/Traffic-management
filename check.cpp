
#include "httplib.h"
#include "json.hpp"

#include <iostream>

using json = nlohmann::json;

int main() {
    json data;
    data["project"] = "Traffic Management";

    std::cout << data.dump() << std::endl;

    httplib::Server server;

    std::cout << "Libraries loaded successfully!" << std::endl;

    return 0;
}
