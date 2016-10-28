#include "config.hpp"
#include <unordered_map>
#include <fstream>
#include <sstream>

namespace rogue {

namespace {

const char CONFIG_FILE[] = "config.cfg";

}

int GetConfigValue(const std::string& name) {
    static std::unordered_map<std::string, int> config;
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        std::ifstream file(CONFIG_FILE);
        std::string buff;
        while (std::getline(file, buff)) {
            std::istringstream line(buff);
            std::string key;
            if (std::getline(line, key, '=')) {
                std::string value;
                if (std::getline(line, value)) {
                    config[key] = std::stoi(value, 0, 0);
                }
            }
        }
    }
    return config[name];
}

}
