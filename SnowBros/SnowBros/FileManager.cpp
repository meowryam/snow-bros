
#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <filesystem>

string FileManager::getSavePath(const string& username)
{
    return "saves/" + username + ".txt";
}

bool FileManager::savePlayerData(const PlayerData& data) 
{
    filesystem::create_directories("saves");

    ofstream file(getSavePath(data.getUsername()));
    if (!file.is_open()) return false;

    file << "username=" << data.getUsername() << "\n";
    file << "current_level=" << data.getCurrentLevel() << "\n";
    file << "lives=" << data.getLives() << "\n";
    file << "gem_count=" << data.getGemCount() << "\n";
    file << "high_score=" << data.getHighscore() << "\n";

    return true;
}

bool FileManager::loadPlayerData(const string& username, PlayerData& outData)
{
     ifstream file(getSavePath(username));
    if (!file.is_open()) return false; //new player

 string line;
 while (getline(file, line)) //line by line read
    {
	 istringstream ss(line); 
       string key, value;
        if (getline(ss, key, '=') && getline(ss, value)) 
        {
            if (key == "username")    
                outData.setUsername(value);

            else if (key == "current_level") 
                outData.setCurrentLevel(stoi(value));

            else if (key == "lives") 
                outData.setLives(stoi(value));

            else if (key == "gem_count")  
                outData.setGemCount(stoi(value));

            else if (key == "high_score")   
                outData.setHighscore(stoi(value));
        }
    }
    return true;
}

bool FileManager::saveExists(const string& username) 
{
  ifstream file(getSavePath(username));
    return file.is_open();
}
   