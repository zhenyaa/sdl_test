//
// Created by zhenya on 06.02.2026.
//

#ifndef SDL_TEST_UTILS_H
#define SDL_TEST_UTILS_H
#include <string>
#include <nlohmann/json.hpp>
namespace FileUtils {
    std::optional<nlohmann::json> loadJSON(const std::string& path, const std::string &sprite_name);
    std::string readTextFile(const std::string& path);
    void saveJSON(const std::string& path, const nlohmann::json& j);
}
#endif //SDL_TEST_UTILS_H