/*
#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <filesystem>

std::string FileManager::getSavePath(const std::string& username) {
    return "saves/" + username + ".txt";
}

bool FileManager::savePlayerData(const PlayerData& data) {
    // Create saves/ directory if it doesn't exist
    std::filesystem::create_directories("saves");

    std::ofstream file(getSavePath(data.getUsername()));
    if (!file.is_open()) return false;

    file << "username=" << data.getUsername() << "\n";
    file << "current_level=" << data.getCurrentLevel() << "\n";
    file << "lives=" << data.getLives() << "\n";
    file << "gem_count=" << data.getGemCount() << "\n";
    file << "high_score=" << data.getHighscore() << "\n";

    return true;
}

bool FileManager::loadPlayerData(const std::string& username, PlayerData& outData) {
    std::ifstream file(getSavePath(username));
    if (!file.is_open()) return false;  // no save file = new player

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string key, value;
        if (std::getline(ss, key, '=') && std::getline(ss, value)) {
            if (key == "username")      outData.setUsername(value);
            else if (key == "current_level") outData.setCurrentLevel(std::stoi(value));
            else if (key == "lives")         outData.setLives(std::stoi(value));
            else if (key == "gem_count")     outData.setGemCount(std::stoi(value));
            else if (key == "high_score")    outData.setHighscore(std::stoi(value));
        }
    }
    return true;
}

bool FileManager::saveExists(const std::string& username) {
    std::ifstream file(getSavePath(username));
    return file.is_open();
}
   */