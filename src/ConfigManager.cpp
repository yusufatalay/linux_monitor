#include "ConfigManager.h"
#include <fstream>
#include <optional>
#include <sstream>
#include <string>

bool ConfigManager::loadFromFile(const std::string &filename) {
  std::ifstream configFile(filename);
  if (!configFile.is_open()) {
    return false;
  }

  std::string line;
  while (std::getline(configFile, line)) {
    if (line.empty() || line[0] == '#') {
      continue;
    }

    std::istringstream iss(line);
    std::string key, value;
    if (std::getline(iss, key, '=') && std::getline(iss, value)) {
      key.erase(0, key.find_first_not_of(" \t"));
      key.erase(key.find_last_not_of(" \t") + 1);
      value.erase(0, key.find_first_not_of(" \t"));
      value.erase(key.find_last_not_of(" \t") + 1);

      if (!key.empty() && !value.empty()) {
        configData_[key] = value;
      }
    }
  }
  return true;
}

std::optional<double> ConfigManager::getDouble(const std::string &key) const {
  auto it = configData_.find(key);
  if (it != configData_.end()) {
    try {
      return std::stod(it->second);
    } catch (const std::exception &) {
      return std::nullopt;
    }
  }

  return std::nullopt;
}
