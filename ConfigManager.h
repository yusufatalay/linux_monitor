#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <map>
#include <optional>
#include <string>

class ConfigManager {
public:
  bool loadFromFile(const std::string &filename);

  std::optional<double> getDouble(const std::string &key) const;

private:
  std::map<std::string, std::string> configData_;
};

#endif
