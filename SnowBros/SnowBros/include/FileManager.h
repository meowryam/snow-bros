#pragma once
#include <string>
#include "PlayerData.h"

class FileManager {

private:
    static string getSavePath(const string& username); //static means we dont have to make objects out of it
public:
    static bool savePlayerData(const PlayerData& data); 
    static bool loadPlayerData(const string& username, PlayerData& outData);
    static bool saveExists(const string& username);

};