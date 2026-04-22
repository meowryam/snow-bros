
/*
#pragma once
#include <string>
#include "PlayerData.h"

class FileManager {
public:
    // Saves player data to saves/<username>.txt
    static bool savePlayerData(const PlayerData& data);

    // Loads player data from saves/<username>.txt
    // Returns false if file doesn't exist (new player)
    static bool loadPlayerData(const std::string& username, PlayerData& outData);

    // Checks if a save file exists for this user
    static bool saveExists(const std::string& username);

private:
    static std::string getSavePath(const std::string& username);
};
*/